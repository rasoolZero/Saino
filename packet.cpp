#include "packet.h"



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

Packet::Packet(const QByteArray &bytes)
{
    auto actualChecksumBytes = bytes.mid(bytes.size()-footer.size()-sizeof(checksum_t),sizeof(checksum_t));
    checksum_t actualChecksum = byteArrayConvert<checksum_t>(actualChecksumBytes);
    checksum_t calculatedChecksum = calculateChecksum(bytes);
    if(actualChecksum != calculatedChecksum)
        BadChecksum().raise();

}
