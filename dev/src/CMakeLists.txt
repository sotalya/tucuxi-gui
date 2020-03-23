#Doxygen
add_subdirectory(doc)

#Sub-directories
add_subdirectory(core)
add_subdirectory(databases)
add_subdirectory(plugins)
add_subdirectory(chart)
add_subdirectory(report)
add_subdirectory(cliutils)
add_subdirectory(gui)
add_subdirectory(cli)

#Install the drug directory, containing the XML
install(DIRECTORY drugs/ DESTINATION ${DRUGS_INSTALL} OPTIONAL)
