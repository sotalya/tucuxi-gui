include (../../tucuxi.pri)

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = genericlocaldb
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}/dbs

# YTA: To solve issues with namespaces
QMAKE_CXXFLAGS += -fpermissive

QT += sql

PLUGIN_ID=ch.heig-vd.ezechiel.genericlocaldb

INCLUDEPATH += ../../core/database \
    ../../core \
    ../../core/dal \
    ../../core/utils \
    ../../core/interfaces \
    ../../apputils/src

HEADERS	+= genericlocaldb.h

SOURCES += genericlocaldb.cpp


CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/$${PLUGIN_ID}_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
#DEPENDANCIES = ezechiel-core ezechiel-math
DEPENDANCIES = ezechiel-core ezechiel-apputils ezechiel-repository ezechiel-math ezechiel-report
for(dep, DEPENDANCIES) {
   LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../../translation.pri)

#Install the localdb database
db.path     =  $${BUILD_DIR}/$${DIST_DIR}/data/ch.heig-vd.ezechiel.genericlocaldb
db.files    =  genericlocal.db
INSTALLS    += db

#Third party libraries headers
include(../../botan.pri)
