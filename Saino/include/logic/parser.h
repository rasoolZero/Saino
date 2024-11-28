#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QSharedPointer>
#include <SPL/packet.h>
#include <SPL/parser.h>

class Parser : public SPL::Parser
{
    Q_OBJECT

    static QSharedPointer<Parser> instance;
    explicit Parser(QObject *parent = nullptr);
    Parser(const Parser &other) = delete;
    Parser &operator=(const Parser &other) = delete;

public:
    static Parser &getInstance();
};
#endif // PARSER_H
