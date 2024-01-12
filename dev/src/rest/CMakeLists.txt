set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

### Build
set(ADMIN_HEADER_FILES
    src/dal/clinical.h
    src/dal/curvetypes.h
    src/dal/email.h
    src/dal/graphinformationselection.h
    src/dal/institute.h
    src/dal/interpretation.h
    src/dal/interpretationanalysis.h
    src/dal/interpretationrequest.h
    src/dal/location.h
    src/dal/measure.h
    src/dal/partialrequest.h
    src/dal/patient.h
    src/dal/person.h
    src/dal/phone.h
    src/dal/practician.h
    src/dal/sentencepalette.h
    src/dal/steptypes.h
    src/dal/validationstatus.h
    
    src/adminfactory.h
    src/dataxmlexport.h
    src/interpretationtorequestxmlexport.h
    src/interpretationxmlexport.h
    src/interpretationxmlimport.h
    src/requestsclient.h
    src/stdadminrepository.h
    
    errors_admin.h
)

set(ADMIN_SOURCE_FILES
    src/dal/clinical.cpp
    src/dal/email.cpp
    src/dal/graphinformationselection.cpp
    src/dal/institute.cpp
    src/dal/interpretation.cpp
    src/dal/interpretationanalysis.cpp
    src/dal/interpretationrequest.cpp
    src/dal/location.cpp
    src/dal/measure.cpp
    src/dal/partialrequest.cpp
    src/dal/patient.cpp
    src/dal/person.cpp
    src/dal/phone.cpp
    src/dal/practician.cpp
    src/dal/sentencepalette.cpp
    src/dal/validationstatus.cpp
    
    src/dataxmlexport.cpp
    src/interpretationtorequestxmlexport.cpp
    src/interpretationxmlexport.cpp
    src/interpretationxmlimport.cpp
    src/requestsclient.cpp
)

add_library(admin 
                STATIC
                    ${ADMIN_HEADER_FILES}
                    ${ADMIN_SOURCE_FILES}
)

target_include_directories(admin
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
      trans/admin_fr.ts
)
qt_add_translations(admin TS_FILES ${translation_files})

target_link_libraries(admin
                PRIVATE
                    core
                    ${QT_COMPONENTS}
)

# Install rules
include(GNUInstallDirs)
install(TARGETS admin
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
