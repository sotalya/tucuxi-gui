include (../../../tucuxi.pri)

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = genericlocaldb
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}/plugins

# YTA: To solve issues with namespaces
QMAKE_CXXFLAGS += -fpermissive

QT += sql

PLUGIN_ID=ch.heig-vd.ezechiel.genericrepository

INCLUDEPATH += ../../../core/database \
    ../../../core \
    ../../../core/utils  \
    ../../../core/interfaces

HEADERS	+= genericrepository.h

SOURCES += genericrepository.cpp


CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/$${PLUGIN_ID}_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-core
for(dep, DEPENDANCIES) {
   LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../../../translation.pri)


#Third party libraries headers
include(../../../botan.pri)
