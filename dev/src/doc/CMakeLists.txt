#If doxygen is present, add the possibility to generate it
include(FindDoxygen)
if (DOXYGEN)
	#Where are the sources to use (CPP)
	set(DOXYGEN_SOURCE_DIRS ${DOXYGEN_SOURCE_FILES}
			${CMAKE_SOURCE_DIR}/src/core
			${CMAKE_SOURCE_DIR}/src/cli
			${CMAKE_SOURCE_DIR}/src/gui
			${CMAKE_SOURCE_DIR}/src/plugins
			${CMAKE_SOURCE_DIR}/src/chart
			${CMAKE_SOURCE_DIR}/src/databases)
	#Output folder
	set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)

	#Replace the ; by spaces (list conversion)
	string(REGEX REPLACE ";" " " DOXYGEN_SOURCE_LIST "${DOXYGEN_SOURCE_DIRS}")

	#Parses the Doxygen.cfg file to replace the variables
	configure_file(Doxygen.cfg.cmake ${CMAKE_CURRENT_BINARY_DIR}/Doxygen.cfg)

	#Add a custom target for the doxygen documentation
	add_custom_target(doxygen
					${DOXYGEN} ${CMAKE_CURRENT_BINARY_DIR}/Doxygen.cfg 2>&1 >> Doxygen.log)
	add_dependencies(doc doxygen)
	#Install the produced documentation in doxygen/
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen DESTINATION ${DOC_INSTALL} OPTIONAL)
endif (DOXYGEN)

