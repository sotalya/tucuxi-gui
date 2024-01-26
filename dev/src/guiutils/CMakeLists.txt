set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

### Build
set(GUIUTILS_HEADER_FILES
    src/controllers/abstractviewcontroller.h
    src/controllers/adjustmenttabcontroller.h
    src/controllers/covariatetabcontroller.h
    src/controllers/dosagetabcontroller.h
    src/controllers/drugtabcontroller.h
    src/controllers/interpretationcontroller.h
    src/controllers/measuretabcontroller.h
    src/controllers/targettabcontroller.h
    src/controllers/validationtabcontroller.h

    src/models/abstractentitylistmodel.h
    src/models/abstractentitymodel.h
    src/models/partialrequestlistmodel.h
    src/models/sortfilterentitylistproxymodel.h

    src/widgets/persistentfiledialog.h
    src/widgets/validatedrugdialog.h

    src/appglobals.h
    src/appmode.h
    src/calculationcontroller.h
    src/chartdatacontroller.h
    src/demorequestfileclient.h
    src/demorequestsclient.h
    src/drugtohtml.h
    src/fakepatientscreator.h
    src/flowcontroller.h
    src/mainwindowcontroller.h
    src/multithreadedcalculationcontroller.h
    src/predictionspeccopier.h
    src/requestscontroller.h
    src/startupscreen.h
    src/startupwindow.h
    src/unavailablefunctionality.h
    src/websocketclientwrapper.h
    src/websockettransport.h

    errors_guiutils.h
)

set(GUIUTILS_SOURCE_FILES
    src/controllers/abstractviewcontroller.cpp
    src/controllers/adjustmenttabcontroller.cpp
    src/controllers/covariatetabcontroller.cpp
    src/controllers/dosagetabcontroller.cpp
    src/controllers/drugtabcontroller.cpp
    src/controllers/interpretationcontroller.cpp
    src/controllers/measuretabcontroller.cpp
    src/controllers/targettabcontroller.cpp
    src/controllers/validationtabcontroller.cpp

    src/models/abstractentitylistmodel.cpp
    src/models/abstractentitymodel.cpp
    src/models/partialrequestlistmodel.cpp
    src/models/sortfilterentitylistproxymodel.cpp

    src/widgets/persistentfiledialog.cpp
    src/widgets/validatedrugdialog.cpp

    src/appglobals.cpp
    src/appmode.cpp
    src/calculationcontroller.cpp
    src/chartdatacontroller.cpp
    src/demorequestfileclient.cpp
    src/demorequestsclient.cpp
    src/drugtohtml.cpp
#    src/ezchart.cpp
    src/fakepatientscreator.cpp
    src/flowcontroller.cpp
    src/mainwindowcontroller.cpp
    src/multithreadedcalculationcontroller.cpp
    src/predictionspeccopier.cpp
    src/requestscontroller.cpp
    src/startupscreen.cpp
    src/startupwindow.cpp
    src/unavailablefunctionality.cpp
    src/websocketclientwrapper.cpp
    src/websockettransport.cpp
)

set(GUIUTILS_RESOURCES_FILES
    qml/qml.qrc

    resources/guiutils.qrc
)

set(GUIUTILS_FORM_FILES
    src/widgets/validatedrugdialog.ui
)

if(config_externalreport)
    # TODO Add externalreport configs
endif(config_externalreport)

add_library(guiutils
                STATIC
                    ${GUIUTILS_HEADER_FILES}
                    ${GUIUTILS_SOURCE_FILES}
                    ${GUIUTILS_RESOURCES_FILES}
                    ${GUIUTILS_FORM_FILES}
)

target_include_directories(guiutils
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/icons
                    ${CMAKE_CURRENT_SOURCE_DIR}/qml
                    ${CMAKE_CURRENT_SOURCE_DIR}/reports
                    ${CMAKE_CURRENT_SOURCE_DIR}/resources
                    ${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets
                PRIVATE
                    ${CORE_INCLUDES}
                    ${ADMIN_INCLUDES}
                    ${APPUTILS_INCLUDES}
                    ${REST_INCLUDES}
                    ${FLATREQUESTS_INCLUDES}
                    ${TUCUXI_BASE_DIR_INCLUDE}
                    ${LIBS_INCLUDES}
)

# Translations
set(translation_files
      trans/guiutils_fr.ts
)
qt_add_translations(guiutils TS_FILES ${translation_files})

target_link_libraries(guiutils
                PRIVATE
                    core
                    admin
                    apputils
                    rest
                    ${QT_COMPONENTS}
)

# Install rules
include(GNUInstallDirs)
install(TARGETS guiutils
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
