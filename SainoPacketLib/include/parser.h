#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QSharedPointer>
#include "common.h"
#include "packet.h"

namespace SainoPacket {
class Parser : public QObject
{
    Q_OBJECT

    bool isMsgcounterValid(msgcounter_t newMsgCounter);

    msgcounter_t msgCounter = 0;
    QByteArray totalBytes;
    const qsizetype maxSize = 20000;

public:
    explicit Parser(QObject *parent = nullptr);
    void reset();
    void parseData(QByteArray data);
    virtual ~Parser() = default;
signals:
    void packetGenerated(Packet packet);
};
} // namespace SainoPacket
#endif // PARSER_H
