#Plugin ID
set(PLUGIN_ID ch.heig-vd.ezechiel.bayesian)

#Models ID
add_definitions(-DBAYESIAN_ID=\"${PLUGIN_ID}\")

#Include core parts
include_directories(
${CMAKE_SOURCE_DIR}/src/core/interfaces
${CMAKE_SOURCE_DIR}/src/core/utils
${CMAKE_SOURCE_DIR}/src/core
)
#Qt part
include(${LOCAL_QT_USE_FILE})

#Sources
set(BAYESIAN_HDRS
	bayesian.h
)
set(BAYESIAN_SRCS
	bayesian.cpp
)

#Translations
file(GLOB BAYESIAN_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(BAYESIAN_QM ch.heig-vd.ezechiel.bayesian ${BAYESIAN_SRCS} ${BAYESIAN_TS})
## DumbPost
QT4_WRAP_CPP(BAYESIAN_MOC ${BAYESIAN_HDRS})
add_library(bayesian SHARED ${BAYESIAN_SRCS} ${BAYESIAN_MOC} ${BAYESIAN_QM})
target_link_libraries(bayesian ${QT_LIBRARIES} ${Boost_LIBRARIES} ezechiel-core)
add_dependencies(post_engines bayesian)
#Install in the test directory
install(TARGETS bayesian DESTINATION ${POSTENGINES_INSTALL} OPTIONAL)
