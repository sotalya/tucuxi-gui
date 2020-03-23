include (../tucuxi.pri)

TARGET      = ezechiel-processingserver
TEMPLATE    = app
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}

#Removing the QtGui module (the maths must NOT use any gui element)
QT -= gui

DEPENDPATH += src
INCLUDEPATH += src ../processing/src ../core/dal ../core/interfaces ../core/utils ../core ../core/doublify ../math/src

SOURCES += \
    src/main.cpp \
    src/processingproxy.cpp \
    src/processingserver.cpp \
    src/processingclient.cpp \
    src/processingrequest.cpp \
    src/processingresponse.cpp

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/processingserver_fr.ts

#Translation configuration
include(../translation.pri)

HEADERS += \
    src/processingproxy.h \
    src/processingserver.h \
    src/processingclient.h \
    src/networkutils.h \
    src/processingrequest.h \
    src/processingresponse.h


#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-processing ezechiel-core ezechiel-math
for(dep, DEPENDANCIES) {
    LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}
