#Plugin ID
set(PLUGIN_ID ch.heig-vd.ezechiel.csp)

#Models ID
add_definitions(-DCSP_ID=\"${PLUGIN_ID}\")

#Include core parts
include_directories(
${CMAKE_SOURCE_DIR}/src/core/interfaces
${CMAKE_SOURCE_DIR}/src/core/utils
${CMAKE_SOURCE_DIR}/src/core
${Boost_INCLUDE_DIRS}
)
#Qt part
include(${LOCAL_QT_USE_FILE})

#Sources
set(CSP_HDRS
   csp.h
)
set(CSP_SRCS
   csp.cpp
)

#Translations
file(GLOB CSP_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(CSP_QM ch.heig-vd.ezechiel.csp ${CSP_SRCS} ${CSP_TS})
## CSP
QT4_WRAP_CPP(CSP_MOC ${CSP_HDRS})
add_library(csp SHARED ${CSP_SRCS} ${CSP_MOC} ${CSP_QM})
target_link_libraries(csp ${QT_LIBRARIES} ${Boost_LIBRARIES} ezechiel-core)
#Install in the test directory
install(TARGETS csp DESTINATION ${REVERSEENGINES_INSTALL} OPTIONAL)
