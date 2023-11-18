include (../tucuxi.pri)

TARGET      = tucuxi-gui-admin
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

include ($${TUCUXIROOTDIR}/make/qtcreator/includepaths.pri)
include ($${TUCUXIROOTDIR}/make/qtcreator/tucucommon.pri)

# To solve issues with namespaces
# QMAKE_CXXFLAGS += -fpermissive

#Removing the QtGui module (the admin must NOT use any gui element)
QT -= gui


INCLUDEPATH += ..

HEADERS	+= \
    src/dal/sentencepalette.h \
    src/stdadminrepository.h \
    src/dal/email.h \
    src/dal/institute.h \
    src/dal/location.h \
    src/dal/patient.h \
    src/dal/person.h \
    src/dal/phone.h \
    src/dal/practician.h \
    src/dal/measure.h \
    src/dal/clinical.h \
    src/adminfactory.h \
    src/dal/interpretationrequest.h \
    src/dal/interpretation.h \
    src/dal/partialrequest.h \
    errors_admin.h \
    src/dal/interpretationanalysis.h \
    src/dal/validationstatus.h \
    src/requestsclient.h \
    src/dal/graphinformationselection.h \
    src/dal/steptypes.h \
    src/dal/curvetypes.h \
    src/interpretationxmlexport.h \
    src/interpretationtorequestxmlexport.h \
    src/interpretationxmlimport.h \
    src/dataxmlexport.h

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/admin_fr.ts

#Translation configuration
include(../translation.pri)

SOURCES += \
    src/dal/location.cpp \
    src/dal/phone.cpp \
    src/dal/email.cpp \
    src/dal/patient.cpp \
    src/dal/institute.cpp \
    src/dal/practician.cpp \
    src/dal/person.cpp \
    src/dal/clinical.cpp \
    src/dal/measure.cpp \
    src/dal/interpretationrequest.cpp \
    src/dal/interpretation.cpp \
    src/dal/partialrequest.cpp \
    src/dal/interpretationanalysis.cpp \
    src/dal/sentencepalette.cpp \
    src/dal/validationstatus.cpp \
    src/requestsclient.cpp \
    src/dal/graphinformationselection.cpp \
    src/interpretationxmlexport.cpp \
    src/interpretationtorequestxmlexport.cpp \
    src/interpretationxmlimport.cpp \
    src/dataxmlexport.cpp

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = tucuxi-gui-core
for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}
