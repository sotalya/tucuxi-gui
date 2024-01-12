set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

### Build
set(REST_HEADER_FILES
    builders/acklistmessagebuilder.h
    builders/ackrequestmessagebuilder.h
    builders/covariateidtranslator.h
    builders/drugidtranslator.h
    builders/formulationandroutetranslator.h
    builders/interpretationrequestbuilder.h
    builders/messagebuilder.h
    builders/messagecontent.h
    builders/querylistmessagebuilder.h
    builders/queryrequestmessagebuilder.h
    builders/replylistmessagebuilder.h
    builders/replyrequestmessagebuilder.h
    builders/routetranslator.h
    builders/xmlmessagebuilder.h

    importexport/interpretationrequesttoxmlrest.h
    importexport/xmlresttointerpretationrequest.h

    model/acklistxmlmessage.h
    model/acknotificationxmlmessage.h
    model/ackrequestxmlmessage.h
    model/ingoingxmlmessage.h
    model/message.h
    model/notificationxmlmessage.h
    model/outgoingxmlmessage.h
    model/querylistxmlmessage.h
    model/queryrequestxmlmessage.h
    model/replylistxmlmessage.h
    model/replyrequestxmlmessage.h
    model/xmlmessage.h

    network/acklistrequest.h
    network/ackrequestrequest.h
    network/mirthrequest.h
    network/networkaccessmanager.h
    network/notificationrequest.h
    network/querylistrequest.h
    network/queryrequestrequest.h

    utils/controlid.h

    errors_rest.h
    requestsclientprocessing.h
    resterrors.h
    restlogger.h
    restrequestsclient.h
)

set(REST_SOURCE_FILES
    builders/acklistmessagebuilder.cpp
    builders/ackrequestmessagebuilder.cpp
    builders/covariateidtranslator.cpp
    builders/drugidtranslator.cpp
    builders/formulationandroutetranslator.cpp
    builders/interpretationrequestbuilder.cpp
    builders/messagebuilder.cpp
    builders/messagecontent.cpp
    builders/querylistmessagebuilder.cpp
    builders/queryrequestmessagebuilder.cpp
    builders/replylistmessagebuilder.cpp
    builders/replyrequestmessagebuilder.cpp
    builders/routetranslator.cpp
    builders/xmlmessagebuilder.cpp

    importexport/interpretationrequesttoxmlrest.cpp
    importexport/xmlresttointerpretationrequest.cpp

    model/acklistxmlmessage.cpp
    model/acknotificationxmlmessage.cpp
    model/ackrequestxmlmessage.cpp
    model/ingoingxmlmessage.cpp
    model/message.cpp
    model/notificationxmlmessage.cpp
    model/outgoingxmlmessage.cpp
    model/querylistxmlmessage.cpp
    model/queryrequestxmlmessage.cpp
    model/replylistxmlmessage.cpp
    model/replyrequestxmlmessage.cpp
    model/xmlmessage.cpp

    network/acklistrequest.cpp
    network/ackrequestrequest.cpp
    network/mirthrequest.cpp
    network/networkaccessmanager.cpp
    network/notificationrequest.cpp
    network/querylistrequest.cpp
    network/queryrequestrequest.cpp

    utils/controlid.cpp

    requestsclientprocessing.cpp
    restlogger.cpp
    restrequestsclient.cpp
)

set(REST_RESOURCES_FILES
    resources/rest.qrc
)

add_library(rest
                STATIC
                    ${REST_HEADER_FILES}
                    ${REST_SOURCE_FILES}
                    ${REST_RESOURCES_FILES}
)

target_include_directories(rest
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/builders/
                    ${CMAKE_CURRENT_SOURCE_DIR}/importexport/
                    ${CMAKE_CURRENT_SOURCE_DIR}/model/
                    ${CMAKE_CURRENT_SOURCE_DIR}/network/
                    ${CMAKE_CURRENT_SOURCE_DIR}/resources/
                    ${CMAKE_CURRENT_SOURCE_DIR}/utils/
                PRIVATE
                    ${ADMIN_INCLUDES}
                    ${CORE_INCLUDES}
                    ${TUCUXI_BASE_DIR_INCLUDE}
                    ${LIBS_INCLUDES}
)

target_link_libraries(rest
                PRIVATE
                    core
                    admin
                    ${QT_COMPONENTS}
)

# Install rules
include(GNUInstallDirs)
install(TARGETS rest
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
