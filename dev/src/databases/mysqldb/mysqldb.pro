include (../../tucuxi.pri)

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = mysqldb
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}/dbs

QT += sql

INCLUDEPATH += ../../core/database \
    ../../core \
    ../../core/utils  \
    ../../core/interfaces

HEADERS	+= mysqldb.h

SOURCES += mysqldb.cpp


CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/mysqldb_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-core
for(dep, DEPENDANCIES) {
   LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../../translation.pri)

#Third party libraries headers
include(../../botan.pri)
