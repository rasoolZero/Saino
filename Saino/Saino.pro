QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SRC = src/
SRC_GUI = $${SRC}/gui
SRC_LOGIC = $${SRC}/logic
SRC_SERIAL = $${SRC}/serial

INCLUDE = include/
INCLUDE_GUI = $${INCLUDE}/gui
INCLUDE_LOGIC = $${INCLUDE}/logic
INCLUDE_SERIAL = $${INCLUDE}/serial

SOURCES += \
    $${SRC_GUI}/config.cpp \
    $${SRC_LOGIC}/datastorage.cpp \
    $${SRC_LOGIC}/excelhelper.cpp \
    $${SRC_GUI}/led.cpp \
    $${SRC}/main.cpp \
    $${SRC_GUI}/mainwindow.cpp \
    $${SRC_LOGIC}/parser.cpp \
    $${SRC_GUI}/qcgaugewidget.cpp \
    $${SRC_SERIAL}/serialcontroller.cpp \
    $${SRC_SERIAL}/serialmanager.cpp \
    src/gui/customgauge.cpp

HEADERS += \
    $${INCLUDE_GUI}/config.h \
    $${INCLUDE_LOGIC}/datastorage.h \
    $${INCLUDE_LOGIC}/excelhelper.h \
    $${INCLUDE_GUI}/led.h \
    $${INCLUDE_GUI}/mainwindow.h \
    $${INCLUDE_LOGIC}/parser.h \
    $${INCLUDE_GUI}/qcgaugewidget.h \
    $${INCLUDE_SERIAL}/serialcontroller.h \
    $${INCLUDE_SERIAL}/serialmanager.h \
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
