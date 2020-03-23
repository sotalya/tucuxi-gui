include (../tucuxi.pri)
include (../QtWebApp/QtWebApp/httpserver/httpserver.pri)

QT += core xml xmlpatterns network
QT -= gui
!win32 {
QT += webengine webenginewidgets
}

CONFIG += console c++11
CONFIG -= app_bundle

TARGET   = ezechiel-restserver
TEMPLATE = app
DESTDIR  = $${OUT_PWD}/../$${DIST_DIR}/rest

DEPENDPATH  += network utils ..
INCLUDEPATH += ..

SOURCES += \
    main.cpp \
    utils/settings.cpp \
    network/requestmapper.cpp \
    network/querymapper.cpp \
    network/notifymapper.cpp \
    network/querylisthandler.cpp \
    network/acklisthandler.cpp \
    network/ackrequesthandler.cpp \
    network/queryrequesthandler.cpp \
    network/errorhandler.cpp \
    network/notificationhandler.cpp

HEADERS += \
    utils/settings.h \
    network/requestmapper.h \
    network/querymapper.h \
    network/notifymapper.h \
    network/querylisthandler.h \
    network/acklisthandler.h \
    network/ackrequesthandler.h \
    network/queryrequesthandler.h \
    network/errorhandler.h \
    network/notificationhandler.h

OTHER_FILES += \
    files/etc/webapp.ini

CODECFORTR   += UTF-8
TRANSLATIONS += $${TRANS_DIR}/restserver_fr.ts

DEPENDENCIES = ezechiel-rest ezechiel-core
for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}

config.path  = $${DESTDIR}/etc/
config.files = files/etc/webapp.ini

example.path  = $${DESTDIR}/example/
example.files = files/example/reply_request.xml

INSTALLS += config example
