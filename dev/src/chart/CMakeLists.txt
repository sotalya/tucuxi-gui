include_directories(
	${Qwt6_INCLUDE_DIR}
	../core
	../core/config
	../core/interfaces
	../core/utils
)

set(QT_USE_QTSVG true)
include(${LOCAL_QT_USE_FILE})

#Sources
set(CHART_HDRS
	chart.h
	chartitemgroup.h
	curve.h
	chartcolormanager.h
	dot.h
	line.h
	percentilecurve.h
	plotpicker.h
	targetinterval.h
	timescaledraw.h
)
set(CHART_SRCS
	chart.cpp
	chartitemgroup.cpp
	chartcolormanager.cpp
	curve.cpp
	dot.cpp
	line.cpp
	percentilecurve.cpp
	plotpicker.cpp
	targetinterval.cpp
	timescaledraw.cpp
)

#Translations
file(GLOB CHART_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(CHART_QM chart ${CHART_SRCS} ${CHART_TS})
#Moc precompiler
QT4_WRAP_CPP(CHART_MOC ${CHART_HDRS})
#Library creation
add_library(ezechiel-chart ${CHART_SRCS} ${CHART_MOC} ${CHART_QM})
target_link_libraries(ezechiel-chart ${Qwt6_Qt4_LIBRARY} ${QT_LIBRARIES} ezechiel-core)

#Install in usr/lib
if (${BUILD_SHARED_LIBS})
	install(TARGETS ezechiel-chart DESTINATION ${LIBS_INSTALL})
endif (${BUILD_SHARED_LIBS})
