TEMPLATE = subdirs

SUBDIRS += \
    SainoPacketLib \
    Saino

Saino.depends = QXlsx SainoPacketLib
