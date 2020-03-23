include (../tucuxi.pri)

QT += core gui xml xmlpatterns network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
!win32 {
QT += webengine webenginewidgets
}

TARGET   = ezechiel-restclient
TEMPLATE = app
DESTDIR  = $${OUT_PWD}/../$${DIST_DIR}/rest

DEPENDPATH  += gui model utils ..
INCLUDEPATH += ..

SOURCES += \
    main.cpp \
    gui/mainwindow.cpp \
    gui/requesttestwidget.cpp \
    gui/abstracttestwidget.cpp \
    gui/listtestwidget.cpp \
    gui/ackrequesttestwidget.cpp \
    gui/notificationtestwidget.cpp \
    model/testmessagevalidator.cpp \
    model/querylisttestmessage.cpp \
    model/queryrequesttestmessage.cpp \
    model/replylisttestmessage.cpp \
    model/replyrequesttestmessage.cpp \
    model/notificationtestmessage.cpp \
    model/acknotificationtestmessage.cpp \
    utils/xmltestlibrary.cpp \
    utils/meldworkerthread.cpp \
    utils/settings.cpp

HEADERS += \
    gui/mainwindow.h \
    gui/requesttestwidget.h \
    gui/abstracttestwidget.h \
    gui/listtestwidget.h \
    gui/ackrequesttestwidget.h \
    gui/notificationtestwidget.h \
    model/testmessagevalidator.h \
    model/querylisttestmessage.h \
    model/queryrequesttestmessage.h \
    model/replylisttestmessage.h \
    model/replyrequesttestmessage.h \
    model/notificationtestmessage.h \
    model/acknotificationtestmessage.h \
    utils/utils.h \
    utils/xmltestlibrary.h \
    utils/meldworkerthread.h \
    utils/settings.h \
    errors_restclient.h

FORMS += \
    gui/mainwindow.ui \
    gui/requesttestwidget.ui \
    gui/listtestwidget.ui

RESOURCES += \
    resources/restclient.qrc

CODECFORTR   += UTF-8
TRANSLATIONS += $${TRANS_DIR}/restclient_fr.ts

DEPENDENCIES = ezechiel-rest ezechiel-core
for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}
