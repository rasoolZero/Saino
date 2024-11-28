#include "serial/serialmanager.h"

QSerialPort::Parity SerialManager::getParity() const
{
    return parity;
}

void SerialManager::setParity(const QSerialPort::Parity &newParity)
{
    if (parity == newParity)
        return;
    parity = newParity;
}

void SerialManager::setPort(const QString &newPort)
{
    port = newPort;
}

QSerialPort::StopBits SerialManager::getStopbit() const
{
    return stopbit;
}

void SerialManager::setStopbit(const QSerialPort::StopBits &newStopbit)
{
    stopbit = newStopbit;
}

qint32 SerialManager::getRate() const
{
    return rate;
}

void SerialManager::setRate(qint32 newRate)
{
    rate = newRate;
}

QString SerialManager::getPort() const
{
    return port;
}

SerialManager::SerialManager(QObject *parent)
    : QObject{parent}
{}

SerialManager &SerialManager::getInstance()
{
    if (!instance)
        instance = QSharedPointer<SerialManager>(new SerialManager);
    return *instance;
}

QList<QSerialPortInfo> SerialManager::getPorts()
{
    return QSerialPortInfo::availablePorts();
}

QList<qint32> SerialManager::getBaudRates()
{
    return QSerialPortInfo::standardBaudRates();
}

const QList<QSerialPort::Parity> &SerialManager::getParities()
{
    return parities;
}

const QList<QSerialPort::StopBits> &SerialManager::getStopbits()
{
    return stopbits;
}

QSharedPointer<SerialManager> SerialManager::instance;
QList<QSerialPort::Parity> SerialManager::parities = {QSerialPort::NoParity,
                                                      QSerialPort::EvenParity,
                                                      QSerialPort::OddParity,
                                                      QSerialPort::SpaceParity,
                                                      QSerialPort::MarkParity};
QList<QSerialPort::StopBits> SerialManager::stopbits = {QSerialPort::OneStop,
                                                        QSerialPort::OneAndHalfStop,
                                                        QSerialPort::TwoStop};
