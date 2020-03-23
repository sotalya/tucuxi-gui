#EzeCHieL configuration
include (../../tucuxi.pri)

#Plugin ID
PLUGIN_ID=ch.heig-vd.ezechiel.analytic

#Models ID
DEFINES += ANALYTIC_ID=\\\"$${PLUGIN_ID}\\\" \
    A3C_EXTRA_K_ID=\\\"$${PLUGIN_ID}.linear.3comp.extra.k\\\" \
    A3C_BOLUS_K_ID=\\\"$${PLUGIN_ID}.linear.3comp.bolus.k\\\" \
    A3C_INFUS_K_ID=\\\"$${PLUGIN_ID}.linear.3comp.infu.k\\\" \
    A2C_EXTRA_K_ID=\\\"$${PLUGIN_ID}.linear.2comp.extra.k\\\" \
    A2C_BOLUS_K_ID=\\\"$${PLUGIN_ID}.linear.2comp.bolus.k\\\" \
    A2C_INFUS_K_ID=\\\"$${PLUGIN_ID}.linear.2comp.infu.k\\\" \
    A3C_EXTRA_CLV_ID=\\\"$${PLUGIN_ID}.linear.3comp.extra.clv\\\" \
    A3C_BOLUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.3comp.bolus.clv\\\" \
    A3C_INFUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.3comp.infu.clv\\\" \
    A2C_EXTRA_CLV_ID=\\\"$${PLUGIN_ID}.linear.2comp.extra.clv\\\" \
    A2C_BOLUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.2comp.bolus.clv\\\" \
# YTA : We'll have to change this ID
    A2C_INFUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.2comp.infu.clv\\\" \
    A1C_EXTRA_K_ID=\\\"$${PLUGIN_ID}.linear.1comp.extra.k\\\" \
    A1C_EXTRA_CLV_ID=\\\"$${PLUGIN_ID}.linear.1comp.extra.clv\\\" \
    A1C_BOLUS_K_ID=\\\"$${PLUGIN_ID}.linear.1comp.bolus.k\\\" \
    A1C_BOLUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.1comp.bolus.clv\\\" \
    A1C_INFUS_K_ID=\\\"$${PLUGIN_ID}.linear.1comp.infu.k\\\" \
    A1C_INFUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.1comp.infu.clv\\\" \
    A1C_GAMMA_EXTRA_K_ID=\\\"$${PLUGIN_ID}.linear.1comp.gamma.extra.k\\\" \
    A1C_GAMMA_EXTRA_CLV_ID=\\\"$${PLUGIN_ID}.linear.1comp.gamma.extra.clv\\\"

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = analytic
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}/plugins

INCLUDEPATH += ../../core ../../core/interfaces ../../core/utils ../../core/dal

HEADERS	+= analytic.h \
    analyticlinear3compbolus.h \
    analyticlinear3compinf.h \
    analyticlinear3compextra.h \
    analyticlinear2compbolus.h \
    analyticlinear2compextra.h \
    analyticlinear2compinf.h \
    analyticlineargenericinf.h \
    analyticlinear1compextra.h \
    analyticlinear1compbolus.h \
    analyticlinear1compinf.h \
    analyticnonlinear1compgammaextra.h

SOURCES += analytic.cpp \
    analyticlinear3compbolus.cpp \
    analyticlinear3compinf.cpp \
    analyticlinear3compextra.cpp \
    analyticlinear2compbolus.cpp \
    analyticlinear2compinf.cpp \
    analyticlinear2compextra.cpp \
    analyticlineargenericinf.cpp \
    analyticlinear1compextra.cpp \
    analyticlinear1compbolus.cpp \
    analyticlinear1compinf.cpp \
    analyticnonlinear1compgammaextra.cpp

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/$${PLUGIN_ID}_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-core ezechiel-math
for(dep, DEPENDANCIES) {
    LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../../translation.pri)

#Third party shared libraries
include(../../boost.pri)
