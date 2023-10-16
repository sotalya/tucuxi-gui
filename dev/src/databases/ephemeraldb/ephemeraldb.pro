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
        LIBS += C:\xerces-c\lib\xerces-c_3D.lib
    }
    CONFIG(release, debug|release) {
        LIBS += C:\xerces-c\lib\xerces-c_3.lib
    }
}
unix{
    LIBS+=/home/ythoma/Documents/xerces-c-3.2.4/src/.libs/libxerces-c.a
}

#Translation configuration
include(../../translation.pri)

