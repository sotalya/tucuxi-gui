#EzeCHieL configuration
include(../tucuxi.pri)

CONFIG += gui svg

TARGET      = ezechiel-chart
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}

QT += printsupport

INCLUDEPATH += ../core/database \
    ../core \
    ../core/config \
    ../core/security \
    ../core/interfaces \
    ../core/utils \
    ../core/dal

SOURCES += \
    timescaledraw.cpp \
    plotpicker.cpp \
    chartitemgroup.cpp \
    curve.cpp \
    chart.cpp \
    percentilecurve.cpp \
    dot.cpp \
    line.cpp \
    targetinterval.cpp \
    chartcolormanager.cpp \
    item.cpp

HEADERS += \
    timescaledraw.h \
    plotpicker.h \
    chartitemgroup.h \
    curve.h \
    chart.h \
    percentilecurve.h \
    chartitem.h \
    dot.h \
    line.h \
    targetinterval.h \
    chartcolormanager.h \
    item.h


CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/chart_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-core
for(dep, DEPENDANCIES) {
   LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../translation.pri)

#Third party libraries headers
include(../qwt.pri)
include(../botan.pri)
