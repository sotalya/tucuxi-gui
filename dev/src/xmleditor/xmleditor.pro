#EzeCHieL configuration
include(../tucuxi.pri)

TARGET      = editor
TEMPLATE    = app
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}

QT += widgets gui

##################################
# REMOVE THE ../gui/* DEPENDENCIES
# IN INCLUDEPATH, SOURCES, HEADERS
##################################

#DEPENDPATH += \
#    models \
#    parsers \
#    utils \
#    widgets \
#    logic
INCLUDEPATH += ../gui/interfaces \
    models \
    parsers \
    utils \
    widgets \
    logic \
    ./ \
    ../ \
    ../core \
    ../core/config \
    ../core/security \
    ../core/interfaces \
    ../core/parsers \
    ../core/utils

SOURCES += ../gui/interfaces/ezpanel.cpp \
    main.cpp \
    editorpanel.cpp \
    xmleditormessagehandler.cpp \
    xmleditorerrors.cpp \
    models/domitemmodel.cpp \
    models/domitem.cpp \
    models/domitemfactory.cpp \
    models/inneritem.cpp \
    models/outeritem.cpp \
    models/drugitem.cpp \
    models/dispositionitem.cpp \
    models/intakeitem.cpp \
    models/comethitem.cpp \
    models/headitem.cpp \
    models/historyitem.cpp \
    models/creatoritem.cpp \
    models/revisionsitem.cpp \
    models/revisionitem.cpp \
    models/dosagesitem.cpp \
    models/dosageitem.cpp \
    models/dispositionsitem.cpp \
    models/comethsitem.cpp \
    models/intakesitem.cpp \
    models/intervalsitem.cpp \
    models/targetsitem.cpp \
    models/targetitem.cpp \
    models/errormodelitem.cpp \
    models/parametersitem.cpp \
    models/parameteritem.cpp \
    models/correlationsitem.cpp \
    models/correlationitem.cpp \
    models/covariatesitem.cpp \
    models/covariateitem.cpp \
    models/operationsitem.cpp \
    models/operationitem.cpp \
    models/commentitem.cpp \
    widgets/domitemwidget.cpp \
    widgets/stackeddomitemwidget.cpp \
    widgets/domitemwidgetfactory.cpp \
    widgets/emptyitemwidget.cpp \
    widgets/drugheaditemwidget.cpp \
    widgets/dispositionheaditemwidget.cpp \
    widgets/intakeheaditemwidget.cpp \
    widgets/comethheaditemwidget.cpp \
    widgets/dosageitemwidget.cpp \
    widgets/targetitemwidget.cpp \
    widgets/errormodelitemwidget.cpp \
    widgets/parameteritemwidget.cpp \
    widgets/correlationitemwidget.cpp \
    widgets/operationitemwidget.cpp \
    widgets/covariateitemwidget.cpp \
    widgets/commentitemwidget.cpp \
    widgets/unitcombobox.cpp \
    widgets/translationwidget.cpp \
    widgets/intervalsitemwidget.cpp \
    widgets/durationspinbox.cpp \
    widgets/translatableplaintextedit.cpp \
    widgets/translatablelineedit.cpp

HEADERS += ../gui/interfaces/ezpanel.h \
    editorpanel.h \
    xmleditormessagehandler.h \
    xmleditorerrors.h \
    models/domitemmodel.h \
    models/domitem.h \
    models/domitemfactory.h \
    models/inneritem.h \
    models/outeritem.h \
    models/drugitem.h \
    models/dispositionitem.h \
    models/intakeitem.h \
    models/comethitem.h \
    models/headitem.h \
    models/historyitem.h \
    models/creatoritem.h \
    models/revisionsitem.h \
    models/revisionitem.h \
    models/dosagesitem.h \
    models/dosageitem.h \
    models/dispositionsitem.h \
    models/comethsitem.h \
    models/intakesitem.h \
    models/intervalsitem.h \
    models/targetsitem.h \
    models/targetitem.h \
    models/errormodelitem.h \
    models/parametersitem.h \
    models/parameteritem.h \
    models/correlationsitem.h \
    models/correlationitem.h \
    models/covariatesitem.h \
    models/covariateitem.h \
    models/operationsitem.h \
    models/operationitem.h \
    models/commentitem.h \
    widgets/domitemwidget.h \
    widgets/stackeddomitemwidget.h \
    widgets/domitemwidgetfactory.h \
    widgets/emptyitemwidget.h \
    widgets/drugheaditemwidget.h \
    widgets/dispositionheaditemwidget.h \
    widgets/intakeheaditemwidget.h \
    widgets/comethheaditemwidget.h \
    widgets/dosageitemwidget.h \
    widgets/targetitemwidget.h \
    widgets/errormodelitemwidget.h \
    widgets/parameteritemwidget.h \
    widgets/correlationitemwidget.h \
    widgets/operationitemwidget.h \
    widgets/covariateitemwidget.h \
    widgets/commentitemwidget.h \
    widgets/unitcombobox.h \
    widgets/translationwidget.h \
    widgets/intervalsitemwidget.h \
    widgets/durationspinbox.h \
    widgets/translatableplaintextedit.h \
    widgets/translatablelineedit.h

FORMS += \
    editorpanel.ui \
    widgets/drugheaditemwidget.ui \
    widgets/dispositionheaditemwidget.ui \
    widgets/intakeheaditemwidget.ui \
    widgets/comethheaditemwidget.ui \
    widgets/dosageitemwidget.ui \
    widgets/targetitemwidget.ui \
    widgets/errormodelitemwidget.ui \
    widgets/parameteritemwidget.ui \
    widgets/correlationitemwidget.ui \
    widgets/operationitemwidget.ui \
    widgets/covariateitemwidget.ui \
    widgets/commentitemwidget.ui \
    widgets/intervalsitemwidget.ui \
    widgets/emptyitemwidget.ui

RESOURCES += \
    resources/editor.qrc

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/xmleditor_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-core
for(dep, DEPENDANCIES) {
   LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../translation.pri)

#Third party shared libraries
include(../qwt.pri)
include(../botan.pri)
