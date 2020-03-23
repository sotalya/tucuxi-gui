#Include core parts
include_directories(
../core
../core/config
../core/interfaces
../core/security
../core/database
../core/utils
../chart
./
./actions
${Qwt6_INCLUDE_DIR}
)

#Qt part
include(${LOCAL_QT_USE_FILE})

#Sources
set(CLIUTILS_HDRS
	climanager.h
	climessagehandler.h
	action.h
    actions/listaction.h
    actions/patientaction.h
    actions/predictionaction.h
)
set(CLIUTILS_SRCS
	climanager.cpp
	climessagehandler.cpp
	cli_errors.h
    actions/listaction.cpp
    actions/patientaction.cpp
    actions/predictionaction.cpp
)

#Preprocess the cli_error.h file to find the real tr()
add_custom_command(	OUTPUT ${CMAKE_BINARY_DIR}/cli_errors.h
							COMMAND gcc -E -CC -D PREPROC_ONLY ${CMAKE_CURRENT_LIST_DIR}/cli_errors.h -o ${CMAKE_BINARY_DIR}/cli_errors.h
							)
#Translations
file(GLOB CLIUTILS_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(CLI_QM cli ${CMAKE_BINARY_DIR}/cli_errors.h ${CLIUTILS_SRCS} ${CLIUTILS_TS})
#Moc precompiler
QT4_WRAP_CPP(CLIUTILS_MOC ${CLIUTILS_HDRS})
#Library creation
add_library(ezechiel-cliutils ${CLIUTILS_SRCS} ${CLIUTILS_MOC} ${CLIUTILS_QM})
target_link_libraries(ezechiel-cliutils ${QT_LIBRARIES} ezechiel-core ezechiel-chart ezechiel-report)

#Install in usr/lib
if (${BUILD_SHARED_LIBS})
	install(TARGETS ezechiel-cliutils DESTINATION ${LIBS_INSTALL})
endif (${BUILD_SHARED_LIBS})
