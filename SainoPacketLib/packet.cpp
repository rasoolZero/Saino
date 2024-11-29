#include "packet.h"
#include <QDebug>

qsizetype SPL::PacketData::dataSize()
{
    return sizeof(PacketData::data) + sizeof(PacketData::factor) + sizeof(PacketData::id)
           + sizeof(PacketData::reserve);
}

SPL::checksum_t SPL::Packet::calculateChecksum(const QByteArray &bytes) const
{
    // start from the first byte after header
    const qsizetype startIndex = header.size();
    checksum_t checksum = 0;
    // traverse all bytes as quint8
    auto data = reinterpret_cast<const quint8 *>(bytes.data());
    // loop until footer and sum each byte to checksum
    for (qsizetype i = startIndex;
         i < (bytes.size() - footer.size() - static_cast<qsizetype>(sizeof(checksum_t)));
         i++)
        checksum += data[i];
    return checksum;
}

void SPL::Packet::evaluatePacketData(const QByteArray &bytes)
{
    static const auto packetDataSize = PacketData::dataSize();
    msgCounter = bytes[header.size()];
    idN = bytes.at(header.size() + sizeof(msgcounter_t));
    // extract actual data between IdNumber and checksum
    const QByteArray allData = bytes.mid(header.size() + sizeof(msgcounter_t) + sizeof(idnumber_t),
                                         idN * packetDataSize);
    qDebug() << "msg counter:" << msgCounter;
    // loop through each packet data part
    for (qsizetype i = 0; i < idN * packetDataSize; i += packetDataSize) {
        // get the data of i-th packet
        auto onePacketData = allData.mid(i, packetDataSize);
        // extract id, reserve, data, factor
        id_t id = byteArrayConvert<id_t>(onePacketData.mid(0, sizeof(id_t)));
        reserve_t reserve = byteArrayConvert<reserve_t>(
            onePacketData.mid(sizeof(id_t), sizeof(reserve_t)));
        data_t data = byteArrayConvert<data_t>(
            onePacketData.mid(sizeof(id_t) + sizeof(reserve_t), sizeof(data_t)));
        factor_t factor = byteArrayConvert<factor_t>(
            onePacketData.mid(sizeof(id_t) + sizeof(reserve_t) + sizeof(data_t), sizeof(factor_t)));
        // construct PacketData and push it
        this->allPacketData.push_back(PacketData(data, factor, id, reserve));
    }
}

void SPL::Packet::removeBadData()
{
    auto &allInfo = SPL::getInfo();
    QList<PacketData>::iterator it = allPacketData.begin();
    while (it != allPacketData.end()) {
        auto id = it->getId();
        // if id doesn't exist min and max will be 0
        qreal min = static_cast<qreal>(allInfo[id].minValue);
        qreal max = static_cast<qreal>(allInfo[id].maxValue);
        qreal value = it->getValue();
        // if value is not between min and max, or if the id doesn't exist
        if (value < min || value > max || min == max) {
            it = allPacketData.erase(it);
            --this->idN;
            // errors should have a factor of 1 (based on documentations and meetings)
        } else if (allInfo[id].isError && it->getFactor() > 1.0f) {
            it = allPacketData.erase(it);
            --this->idN;
        } else
            ++it;
    }
}

SPL::msgcounter_t SPL::Packet::getMsgCounter() const
{
    return msgCounter;
}

const QList<SPL::PacketData> &SPL::Packet::getAllPackets() const
{
    return this->allPacketData;
}

SPL::Packet::Packet(const QByteArray &bytes)
{
    auto actualChecksumBytes = bytes.mid(bytes.size() - footer.size() - sizeof(checksum_t),
                                         sizeof(checksum_t));
    checksum_t actualChecksum = byteArrayConvert<checksum_t>(actualChecksumBytes);
    checksum_t calculatedChecksum = this->calculateChecksum(bytes);
    if (actualChecksum != calculatedChecksum)
        BadChecksum().raise();
    evaluatePacketData(bytes);
    removeBadData();
}

SPL::data_t SPL::PacketData::getData() const
{
    return data;
}

SPL::factor_t SPL::PacketData::getFactor() const
{
    return factor;
}

SPL::id_t SPL::PacketData::getId() const
{
    return id;
}

SPL::reserve_t SPL::PacketData::getReserve() const
{
    return reserve;
}

qreal SPL::PacketData::getValue() const
{
    return value;
}

bool SPL::PacketData::operator==(id_t id)
{
    return this->id == id;
}

SPL::PacketData::PacketData(data_t data, factor_t factor, id_t id, reserve_t reserve)
    : data(std::move(data))
    , factor(std::move(factor))
    , id(std::move(id))
    , reserve(std::move(reserve))
{
    // based on documentations and meetings, factor 0 should be evaluated as 1
    if (factor == 0)
        factor = 1;
    value = static_cast<qreal>(data) / static_cast<qreal>(factor);
    qDebug() << __FUNCTION__;
    qDebug() << "id:" << id;
    qDebug() << "reserve:" << reserve;
    qDebug() << "data:" << data;
    qDebug() << "factor:" << factor;
    qDebug() << "VALUE:" << value;
}
