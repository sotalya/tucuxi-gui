include_directories(
	${Qwt6_INCLUDE_DIR}
	../core
	../core/config
	../core/interfaces
	../core/utils
	../chart
)

set(QT_USE_QTSVG true)
add_definitions(${QT_DEFINITIONS})
include(${LOCAL_QT_USE_FILE})

#Sources
set(REPORT_HDRS
	report.h
	reportsmodel.h
)
set(REPORT_SRCS
	report.cpp
	reportsmodel.cpp
)

#Translations
file(GLOB REPORT_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(REPORT_QM report ${REPORT_SRCS} ${REPORT_TS})
#Moc precompiler
QT4_WRAP_CPP(REPORT_MOC ${REPORT_HDRS})
#Library creation
add_library(ezechiel-report ${REPORT_SRCS} ${REPORT_MOC} ${REPORT_QM})
target_link_libraries(ezechiel-report ${Qwt6_Qt4_LIBRARY} ${QT_LIBRARIES} ezechiel-chart)

#Install in usr/lib
if (${BUILD_SHARED_LIBS})
	install(TARGETS ezechiel-report DESTINATION ${LIBS_INSTALL})
endif (${BUILD_SHARED_LIBS})
