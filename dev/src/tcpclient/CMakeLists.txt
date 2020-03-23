#Include core parts
include_directories(
../core
../core/config
../core/interfaces
../core/security
../core/database
../core/utils
../chart
../cliutils
../cliutils/actions
./
./actions
${CMAKE_CURRENT_BINARY_DIR}/../cli
${Qwt6_INCLUDE_DIR}
)

#Qt part
include(${LOCAL_QT_USE_FILE})

#Sources
set(CLI_SRCS
	main.cpp
)

#Preprocess the cli_error.h file to find the real tr()
add_custom_command(	OUTPUT ${CMAKE_BINARY_DIR}/cli_errors.h
							COMMAND gcc -E -CC -D PREPROC_ONLY ${CMAKE_CURRENT_LIST_DIR}/cli_errors.h -o ${CMAKE_BINARY_DIR}/cli_errors.h
							)
#Translations
file(GLOB CLI_TS ${CMAKE_CURRENT_LIST_DIR}/trans/*.ts)
declare_translation(CLI_QM cli ${CMAKE_BINARY_DIR}/cli_errors.h ${CLI_SRCS} ${CLI_TS})
#Moc precompiler
QT4_WRAP_CPP(CLI_MOC ${CLI_HDRS})
#Ressource files
QT4_ADD_RESOURCES(CLI_RCC cli.qrc ../core/resources/core.qrc)
#Library creation
add_executable(ezechiel-cli ${CLI_SRCS} ${CLI_MOC} ${CLI_RCC} ${GUI_RCC} ${CLI_TRANS_RCC} ${CLI_QM})
target_link_libraries(ezechiel-cli ${QT_LIBRARIES} ezechiel-core ezechiel-chart ezechiel-report ezechiel-cliutils)

#Install in usr/bin
install(TARGETS ezechiel-cli DESTINATION ${BIN_INSTALL})
