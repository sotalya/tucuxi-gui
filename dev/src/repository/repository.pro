include (../tucuxi.pri)

TARGET      = ezechiel-repository
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}

# YTA: To solve issues with namespaces
# QMAKE_CXXFLAGS += -fpermissive

#Removing the QtGui module (the repository must NOT use any gui element)
QT -= gui

INCLUDEPATH += ../core ../core/utils ../core/interfaces \
../core/dal ../admin/src/dal ../admin/src ../apputils/src \
../report

HEADERS +=  \
    src/dbplugin.h \
    src/genericdatabase.h

SOURCES += \
    src/dbplugin.cpp

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/repository_fr.ts

#Translation configuration
include(../translation.pri)

#Third party libraries headers
include(../botan.pri)

