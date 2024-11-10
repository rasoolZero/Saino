#include "packet.h"
#include "datastorage.h"
#include <QDebug>


qsizetype PacketData::dataSize()
{
    return sizeof(PacketData::data) + sizeof(PacketData::factor)
            + sizeof(PacketData::id) + sizeof(PacketData::reserve);
}

checksum_t Packet::calculateChecksum(const QByteArray & bytes){
    const qsizetype startIndex = header.size();
    checksum_t checksum = 0;
    auto data = reinterpret_cast<const quint8*>(bytes.data());
    for(qsizetype i = startIndex;i < (bytes.size()-footer.size()-static_cast<qsizetype>(sizeof(checksum_t)));i++)
        checksum += data[i];
    return checksum;
}

void Packet::evaluatePacketData(const QByteArray &bytes)
{
    static const auto packetDataSize = PacketData::dataSize();
    msgCounter = bytes[header.size()];
    idN = bytes.at(header.size() + sizeof(msgcounter_t));
    const QByteArray allData = bytes.mid(header.size() + sizeof(msgcounter_t) + sizeof(idnumber_t), idN * packetDataSize);
    for(idnumber_t i = 0;i < idN * packetDataSize;i+=packetDataSize){
        auto onePacketData = allData.mid(i,packetDataSize);
        id_t id = byteArrayConvert<id_t>(onePacketData.mid(0,sizeof(id_t)));
        reserve_t reserve = byteArrayConvert<reserve_t>(onePacketData.mid(sizeof(id_t),sizeof(reserve_t)));
        data_t data = byteArrayConvert<data_t>(onePacketData.mid(sizeof(id_t)+sizeof(reserve_t),sizeof(data_t)));
        factor_t factor = byteArrayConvert<factor_t>(onePacketData.mid(sizeof(id_t)+sizeof(reserve_t)+sizeof(data_t),sizeof(factor_t)));
        this->allPacketData.push_back(PacketData(data,factor,id,reserve));
    }
}

void Packet::removeBadData()
{
    const auto& dataInfo = DataStorage::getInstance().getInfo();
    QList<PacketData>::iterator it = allPacketData.begin();
    while (it != allPacketData.end()) {
        qreal min = static_cast<qreal>(dataInfo[it->getId()].minValue);
        qreal max = static_cast<qreal>(dataInfo[it->getId()].maxValue);
        qreal value = it->getValue();
        if (value < min || value > max){
            it = allPacketData.erase(it);
            --this->idN;
        }
        else
            ++it;
    }
}

const QList<PacketData> &Packet::getAllPackets()
{
    return this->allPacketData;
}

Packet::Packet(const QByteArray &bytes)
{
    auto actualChecksumBytes = bytes.mid(bytes.size()-footer.size()-sizeof(checksum_t),sizeof(checksum_t));
    checksum_t actualChecksum = byteArrayConvert<checksum_t>(actualChecksumBytes);
    checksum_t calculatedChecksum = calculateChecksum(bytes);
    if(actualChecksum != calculatedChecksum)
        BadChecksum().raise();
    evaluatePacketData(bytes);
    removeBadData();
    if(idN == 0 || this->allPacketData.size() == 0)
        EmptyPacket().raise();
}

data_t PacketData::getData() const
{
    return data;
}

factor_t PacketData::getFactor() const
{
    return factor;
}

id_t PacketData::getId() const
{
    return id;
}

reserve_t PacketData::getReserve() const
{
    return reserve;
}

qreal PacketData::getValue() const
{
    return value;
}

bool PacketData::operator==(id_t id)
{
    return this->id == id;
}

PacketData::PacketData(data_t data, factor_t factor, id_t id, reserve_t reserve) : data(std::move(data)),
    factor(std::move(factor)),
    id(std::move(id)),
    reserve(std::move(reserve))
{
    qDebug() << __FUNCTION__;
    qDebug() << "data:" << data;
    qDebug() << "factor:" << factor;
    qDebug() << "id:" << id;
    qDebug() << "reserve:" << reserve;
    if(factor == 0)
        factor = 1;
    value = static_cast<qreal>(data)/static_cast<qreal>(factor);
}
