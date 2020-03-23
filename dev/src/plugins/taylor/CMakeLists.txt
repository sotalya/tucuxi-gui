#Plugin ID
set(PLUGIN_ID ch.heig-vd.ezechiel.taylor)

#Models ID
add_definitions(-DTAYLOR_ID=\"${PLUGIN_ID}\")

#Include core parts
include_directories(
${CMAKE_SOURCE_DIR}/src/core/interfaces
${CMAKE_SOURCE_DIR}/src/core/utils
${CMAKE_SOURCE_DIR}/src/core
)
#Qt part
include(${LOCAL_QT_USE_FILE})

#Sources
set(TAYLOR_HDRS
	taylor.h
)
set(TAYLOR_SRCS
	taylor.cpp
)

#Translations
file(GLOB TAYLOR_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(TAYLOR_QM ch.heig-vd.ezechiel.taylor ${TAYLOR_SRCS} ${TAYLOR_TS})
## Taylor
QT4_WRAP_CPP(TAYLOR_MOC ${TAYLOR_HDRS})
add_library(taylor SHARED ${TAYLOR_SRCS} ${TAYLOR_MOC} ${TAYLOR_QM})
target_link_libraries(taylor ${QT_LIBRARIES} ${Boost_LIBRARIES} ezechiel-core)
add_dependencies(prediction_engines taylor)
#Install in the test directory
install(TARGETS taylor DESTINATION ${ENGINES_INSTALL} OPTIONAL)
