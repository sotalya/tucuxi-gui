include (../../tucuxi.pri)

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = ephemeraldb
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../../$${DIST_DIR}/dbs

INCLUDEPATH += ../..


HEADERS	+= ephemeraldb.h

SOURCES += ephemeraldb.cpp


CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/ephemeraldb_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = tucuxi-gui-apputils tucuxi-gui-admin tucuxi-gui-core
for(dep, DEPENDENCIES) {
    LIBNAME = $${DESTDIR}/../$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}

win32{
    CONFIG(debug, debug|release) {
        LIBS += $$PWD/../../../libs/xerces-c/build/lib/xerces-c_3D.lib
    }
    CONFIG(release, debug|release) {
        LIBS += $$PWD/../../../libs/xerces-c/build/lib/xerces-c_3.lib
    }
}
unix{
    LIBS+=$$PWD/../../../libs/xerces-c/build/src/libxerces-c.a
    #LIBS+=$$PWD/../../../libs/xerces-c/src/.libs/libxerces-c.a
#    INCLUDEPATH+=../../../libs/xerces-c/build/src
}

#Translation configuration
include(../../translation.pri)

