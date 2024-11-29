#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include <QObject>
#include <QSharedPointer>
#include "common.h"
#include "packet.h"

namespace SPL {
class Parser : public QObject
{
    Q_OBJECT

    bool isMsgcounterValid(msgcounter_t newMsgCounter) const;

    msgcounter_t msgCounter = 0;
    QByteArray totalBytes;
    const qsizetype maxSize = 20000;

public:
    explicit Parser(QObject *parent = nullptr);
    // sets msgCounter to 0 and clears totalBytes
    void reset();
    void parseData(const QByteArray &data);
    virtual ~Parser() = default;
signals:
    void packetGenerated(Packet packet);
};
} // namespace SPL
#endif // SPL_PARSER_H
