include (../tucuxi.pri)

QT += core xml xmlpatterns network
QT -= gui

TARGET   = tucuxi-gui-rest
TEMPLATE = lib
DESTDIR  = $${OUT_PWD}/../$${DIST_DIR}

DEPENDPATH  += model network ..
INCLUDEPATH += ..

SOURCES += \
    builders/formulationandroutetranslator.cpp \
    builders/messagecontent.cpp \
    builders/messagebuilder.cpp \
    builders/querylistmessagebuilder.cpp \
    builders/xmlmessagebuilder.cpp \
    builders/replylistmessagebuilder.cpp \
    builders/acklistmessagebuilder.cpp \
    builders/queryrequestmessagebuilder.cpp \
    builders/replyrequestmessagebuilder.cpp \
    builders/ackrequestmessagebuilder.cpp \
    model/message.cpp \
    model/xmlmessage.cpp \
    model/ingoingxmlmessage.cpp \
    model/outgoingxmlmessage.cpp \
    model/querylistxmlmessage.cpp \
    model/queryrequestxmlmessage.cpp \
    model/replylistxmlmessage.cpp \
    model/replyrequestxmlmessage.cpp \
    model/notificationxmlmessage.cpp \
    model/acklistxmlmessage.cpp \
    model/ackrequestxmlmessage.cpp \
    model/acknotificationxmlmessage.cpp \
    network/networkaccessmanager.cpp \
    network/mirthrequest.cpp \
    network/querylistrequest.cpp \
    network/queryrequestrequest.cpp \
    network/notificationrequest.cpp \
    network/acklistrequest.cpp \
    network/ackrequestrequest.cpp \
    utils/controlid.cpp \
    importexport/xmlresttointerpretationrequest.cpp \
    importexport/interpretationrequesttoxmlrest.cpp \
    builders/interpretationrequestbuilder.cpp \
    builders/drugidtranslator.cpp \
    restlogger.cpp \
    restrequestsclient.cpp \
    builders/routetranslator.cpp \
    requestsclientprocessing.cpp \
    builders/covariateidtranslator.cpp

HEADERS += \
    builders/formulationandroutetranslator.h \
    resterrors.h \
    builders/messagecontent.h \
    builders/messagebuilder.h \
    builders/querylistmessagebuilder.h \
    builders/xmlmessagebuilder.h \
    builders/replylistmessagebuilder.h \
    builders/acklistmessagebuilder.h \
    builders/queryrequestmessagebuilder.h \
    builders/replyrequestmessagebuilder.h \
    builders/ackrequestmessagebuilder.h \
    model/message.h \
    model/xmlmessage.h \
    model/ingoingxmlmessage.h \
    model/outgoingxmlmessage.h \
    model/querylistxmlmessage.h \
    model/queryrequestxmlmessage.h \
    model/replylistxmlmessage.h \
    model/replyrequestxmlmessage.h \
    model/notificationxmlmessage.h \
    model/acklistxmlmessage.h \
    model/ackrequestxmlmessage.h \
    model/acknotificationxmlmessage.h \
    network/networkaccessmanager.h \
    network/mirthrequest.h \
    network/querylistrequest.h \
    network/queryrequestrequest.h \
    network/notificationrequest.h \
    network/acklistrequest.h \
    network/ackrequestrequest.h \
    utils/controlid.h \
    importexport/xmlresttointerpretationrequest.h \
    importexport/interpretationrequesttoxmlrest.h \
    builders/interpretationrequestbuilder.h \
    errors_rest.h \
    builders/drugidtranslator.h \
    restlogger.h \
    restrequestsclient.h \
    builders/routetranslator.h \
    requestsclientprocessing.h \
    builders/covariateidtranslator.h

RESOURCES += \
    resources/rest.qrc

CODECFORTR   += UTF-8
TRANSLATIONS += $${TRANS_DIR}/rest_fr.ts

DEPENDENCIES =  tucuxi-gui-admin tucuxi-gui-core
for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}
