#include "serial/serialcontroller.h"
#include <QDebug>
#include <QException>
#include "logic/parser.h"
#include "serial/serialmanager.h"

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
    if (!instance)
        instance = QSharedPointer<SerialController>(new SerialController);
    return *instance;
}

SerialController::~SerialController()
{
    this->disconnect();
    this->readerThread.wait();
    this->readerThread.quit();
}

bool SerialController::connect()
{
    auto &manager = SerialManager::getInstance();
    if (manager.getPort() == "") {
        EmptyPortName().raise();
    }
    port->close();
    port->setBaudRate(manager.getRate());
    port->setPortName(manager.getPort());
    port->setParity(manager.getParity());
    port->setStopBits(manager.getStopbit());
    port->open(QIODevice::ReadOnly);
    if (port->isOpen())
        readerThread.start();
    return port->isOpen();
}

bool SerialController::disconnect()
{
    if (port->isOpen()) {
        reader.lock();
        port->close();
        reader.unlock();
        readerThread.quit();

    } else
        return false;
    return true;
}

QString SerialController::error()
{
    return port->errorString();
}

QSharedPointer<SerialController> SerialController::instance;

void PortReader::lock()
{
    this->mLock.lock();
}

void PortReader::unlock()
{
    this->mLock.unlock();
}

PortReader::PortReader(QSharedPointer<QSerialPort> port, QObject *parent)
    : QObject{parent}
    , port{port}
{}

void PortReader::process()
{
    auto &instance = Parser::getInstance();
#ifdef QT_DEBUG
    int counter = 1;
#endif
    while (port->isOpen()) {
        lock();
        if (port->waitForReadyRead(5)) {
            instance.parseData(port->readAll());
#ifdef QT_DEBUG
            qDebug() << "COUNTER:" << counter++;
#endif
        }
        unlock();
    }
}
