QT -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common.cpp \
    packet.cpp \
    parser.cpp

HEADERS += \
    include/SPL/common.h \
    include/SPL/packet.h \
    include/SPL/parser.h

INCLUDEPATH += include/SPL

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
QMAKE_CXXFLAGS_RELEASE *= /O2
QMAKE_CXXFLAGS_RELEASE *= /GL
*msvc* { # visual studio spec filter
      QMAKE_CXXFLAGS_DEBUG += -MP
}


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    SainoPacketLib.pri
