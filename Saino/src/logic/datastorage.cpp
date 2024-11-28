#include "logic/DataStorage.h"

QSharedPointer<DataStorage> DataStorage::instance = nullptr;

DataStorage &DataStorage::getInstance()
{
    if (!instance)
        instance = QSharedPointer<DataStorage>(new DataStorage);
    return *instance;
}

const QList<SPL::Packet> &DataStorage::getPackets()
{
    return this->packets;
}

void DataStorage::reset()
{
    this->packets.clear();
}

void DataStorage::newPacket(SPL::Packet packet)
{
    packets.push_back(packet);
}

DataStorage::DataStorage(QObject *parent)
    : QObject(parent)
{}
