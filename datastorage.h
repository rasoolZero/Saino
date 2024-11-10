#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include "packet.h"


struct DataInfo{
    bool isError;
    QString name;
    data_t minValue;
    data_t maxValue;
};

class DataStorage : public QObject
{
    Q_OBJECT
    typedef QMap<id_t,DataInfo> infomap;
    static QSharedPointer<DataStorage> instance;
    explicit DataStorage(QObject *parent = nullptr);
    DataStorage(const DataStorage &other) = delete;
    DataStorage& operator=(const DataStorage &other) = delete;
    infomap allInfo;
    void populateInfo();
    QList<Packet> packets;

public:
    inline static const id_t allIDs[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                                               0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
    static DataStorage& getInstance();
    const infomap & getInfo() const;
public slots:
    void newPacket(Packet packet);
signals:
};

#endif // DATASTORAGE_H
