include (../../tucuxi.pri)

#Plugin ID
PLUGIN_ID=ch.heig-vd.ezechiel.monte_carlo

#Engine ID
DEFINES += MONTE_CARLO_ID=\\\"$${PLUGIN_ID}\\\"

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = montecarlo
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}/plugins

INCLUDEPATH += ../../core ../../core/interfaces ../../core/utils ../../core/dal

HEADERS	+= montecarlo.h \
    percentilethread.h \
    cholesky.hpp \
    curvegeneratorthread.h

SOURCES += montecarlo.cpp \
    percentilethread.cpp \
    curvegeneratorthread.cpp


CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/$${PLUGIN_ID}_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-core ezechiel-math
for(dep, DEPENDANCIES) {
   LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../../translation.pri)

#Module extra dependencies
include(../../boost.pri)
