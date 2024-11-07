#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSharedPointer>
#include <QException>

class EmptyPortName : public QException{
public:


    // QException interface
public:
    void raise() const override
    {
        throw *this;
    }
    QException *clone() const override
    {
        return new MyException(*this);
    }
};

class SerialController : public QObject
{

    Q_OBJECT

    QSharedPointer<QSerialPort> port;

    static QSharedPointer<SerialController> instance;
    explicit SerialController(QObject *parent = nullptr);
    SerialController(const SerialController& other) = delete;
public:
    static SerialController& getInstance();

    bool connect();
    void disconnect();
    QString error();
signals:
};

#endif // SERIALCONTROLLER_H
