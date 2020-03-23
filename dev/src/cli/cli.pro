#EzeCHieL configuration
include(../tucuxi.pri)

CONFIG += console

TARGET      = ezechiel-cli
TEMPLATE    = app
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

QT -= gui
QT += network
!win32 {
QT += webengine webenginewidgets
}

DEPENDPATH += .$${DESTDIR}
INCLUDEPATH += ..

HEADERS += \
    rlutil.h \
    errors_cli.h

SOURCES += main.cpp

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/cli_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = ezechiel-apputils ezechiel-cliutils
!config_demo {
DEPENDENCIES += ezechiel-rest
}
DEPENDENCIES += ezechiel-admin ezechiel-core
    DEPENDENCIES += ezechiel-processingtucucore

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
include(../eigen.pri)

#Install the definition xml file
def.path    =  $${OUT_PWD}/../$${DIST_DIR}
macx{
    def.path = $${OUT_PWD}/../$${DIST_DIR}/$${TARGET}.app/Contents/Resources
}
def.files   =  ../core/xml/definitions.xml
INSTALLS    += def

macx {
    dbs.path = $${OUT_PWD}/../$${DIST_DIR}/$${TARGET}.app/Contents/Resources/dbs
    dbs.files =  $${OUT_PWD}/../$${DIST_DIR}/dbs/*.dylib
    INSTALLS    += dbs
}


win32{
LIBS += Iphlpapi.lib
    config_tucucore_lib {
       LIBS += $${TUCUXIROOTDIR}\make\visualstudio\tucucommon\x64\Debug\TucuCommon\TucuCommon.lib \
        $${TUCUXIROOTDIR}\make\visualstudio\tucucore\x64\Debug\TucuCore\TucuCore.lib
    }
}

unix{
    config_tucucore_lib {
        LIBS += $${TUCUXIROOTDIR}/bin/tucucore.a \
                $${TUCUXIROOTDIR}/bin/tucucommon.a
        LIBS += \
            $${TUCUXIROOTDIR}/bin/tinyjs.a
    }

    !macx {
        LIBS += \
            /usr/lib/x86_64-linux-gnu/libdl.so
    }

}
