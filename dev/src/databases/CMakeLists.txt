set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

## Only ephemeraldb have a CMakeList for Qt6, the other are for Qt4
# add_subdirectory(localdb/)
# add_subdirectory(genericlocaldb/)
add_subdirectory(ephemeraldb/)
# add_subdirectory(mysqldb/)

get_target_property(DB_INCLUDES ephemeraldb INTERFACE_INCLUDE_DIRECTORIES)

add_library(databases INTERFACE)
target_include_directories(databases
                INTERFACE
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${DB_INCLUDES}
)

target_link_libraries(databases INTERFACE ephemeraldb)
