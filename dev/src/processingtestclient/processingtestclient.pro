include (../tucuxi.pri)

TARGET      = ezechiel-processingtestclient
TEMPLATE    = app
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}

#Removing the QtGui module (the maths must NOT use any gui element)
QT -= gui

DEPENDPATH += src
INCLUDEPATH += src ../processing/src ../core/dal ../core/interfaces ../core/utils ../core ../core/doublify ../math/src \
    ../processingserver/src

SOURCES += \
    src/main.cpp \
    ../processingserver/src/processingproxy.cpp \
    ../processingserver/src/processingrequest.cpp \
    ../processingserver/src/processingresponse.cpp

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/processingtestclient_fr.ts

#Translation configuration
include(../translation.pri)

HEADERS += \
    ../processingserver/src/processingproxy.h \
    ../processingserver/src/networkutils.h \
    ../processingserver/src/processingrequest.h \
    ../processingserver/src/processingresponse.h


#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-processing ezechiel-core ezechiel-math
for(dep, DEPENDANCIES) {
    LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}
