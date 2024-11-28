#include "parser.h"
#include <QDebug>
#include <QMetaType>
#include "packet.h"

void SPL::Parser::reset()
{
    msgCounter = 0;
    totalBytes.clear();
}

void SPL::Parser::parseData(QByteArray data)
{
    static const auto header = Packet::header;
    static const auto footer = Packet::footer;
    static const auto minimumSize = Packet::minimumSize;
    if (totalBytes.size() >= maxSize)
        totalBytes.truncate(maxSize / 2);
    totalBytes += data;
#ifdef QT_DEBUG
    qDebug() << "total bytes:" << totalBytes;
#endif
    if (totalBytes.size() < minimumSize)
        return;
    auto headerIndex = totalBytes.indexOf(header);

    while (headerIndex != -1) {
        auto msgCounterIndex = headerIndex + header.size();
        msgcounter_t newMsgCounter = totalBytes[msgCounterIndex];
        if (!isMsgcounterValid(newMsgCounter)) {
            headerIndex = totalBytes.indexOf(header, headerIndex + 1);
            continue;
        }
        auto footerIndex = totalBytes.indexOf(footer, headerIndex);
        qsizetype idNIndex = headerIndex + header.size() + sizeof(msgCounter);

        while (footerIndex != -1) {
            idnumber_t idN = totalBytes.at(idNIndex);
            auto packetSize = footerIndex - idNIndex - sizeof(checksum_t) - sizeof(idnumber_t);
            if (packetSize == idN * PacketData::dataSize()) {
                QByteArray packetData = totalBytes.mid(headerIndex,
                                                       footerIndex - headerIndex + footer.size());

#ifdef QT_DEBUG
                qDebug() << "Extracted packet data:" << packetData;
#endif
                try {
                    Packet packet(packetData);
                    emit packetGenerated(packet);
                    totalBytes.remove(0, footerIndex + 1);
                    msgCounter = newMsgCounter + 1;
                } catch (const BadChecksum &) {
#ifdef QT_DEBUG
                    qDebug() << "Bad Checksum";
#endif
                }
#ifdef QT_DEBUG
                qDebug() << "total bytes:" << totalBytes;
#endif
                break;
            }
            footerIndex = totalBytes.indexOf(footer, footerIndex + 1);
        }

        headerIndex = totalBytes.indexOf(header, headerIndex + 1);
    }
}

SPL::Parser::Parser(QObject *parent)
    : QObject(parent)
{
    this->msgCounter = 0;
    this->totalBytes.clear();
    qRegisterMetaType<Packet>("Packet");
}

bool SPL::Parser::isMsgcounterValid(msgcounter_t newMsgCounter)
{
    const auto maxMsgCounter = std::numeric_limits<msgcounter_t>::max();
    return newMsgCounter >= this->msgCounter
           || (this->msgCounter >= maxMsgCounter * (3.f / 4.f)
               && newMsgCounter <= maxMsgCounter * (1.f / 4.f));
}
