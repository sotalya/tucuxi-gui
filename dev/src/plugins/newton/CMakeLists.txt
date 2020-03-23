#Plugin ID
set(PLUGIN_ID ch.heig-vd.ezechiel.newton)

#Models ID
add_definitions(-DNEWTON_ID=\"${PLUGIN_ID}\")

#Include core parts
include_directories(
${CMAKE_SOURCE_DIR}/src/core/interfaces
${CMAKE_SOURCE_DIR}/src/core/utils
${CMAKE_SOURCE_DIR}/src/core
)
#Qt part
include(${LOCAL_QT_USE_FILE})

#Sources
set(NEWTON_HDRS
   newtonreverse.h
)
set(NEWTON_SRCS
   newtonreverse.cpp
)

#Translations
file(GLOB NEWTON_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(NEWTON_QM ch.heig-vd.ezechiel.newton ${NEWTON_SRCS} ${NEWTON_TS})
## Newton
QT4_WRAP_CPP(NEWTON_MOC ${NEWTON_HDRS})
add_library(newton SHARED ${NEWTON_SRCS} ${NEWTON_MOC} ${NEWTON_QM})
target_link_libraries(newton ${QT_LIBRARIES} ${Boost_LIBRARIES} ezechiel-core)
add_dependencies(engines newton)
#Install in the test directory
install(TARGETS newton DESTINATION ${REVERSEENGINES_INSTALL} OPTIONAL)
