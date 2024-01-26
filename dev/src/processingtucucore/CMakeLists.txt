set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(TUCUXI-CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/tucuxi-core)
add_subdirectory(${TUCUXI-CORE_PATH}/src/tucucore tucucore)
add_subdirectory(${TUCUXI-CORE_PATH}/src/tucucrypto tucucrypto)
add_subdirectory(${TUCUXI-CORE_PATH}/src/tucucommon tucucommon)
if(config_sign)
    add_subdirectory(${TUCUXI-CORE_PATH}/src/tucusign tucusign)
endif(config_sign)

### Build
set(PROCESSINGTUCUCORE_HEADER_FILES
    src/drugs2manager.h
    src/guitotucucoretranslator.h
    src/processingtucucore.h
    src/tqflogger.h
    src/tucucoretoguitranslator.h
)

set(PROCESSINGTUCUCORE_SOURCE_FILES
    src/drugs2manager.cpp
    src/guitotucucoretranslator.cpp
    src/processingtucucore.cpp
    src/tqflogger.cpp
    src/tucucoretoguitranslator.cpp
)

add_library(processingtucucore
                STATIC
                    ${PROCESSINGTUCUCORE_HEADER_FILES}
                    ${PROCESSINGTUCUCORE_SOURCE_FILES}
)

target_include_directories(processingtucucore
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/
                PRIVATE
                    ${CORE_INCLUDES}
                    ${TUCUXI-CORE_PATH}
                    ${TUCUXI_BASE_DIR_INCLUDE}
                    ${LIBS_INCLUDES}
)


# Translations
set(translation_files
      trans/processing_fr.ts
)
qt_add_translations(processingtucucore TS_FILES ${translation_files})

target_link_libraries(processingtucucore
                PRIVATE
                    core
                    tucucore
                    tucucommon
                    ${QT_COMPONENTS}
)

# Install rules
include(GNUInstallDirs)
install(TARGETS processingtucucore
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
