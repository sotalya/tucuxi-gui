include (../tucuxi.pri)

TARGET      = tucuxi-gui-flatimport
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

#Removing the QtGui module (the admin must NOT use any gui element)
QT -= gui


INCLUDEPATH += ..

HEADERS	+= \
    flatimportinterpretationrequestbuilder.h \
    flatimportrequestfileclient.h \
    flatimportrequestsclientprocessing.h

CODECFORTR      += UTF-8
#TRANSLATIONS    += $${TRANS_DIR}/admin_fr.ts

#Translation configuration
include(../translation.pri)

SOURCES += \
    flatimportinterpretationrequestbuilder.cpp \
    flatimportrequestfileclient.cpp \
    flatimportrequestsclientprocessing.cpp

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = tucuxi-gui-core tucuxi-gui-admin tucuxi-gui-apputils tucuxi-giu-rest
for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}
