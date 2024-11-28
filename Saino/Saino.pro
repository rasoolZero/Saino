QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/gui/config.cpp \
    src/logic/datastorage.cpp \
    src/logic/excelhelper.cpp \
    src/gui/led.cpp \
    src//main.cpp \
    src/gui/mainwindow.cpp \
    src/logic/parser.cpp \
    src/gui/qcgaugewidget.cpp \
    src/serial/serialcontroller.cpp \
    src/serial/serialmanager.cpp \
    src/gui/customgauge.cpp

HEADERS += \
    include/gui/config.h \
    include/logic/datastorage.h \
    include/logic/excelhelper.h \
    include/gui/led.h \
    include/gui/mainwindow.h \
    include/logic/parser.h \
    include/gui/qcgaugewidget.h \
    include/serial/serialcontroller.h \
    include/serial/serialmanager.h \
    include/gui/customgauge.h

INCLUDEPATH += include/

FORMS += \
    form/config.ui \
    form/mainwindow.ui

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
*msvc* { # visual studio spec filter
      QMAKE_CXXFLAGS_DEBUG += -MP
}

RESOURCES += \
    assets.qrc \
    breeze.qrc


RC_ICONS = assets/icon.ico
