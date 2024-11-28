TEMPLATE = subdirs

SUBDIRS += \
    SainoPacketLib \
    QXlsx \
    Saino

Saino.depends = QXlsx SainoPacketLib
