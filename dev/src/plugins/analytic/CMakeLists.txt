#Plugin ID
set(PLUGIN_ID ch.heig-vd.ezechiel.analytic)

#Models ID
add_definitions("-DANALYTIC_ID=\"${PLUGIN_ID}\"")
add_definitions("-DA3C_EXTRA_ID=\"${PLUGIN_ID}.3comp.extra\"")
add_definitions("-DA3C_BOLUS_ID=\"${PLUGIN_ID}.3comp.bolus\"")
add_definitions("-DA3C_INFUS_ID=\"${PLUGIN_ID}.3comp.infu\"")
add_definitions("-DA2C_EXTRA_ID=\"${PLUGIN_ID}.2comp.extra\"")
add_definitions("-DA2C_BOLUS_ID=\"${PLUGIN_ID}.2comp.bolus\"")
add_definitions("-DA2C_INFUS_ID=\"${PLUGIN_ID}.2comp.infu\"")
add_definitions("-DA1C_EXTRA_ID=\"${PLUGIN_ID}.1comp.extra\"")
add_definitions("-DA1C_BOLUS_ID=\"${PLUGIN_ID}.1comp.bolus\"")
add_definitions("-DA1C_INFUS_ID=\"${PLUGIN_ID}.1comp.infu\"")

#Include core parts
include_directories(
${CMAKE_SOURCE_DIR}/src/core/interfaces
${CMAKE_SOURCE_DIR}/src/core/utils
${CMAKE_SOURCE_DIR}/src/core
)
#Qt part
include(${LOCAL_QT_USE_FILE})

#Sources
set(ANALYTIC_HDRS
	analytic.h
    analyticlinear3compbolus.h
    analyticlinear3compinf.h
    analyticlinear3compextra.h
    analyticlinear2compbolus.h
    analyticlinear2compextra.h
    analyticlinear2compinf.h
    analyticlinear1compbolus.h
    analyticlinear1compextra.h
    analyticlinear1compinf.h
)
set(ANALYTIC_SRCS
	analytic.cpp
    analyticlinear3compbolus.cpp
    analyticlinear3compinf.cpp
    analyticlinear3compextra.cpp
    analyticlinear2compbolus.cpp
    analyticlinear2compinf.cpp
    analyticlinear1compbolus.cpp
    analyticlinear1compextra.cpp
    analyticlinear1compinf.cpp
    analyticlinear2compextra.cpp
)

#Translations
file(GLOB ANALYTIC_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(ANALYTIC_QM ch.heig-vd.ezechiel.analytic.intra ${ANALYTIC_SRCS} ${ANALYTIC_TS})
## Sinus
QT4_WRAP_CPP(ANALYTIC_MOC ${ANALYTIC_HDRS})
add_library(analytic SHARED ${ANALYTIC_SRCS} ${ANALYTIC_MOC} ${ANALYTIC_QM})
target_link_libraries(analytic ${QT_LIBRARIES} ezechiel-core)
add_dependencies(models analytic)
#Install in the test directory
install(TARGETS analytic DESTINATION ${MODELS_INSTALL} OPTIONAL)
