#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QMetaEnum>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSharedPointer>

class SerialManager : public QObject
{
public:
    using Parity = QSerialPort::Parity;
    using StopBits = QSerialPort::StopBits;
    using BaudRate = QSerialPort::BaudRate;

private:
    Q_OBJECT

    Parity parity = Parity::OddParity;
    StopBits stopbit = StopBits::OneStop;
    qint32 rate = BaudRate::Baud115200;
    QString port;

    static QSharedPointer<SerialManager> instance;
    static QList<Parity> parities;
    static QList<StopBits> stopbits;
    explicit SerialManager(QObject *parent = nullptr);
    SerialManager(const SerialManager &) = delete;
    SerialManager &operator=(const SerialManager &other) = delete;

public:
    static SerialManager &getInstance();
    static QList<QSerialPortInfo> getPorts();
    static QList<qint32> getBaudRates();
    static const QList<Parity> &getParities();
    static const QList<StopBits> &getStopbits();

    Parity getParity() const;
    void setParity(const Parity &newParity);

    void setPort(const QString &newPort);

    StopBits getStopbit() const;
    void setStopbit(const StopBits &newStopbit);

    qint32 getRate() const;
    void setRate(qint32 newRate);

    QString getPort() const;

signals:

public slots:
};
template<typename QEnum>
QString QtEnumToString(const QEnum value)
{
    return QString(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

#endif // SERIALMANAGER_H
