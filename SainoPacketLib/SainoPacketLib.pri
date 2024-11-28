LIBTARGET = SainoPacketLib
BASEDIR   = $${PWD}
INCLUDEPATH *= $${BASEDIR}/include
LIBNAME = SainoPacketLib

win32 {
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/debug -L$$OUT_PWD/../$${LIBNAME}/debug
        !isEmpty(DESTDIR) : LIBS += -L$${DESTDIR} -L$${DESTDIR}/$${LIBNAME}/debug
    } else {
        LIBS += -L$$PWD/release -L$$OUT_PWD/../$${LIBNAME}/release
        !isEmpty(DESTDIR) : LIBS += -L$${DESTDIR} -L$${DESTDIR}/$${LIBNAME}/release
    }
} else {
    LIBS += -L$$PWD -L$$OUT_PWD/../$${LIBNAME}
}



LIBS += -l$${LIBNAME}

