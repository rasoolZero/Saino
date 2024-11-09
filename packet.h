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

class PacketData{
    quint32 data;
    quint32 factor;
    quint8 id;
    qint8 reserve;
    public:
    static qsizetype dataSize();
};

class Packet
{
public:
    inline static const QByteArray header = QByteArray::fromHex("A5A5A5A5");
    inline static const QByteArray footer = QByteArray::fromHex("55");
    inline static const qsizetype minimumSize = (header.size() + footer.size() + sizeof(msgcounter_t) + sizeof(idnumber_t) + sizeof(checksum_t) + PacketData::dataSize());

    Packet(const QByteArray& bytes);
private:
    checksum_t calculateChecksum(const QByteArray & bytes);
    msgcounter_t msgCounter;
    idnumber_t idN;
    QList<PacketData> allPacketData;

};

#endif // PACKET_H
