QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    config.cpp \
    datastorage.cpp \
    excelhelper.cpp \
    led.cpp \
    main.cpp \
    mainwindow.cpp \
    packet.cpp \
    parser.cpp \
    qcgaugewidget.cpp \
    serialcontroller.cpp \
    serialmanager.cpp

HEADERS += \
    config.h \
    datastorage.h \
    excelhelper.h \
    led.h \
    mainwindow.h \
    packet.h \
    parser.h \
    qcgaugewidget.h \
    serialcontroller.h \
    serialmanager.h

FORMS += \
    config.ui \
    mainwindow.ui

# SainoPacketLib
include(../SainoPacketLib/SainoPacketLib.pri)

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=../QXlsx
QXLSX_HEADERPATH=../QXlsx/header/
QXLSX_SOURCEPATH=../QXlsx/source/
include(../QXlsx/QXlsx.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
QMAKE_CXXFLAGS_RELEASE *= /O2
QMAKE_CXXFLAGS_RELEASE *= /GL

RESOURCES += \
    assets.qrc \
    breeze.qrc

*msvc* { # visual studio spec filter
      QMAKE_CXXFLAGS_DEBUG += -MP
}

RC_ICONS = assets/icon.ico
