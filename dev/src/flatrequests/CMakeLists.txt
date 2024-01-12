set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

### Build
set(FLATREQUESTS_HEADER_FILES
        flatinterpretationrequestbuilder.h
        flatrequestfileclient.h
        flatrequestsclientprocessing.h
)

set(FLATREQUESTS_SOURCE_FILES
        flatinterpretationrequestbuilder.cpp
        flatrequestfileclient.cpp
        flatrequestsclientprocessing.cpp
)

add_library(flatrequests
                STATIC
                    ${FLATREQUESTS_HEADER_FILES}
                    ${FLATREQUESTS_SOURCE_FILES}
)

target_include_directories(flatrequests
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                PRIVATE
                    ${CORE_INCLUDES}
                    ${ADMIN_INCLUDES}
                    ${APPUTILS_INCLUDES}
                    ${REST_INCLUDES}
                    ${TUCUXI_BASE_DIR_INCLUDE}
                    ${LIBS_INCLUDES}
)

# Translations
set(translation_files
      trans/admin_fr.ts
)
qt_add_translations(flatrequests TS_FILES ${translation_files})

target_link_libraries(flatrequests
                PRIVATE
                    core
                    admin
                    apputils
                    rest
                    ${QT_COMPONENTS}
)

# Install rules
include(GNUInstallDirs)
install(TARGETS flatrequests
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
