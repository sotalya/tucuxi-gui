#Plugin ID
set(PLUGIN_ID ch.heig-vd.ezechiel.monte_carlo)

#Models ID
add_definitions(-DMONTE_CARLO_ID=\"${PLUGIN_ID}\")

#Include core parts
include_directories(
${CMAKE_SOURCE_DIR}/src/core/interfaces
${CMAKE_SOURCE_DIR}/src/core/utils
${CMAKE_SOURCE_DIR}/src/core
)
#Qt part
include(${LOCAL_QT_USE_FILE})

#Sources
set(MONTECARLO_HDRS
	montecarlo.h
	percentilethread.h
)
set(MONTECARLO_SRCS
	montecarlo.cpp
	percentilethread.cpp
)

#Translations
file(GLOB MONTECARLO_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(MONTECARLO_QM ch.heig-vd.ezechiel.monte_carlo ${MONTECARLO_SRCS} ${MONTECARLO_TS})
## Bayesian
QT4_WRAP_CPP(MONTECARLO_MOC ${MONTECARLO_HDRS})
add_library(montecarlo SHARED ${MONTECARLO_SRCS} ${MONTECARLO_MOC} ${MONTECARLO_QM})
target_link_libraries(montecarlo ${QT_LIBRARIES} ${Boost_LIBRARIES} ezechiel-core)
add_dependencies(prediction_engines montecarlo)
#Install in the test directory
install(TARGETS montecarlo DESTINATION ${ENGINES_INSTALL} OPTIONAL)
