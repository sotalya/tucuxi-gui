include (../../tucuxi.pri)

#Normally included in the pluging option, the dll option must be specified if the static option was specified
#CONFIG += console
CONFIG += plugin dll

TARGET      = ephemeraldb
#TEMPLATE    = app
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../../$${DIST_DIR}/dbs
#QT += sql

INCLUDEPATH += ../..


HEADERS	+= ephemeraldb.h #\
#    proto/db.pb.h \
#    server.h

SOURCES += ephemeraldb.cpp
#        proto/db.pb.cc \
#        main.cpp


CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/ephemeraldb_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = ezechiel-apputils ezechiel-admin ezechiel-core
#ezechiel-core ezechiel-report #ezechiel-repository
for(dep, DEPENDENCIES) {
    LIBNAME = $${DESTDIR}/../$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../../translation.pri)

#include(../../zmq.pri)

