#EzeCHieL configuration
include (../tucuxi.pri)

TARGET      = ezechiel-cliutils
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

#Removing the QtGui module (the core must NOT use any gui element)
# QT -= gui
QT += network

INCLUDEPATH += ..


SOURCES += \
#    climessagehandler.cpp \
    actions/listaction.cpp \
    actions/patientaction.cpp \
    actions/predictionaction.cpp \
    climanager.cpp \
    action.cpp \
    actions/importaction.cpp \
    actions/restaction.cpp \
    actions/addaction.cpp \
    actions/exportaction.cpp \
    actions/exchangeaction.cpp \
    actions/useraction.cpp \
    actions/sniffaction.cpp

HEADERS += action.h \
#    climessagehandler.h \
    actions/listaction.h \
    actions/patientaction.h \
    actions/predictionaction.h \
    climanager.h \
    actions/importaction.h \
    actions/restaction.h \
    actions/addaction.h \
    actions/exportaction.h \
    sphinxmacros.h \
    actions/exchangeaction.h \
    actions/useraction.h \
    errors_cliutils.h \
    actions/sniffaction.h

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/cliutils_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = ezechiel-core ezechiel-admin ezechiel-apputils
!config_demo {
DEPENDENCIES += ezechiel-rest
}

for(dep, DEPENDENCIES) {
   LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../translation.pri)

#Third party libraries headers
#include(../qwt.pri)
include(../botan.pri)
