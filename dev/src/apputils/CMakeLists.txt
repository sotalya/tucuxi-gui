set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

### Build
set(APPUTILS_HEADER_FILES
    ../current_version.h
    
    src/appcore.h
    src/apputilsfactory.h
    src/apputilsrepository.h
    src/calculationrequestlogger.h
    src/chartdataexporter.h
    src/druglister.h
    src/drugmanager.h
    src/modelvstreatmentcompatibilitychecker.h
    src/predictionspecexporter.h
    src/predictionspecimporter.h
    src/user.h
    
    errors_apputils.h
)

set(APPUTILS_SOURCE_FILES
    src/appcore.cpp
    src/calculationrequestlogger.cpp
    src/chartdataexporter.cpp
    src/druglister.cpp
    src/drugmanager.cpp
    src/modelvstreatmentcompatibilitychecker.cpp
    src/predictionspecexporter.cpp
    src/predictionspecimporter.cpp
    src/user.cpp
)

if(NOT config_nobotan)
    set(APPUTILS_HEADER_FILES
        ${APPUTILS_HEADER_FILES}
        src/datacrypter.h
        src/securitymanager.h
    )

    set(APPUTILS_SOURCE_FILES
        ${APPUTILS_SOURCE_FILES}
        src/datacrypter.cpp
        src/securitymanager.cpp
    )
endif(NOT config_nobotan)

add_library(apputils 
                STATIC
                    ${APPUTILS_HEADER_FILES}
                    ${APPUTILS_SOURCE_FILES}
)

target_include_directories(apputils
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/dal
                PRIVATE
                    ${CORE_INCLUDES}
                    ${TUCUXI_BASE_DIR_INCLUDE}
                    ${LIBS_INCLUDES}
)

# Translations
set(translation_files
      trans/apputils_fr.ts
)
qt_add_translations(apputils TS_FILES ${translation_files})

target_link_libraries(apputils
                PRIVATE
                    core
                    botan
                    ${QT_COMPONENTS}
)

# Install rules
include(GNUInstallDirs)
install(TARGETS apputils
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
