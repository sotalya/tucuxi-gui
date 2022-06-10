//@@license@@

#include "errors_drugeditor.h"
#include "gui/errors_gui.h"

#include "models/sortfilterentitylistproxymodel.h"

#include "undoredocontroller.h"

#include "models/covariatelistmodel.h"
#include "models/covariatemodel.h"
#include "models/domainlistmodel.h"
#include "models/druglistmodel.h"
#include "models/drugvariatelistmodel.h"
#include "models/sortfilterentitylistproxymodel.h"
#include "models/studylistmodel.h"
#include "models/targetlistmodel.h"
#include "models/targetmodel.h"

#include "models/editorlistmodel.h"
#include "models/editormodel.h"
#include "models/doselistmodel.h"

#include "admin/src/dal/validationstatus.h"

////probably temp ###############################
//#include "processing/src/processingmath.h"

#include "apputils/src/appcore.h"
#include "apputils/src/apputilsfactory.h"
////end of probably temp ########################


#include "apputils/src/apputilsrepository.h"

#include "admin/src/adminfactory.h"


#include "core/dal/drug/drug.h"
#include "core/dal/dosage.h"
#include "core/dal/covariate.h"
#include "core/utils/logging.h"
#include "apputils/src/drugxmlexport.h"
#include "apputils/src/drugxmlimport.h"

#include <QApplication>
#include <QObject>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>

#include <QDebug>

void parseOptions();
void initResources();

#include "core/dal/drug/drug.h"

#include <iostream>


#define QML_CONTROLS_NAMESPACE "drugeditor.qml.controls"


//#include "QObject"


void test()
{
    QString inputFileName = "filename.xml";
    QString outputFileName = "filenameout.xml";

    DrugXmlImport importer;

    ezechiel::GuiCore::DrugModel *drug = importer.load(inputFileName);
    if (!drug) {
        std::cout << "Can not import" << std::endl;
        return;
    }
    DrugXmlExport exporter;
    if (!exporter.save(drug,outputFileName)) {
        std::cout << "Can not export";
        return;
    }

    // Ready to compare the files



}

#define CHECK_REGISTER(registering) \
    if ((registering)== -1 ) { \
        std::cout << "Can not register"; \
        exit(0); \
    }

int main(int argc, char *argv[])
{



    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/FlowInformationPanel.qml"),QML_CONTROLS_NAMESPACE , 1, 0, "FlowInformationPanel"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/Flow.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "Flow"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/FlowMenu.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "FlowMenu"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/FlowMenuButton.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "FlowMenuButton"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/FlowTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "FlowTab"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/CommentsView.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "CommentsView"));


    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/RequestList.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "RequestList"));


    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering DAL objects.");
    CHECK_REGISTER(qmlRegisterUncreatableType<UndoRedoController>("ezechiel", 1, 0, "UndoRedoController", QObject::tr("Cannot instantiate type 'UndoRedoController'")));



    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering model objects.");
    CHECK_REGISTER(qmlRegisterUncreatableType<CovariateListModel>("ezechiel", 1, 0, "CovariateListModel", QObject::tr("Cannot instantiate type 'CovariateListModel'")));
    CHECK_REGISTER(qmlRegisterUncreatableType<CovariateModel>("ezechiel", 1, 0, "CovariateModel", QObject::tr("Cannot instantiate type 'CovariateModel'")));
    CHECK_REGISTER(qmlRegisterUncreatableType<DomainListModel>("ezechiel", 1, 0, "DomainListModel", QObject::tr("Cannot instantiate type 'DomainListModel'")));
    CHECK_REGISTER(qmlRegisterUncreatableType<DoseListModel>("ezechiel", 1, 0, "DosageListModel", QObject::tr("Cannot instantiate type 'DosageListModel'")));
//    CHECK_REGISTER(qmlRegisterUncreatableType<DoseModel>("ezechiel", 1, 0, "DosageModel", QObject::tr("Cannot instantiate type 'DosageModel'")));
    CHECK_REGISTER(qmlRegisterUncreatableType<DrugListModel>("ezechiel", 1, 0, "DrugListModel", QObject::tr("Cannot instantiate type 'DrugListModel'")));
    CHECK_REGISTER(qmlRegisterUncreatableType<DrugVariateListModel>("ezechiel", 1, 0, "DrugVariateListModel", QObject::tr("Cannot instantiate type 'DrugVariateListModel'")));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::DrugModelList*>("DrugList*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::DrugModel*>("Drug*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::MetaData*>("MetaData*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::EditorList*>("EditorList*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::TranslatableString*>("TranslatableString*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::ValidDoses*>("ValidDoses*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::DoseList*>("DoseList*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::IdentifiableAmount*>("IdentifiableAmount*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::IntervalList*>("IntervalList*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::ValidIntervals*>("ValidIntervals*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::InfusionList*>("InfusionList*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::ValidInfusions*>("ValidInfusions*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::TargetList*>("TargetList*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::ADME*>("ADME*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::Halflife*>("Halflife*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::ErrorModel*>("ErrorModel*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::Parameters*>("Parameters*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::Bsv*>("Bsv*"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::DrugVariateList*>("DrugVariateList*"));
    CHECK_REGISTER(qRegisterMetaType<QMetaType::Type>("Type"));
    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::DrugVariate*>("DrugVariate*"));

    CHECK_REGISTER(qRegisterMetaType<ezechiel::GuiCore::Unit>("Unit"));

    //    qRegisterMetaType<ezechiel::GuiCore::TranslatableStringList*>("TranslatableString*");
    //    qmlRegisterUncreatableType<InstituteListModel>("ezechiel", 1, 0, "InstituteListModel", QObject::tr("Cannot instantiate type 'InstituteListModel'"));
//    qmlRegisterUncreatableType<CommentListModel>("ezechiel",1 ,0 ,"editor");



    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering other types.");
    CHECK_REGISTER(qmlRegisterType<ProxyModelFilter>("ezechiel", 1, 0, "ProxyModelFilter"));



    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering QML components.");
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/AdjustmentTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "AdjustmentTab"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/ApplicationBar.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "ApplicationBar"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/ApplicationBarButton.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "ApplicationBarButton"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/Chart.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "Chart"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/CovariateTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "CovariateTab"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/DatePicker.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "DatePicker"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/DemoBanner.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "DemoBanner"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/DosageTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "DosageTab"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/DrugTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "DrugTab"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/FlowInformationPanel.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "FlowInformationPanel"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/Flow.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "Flow"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/FlowMenu.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "FlowMenu"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/FlowMenuButton.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "FlowMenuButton"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/FlowTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "FlowTab"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/MeasureTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "MeasureTab"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/RequestList.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "RequestList"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/TargetTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "TargetTab"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/TimePicker.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "TimePicker"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/ValidationTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "ValidationTab"));
    CHECK_REGISTER(qmlRegisterType(QUrl("qrc:/editorControls/HomeTab.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "HomeTab"));




    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering QML singletons.");
    CHECK_REGISTER(qmlRegisterSingletonType(QUrl("qrc:/styles/Style.qml"), "guiutils.qml.styles", 1, 0, "Style"));
//    CHECK_REGISTER(qmlRegisterSingletonType(QUrl("qrc:/controls/Status.qml"), QML_CONTROLS_NAMESPACE, 1, 0, "Status"));
    qmlRegisterUncreatableType<ValidationStatus>("ezechiel", 1, 0, "Status", QObject::tr("Cannot instantiate type 'Status'"));



    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("EzeCHieL Drug Editor");
    QCoreApplication::setApplicationVersion("0.1.0");


    Q_INIT_RESOURCE(qml);
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Qml.");

    Q_INIT_RESOURCE(guiutils);
    Q_INIT_RESOURCE(core);


    initResources();
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Resources.");



    UndoRedoController *urc = new UndoRedoController();

    urc->setResizeMode(QQuickView::SizeRootObjectToView);

    urc->show();


    return app.exec();
}

void parseOptions()
{
}

void initResources()
{
    ezechiel::GuiCore::Core::setInstance(new ezechiel::GuiAppUtils::AppCore());

//    CORE->setProcessingInterface(new ProcessingMath());
    APPCORE->drugManager();


}

void resetStaticModel(){

    QScopedPointer<QList<ezechiel::GuiCore::DrugModel*>> drugs(new QList<ezechiel::GuiCore::DrugModel*>());


    if (APPUTILSREPO->getDrugsList(*drugs.data()).error != ezechiel::GuiCore::NoError)
        EXLOG(QtFatalMsg, ezechiel::gui::NOEZERROR, "Drugs list not retreived."); //FIXME wrong error

//    patientListModel->setModelData(patients.data());
//    drugListModel->setModelData(drugs.data());
}

