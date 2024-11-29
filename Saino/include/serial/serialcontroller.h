#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QException>
#include <QMutex>
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

// worker class used in SerialController
// reads and sends received data on a separate thread
class PortReader : public QObject
{
    Q_OBJECT
    const QSharedPointer<QSerialPort> port;

    // mutex for safety on port opening and closing
    QMutex mLock;

public:
    void lock();
    void unlock();
    explicit PortReader(QSharedPointer<QSerialPort> port, QObject *parent = nullptr);

public slots:
    void process();
};

// singleton class, handles multithreaded serial connection
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

    // throws EmptyPortName if no port has been selected
    bool connect();
    bool disconnect();
    QString error();
};

#endif // SERIALCONTROLLER_H
