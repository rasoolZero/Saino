#include "DataStorage.h"

QSharedPointer<DataStorage> DataStorage::instance = nullptr;

DataStorage& DataStorage::getInstance()
{
    if (!instance)
        instance = QSharedPointer<DataStorage>(new DataStorage);
    return *instance;
}

const DataStorage::infomap &DataStorage::getInfo() const
{
    return this->allInfo;
}

void DataStorage::newPacket(Packet packet)
{
    packets.push_back(packet);
}

DataStorage::DataStorage(QObject *parent)
    : QObject(parent)
{
    populateInfo();
}

void DataStorage::populateInfo()
{
    allInfo[0x01] = {false, "OIL PRESSURE", 0, 1000};
    allInfo[0x02] = {false, "OIL TEMPERATURE", 0, 400};
    allInfo[0x03] = {false, "FUEL FLOW", 0, 800};
    allInfo[0x04] = {false, "FUEL", 0, 800};
    allInfo[0x05] = {false, "EGT", 0, 400};
    allInfo[0x06] = {false, "TORQUE", 0, 400};
    allInfo[0x07] = {false, "INDICATED POWER", 0, 400};
    allInfo[0x08] = {false, "Frictional Power", 0, 400};
    allInfo[0x09] = {false, "Thermal efficiency", 0, 100};
    allInfo[0x0A] = {false, "Air-Fuel ratio", 0, 20};
    allInfo[0x0B] = {false, "MOTOR SPEED", 0, 1000};
    allInfo[0x0C] = {false, "OUTPUT AIR SPEED", 0, 1000};
    allInfo[0x0D] = {false, "VIBRATION", 0, 100};
    allInfo[0x0E] = {false, "BODY TEMP", 0, 400};
    allInfo[0x0F] = {false, "AIR TEMP", 0, 400};
    allInfo[0x11] = {true, "OIL PRESSURE SENSOR ERROR", 0, 1};
    allInfo[0x12] = {true, "OIL TEMPERATURE SENSOR ERROR", 0, 1};
    allInfo[0x13] = {true, "FUEL FLOW SENSOR ERROR", 0, 1};
    allInfo[0x14] = {true, "FUEL SENSOR ERROR", 0, 1};
    allInfo[0x15] = {true, "EGT SENSOR ERROR", 0, 1};
    allInfo[0x16] = {true, "TORQUE SENSOR ERROR", 0, 1};
    allInfo[0x17] = {true, "INDICATED POWER SENSOR ERROR", 0, 1};
    allInfo[0x18] = {true, "Frictional Power SENSOR ERROR", 0, 1};
    allInfo[0x19] = {true, "Thermal efficiency SENSOR ERROR", 0, 1};
    allInfo[0x1A] = {true, "Air-Fuel ratio SENSOR ERROR", 0, 1};
    allInfo[0x1B] = {true, "MOTOR SPEED SENSOR ERROR", 0, 1};
    allInfo[0x1C] = {true, "OUTPUT AIR SPEED SENSOR ERROR", 0, 1};
    allInfo[0x1D] = {true, "VIBRATION SENSOR ERROR", 0, 1};
    allInfo[0x1E] = {true, "BODY TEMP SENSOR ERROR", 0, 1};
    allInfo[0x1F] = {true, "AIR TEMP SENSOR ERROR", 0, 1};
}
