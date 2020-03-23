include (../QtWebApp/QtWebApp/httpserver/httpserver.pri)

QT += core xml xmlpatterns network
QT -= gui
!win32 {
QT += webengine webenginewidgets
}

CONFIG += console c++11
CONFIG -= app_bundle

TARGET   = ezechiel-processingrestserver
TEMPLATE = app
# DESTDIR  = $${OUT_PWD}/../$${DIST_DIR}/rest

DEPENDPATH  += network utils ..
INCLUDEPATH += ..

SOURCES += \
    main.cpp \
    utils/settings.cpp \
    network/requestmapper.cpp \
    network/querymapper.cpp \
    network/errorhandler.cpp \
    network/processingrequesthandler.cpp

HEADERS += \
    utils/settings.h \
    network/requestmapper.h \
    network/querymapper.h \
    network/errorhandler.h \
    network/processingrequesthandler.h

OTHER_FILES += \
    files/etc/webapp.ini

CODECFORTR   += UTF-8
TRANSLATIONS += $${TRANS_DIR}/restserver_fr.ts


config.path  = $$OUT_PWD/etc/
config.files = files/etc/webapp.ini

INSTALLS += config
