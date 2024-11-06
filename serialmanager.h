#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSharedPointer>
#include <QMetaEnum>

class SerialManager : public QObject
{
    Q_OBJECT

    QSerialPort::Parity parity = QSerialPort::Parity::OddParity;
    QSerialPort::StopBits stopbit = QSerialPort::StopBits::OneStop;
    qint32 rate = QSerialPort::Baud115200;
    QString port;

    static QSharedPointer<SerialManager> instance;
    static QList<QSerialPort::Parity> parities;
    static QList<QSerialPort::StopBits> stopbits;
    explicit SerialManager(QObject *parent = nullptr);
    SerialManager(const SerialManager &) = delete;

public:
    static SerialManager& getInstance();
    static QList<QSerialPortInfo> getPorts();
    static QList<qint32> getBaudRates();
    static const QList<QSerialPort::Parity>& getParities();
    static const QList<QSerialPort::StopBits>& getStopbits();


    QSerialPort::Parity getParity() const;
    void setParity(const QSerialPort::Parity &newParity);

    void setPort(const QString &newPort);

    QSerialPort::StopBits getStopbit() const;
    void setStopbit(const QSerialPort::StopBits &newStopbit);

    qint32 getRate() const;
    void setRate(qint32 newRate);

signals:

public slots:

};
template<typename QEnum>
QString QtEnumToString (const QEnum value)
{
    return QString(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

#endif // SERIALMANAGER_H
