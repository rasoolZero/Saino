#include "serialcontroller.h"
#include "serialmanager.h"
#include <QException>
#include "parser.h"

SerialController::SerialController(QObject *parent)
    : QObject{parent}
    , port{QSharedPointer<QSerialPort>::create(this)}
{
    QObject::connect(port.get(),&QSerialPort::readyRead,this,&SerialController::dataReady);
    QObject::connect(this,&SerialController::dataRecieved,&Parser::getInstance(),&Parser::parseData);
}

SerialController &SerialController::getInstance()
{
    if(!instance)
        instance = QSharedPointer<SerialController>(new SerialController);
    return *instance;
}

bool SerialController::connect()
{
    auto& manager = SerialManager::getInstance();
    if(manager.getPort() == ""){
        EmptyPortName().raise();
    }
    port->close();
    port->setBaudRate(manager.getRate());
    port->setPortName(manager.getPort());
    port->setParity(manager.getParity());
    port->setStopBits(manager.getStopbit());
    port->open(QIODevice::ReadOnly);
    return port->isOpen();
}

bool SerialController::disconnect()
{
    if(port->isOpen())
        port->close();
    else
        return false;
    return true;
}

QString SerialController::error()
{
    return port->errorString();
}

void SerialController::dataReady()
{
    if(port->isOpen())
        emit dataRecieved(port->readAll());
}

QSharedPointer<SerialController> SerialController::instance;
