#include "parser.h"
#include "packet.h"
#include "datastorage.h"
#include <QDebug>
#include <QMetaType>

QSharedPointer<Parser> Parser::instance = nullptr;


Parser& Parser::getInstance()
{
    if (!instance)
        instance = QSharedPointer<Parser>(new Parser);
    return *instance;
}

void Parser::reset()
{
    msgCounter = 0;
    totalBytes.clear();
}

void Parser::parseData(QByteArray data)
{
    qDebug() << __FUNCTION__ << "\n" << data;
    static const auto header = Packet::header;
    static const auto footer = Packet::footer;
    static const auto minimumSize = Packet::minimumSize;
    if(totalBytes.size()>=maxSize)
        totalBytes.truncate(maxSize/2);
    totalBytes+=data;
    qDebug() << "total bytes:" << totalBytes;
    if(totalBytes.size() < minimumSize)
        return;
    auto headerIndex = totalBytes.indexOf(header);

    while(headerIndex != -1){
        auto msgCounterIndex = headerIndex+header.size();
        msgcounter_t newMsgCounter = totalBytes[msgCounterIndex];
        if (!isMsgcounterValid(newMsgCounter)){
            headerIndex = totalBytes.indexOf(header,headerIndex+1);
            continue;
        }
        auto footerIndex = totalBytes.indexOf(footer,headerIndex);
        qsizetype idNIndex = headerIndex + header.size() + sizeof(msgCounter);

        while(footerIndex != -1){
            idnumber_t idN = totalBytes.at(idNIndex);
            auto packetSize = footerIndex - idNIndex - sizeof(checksum_t) - sizeof(idnumber_t);
            if(packetSize == idN * PacketData::dataSize()){
                QByteArray packetData = totalBytes.mid(headerIndex, footerIndex - headerIndex + footer.size());

                qDebug() << "Extracted packet data:" << packetData;

                try{
                    Packet packet(packetData);
                    emit packetGenerated(packet);
                    totalBytes.remove(0, footerIndex + 1);
                    msgCounter=newMsgCounter+1;
                }
                catch(const BadChecksum &){
                    qDebug() << "Bad Checksum";
                }
                qDebug() << "total bytes:" << totalBytes;
                break;
            }
            footerIndex = totalBytes.indexOf(footer,footerIndex+1);
        }

        headerIndex = totalBytes.indexOf(header,headerIndex+1);
    }
}

Parser::Parser(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<Packet>("Packet");
    connect(this,&Parser::packetGenerated,&DataStorage::getInstance(),&DataStorage::newPacket);
}

bool Parser::isMsgcounterValid(msgcounter_t newMsgCounter)
{
    const auto maxMsgCounter = std::numeric_limits<msgcounter_t>::max();
    return newMsgCounter >= this->msgCounter || (this->msgCounter>=maxMsgCounter*(3.f/4.f) && newMsgCounter<=maxMsgCounter*(1.f/4.f));
}
