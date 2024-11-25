#include "serialcontroller.h"
#include "serialmanager.h"
#include <QException>
#include "parser.h"

SerialController::SerialController(QObject *parent)
    : QObject{parent}
    , port{QSharedPointer<QSerialPort>::create(this)}
    , reader{port}
{
    QObject::connect(&readerThread, SIGNAL(started()), &reader, SLOT(process()));
    reader.moveToThread(&readerThread);
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
    if(port->isOpen())
        readerThread.start();
    return port->isOpen();
}

bool SerialController::disconnect()
{
    if(port->isOpen()){
        port->close();
        readerThread.quit();
    }
    else
        return false;
    return true;
}

QString SerialController::error()
{
    return port->errorString();
}

QSharedPointer<SerialController> SerialController::instance;

PortReader::PortReader(QSharedPointer<QSerialPort> port, QObject *parent):
    QObject{parent},
    port{port}
{
    QObject::connect(this,&PortReader::dataRecieved,&Parser::getInstance(),&Parser::parseData);
}

void PortReader::process()
{
    while(port->isOpen()){
        if (port->waitForReadyRead(5)) {
            emit dataRecieved(port->readAll());
        }
    }
}
