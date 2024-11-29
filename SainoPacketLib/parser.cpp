#include "parser.h"
#include <QDebug>
#include <QMetaType>
#include "packet.h"

void SPL::Parser::reset()
{
    msgCounter = 0;
    totalBytes.clear();
}

void SPL::Parser::parseData(const QByteArray &data)
{
    static const auto header = Packet::header;
    static const auto footer = Packet::footer;
    static const auto minimumSize = Packet::minimumSize;
    // if all saved bytes is larger than max size, removed the first half
    if (totalBytes.size() >= maxSize)
        totalBytes.truncate(maxSize / 2);
    totalBytes += data;
    qDebug() << "total bytes:" << totalBytes;

    // if no packet can be parsed yet, return
    if (totalBytes.size() < minimumSize)
        return;
    // find first index of header
    auto headerIndex = totalBytes.indexOf(header);
    while (headerIndex != -1) {
        bool packetFound = false;

        // get the message counter of packet, if it's not valid, search for the next header
        auto msgCounterIndex = headerIndex + header.size();
        msgcounter_t newMsgCounter = byteArrayConvert<msgcounter_t>(
            totalBytes.mid(msgCounterIndex, sizeof(msgcounter_t)));

        if (!isMsgcounterValid(newMsgCounter)) {
            headerIndex = totalBytes.indexOf(header, headerIndex + 1);
            continue;
        }

        // find the footer
        auto footerIndex = totalBytes.indexOf(footer, headerIndex);
        qsizetype idNIndex = headerIndex + header.size() + sizeof(msgCounter);

        while (footerIndex != -1) {
            // get id number of packet and its packet size
            // check the total size of packet data (between id number and footer)
            idnumber_t idN = byteArrayConvert<idnumber_t>(
                totalBytes.mid(idNIndex, sizeof(idnumber_t)));
            auto packetSize = footerIndex - idNIndex - sizeof(checksum_t) - sizeof(idnumber_t);
            if (packetSize == idN * PacketData::dataSize()) {
                // get the packet data between header and footer
                QByteArray packetData = totalBytes.mid(headerIndex,
                                                       footerIndex - headerIndex + footer.size());

                qDebug() << "Extracted packet data:" << packetData;
                try {
                    // try to construct a packet
                    Packet packet(packetData);
                    emit packetGenerated(packet);
                    // remove used data, increase expected message counter
                    totalBytes.remove(0, footerIndex);
                    msgCounter = newMsgCounter + 1;
                    packetFound = true;
                } catch (const BadChecksum &) {
                    qDebug() << "Bad Checksum";
                }
                qDebug() << "total bytes:" << totalBytes;
                break;
            }
            // find the next footer if the currently extracted packet is not valid
            footerIndex = totalBytes.indexOf(footer, footerIndex + 1);
        }

        // find the index of next header
        if (packetFound)
            headerIndex = totalBytes.indexOf(header);
        else
            headerIndex = totalBytes.indexOf(header, headerIndex + 1);
    }
}

SPL::Parser::Parser(QObject *parent, qsizetype maxSize)
    : QObject(parent)
    , maxSize(maxSize)
{
    this->msgCounter = 0;
    this->totalBytes.clear();
    // register Packet so it can be used in packetGenerated signal
    qRegisterMetaType<Packet>("Packet");
}

bool SPL::Parser::isMsgcounterValid(msgcounter_t newMsgCounter) const
{
    // new message counter is valid if if is le than message counter
    // or if it is in the first quarter and message counter is in the last quarter
    // which means a full rotation and overflow of message counter
    const auto maxMsgCounter = std::numeric_limits<msgcounter_t>::max();
    qDebug() << "msgcounter thershold:" << maxMsgCounter * (3.f / 4.f);
    qDebug() << "msgcounter thershold:" << maxMsgCounter * (1.f / 4.f);
    return newMsgCounter >= this->msgCounter
           || (this->msgCounter >= maxMsgCounter * (3.f / 4.f)
               && newMsgCounter <= maxMsgCounter * (1.f / 4.f));
}
