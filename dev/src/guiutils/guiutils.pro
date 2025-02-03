
include (../tucuxi.pri)

TARGET      = tucuxi-gui-guiutils
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}


CONFIG(debug) {
    CONFIG += qml_debug
}

QT += gui printsupport quick quickwidgets printsupport svg network webchannel websockets # webenginecore webenginewidgets

!config_internalreport {
    #Install the report files
    unix {
        reportsource = $$PWD/reports
        reportpath    =  $${DESTDIR}
    }
    win32 {
        reportsource = $$PWD/reports
        reportpath    =  $${DESTDIR}/reports
    }

    macx {
        reportpath = $${DESTDIR}/$${TARGET}.app/Contents/Resources
    }
    android {
        reportpath = /assets
    }
    copydata.commands = $$quote($(COPY_DIR) $${reportsource} $${reportpath}$$escape_expand(\n\t))
    win32 {
        reportpath ~= s,/,\\,g
        reportsource ~= s,/,\\,g
        copydata.commands = $(COPY_DIR) $$shell_quote($${reportsource}) $$shell_quote($${reportpath})$$escape_expand(\n\t))
    }

    unix {
        first.depends = $(first) copydata
    }
    else {
        first.depends = $(first) copydata
    }
    export(first.depends)

    export(copydata.commands)
    unix {
        QMAKE_EXTRA_TARGETS += first copydata
    }
    else {
        QMAKE_EXTRA_TARGETS += first copydata
    }
}

DEPENDPATH += .$${OUT_PWD}/../$${DIST_DIR}
INCLUDEPATH +=	.. \
    $${TUCUXIROOTDIR}/src \
    $${TUCUXIROOTDIR}/test \
    $${TUCUXIROOTDIR}/libs/date \
    $${TUCUXIROOTDIR}/libs/eigen \
    $${TUCUXIROOTDIR}/libs/spdlog/include

SOURCES += \
#    src/ezchart.cpp \
    src/startupwindow.cpp \
    src/models/sortfilterentitylistproxymodel.cpp \
    src/models/abstractentitylistmodel.cpp \
    src/models/abstractentitymodel.cpp \
    src/controllers/interpretationcontroller.cpp \
    src/models/partialrequestlistmodel.cpp \
    src/startupscreen.cpp \
    src/widgets/persistentfiledialog.cpp \
    src/demorequestsclient.cpp \
    src/calculationcontroller.cpp \
    src/flowcontroller.cpp \
    src/websockettransport.cpp \
    src/websocketclientwrapper.cpp \
    src/drugtohtml.cpp \
    src/predictionspeccopier.cpp \
    src/mainwindowcontroller.cpp \
    src/fakepatientscreator.cpp \
    src/requestscontroller.cpp \
    src/chartdatacontroller.cpp \
    src/controllers/measuretabcontroller.cpp \
    src/controllers/abstractviewcontroller.cpp \
    src/controllers/dosagetabcontroller.cpp \
    src/controllers/targettabcontroller.cpp \
    src/controllers/covariatetabcontroller.cpp \
    src/controllers/adjustmenttabcontroller.cpp \
    src/controllers/drugtabcontroller.cpp \
    src/controllers/validationtabcontroller.cpp \
    src/demorequestfileclient.cpp \
    src/widgets/validatedrugdialog.cpp \
    src/multithreadedcalculationcontroller.cpp \
    src/unavailablefunctionality.cpp \
    src/appmode.cpp \
    src/appglobals.cpp

config_connected {
    SOURCES +=
}

HEADERS +=  \
    src/startupwindow.h \
    src/models/sortfilterentitylistproxymodel.h \
    errors_guiutils.h \
    src/models/abstractentitylistmodel.h \
    src/models/abstractentitymodel.h \
    src/controllers/interpretationcontroller.h \
    src/models/partialrequestlistmodel.h \
    src/startupscreen.h \
    src/widgets/persistentfiledialog.h \
    src/demorequestsclient.h \
    src/calculationcontroller.h \
    src/flowcontroller.h \
    src/websockettransport.h \
    src/websocketclientwrapper.h \
    src/drugtohtml.h \
    src/predictionspeccopier.h \
    src/mainwindowcontroller.h \
    src/fakepatientscreator.h \
    src/requestscontroller.h \
    src/chartdatacontroller.h \
    src/controllers/measuretabcontroller.h \
    src/controllers/abstractviewcontroller.h \
    src/controllers/dosagetabcontroller.h \
    src/controllers/targettabcontroller.h \
    src/controllers/covariatetabcontroller.h \
    src/controllers/adjustmenttabcontroller.h \
    src/controllers/drugtabcontroller.h \
    src/controllers/validationtabcontroller.h \
    src/demorequestfileclient.h \
    src/widgets/validatedrugdialog.h \
    src/multithreadedcalculationcontroller.h \
    src/unavailablefunctionality.h \
    src/appmode.h \
    src/appglobals.h

config_connected {
HEADERS +=
}

RESOURCES += \
    resources/guiutils.qrc \
    qml/qml.qrc

FORMS += \
    src/widgets/validatedrugdialog.ui

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/guiutils_fr.ts
QML_IMPORT_PATH += $$PWD/..

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = tucuxi-gui-core tucuxi-gui-admin tucuxi-gui-apputils tucuxi-gui-rest

for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../translation.pri)

#Third party shared libraries
#include(../qwt.pri)
include(../botan.pri)

