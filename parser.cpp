#include "parser.h"
#include "packet.h"
#include "datastorage.h"
#include <QDebug>


QSharedPointer<Parser> Parser::instance = nullptr;


Parser& Parser::getInstance()
{
    if (!instance)
        instance = QSharedPointer<Parser>(new Parser);
    return *instance;
}

void Parser::parseData(QByteArray data)
{
    qDebug() << __FUNCTION__ << "\n" << data;
    static const auto header = Packet::header;
    static const auto footer = Packet::footer;
    static const auto minimumSize = Packet::minimumSize;

    totalBytes+=data;
    qDebug() << "total bytes:" << totalBytes;
    if(totalBytes.size() < minimumSize)
        return;

    auto headerIndex = totalBytes.indexOf(header + QByteArray(1,msgCounter));

    while(headerIndex != -1){
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
                    msgCounter++;
                }
                catch(const BadChecksum &){
                    qDebug() << "Bad Checksum";
                }

                catch(const EmptyPacket &){
                    qDebug() << "Empty Packet";
                    totalBytes.remove(0, footerIndex + 1);
                    msgCounter++;
                }
                qDebug() << "total bytes:" << totalBytes;
                break;
            }
            footerIndex = totalBytes.indexOf(footer,footerIndex+1);
        }

        headerIndex = totalBytes.indexOf(header + QByteArray(1,msgCounter),headerIndex+1);
    }
}

Parser::Parser(QObject *parent)
    : QObject(parent)
{
    connect(this,&Parser::packetGenerated,&DataStorage::getInstance(),&DataStorage::newPacket);
}
