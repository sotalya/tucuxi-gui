#Use of Qt plugins and Qt SQL
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(${QT_DEFINITIONS})
set(QT_USE_QTSQL true)
include(${LOCAL_QT_USE_FILE})

#Source dirs
include_directories(
	${CMAKE_SOURCE_DIR}/src/core/interfaces
	${CMAKE_SOURCE_DIR}/src/core/utils
	${CMAKE_SOURCE_DIR}/src/core/
	${CMAKE_SOURCE_DIR}/tests/
)

#Sources
set(LOCALDB_HDRS
	localdb.h

)
set(LOCALDB_SRCS
	localdb.cpp
)

#Moc precompiler
QT4_WRAP_CPP(LOCALDB_MOC ${LOCALDB_HDRS})

#Library creation
add_library(localdb SHARED ${LOCALDB_SRCS} ${LOCALDB_MOC})
target_link_libraries(localdb ${QT_LIBRARIES} ezechiel-core ${SPLIB})
   
add_dependencies(localdb databases)
install(TARGETS localdb DESTINATION ${DATABASES_INSTALL})

#Install the local.db file along
install(FILES local.db DESTINATION ${PLUGINSDATA_INSTALL}/ch.heig-vd.ezechiel.localdb/)

#Test part
add_executable(test-localdb EXCLUDE_FROM_ALL localdbtest.cpp ${MSG_HANDLER})
target_link_libraries(test-localdb ${QT_LIBRARIES} localdb ezechiel-core)
add_test(
   NAME LocalDB
   COMMAND ${CMAKE_COMMAND} -E chdir ${TESTS_INSTALL}/plugins/localdb ./test-localdb)
add_custom_command(TARGET test-localdb POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${TESTS_INSTALL}/plugins
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${TESTS_INSTALL}/plugins/localdb
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/localtest.db ${TESTS_INSTALL}/plugins/localdb/local.db
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/definitionstest.xml ${TESTS_INSTALL}/plugins/localdb/definitions.xml
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/test-localdb ${TESTS_INSTALL}/plugins/localdb/
                   )
add_dependencies(auto-tests test-localdb)
