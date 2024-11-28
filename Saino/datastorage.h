#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include <QVector>
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

    enum DataID {
        OIL_PRESSURE = 0x01,
        OIL_TEMPERATURE = 0x02,
        FUEL_FLOW = 0x03,
        FUEL = 0x04,
        EGT = 0x05,
        TORQUE = 0x06,
        INDICATED_POWER = 0x07,
        FRICTIONAL_POWER = 0x08,
        THERMAL_EFFICIENCY = 0x09,
        AIR_FUEL_RATIO = 0x0A,
        MOTOR_SPEED = 0x0B,
        OUTPUT_AIR_SPEED = 0x0C,
        VIBRATION = 0x0D,
        BODY_TEMP = 0x0E,
        AIR_TEMP = 0x0F,
        OIL_PRESSURE_SENSOR_ERROR = 0x11,
        OIL_TEMPERATURE_SENSOR_ERROR = 0x12,
        FUEL_FLOW_SENSOR_ERROR = 0x13,
        FUEL_SENSOR_ERROR = 0x14,
        EGT_SENSOR_ERROR = 0x15,
        TORQUE_SENSOR_ERROR = 0x16,
        INDICATED_POWER_SENSOR_ERROR = 0x17,
        FRICTIONAL_POWER_SENSOR_ERROR = 0x18,
        THERMAL_EFFICIENCY_SENSOR_ERROR = 0x19,
        AIR_FUEL_RATIO_SENSOR_ERROR = 0x1A,
        MOTOR_SPEED_SENSOR_ERROR = 0x1B,
        OUTPUT_AIR_SPEED_SENSOR_ERROR = 0x1C,
        VIBRATION_SENSOR_ERROR = 0x1D,
        BODY_TEMP_SENSOR_ERROR = 0x1E,
        AIR_TEMP_SENSOR_ERROR = 0x1F
    };

    static DataStorage& getInstance();
    const infomap & getInfo() const;
    QVector<DataID> allErrorCodes() const;
    QVector<DataID> allDataCodes() const;
    const QList<Packet> & getPackets();
    void reset();
public slots:
    void newPacket(Packet packet);
signals:
};

#endif // DATASTORAGE_H
