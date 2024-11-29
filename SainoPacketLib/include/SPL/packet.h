#ifndef SPL_PACKET_H
#define SPL_PACKET_H
#include <QException>
#include <QMetaType>
#include <QObject>
#include "common.h"

namespace SPL {
class BadChecksum : public QException
{
    // QException interface
public:
    void raise() const override { throw *this; }
    QException *clone() const override { return new BadChecksum(*this); }
};

// this function is used to convert byte arrays to other types
// if the running system is big-endian, the bytes are reversed
// since the dara received is in little-endian
template<typename T>
T byteArrayConvert(const QByteArray &bytes)
{
    QByteArray bytesCopy = bytes;
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    std::reverse(bytesCopy.begin(), bytesCopy.end());
#endif
    auto number = reinterpret_cast<const T *>(bytesCopy.data());
    return *number;
}

// holds a single data field in a packet, contains data, factor, id and reserve
// value is data/factor, if factor is 0, it'll be considered 1
class PacketData
{
    data_t data;
    factor_t factor;
    id_t id;
    reserve_t reserve;
    qreal value;

public:
    PacketData(data_t data, factor_t factor, id_t id, reserve_t reserve);

    static qsizetype dataSize();
    data_t getData() const;
    factor_t getFactor() const;
    id_t getId() const;
    reserve_t getReserve() const;
    qreal getValue() const;

    bool operator==(id_t id);
};

// holds data for a full packet, containing MsgCounter, id number and packet data
class Packet
{
public:
    inline static const QByteArray header = QByteArray::fromHex("A5A5A5A5");
    inline static const QByteArray footer = QByteArray::fromHex("55");
    inline static const qsizetype minimumSize = (header.size() + footer.size()
                                                 + sizeof(msgcounter_t) + sizeof(idnumber_t)
                                                 + sizeof(checksum_t) + PacketData::dataSize());

    // can throw BadChecksum
    Packet(const QByteArray &bytes = {});
    const QList<PacketData> &getAllPackets() const;
    msgcounter_t getMsgCounter() const;

private:
    checksum_t calculateChecksum(const QByteArray &bytes) const;
    void evaluatePacketData(const QByteArray &bytes);
    void removeBadData();

    msgcounter_t msgCounter;
    idnumber_t idN;
    QList<PacketData> allPacketData;
};

} // namespace SPL

// packet might be used in signal/slots between threads
Q_DECLARE_METATYPE(SPL::Packet)

#endif // SPL_PACKET_H
