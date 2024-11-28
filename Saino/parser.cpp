#include "parser.h"
#include <QDebug>
#include <QMetaType>
#include "datastorage.h"
#include <SPL/packet.h>

QSharedPointer<Parser> Parser::instance = nullptr;

Parser& Parser::getInstance()
{
    if (!instance)
        instance = QSharedPointer<Parser>(new Parser);
    return *instance;
}

Parser::Parser(QObject *parent)
    : SPL::Parser(parent)
{
    connect(this,&Parser::packetGenerated,&DataStorage::getInstance(),&DataStorage::newPacket);
}
