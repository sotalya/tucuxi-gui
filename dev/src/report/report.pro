#EzeCHieL configuration
include(../tucuxi.pri)

CONFIG += gui

TARGET      = ezechiel-report
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

QT += webkitwidgets printsupport svg

INCLUDEPATH += \
    builder \
    dao \
    tools \
    ../core \
    ../core/database \
    ../core/dao \
    ../core/config \
    ../core/security \
    ../core/interfaces \
    ../core/utils \
    ../core/dal \
    ../core/dal/drug \
    ../apputils/src \
    ../chart \
    ../admin/src \
    ../databases/ephemeraldb \
    ../admin/src/dal

SOURCES += report.cpp \
    reportdata.cpp \
    reporttemplate.cpp \
    reporttemplatefactory.cpp \
    reporttemplatexmlfactory.cpp \
    tools/reportexporttoolfactory.cpp \
    tools/reportexporttool.cpp \
    tools/reportpdfexporttool.cpp \
    dao/reportsqlitedao.cpp \
    builder/reportbuilder.cpp \
    builder/teambuildermodule.cpp \
    builder/recipientbuildermodule.cpp \
    builder/analystbuildermodule.cpp \
    builder/patientbuildermodule.cpp \
    builder/chartbuildermodule.cpp \
    builder/parametersbuildermodule.cpp \
    builder/covariatesbuildermodule.cpp \
    builder/targetbuildermodule.cpp \
    builder/concentrationsbuildermodule.cpp \
    builder/samplebuildermodule.cpp \
    builder/dosagebuildermodule.cpp \
    builder/treatmentbuildermodule.cpp \
    builder/interpretationbuildermodule.cpp \
    builder/basebuildermodule.cpp \
    builder/newreportbuilder.cpp \
    reportfactory.cpp \
    defaultreport.cpp \
    customreport.cpp \
    builder/clonebuilder.cpp \
    compactreport.cpp

HEADERS += report.h \
    reportdata.h \
    reporttemplate.h \
    reporttemplatefactory.h \
    reporttemplatexmlfactory.h \
    identifiers.h \
    tools/reportexporttool.h \
    tools/reportexporttoolfactory.h \
    tools/reportpdfexporttool.h \
    dao/reportsqlitedao.h \
    builder/reportbuilder.h \
    builder/teambuildermodule.h \
    builder/recipientbuildermodule.h \
    builder/analystbuildermodule.h \
    builder/patientbuildermodule.h \
    builder/chartbuildermodule.h \
    builder/parametersbuildermodule.h \
    builder/covariatesbuildermodule.h \
    builder/targetbuildermodule.h \
    builder/concentrationsbuildermodule.h \
    builder/samplebuildermodule.h \
    builder/dosagebuildermodule.h \
    builder/treatmentbuildermodule.h \
    builder/interpretationbuildermodule.h \
    builder/basebuildermodule.h \
    builder/newreportbuilder.h \
    builder/reportbuildermodule.h \
    reportfactory.h \
    defaultreport.h \
    customreport.h \
    builder/clonebuilder.h \
    reportrepository.h \
    compactreport.h

RESOURCES += \
    resources/report.qrc

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/report_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = ezechiel-core ezechiel-admin ezechiel-apputils
for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../translation.pri)

#Third party libraries headers
include(../botan.pri)

#Install the report files (unused for now)
#report.path  = $${OUT_PWD}/../$${DIST_DIR}/report
#report.files = resources/templates.xml resources/templates
#INSTALLS += report
