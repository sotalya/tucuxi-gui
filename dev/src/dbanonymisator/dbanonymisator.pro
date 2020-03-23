#EzeCHieL configuration
include(../tucuxi.pri)

TARGET   = dbanonymisator
TEMPLATE = app
DESTDIR  = $${BUILD_DIR}/$${DIST_DIR}

QT += gui sql
QT -= xml xmlpatterns network script
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    dbhandler.cpp

HEADERS += \
    mainwindow.h \
    dbhandler.h

FORMS += \
    mainwindow.ui

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/dbanonymisator_fr.ts

#Translation configuration
include(../translation.pri)

#Third party shared libraries
include(../botan.pri)
