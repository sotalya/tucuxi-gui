#EzeCHieL configuration
include(../tucuxi.pri)

CONFIG += console

TARGET      = ezechiel-tcpclient
TEMPLATE    = app
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}

#DEPENDPATH += $${BUILD_DIR}/$${DIST_DIR}
INCLUDEPATH += ../gui src
SOURCES += src/main.cpp src/tcpclient.cpp

HEADERS += src/tcpclient.h
