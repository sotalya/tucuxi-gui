#EzeCHieL configuration
include(../../tucuxi.pri)

#Plugin ID
PLUGIN_ID=ch.heig-vd.ezechiel.rk4

#Models ID
DEFINES += RK4_ID=\\\"$${PLUGIN_ID}\\\" \
    RK4_LIN_1C_EXTRA_K_ID=\\\"$${PLUGIN_ID}.linear.1comp.extra.k\\\" \
    RK4_LIN_1C_EXTRA_CLV_ID=\\\"$${PLUGIN_ID}.linear.1comp.extra.clv\\\" \
    RK4_LIN_1C_BOLUS_K_ID=\\\"$${PLUGIN_ID}.linear.1comp.bolus.k\\\" \
    RK4_LIN_1C_BOLUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.1comp.bolus.clv\\\" \
    RK4_LIN_1C_INFUS_K_ID=\\\"$${PLUGIN_ID}.linear.1comp.infus.k\\\" \
    RK4_LIN_1C_INFUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.1comp.infus.clv\\\" \
    RK4_LIN_2C_EXTRA_K_ID=\\\"$${PLUGIN_ID}.linear.2comp.extra.k\\\" \
    RK4_LIN_2C_EXTRA_CLV_ID=\\\"$${PLUGIN_ID}.linear.2comp.extra.clv\\\" \
    RK4_LIN_2C_BOLUS_K_ID=\\\"$${PLUGIN_ID}.linear.2comp.bolus.k\\\" \
    RK4_LIN_2C_BOLUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.2comp.bolus.clv\\\" \
    RK4_LIN_2C_INFUS_K_ID=\\\"$${PLUGIN_ID}.linear.2comp.infu.k\\\" \
    RK4_LIN_2C_INFUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.2comp.infu.clv\\\" \
    RK4_LIN_3C_EXTRA_K_ID=\\\"$${PLUGIN_ID}.linear.3comp.extra.k\\\" \
    RK4_LIN_3C_EXTRA_CLV_ID=\\\"$${PLUGIN_ID}.linear.3comp.extra.clv\\\" \
    RK4_LIN_3C_BOLUS_K_ID=\\\"$${PLUGIN_ID}.linear.3comp.bolus.k\\\" \
    RK4_LIN_3C_BOLUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.3comp.bolus.clv\\\" \
    RK4_LIN_3C_INFUS_K_ID=\\\"$${PLUGIN_ID}.linear.3comp.infu.k\\\" \
    RK4_LIN_3C_INFUS_CLV_ID=\\\"$${PLUGIN_ID}.linear.3comp.infu.clv\\\" \
    RK4_MM_1C_INFUS_ID=\\\"$${PLUGIN_ID}.mm.1comp.infu\\\" \
    RK4_MM_1C_EXTRA_ID=\\\"$${PLUGIN_ID}.mm.1comp.extra\\\" \
    RK4_MM_1C_EXTRAINF_ID=\\\"$${PLUGIN_ID}.mm.1comp.extrainf\\\" \
    RK4_MM_1C_EXTRA_VMV_ID=\\\"$${PLUGIN_ID}.mm.1comp.extra.vmv\\\" \
    RK4_MM_1C_INF_VMV_ID=\\\"$${PLUGIN_ID}.mm.1comp.infu.vmv\\\" \
    RK4_MM_1C_BOLUS_VMV_ID=\\\"$${PLUGIN_ID}.mm.1comp.bolus.vmv\\\" \
    RK4_MM_1C_BOLUS_ID=\\\"$${PLUGIN_ID}.mm.1comp.bolus\\\" \
    RK4_MM_2C_INFUS_K_ID=\\\"$${PLUGIN_ID}.mm.2comp.infus.k\\\" \
    RK4_MM_2C_INFUS_CLV_ID=\\\"$${PLUGIN_ID}.mm.2comp.infus.clv\\\" \
    RK4_MM_2C_EXTRA_K_ID=\\\"$${PLUGIN_ID}.mm.2comp.extra.k\\\" \
    RK4_MM_2C_EXTRA_CLV_ID=\\\"$${PLUGIN_ID}.mm.2comp.extra.clv\\\" \
    RK4_MM_2C_BOLUS_K_ID=\\\"$${PLUGIN_ID}.mm.2comp.bolus.k\\\" \
    RK4_MM_2C_BOLUS_CLV_ID=\\\"$${PLUGIN_ID}.mm.2comp.bolus.clv\\\" \
    RK4_MM_3C_INFUS_K_ID=\\\"$${PLUGIN_ID}.mm.3comp.infus.k\\\" \
    RK4_MM_3C_INFUS_CLV_ID=\\\"$${PLUGIN_ID}.mm.3comp.infus.clv\\\" \
    RK4_MM_3C_EXTRA_K_ID=\\\"$${PLUGIN_ID}.mm.3comp.extra.k\\\" \
    RK4_MM_3C_EXTRA_CLV_ID=\\\"$${PLUGIN_ID}.mm.3comp.extra.clv\\\" \
    RK4_MM_3C_BOLUS_K_ID=\\\"$${PLUGIN_ID}.mm.3comp.bolus.k\\\" \
    RK4_MM_3C_BOLUS_CLV_ID=\\\"$${PLUGIN_ID}.mm.3comp.bolus.clv\\\" \

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = rk4
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}/plugins

INCLUDEPATH += ../../core ../../core/interfaces ../../core/utils ../../core/dal

HEADERS	+= \
    rk4.h \
    rk4linear1comp.h \
    rk4linear2comp.h \
    rk4linear3comp.h \
    rk4michaelismenten1comp.h \
    rk4michaelismenten2comp.h \
    rk4michaelismenten3comp.h \
    rk4genericmodel.h

SOURCES += \
    rk4.cpp \
    rk4linear1comp.cpp \
    rk4linear2comp.cpp \
    rk4linear3comp.cpp \
    rk4michaelismenten1comp.cpp \
    rk4michaelismenten2comp.cpp \
    rk4michaelismenten3comp.cpp \
    rk4genericmodel.cpp


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

#Module extra dependencies
