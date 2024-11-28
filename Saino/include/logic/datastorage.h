#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include <SPL/packet.h>

class DataStorage : public QObject
{
    Q_OBJECT
    static QSharedPointer<DataStorage> instance;
    explicit DataStorage(QObject *parent = nullptr);
    DataStorage(const DataStorage &other) = delete;
    DataStorage &operator=(const DataStorage &other) = delete;
    QList<SPL::Packet> packets;

public:
    static DataStorage &getInstance();
    const QList<SPL::Packet> &getPackets();
    void reset();
public slots:
    void newPacket(SPL::Packet packet);
signals:
};

#endif // DATASTORAGE_H
