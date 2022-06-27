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

#Translation configuration
include(../../translation.pri)

