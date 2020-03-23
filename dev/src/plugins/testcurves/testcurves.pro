include (../../tucuxi.pri)

#Plugin ID
PLUGIN_ID=ch.heig-vd.ezechiel.tests

#Models ID
DEFINES += TESTCURVES_ID=\\\"$${PLUGIN_ID}\\\" \
           TEST_CONSTANT_ID=\\\"$${PLUGIN_ID}.constant\\\" \
           TEST_PROPORTIONAL_ID=\\\"$${PLUGIN_ID}.proportional\\\" \
           TEST_CONSTANT_SUM_ID=\\\"$${PLUGIN_ID}.constantsum\\\" \

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = testcurves
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}/plugins

INCLUDEPATH += ../../core ../../core/interfaces ../../core/utils ../../core/dal

HEADERS	+= testcurves.h

SOURCES += testcurves.cpp

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
