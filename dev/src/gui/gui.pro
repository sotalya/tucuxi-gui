include (../tucuxi.pri)

TESTDRUGSPATH = $$DRUGSPATH
isEmpty(TESTDRUGSPATH) {
    error("The variable DRUGSPATH should be set when invoking qmake. Like : qmake DRUGSPATH=<somepath>/tucuxi-drugs/drugfiles")
}

unix {
    # Else the executable is not clickable in Ubuntu
    QMAKE_LFLAGS += -no-pie
}

TARGET      = tucuxi
TEMPLATE    = app
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}
ICON        = ../guiutils/resources/icons/logo.ico

QT += gui printsupport quick quickwidgets printsupport svg webchannel websockets
QT += webenginecore

CONFIG(debug) {
    CONFIG += qml_debug
}

config_demo {
    RESOURCES += resources/demo/demo.qrc
}

# For chrono
CONFIG += c++20

DEPENDPATH += .$${DESTDIR}
INCLUDEPATH +=	..

config_guitest {
    include($$PWD/../../test/gui/tucuxi-gui-test.pri)
    win32 {
        LIBS += "C:\Program Files\Spix\lib\Spix.lib"
    }
    else {
        LIBS += /usr/local/lib/libSpix.a
    }
    QT += testlib
}

include($${TUCUXIROOTDIR}/make/qtcreator/includepaths.pri)

SOURCES += src/main.cpp \
    src/restconfigdialog.cpp \
    src/licensedialog.cpp

QML_IMPORT_PATH += $$PWD/..

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)

DEPENDENCIES = tucuxi-gui-guiutils tucuxi-gui-rest
DEPENDENCIES += tucuxi-gui-admin tucuxi-gui-apputils tucuxi-gui-core tucuxi-gui-flatrequests

DEPENDENCIES += tucuxi-gui-processingtucucore


for(dep, DEPENDENCIES) {
    LIBNAME = $${DESTDIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}

#Install the definition xml file
def.path    =  $${DESTDIR}
macx {
    def.path = $${DESTDIR}/$${TARGET}.app/Contents/Resources/
}
android {
    def.path = /assets/
}
def.files   =  ../core/xml/definitions.xml
INSTALLS    += def

!config_demo {
    #Install the definition tdd file
    drugs2path    =  $${DESTDIR}/drugfiles
    macx {
        drugs2path = $${DESTDIR}/$${TARGET}.app/Contents/Resources/drugfiles
    }
    android {
        drugs2path = /assets/drugfiles
    }
    copydata.commands = $$quote($(COPY_DIR) $$DRUGSPATH/* $${drugs2path}$$escape_expand(\n\t))
    unix {
        createdrugfile.commands = $$quote(mkdir -p $${drugs2path}$$escape_expand(\n\t))
    }
    win32 {
        drugs2path ~= s,/,\\,g
        copydata.commands = $(COPY_DIR) $$shell_quote($$DRUGSPATH) $$shell_quote($${drugs2path})$$escape_expand(\n\t))
    }

    unix {
        first.depends = $(first) createdrugfile copydata
    }
    else {
        first.depends = $(first) copydata
    }
    export(first.depends)

    unix {
        export(createdrugfile.commands)
    }
    export(copydata.commands)
    unix {
        QMAKE_EXTRA_TARGETS += first createdrugfile copydata
    }
    else {
        QMAKE_EXTRA_TARGETS += first copydata
    }
}

#    QMAKE_EXTRA_TARGETS += copydata2
#    ezechiel.depends += copydata2

#    #Install the definition xml file
#    drugs2.path    =  $${DESTDIR}/drugs2
#    macx {
#        drugs2.path = $${DESTDIR}/$${TARGET}.app/Contents/Resources/drugs2
#    }
#    android {
#        drugs2.path = /assets/drugs2
#    }
#    android {
#        drugs2.path = /assets/drugs2
#    }
#    drugs2.files   =  $$DRUGSPATH/*.tdd $$DRUGSPATH/drugs2.xsd
#    INSTALLS    += drugs2

#Install the database plugins
!macx {
    dbs.path    =  $${DESTDIR}/dbs
    dbs.files =  $${DESTDIR}/dbs/*.so
}
macx {
    dbs.path = $${DESTDIR}/$${TARGET}.app/Contents/Resources/dbs
    dbs.files =  $${DESTDIR}/dbs/libephemeraldb.dylib
}
android {
    dbs.path = /assets/dbs
}
INSTALLS    += dbs

#Translation configuration
include(../translation.pri)

#Third party shared libraries
#include(../qwt.pri)
include(../botan.pri)

HEADERS += \
    errors_gui.h \
    src/restconfigdialog.h \
    src/licensedialog.h


DISTFILES += \
    qmldeps.qml

FORMS += \
    src/restconfigdialog.ui

win32{
    LIBS += Iphlpapi.lib
}

unix{

    !macx {
        LIBS += \
            #/usr/lib/x86_64-linux-gnu/libdl.so
    }
}

win32{
    CONFIG(debug, debug|release) {
        LIBS += $$PWD/../../libs/xerces-c/build/lib/xerces-c_3D.lib
    }
    CONFIG(release, debug|release) {
        LIBS += $$PWD/../../libs/xerces-c/build/lib/xerces-c_3.lib
    }
}
unix{
    LIBS+=$$PWD/../../libs/xerces-c/build/src/libxerces-c.a
    #LIBS+=$$PWD/../../libs/xerces-c/src/.libs/libxerces-c.a
}
