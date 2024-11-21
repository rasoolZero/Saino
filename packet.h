#ifndef PACKET_H
#define PACKET_H
#include <QObject>
#include <QException>

class BadChecksum : public QException{
    // QException interface
public:
    void raise() const override
    {
        throw *this;
    }
    QException *clone() const override
    {
        return new BadChecksum(*this);
    }
};


class EmptyPacket : public QException{
    // QException interface
public:
    void raise() const override
    {
        throw *this;
    }
    QException *clone() const override
    {
        return new EmptyPacket(*this);
    }
};


template <typename T>
T byteArrayConvert(const QByteArray &bytes)
{
    QByteArray bytesCopy = bytes;
    #if Q_BYTE_ORDER == Q_BIG_ENDIAN
        std::reverse(bytesCopy.begin(),bytesCopy.end());
    #endif
    auto number = reinterpret_cast<const T *>(bytesCopy.data());
    return *number;
}

typedef quint8 idnumber_t;
typedef quint16 checksum_t;
typedef quint8 msgcounter_t;
typedef quint32 data_t;
typedef quint32 factor_t;
typedef quint8 id_t;
typedef qint8 reserve_t;

class PacketData{
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

class Packet
{
public:
    inline static const QByteArray header = QByteArray::fromHex("A5A5A5A5");
    inline static const QByteArray footer = QByteArray::fromHex("55");
    inline static const qsizetype minimumSize = (header.size() + footer.size() + sizeof(msgcounter_t) + sizeof(idnumber_t) + sizeof(checksum_t) + PacketData::dataSize());

    Packet(const QByteArray& bytes);
    const QList<PacketData> &getAllPackets();
    msgcounter_t getMsgCounter() const;

private:
    checksum_t calculateChecksum(const QByteArray & bytes);
    void evaluatePacketData(const QByteArray& bytes);
    void removeBadData();

    msgcounter_t msgCounter;
    idnumber_t idN;
    QList<PacketData> allPacketData;
};

#endif // PACKET_H
