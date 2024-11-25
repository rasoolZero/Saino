#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QSharedPointer>
#include "packet.h"

class Parser : public QObject
{
    Q_OBJECT

    static QSharedPointer<Parser> instance;
    explicit Parser(QObject *parent = nullptr);
    Parser(const Parser &other) = delete;
    Parser& operator=(const Parser &other) = delete;

    bool isMsgcounterValid(msgcounter_t newMsgCounter);

    msgcounter_t msgCounter = 0;
    QByteArray totalBytes;
    const qsizetype maxSize = 20000;
public:
    static Parser& getInstance();
    void reset();
    void parseData(QByteArray data);

signals:
    void packetGenerated(Packet packet);
};
#endif // PARSER_H
