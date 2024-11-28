TEMPLATE = subdirs

SUBDIRS += \
    SainoPacketLib \
    Saino

Saino.depends = SainoPacketLib
