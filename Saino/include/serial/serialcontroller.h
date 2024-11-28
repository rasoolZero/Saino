#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QException>
#include <QObject>
#include <QSerialPort>
#include <QSharedPointer>
#include <QThread>

class EmptyPortName : public QException
{
public:
    // QException interface
public:
    void raise() const override { throw *this; }
    QException *clone() const override { return new EmptyPortName(*this); }
};

class PortReader : public QObject
{
    Q_OBJECT
    const QSharedPointer<QSerialPort> port;

public:
    explicit PortReader(QSharedPointer<QSerialPort> port, QObject *parent = nullptr);

public slots:
    void process();
};

class SerialController : public QObject
{
    Q_OBJECT

    QSharedPointer<QSerialPort> port;

    static QSharedPointer<SerialController> instance;
    explicit SerialController(QObject *parent = nullptr);
    SerialController(const SerialController &other) = delete;
    SerialController &operator=(const SerialController &other) = delete;

    QThread readerThread;
    PortReader reader;

public:
    static SerialController &getInstance();
    ~SerialController();

    bool connect();
    bool disconnect();
    QString error();
};

#endif // SERIALCONTROLLER_H
