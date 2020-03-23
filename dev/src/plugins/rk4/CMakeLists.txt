#Plugin ID
set(PLUGIN_ID ch.heig-vd.ezechiel.rk4)

#Models ID
add_definitions(-DRK4_ID=\"${PLUGIN_ID}\")


add_definitions(-DRK4_LIN_1C_EXTRA_ID=\"${PLUGIN_ID}.1comp.extra\")
add_definitions(-DRK4_LIN_1C_BOLUS_ID=\"${PLUGIN_ID}.1comp.bolus\")
add_definitions(-DRK4_LIN_1C_INFUS_ID=\"${PLUGIN_ID}.1comp.infu\")
add_definitions(-DRK4_LIN_2C_EXTRA_ID=\"${PLUGIN_ID}.2comp.extra\")
add_definitions(-DRK4_LIN_2C_BOLUS_ID=\"${PLUGIN_ID}.2comp.bolus\")
add_definitions(-DRK4_LIN_2C_INFUS_ID=\"${PLUGIN_ID}.2comp.infu\")
add_definitions(-DRK4_LIN_3C_EXTRA_ID=\"${PLUGIN_ID}.3comp.extra\")
add_definitions(-DRK4_LIN_3C_BOLUS_ID=\"${PLUGIN_ID}.3comp.bolus\")
add_definitions(-DRK4_LIN_3C_INFUS_ID=\"${PLUGIN_ID}.3comp.infu\")
add_definitions(-DRK4_MM_1C_INFUS_ID=\"${PLUGIN_ID}.1comp.mm.infu\")
add_definitions(-DRK4_MM_1C_EXTRA_ID=\"${PLUGIN_ID}.1comp.mm.extra\")
add_definitions(-DRK4_MM_1C_BOLUS_ID=\"${PLUGIN_ID}.1comp.mm.bolus\")
add_definitions(-DRK4_MM_2C_INFUS_ID=\"${PLUGIN_ID}.2comp.mm.infu\")
add_definitions(-DRK4_MM_2C_EXTRA_ID=\"${PLUGIN_ID}.2comp.mm.extra\")
add_definitions(-DRK4_MM_2C_BOLUS_ID=\"${PLUGIN_ID}.2comp.mm.bolus\")
add_definitions(-DRK4_MM_3C_INFUS_ID=\"${PLUGIN_ID}.3comp.mm.infu\")
add_definitions(-DRK4_MM_3C_EXTRA_ID=\"${PLUGIN_ID}.3comp.mm.extra\")
add_definitions(-DRK4_MM_3C_BOLUS_ID=\"${PLUGIN_ID}.3comp.mm.bolus\")
add_definitions(-DRK4_LIN_1C_EXTRA2_ID=\"${PLUGIN_ID}.1comp.2.extra\")

#Include core parts
include_directories(
${CMAKE_SOURCE_DIR}/src/core/interfaces
${CMAKE_SOURCE_DIR}/src/core/utils
${CMAKE_SOURCE_DIR}/src/core
)
#Use XML
set(QT_USE_QTXML true)
#Qt part
include(${LOCAL_QT_USE_FILE})

#Sources
set(RK4_HDRS
        rk4.h
        rk4genericmodel.h
        rk4linear1comp.h
        rk4linear2comp.h
        rk4linear3comp.h
        rk4michaelismenten1comp.h
        rk4michaelismenten2comp.h
        rk4michaelismenten3comp.h
)
set(RK4_SRCS
        rk4.cpp
        rk4genericmodel.cpp
        rk4linear1comp.cpp
        rk4linear2comp.cpp
        rk4linear3comp.cpp
        rk4michaelismenten1comp.cpp
        rk4michaelismenten2comp.cpp
        rk4michaelismenten3comp.cpp
)

#Translations
file(GLOB RK4_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(ANALYTIC_QM ch.heig-vd.ezechiel.rk4 ${RK4_SRCS} ${RK4_TS})
## Sinus
QT4_WRAP_CPP(RK4_MOC ${RK4_HDRS})
add_library(rk4 SHARED ${RK4_SRCS} ${RK4_MOC} ${RK4_QM})
target_link_libraries(rk4 ${QT_LIBRARIES} ezechiel-core)
add_dependencies(models rk4)
#Install in the test directory
install(TARGETS rk4 DESTINATION ${MODELS_INSTALL} OPTIONAL)
