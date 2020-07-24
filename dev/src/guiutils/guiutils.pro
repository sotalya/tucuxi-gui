
include (../tucuxi.pri)

TARGET      = ezechiel-guiutils
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}


CONFIG(debug) {
    CONFIG += qml_debug
}

QT += gui printsupport quick quickwidgets printsupport svg network webchannel websockets
!win32 {
QT += webenginewidgets
}

config_externalreport {
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
    $${TUCUXIROOTDIR}/libs/date-master-20170711 \
    $${TUCUXIROOTDIR}/libs/eigen-3.3.2 \
    $${TUCUXIROOTDIR}/libs/spdlog-master-20170622/include

SOURCES += \
#    src/ezchart.cpp \
    src/models/measurelistmodel.cpp \
    src/startupwindow.cpp \
    src/models/covariatelistmodel.cpp \
    src/models/sortfilterentitylistproxymodel.cpp \
    src/models/patientlistmodel.cpp \
    src/models/practicianlistmodel.cpp \
    src/models/institutelistmodel.cpp \
    src/models/dosagelistmodel.cpp \
    src/models/druglistmodel.cpp \
    src/models/drugvariatelistmodel.cpp \
    src/models/targetlistmodel.cpp \
    src/models/adjustmentlistmodel.cpp \
    src/models/abstractentitylistmodel.cpp \
    src/models/abstractentitymodel.cpp \
    src/controllers/interpretationcontroller.cpp \
    src/models/covariatemodel.cpp \
    src/models/reversemodel.cpp \
    src/models/domainlistmodel.cpp \
    src/models/studylistmodel.cpp \
    src/models/validationmodel.cpp \
    src/models/adjustmentsettingsmodel.cpp \
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
    src/models/measurelistmodel.h \
    src/startupwindow.h \
    src/models/covariatelistmodel.h \
    src/models/sortfilterentitylistproxymodel.h \
    src/models/patientlistmodel.h \
    src/models/practicianlistmodel.h \
    src/models/institutelistmodel.h \
    errors_guiutils.h \
    src/models/dosagelistmodel.h \
    src/models/druglistmodel.h \
    src/models/drugvariatelistmodel.h \
    src/models/targetlistmodel.h \
    src/models/adjustmentlistmodel.h \
    src/models/abstractentitylistmodel.h \
    src/models/abstractentitymodel.h \
    src/controllers/interpretationcontroller.h \
    src/models/covariatemodel.h \
    src/models/reversemodel.h \
    src/models/domainlistmodel.h \
    src/models/studylistmodel.h \
    src/models/validationmodel.h \
    src/models/adjustmentsettingsmodel.h \
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
DEPENDENCIES = ezechiel-core ezechiel-admin ezechiel-apputils ezechiel-rest #ezechiel-report

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

