include (../../tucuxi.pri)

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = localdb
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}/dbs

QT += sql

INCLUDEPATH += \
    ../../core \
    ../../core/database \
    ../../core/utils  \
    ../../core/dal \
    ../../core/interfaces \
    ../../report \
    ../../repository/src \
    ../../apputils/src

HEADERS	+= localdb.h

SOURCES += localdb.cpp


CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/localdb_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-apputils ezechiel-repository ezechiel-core
for(dep, DEPENDANCIES) {
   LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../../translation.pri)

#Install the localdb database
db.path     =  $${BUILD_DIR}/$${DIST_DIR}/data/ch.heig-vd.ezechiel.localdb
db.files    =  local.db
INSTALLS    += db

#Third party libraries headers
include(../../botan.pri)
