#include "serialcontroller.h"
#include "serialmanager.h"
#include "QException"

SerialController::SerialController(QObject *parent)
    : QObject{parent}
    , port{QSharedPointer<QSerialPort>::create(this)}
{
}

SerialController &SerialController::getInstance()
{
    if(!instance)
        instance = QSharedPointer<SerialController>(new SerialController);
    return *instance;
}

bool SerialController::connect()
{
    if(manager.getPort() == ""){
        EmptyPortName().raise();
    }
    port->close();
    auto& manager = SerialManager::getInstance();
    port->setBaudRate(manager.getRate());
    port->setPortName(manager.getPort());
    port->setParity(manager.getParity());
    port->setStopBits(manager.getStopbit());
    port->open(QIODevice::ReadOnly);
    return port->isOpen();
}

void SerialController::disconnect()
{
    port->close();
}

QString SerialController::error()
{
    return port->errorString();
}

QSharedPointer<SerialController> SerialController::instance;
