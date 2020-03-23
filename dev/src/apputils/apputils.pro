include (../tucuxi.pri)

TARGET      = ezechiel-apputils
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

# To solve issues with namespaces
# QMAKE_CXXFLAGS += -fpermissive

#Removing the QtGui module (the maths must NOT use any gui element)
QT -= gui

INCLUDEPATH += ..

HEADERS += \
    src/drugxmlimport.h \
    src/drugxmlexport.h \
    ../current_version.h \
    src/appcore.h \
    src/securitymanager.h \
    src/datacrypter.h \
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
    src/drugxmlimport.cpp \
    src/drugxmlexport.cpp \
    src/appcore.cpp \
    src/securitymanager.cpp \
    src/datacrypter.cpp \
    src/user.cpp \
    src/druglister.cpp \
    src/drugmanager.cpp \
    src/calculationrequestlogger.cpp \
    src/predictionspecexporter.cpp \
    src/predictionspecimporter.cpp \
    src/modelvstreatmentcompatibilitychecker.cpp \
    src/chartdataexporter.cpp

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/apputils_fr.ts


#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = ezechiel-core

for(dep, DEPENDENCIES) {
   LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

macx {
   botan.path = $${DESTDIR}/$${TARGET}.app/Contents/Frameworks
   botan.files =  $${TUCUXIROOTDIR}/libs/botan-2.1.0/libbotan.dylib
   INSTALLS    += botan
}

#Translation configuration
include(../translation.pri)

include(../botan.pri)
