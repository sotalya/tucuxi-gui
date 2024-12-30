set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

### Build
set(EPHEMERALDB_HEADER_FILES
    ephemeraldb.h
)

set(EPHEMERALDB_SOURCE_FILES
    ephemeraldb.cpp
)

add_library(ephemeraldb
                SHARED
                    ${EPHEMERALDB_HEADER_FILES}
                    ${EPHEMERALDB_SOURCE_FILES}
)

target_include_directories(ephemeraldb
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                PRIVATE
                    ${APPUTILS_INCLUDES}
                    ${ADMIN_INCLUDES}
                    ${CORE_INCLUDES}
                    ${TUCUXI_BASE_DIR_INCLUDE}
                    ${LIBS_INCLUDES}
)

# Translations
set(translation_files
      trans/ephemeraldb_fr.ts
)
qt_add_translations(ephemeraldb TS_FILES ${translation_files})

target_link_libraries(ephemeraldb
                PRIVATE
                    #xerces
                    apputils
                    admin
                    core
                    tucucore
                    tucucommon
                    ${QT_COMPONENTS}
)

# Install rules
include(GNUInstallDirs)
install(TARGETS ephemeraldb
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
