include (../tucuxi.pri)

TARGET      = tucuxi-gui-apputils
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

# To solve issues with namespaces
# QMAKE_CXXFLAGS += -fpermissive

#Removing the QtGui module (the maths must NOT use any gui element)
QT -= gui

INCLUDEPATH += ..

HEADERS += \
    ../current_version.h \
    src/appcore.h \
    src/user.h \
    src/druglister.h \
    src/drugmanager.h \
    src/apputilsrepository.h \
    errors_apputils.h \
    src/apputilsfactory.h \
    src/calculationrequestlogger.h \
    src/predictionspecexporter.h \
    src/predictionspecimporter.h \
    src/modelvstreatmentcompatibilitychecker.h \
    src/chartdataexporter.h

android {
    HEADERS += src/botan_all.h
    SOURCES += src/botan_all.cpp
}

SOURCES += \
    src/appcore.cpp \
    src/user.cpp \
    src/druglister.cpp \
    src/drugmanager.cpp \
    src/calculationrequestlogger.cpp \
    src/predictionspecexporter.cpp \
    src/predictionspecimporter.cpp \
    src/modelvstreatmentcompatibilitychecker.cpp \
    src/chartdataexporter.cpp

!config_nobotan {
HEADERS += \
    src/securitymanager.h \
    src/datacrypter.h

SOURCES += \
    src/securitymanager.cpp \
    src/datacrypter.cpp

} # config_nobotan

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/apputils_fr.ts


#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = tucuxi-gui-core

for(dep, DEPENDENCIES) {
   LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

macx {
   botan.path = $${DESTDIR}/$${TARGET}.app/Contents/Frameworks
   botan.files =  $${TUCUXIROOTDIR}/libs/botan/libbotan.dylib
   INSTALLS    += botan
}

#Translation configuration
include(../translation.pri)

include(../botan.pri)
