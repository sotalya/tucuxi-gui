//@@license@@

#include <QApplication>
#include <QObject>
#include <QtQml>
#include <QQuickView>
#include <QObject>
#include <QtQml>
#include <QtWebEngineCore/QtWebEngineCore>
#include <QSystemTrayIcon>

#include "errors_gui.h"
#include "restconfigdialog.h"
#include "rest/network/mirthrequest.h"
#include "rest/network/networkaccessmanager.h"

#include "core/settings.h"
#include "core/dal/chartdata.h"
#include "core/dal/drug/drug.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/dosage.h"
#include "core/dal/covariate.h"
#include "core/dal/predictionspec.h"
#include "core/dal/prediction.h"
#include "core/dal/predictionresult.h"
#include "core/dal/predictive.h"
#include "core/dal/predictiondata.h"
#include "core/dal/drug/halflife.h"
#include "core/dal/drug/concentrations.h"
#include "core/dal/drug/results.h"
#include "core/dal/drug/standardtreatment.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/parameters.h"
#include "core/dal/drug/references.h"
#include "core/dal/drug/editors.h"
#include "core/dal/drug/bsv.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/operation.h"
#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/metadata.h"
#include "core/dal/uncastedvalue.h"
#include "core/dal/percentilepair.h"
#include "admin/src/dal/interpretation.h"
#include "core/utils/logging.h"
#include "core/dal/drug/translatablestring.h"

#include "admin/src/dal/validationstatus.h"

#include "admin/src/adminfactory.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/measure.h"
#include "admin/src/dal/interpretationrequest.h"
#include "admin/src/dal/steptypes.h"
#include "admin/src/dal/clinical.h"
#include "admin/src/requestsclient.h"
#include "admin/src/dal/interpretationanalysis.h"

#include "guiutils/src/requestscontroller.h"

#include "core/processingfactory.h"

#ifdef CONFIG_CONNECTED
#include "rest/network/mirthrequest.h"
#endif

#include "apputils/src/appcore.h"
#include "apputils/src/apputilsfactory.h"

#include "guiutils/src/mainwindowcontroller.h"
#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/measuretabcontroller.h"
#include "guiutils/src/controllers/dosagetabcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "guiutils/src/controllers/covariatetabcontroller.h"
#include "guiutils/src/controllers/adjustmenttabcontroller.h"
#include "guiutils/src/controllers/drugtabcontroller.h"

#include "guiutils/src/models/partialrequestlistmodel.h"
#include "guiutils/src/models/sortfilterentitylistproxymodel.h"

#include "guiutils/src/appmode.h"
#include "guiutils/src/appglobals.h"

#include "rest/restlogger.h"

#ifndef NOSTARTUPSCREEN
#include "guiutils/src/startupscreen.h"
#endif // NOSTARTUPSCREEN

#include "guiutils/src/startupwindow.h"

#include "processingtucucore/src/tqflogger.h"

#include "core/utils/logging.h"

#ifndef NOLICENSE
#ifdef COMPILE_WITH_TUCUCORE
#include "tucucrypto/licensemanager.h"
#else
#include "src/fromtucuxi/licensemanager.h"
#endif // COMPILE_WITH_TUCUCORE
#endif // NOLICENSE

#ifdef COMPILE_WITH_TUCUCORE
#include "tucucore/version.h"
#endif // COMPILE_WITH_TUCUCORE

#include "licensedialog.h"

void parseOptions();
void initResources();

#ifdef CONFIG_CONNECTED
void initRestConfig();
#endif // CONFIG_CONNECTED

#ifdef CONFIG_GUITEST
#include "../test/gui/src/guitest.h"
#endif // CONFIG_GUITEST

// YTA: Demo of a subclass outside core that uses the factory
/*
class NewBsvCreator;

class NewBsv: public Tucuxi::Gui::Core::Bsv
{
    SUBENTITY_UTILS(NewBsv, Tucuxi::Gui::Core::Bsv)

protected:
    NewBsv(AbstractRepository *repo, QObject *parent) : Tucuxi::Gui::Core::Bsv(repo, parent) { std::cout << "********************\nI am a new Bsv\n****************\n";}
};
*/

/*
#include <signal.h>
#include <iostream>
#include <cstdlib>

// Print a message when Tucuxi is stopped by a signal
void signalHandler (int id)
{
//    std::cout << "sig handler " << qPrintable(QObject::tr("Interrupted by signal %1").arg(id)) << std::endl;
}
*/


int main(int argc, char *argv[])
{
/*
    // Catch the interruption signals
    signal(SIGABRT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGFPE, signalHandler);
*/

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);

    QApplication app(argc, argv);

    // This trick allows to handle decimal numbers correctly on non-english OS
    // It could be handled differently by correcting some QML files, like
    // CovariateDialog.qml, MeasureDialog.qml and EntityTextValueEntry.qml
    QLocale::setDefault(QLocale("en_US"));

/*
 * For the day we want to play with the system tray
 *
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        auto systemTrayIcon = new QSystemTrayIcon();
        systemTrayIcon->setIcon(QIcon(":/icons/logo.ico"));
        systemTrayIcon->setVisible(true);
    }
*/

#ifdef QT_DEBUG
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "23654");
#endif
    // YTA: Demo of a subclass outside core that uses the factory
//    Tucuxi::Gui::Core::Bsv::getFactory()->registerCreator(new NewBsv::NewBsvCreator());

    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, QString("Built from Gui Git revision %1").arg(GIT_REVISION));
#ifdef COMPILE_WITH_TUCUCORE
    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, QString("Built from Tucucore Git revision %1").arg(QString::fromStdString(Tucuxi::Core::Version::getGitRevision())));
#endif // COMPILE_WITH_TUCUCORE


    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Registering DAL objects.");

    // Set settings info before CORE is called
    QCoreApplication::setOrganizationName("HEIG-VD");
//    QSettings::setDefaultFormat(QSettings::Format::IniFormat);

    Tucuxi::Gui::GuiUtils::AppMode* appMode = Tucuxi::Gui::GuiUtils::AppMode::getInstance();

    if(appMode->isDemo()){
        QCoreApplication::setApplicationName("Tucuxi - Demo version - Not intended for medical use");
    }
    else{
        QCoreApplication::setApplicationName("Tucuxi - Educational version - Not intended for medical use");
    }

    QCoreApplication::setApplicationVersion("0.3.0");

    CORE->setRunningMode(Tucuxi::Gui::Core::Core::GUI);

    qRegisterMetaType<Tucuxi::Gui::Core::PredictionResult*>("PredictionResult*");
    qRegisterMetaType<Tucuxi::Gui::Core::PercentileData*>("PercentilePair*");
    qRegisterMetaType<Tucuxi::Gui::Core::PercentileDataList*>("PercentileDataList*");
    qRegisterMetaType<Tucuxi::Gui::Core::Predictive*>("Predictive*");
    qRegisterMetaType<Tucuxi::Gui::Core::PredictionData*>("PredictionData*");
    qRegisterMetaType<Tucuxi::Gui::Core::Points*>("Points*");
    qRegisterMetaType<Tucuxi::Gui::Core::ChartData*>("ChartData*");
    qRegisterMetaType<QList<QObject*>*>("QList<QObject*>*");
    qRegisterMetaType<Tucuxi::Gui::Admin::InterpretationRequest*>("InterpretationRequest*");
    qRegisterMetaType<Tucuxi::Gui::Core::Duration>("Duration");
    qRegisterMetaType<Tucuxi::Gui::Admin::Patient*>("Patient*");
    qRegisterMetaType<Tucuxi::Gui::Core::Descriptor>("Descriptor");
    qRegisterMetaType<QMetaType::Type>("QMetaType::Type");
//    qRegisterMetaType<Tucuxi::Gui::Core::QStringTranslation>("QStringTranslation");
    qRegisterMetaType<Tucuxi::Gui::Core::TranslatableString*>("TranslatableString*");
    qRegisterMetaType<Tucuxi::Gui::Admin::Person*>("Person*");
    qRegisterMetaType<Tucuxi::Gui::Admin::Location*>("Location*");
    qRegisterMetaType<Tucuxi::Gui::Admin::Practician*>("Practician*");
    qRegisterMetaType<Tucuxi::Gui::Core::DrugModel*>("DrugModel*");
    qRegisterMetaType<Tucuxi::Gui::Admin::InterpretationAnalysis*>("InterpretationAnalysis*");
    qRegisterMetaType<Tucuxi::Gui::Core::TargetList*>("TargetList*");
    qRegisterMetaType<Tucuxi::Gui::Core::Unit>("Unit");
    qRegisterMetaType<Tucuxi::Gui::Core::DrugModelList*>("DrugList*");
    qRegisterMetaType<Tucuxi::Gui::Core::ActiveSubstance*>("ActiveSubstance*");
    qRegisterMetaType<Tucuxi::Gui::Core::ActiveSubstanceList*>("ActiveSubstanceList*");
    qRegisterMetaType<Tucuxi::Gui::Core::LightActiveSubstance*>("LightActiveSubstance*");
    qRegisterMetaType<Tucuxi::Gui::Core::LightActiveSubstanceList*>("LightActiveSubstanceList*");
    qRegisterMetaType<Tucuxi::Gui::Core::CoreMeasure*>("CoreMeasure*");
    qRegisterMetaType<Tucuxi::Gui::Core::CoreMeasureList*>("CoreMeasureList*");
    qRegisterMetaType<Tucuxi::Gui::Admin::PhoneList*>("PhoneList*");
    qRegisterMetaType<Tucuxi::Gui::Admin::Phone*>("Phone*");
    qRegisterMetaType<Tucuxi::Gui::Core::PatientVariate*>("PatientVariate*");
    qRegisterMetaType<Tucuxi::Gui::Core::DrugVariate*>("DrugVariate*");
    qRegisterMetaType<Tucuxi::Gui::Core::DrugTreatment*>("DrugTreatment*");
    qRegisterMetaType<Tucuxi::Gui::Core::PredictionSpec*>("PredictionSpec*");
    qRegisterMetaType<Tucuxi::Gui::Core::PatientVariateList*>("PatientVariateList*");
    qRegisterMetaType<Tucuxi::Gui::Core::Dosage*>("Dosage*");
    qRegisterMetaType<Tucuxi::Gui::Core::AdjustmentDosage*>("AdjustmentDosage*");
    qRegisterMetaType<Tucuxi::Gui::Core::DosageHistory*>("DosageHistory*");
    qRegisterMetaType<Tucuxi::Gui::Core::CorePatient*>("CorePatient*");
    qRegisterMetaType<Tucuxi::Gui::Core::CorePatientList*>("CorePatientList*");
    qRegisterMetaType<Tucuxi::Gui::Core::ADME*>("ADME*");
    qRegisterMetaType<Tucuxi::Gui::Core::Admin*>("Admin*");
    qRegisterMetaType<Tucuxi::Gui::Core::AdminList*>("AdminList*");
    qRegisterMetaType<Tucuxi::Gui::Core::Admin::Route>("ADME::Route");
    qRegisterMetaType<Tucuxi::Gui::Core::Amount>("Amount");
    qRegisterMetaType<Tucuxi::Gui::Core::Reference*>("Reference*");
    qRegisterMetaType<Tucuxi::Gui::Core::ReferenceList*>("ReferenceList*");
    qRegisterMetaType<Tucuxi::Gui::Core::Editor*>("Editor*");
    qRegisterMetaType<Tucuxi::Gui::Core::EditorList*>("EditorList*");
    // qRegisterMetaType<Tucuxi::Gui::Core::EditorSet*>("EditorSet*");
    qRegisterMetaType<Tucuxi::Gui::Core::IdentifiableAmount*>("IdentifiableAmount*");
    qRegisterMetaType<Tucuxi::Gui::Core::Halflife*>("Halflife*");
    qRegisterMetaType<Tucuxi::Gui::Core::Concentrations*>("Concentrations*");
    qRegisterMetaType<Tucuxi::Gui::Core::Results*>("Results*");
    qRegisterMetaType<Tucuxi::Gui::Core::ValidDoses*>("ValidDoses*");
    qRegisterMetaType<Tucuxi::Gui::Core::ValidIntervals*>("ValidIntervals*");
    qRegisterMetaType<Tucuxi::Gui::Core::ValidInfusions*>("ValidInfusions*");
    qRegisterMetaType<Tucuxi::Gui::Core::DrugVariateList*>("DrugVariateList*");
    qRegisterMetaType<Tucuxi::Gui::Core::Target*>("Target*");
    qRegisterMetaType<Tucuxi::Gui::Core::TargetMethod*>("TargetMethod*");
    qRegisterMetaType<Tucuxi::Gui::Core::TargetMethod::TargetType>("TargetType");
    qRegisterMetaType<Tucuxi::Gui::Core::Operation*>("Operation*");
    qRegisterMetaType<QList<Tucuxi::Gui::Core::Operation*> >("QList<Operation*>");
    qRegisterMetaType<Tucuxi::Gui::Core::Bsv*>("Bsv*");
    qRegisterMetaType<Tucuxi::Gui::Core::Parameter*>("Parameter*");
    qRegisterMetaType<Tucuxi::Gui::Core::ParameterSet*>("ParameterSet*");
    qRegisterMetaType<Tucuxi::Gui::Core::Parameters*>("ParameterList*");
    qRegisterMetaType<Tucuxi::Gui::Core::DrugTreatment*>("DrugTreatment*");
    qRegisterMetaType<Tucuxi::Gui::Core::StandardTreatment*>("StandardTreatment*");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::DrugVariateInfo*>("DrugVariateInfo*");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::DrugVariateInfoList*>("DrugVariateInfoList*");
    qRegisterMetaType<Tucuxi::Gui::Core::TargetEvaluationResult*>("TargetEvaluationResult*");
    qRegisterMetaType<Tucuxi::Gui::Core::TargetEvaluationResults*>("TargetEvaluationResults*");
    qRegisterMetaType<Tucuxi::Gui::Core::Adjustment*>("Adjustment*");
    qRegisterMetaType<Tucuxi::Gui::Core::Adjustments*>("Adjustments*");
    qRegisterMetaType<Tucuxi::Gui::Core::FancyPoint*>("FancyPoint*");
    qRegisterMetaType<Tucuxi::Gui::Core::FancyPoints*>("FancyPoints*");
    qRegisterMetaType<Tucuxi::Gui::Core::ErrorModel*>("ErrorModel*");
    qRegisterMetaType<Tucuxi::Gui::Core::MetaData*>("MetaData*");
    qRegisterMetaType<Tucuxi::Gui::Core::Prediction*>("Prediction*");
    qRegisterMetaType<Tucuxi::Gui::Core::DrugResponseAnalysis*>("Analysis*");
    qRegisterMetaType<Tucuxi::Gui::Core::OperableAmount*>("OperableAmount*");
    qRegisterMetaType<Tucuxi::Gui::Core::OperationType>("OperationType");
    qRegisterMetaType<Tucuxi::Gui::Core::OperableValue*>("OperableValue*");
    qRegisterMetaType<Tucuxi::Gui::Core::OperableValueList*>("OperableValueList*");
    qRegisterMetaType<Tucuxi::Gui::Core::OperationList*>("OperationList*");
    qRegisterMetaType<Tucuxi::Gui::Core::UncastedValue*>("UncastedValue*");
    qRegisterMetaType<Tucuxi::Gui::Core::UncastedValueList*>("UncastedValueList*");
    qRegisterMetaType<Tucuxi::Gui::Core::Admin::Route>("Route");
    qRegisterMetaType<Tucuxi::Gui::Admin::ValidationStatus::StatusType>("StatusType");
    qRegisterMetaType<Tucuxi::Gui::Admin::ClinicalSet*>("ClinicalSet*");
    qRegisterMetaType<Tucuxi::Gui::Admin::Interpretation*>("Interpretation*");
    qRegisterMetaType<Tucuxi::Gui::Admin::Interpretation::InterpretationType>("InterpretationType");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::InterpretationController*>("InterpretationController*");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::MeasureTabController*>("MeasureTabController*");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::DosageTabController*>("DosageTabController*");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::TargetTabController*>("TargetTabController*");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::CovariateTabController*>("CovariateTabController*");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::AdjustmentTabController*>("AdjustmentTabController*");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::DrugTabController*>("DrugTabController*");
    qRegisterMetaType<Tucuxi::Gui::GuiUtils::MainWindowController*>("MainWindowController*");
    qRegisterMetaType<Tucuxi::Gui::Admin::Person::GenderType>("GenderType");
    qRegisterMetaType<Tucuxi::Gui::Admin::SentencesPalettes*>("SentencesPalettes*");
    qRegisterMetaType<Tucuxi::Gui::Admin::Section*>("Section*");
    //qRegisterMetaType<QList<Section*>*>("QList<Section*>*");

    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::SentencesPalettes>("ezechiel", 1, 0, "SentencesPalettes", QObject::tr("Cannot instantiate type 'SentencesPalettes'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::Section>("ezechiel", 1, 0, "Section", QObject::tr("Cannot instantiate type 'Section'"));
    //qmlRegisterUncreatableType<QList<Section*>>("ezechiel", 1, 0, "QList<Section*>", QObject::tr("Cannot instantiate type 'QList<Section*>'"));

    qRegisterMetaType< Tucuxi::Gui::Core::IntList >( "IntList" );
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::InterpretationRequest>("ezechiel", 1, 0, "InterpretationRequest", QObject::tr("Cannot instantiate type 'InterpretationRequest'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::Patient>("ezechiel", 1, 0, "Patient", QObject::tr("Cannot instantiate type 'Patient'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::Phone>("ezechiel", 1, 0, "Phone", QObject::tr("Cannot instantiate type 'Phone'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::Person>("ezechiel", 1, 0, "Person", QObject::tr("Cannot instantiate type 'Person'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::Location>("ezechiel", 1, 0, "Location", QObject::tr("Cannot instantiate type 'Location'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::Practician>("ezechiel", 1, 0, "Practician", QObject::tr("Cannot instantiate type 'Practician'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::DrugModel>("ezechiel", 1, 0, "DrugModel", QObject::tr("Cannot instantiate type 'DrugModel'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::ActiveSubstance>("ezechiel", 1, 0, "ActiveSubstance", QObject::tr("Cannot instantiate type 'ActiveSubstance'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Target>("ezechiel", 1, 0, "Target", QObject::tr("Cannot instantiate type 'Target'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::TargetEvaluationResult>("ezechiel", 1, 0, "TargetEvaluationResult", QObject::tr("Cannot instantiate type 'TargetEvaluationResult'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::TargetEvaluationResults>("ezechiel", 1, 0, "TargetEvaluationResults", QObject::tr("Cannot instantiate type 'TargetEvaluationResults'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::CoreMeasure>("ezechiel", 1, 0, "CoreMeasure", QObject::tr("Cannot instantiate type 'CoreMeasure'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::PatientVariate>("ezechiel", 1, 0, "PatientVariate", QObject::tr("Cannot instantiate type 'PatientVariate'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::DrugVariate>("ezechiel", 1, 0, "DrugVariate", QObject::tr("Cannot instantiate type 'DrugVariate'"));    
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::DrugVariateInfo>("ezechiel", 1, 0, "DrugVariateInfo", QObject::tr("Cannot instantiate type 'DrugVariateInfo'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Adjustment>("ezechiel", 1, 0, "Adjustment", QObject::tr("Cannot instantiate type 'Adjustment'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::DrugTreatment>("ezechiel", 1, 0, "DrugTreatment", QObject::tr("Cannot instantiate type 'Treatment'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::PredictionSpec>("ezechiel", 1, 0, "PredictionSpec", QObject::tr("Cannot instantiate type 'PredictionSpec'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::PatientVariateList>("ezechiel", 1, 0, "PatientVariateList", QObject::tr("Cannot instantiate type 'PatientVariateList'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Dosage>("ezechiel", 1, 0, "Dosage", QObject::tr("Cannot instantiate type 'Dosage'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::DosageHistory>("ezechiel", 1, 0, "DosageHistory", QObject::tr("Cannot instantiate type 'DosageHistory'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::CorePatient>("ezechiel", 1, 0, "CorePatient", QObject::tr("Cannot instantiate type 'CorePatient'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::ADME>("ezechiel", 1, 0, "ADME", QObject::tr("Cannot instantiate type 'ADME'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Halflife>("ezechiel", 1, 0, "Halflife", QObject::tr("Cannot instantiate type 'Halflife'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Concentrations>("ezechiel", 1, 0, "Concentrations", QObject::tr("Cannot instantiate type 'Concentrations'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Results>("ezechiel", 1, 0, "Results", QObject::tr("Cannot instantiate type 'Results'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::ValidDoses>("ezechiel", 1, 0, "ValidDoses", QObject::tr("Cannot instantiate type 'ValidDoses'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::ValidIntervals>("ezechiel", 1, 0, "ValidIntervals", QObject::tr("Cannot instantiate type 'ValidIntervals'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::ValidInfusions>("ezechiel", 1, 0, "ValidInfusions", QObject::tr("Cannot instantiate type 'ValidInfusions'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::DrugVariateList>("ezechiel", 1, 0, "DrugVariateList", QObject::tr("Cannot instantiate type 'DrugVariateList'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Operation>("ezechiel", 1, 0, "Operation", QObject::tr("Cannot instantiate type 'Operation'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::ParameterSet>("ezechiel", 1, 0, "ParameterSet", QObject::tr("Cannot instantiate type 'ParameterSet'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::ErrorModel>("ezechiel", 1, 0, "ErrorModel", QObject::tr("Cannot instantiate type 'ErrorModel'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::MetaData>("ezechiel", 1, 0, "MetaData", QObject::tr("Cannot instantiate type 'MetaData'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Prediction>("ezechiel", 1, 0, "Prediction", QObject::tr("Cannot instantiate type 'Prediction'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::DrugResponseAnalysis>("ezechiel", 1, 0, "Analysis", QObject::tr("Cannot instantiate type 'Analysis'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::OperableAmount>("ezechiel", 1, 0, "OperableAmount", QObject::tr("Cannot instantiate type 'OperableAmount'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::UncastedValue>("ezechiel", 1, 0, "UncastedValue", QObject::tr("Cannot instantiate type 'UncastedValue'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::UncastedValueList>("ezechiel", 1, 0, "UncastedValueList", QObject::tr("Cannot instantiate type 'UncastedValueList'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::Interpretation>("ezechiel", 1, 0, "Interpretation", QObject::tr("Cannot instantiate type 'Interpretation'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::InterpretationController>("ezechiel", 1, 0, "InterpretationController", QObject::tr("Cannot instantiate type 'InterpretationController'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::MeasureTabController>("ezechiel", 1, 0, "MeasureTabController", QObject::tr("Cannot instantiate type 'MeasureTabController'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::TargetTabController>("ezechiel", 1, 0, "TargetTabController", QObject::tr("Cannot instantiate type 'TargetTabController'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::DosageTabController>("ezechiel", 1, 0, "DosageTabController", QObject::tr("Cannot instantiate type 'DosageTabController'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::CovariateTabController>("ezechiel", 1, 0, "CovariateTabController", QObject::tr("Cannot instantiate type 'CovariateTabController'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::AdjustmentTabController>("ezechiel", 1, 0, "AdjustmentTabController", QObject::tr("Cannot instantiate type 'AdjustmentTabController'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::DrugTabController>("ezechiel", 1, 0, "DrugTabController", QObject::tr("Cannot instantiate type 'DrugTabController'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::MainWindowController>("ezechiel", 1, 0, "MainWindowController", QObject::tr("Cannot instantiate type 'MainWindowController'"));

    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Registering model objects.");

    //#ifdef CONFIG_CONNECTED
    //qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::PartialRequestListModel>("ezechiel", 1, 0, "PartialRequestListModel", QObject::tr("Cannot instantiate type 'PartialRequestListModel'"));
    qmlRegisterType<Tucuxi::Gui::GuiUtils::PartialRequestListModel>("ezechiel", 1, 0, "PartialRequestListModel");
//#endif // CONFIG_CONNECTED
//    qmlRegisterUncreatableType<PracticianListModel>("ezechiel", 1, 0, "PracticianListModel", QObject::tr("Cannot instantiate type 'PracticianListModel'"));
#ifdef CONFIG_CONNECTED
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::RequestsController>("ezechiel", 1, 0, "RequestsController", QObject::tr("Cannot instantiate type 'RequestsController'"));
#endif // CONFIG_CONNECTED
//    qmlRegisterUncreatableType<ReverseModel>("ezechiel", 1, 0, "ReverseModel", QObject::tr("Cannot instantiate type 'ReverseModel'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::SortFilterEntityListProxyModel>("ezechiel", 1, 0, "SortFilterEntityListProxyModel", QObject::tr("Cannot instantiate type 'SortFilterEntityListProxyModel'"));
//    qmlRegisterUncreatableType<StudyListModel>("ezechiel", 1, 0, "StudyListModel", QObject::tr("Cannot instantiate type 'StudyListModel'"));
//    qmlRegisterUncreatableType<TargetListModel>("ezechiel", 1, 0, "TargetListModel", QObject::tr("Cannot instantiate type 'TargetListModel'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::GuiUtils::StartupWindow>("ezechiel", 1, 0, "StartupWindow", QObject::tr("Cannot instantiate type 'StartupWindow'"));

    qmlRegisterUncreatableType<Tucuxi::Gui::Core::PredictionResult>("ezechiel", 1, 0, "PredictionResult", QObject::tr("Cannot instantiate type 'PredictionResult'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::PercentileData>("ezechiel", 1, 0, "PercentilePair", QObject::tr("Cannot instantiate type 'PercentilePair'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::PercentileDataList>("ezechiel", 1, 0, "PercentileDataList", QObject::tr("Cannot instantiate type 'PecentilePairs'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Predictive>("ezechiel", 1, 0, "Predictive", QObject::tr("Cannot instantiate type 'Predictive'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::PredictionData>("ezechiel", 1, 0, "PredictionData", QObject::tr("Cannot instantiate type 'PredictionData'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::Points>("ezechiel", 1, 0, "Points", QObject::tr("Cannot instantiate type 'Points'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::ChartData>("ezechiel", 1, 0, "ChartData", QObject::tr("Cannot instantiate type 'ChartData'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Core::CorePatient>("ezechiel", 1, 0, "CorePatient", QObject::tr("Cannot instantiate type 'CorePatient'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::ValidationStatus>("ezechiel", 1, 0, "Status", QObject::tr("Cannot instantiate type 'Status'"));
    qmlRegisterUncreatableType<Tucuxi::Gui::Admin::StepType>("ezechiel", 1, 0, "StepType", QObject::tr("Cannot instantiate type 'StepType'"));

    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Registering other types.");
    qmlRegisterType<Tucuxi::Gui::GuiUtils::ProxyModelFilter>("ezechiel", 1, 0, "ProxyModelFilter");

    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Registering QML singletons.");
    qmlRegisterSingletonType(QUrl("qrc:/controls/Status.qml"), "guiutils.qml.controls", 1, 0, "Status");
    qmlRegisterSingletonType(QUrl("qrc:/controls/ToolTips.qml"), "guiutils.qml.controls", 1, 0, "ToolTips");
    qmlRegisterSingletonType(QUrl("qrc:/styles/Style.qml"), "guiutils.qml.styles", 1, 0, "Style");

    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Registering QML components.");
    qmlRegisterType(QUrl("qrc:/windows/MainWindow.qml"), "guiutils.qml.windows", 1, 0, "MainWindow");
    qmlRegisterType(QUrl("qrc:/windows/StartupWindow.qml"), "guiutils.qml.windows", 1, 0, "StartupWindow");
    qmlRegisterType(QUrl("qrc:/controls/AdjustmentTab.qml"), "guiutils.qml.controls", 1, 0, "AdjustmentTab");
    qmlRegisterType(QUrl("qrc:/controls/ApplicationBar.qml"), "guiutils.qml.controls", 1, 0, "ApplicationBar");
    qmlRegisterType(QUrl("qrc:/controls/ApplicationBarButton.qml"), "guiutils.qml.controls", 1, 0, "ApplicationBarButton");
    qmlRegisterType(QUrl("qrc:/controls/Chart.qml"), "guiutils.qml.controls", 1, 0, "Chart");
    qmlRegisterType(QUrl("qrc:/controls/CovariateTab.qml"), "guiutils.qml.controls", 1, 0, "CovariateTab");
    qmlRegisterType(QUrl("qrc:/controls/DatePicker.qml"), "guiutils.qml.controls", 1, 0, "DatePicker");
    qmlRegisterType(QUrl("qrc:/controls/DemoBanner.qml"), "guiutils.qml.controls", 1, 0, "DemoBanner");
    qmlRegisterType(QUrl("qrc:/controls/DosageTab.qml"), "guiutils.qml.controls", 1, 0, "DosageTab");
    qmlRegisterType(QUrl("qrc:/controls/DrugTab.qml"), "guiutils.qml.controls", 1, 0, "DrugTab");
    qmlRegisterType(QUrl("qrc:/controls/FlowInformationPanel.qml"), "guiutils.qml.controls", 1, 0, "FlowInformationPanel");
    qmlRegisterType(QUrl("qrc:/controls/Flow.qml"), "guiutils.qml.controls", 1, 0, "Flow");
    qmlRegisterType(QUrl("qrc:/controls/StartupScreen.qml"), "guiutils.qml.controls", 1, 0, "Launch");
    qmlRegisterType(QUrl("qrc:/controls/FlowMenu.qml"), "guiutils.qml.controls", 1, 0, "FlowMenu");
    qmlRegisterType(QUrl("qrc:/controls/FlowMenuButton.qml"), "guiutils.qml.controls", 1, 0, "FlowMenuButton");
    qmlRegisterType(QUrl("qrc:/controls/MeasureTab.qml"), "guiutils.qml.controls", 1, 0, "MeasureTab");
    qmlRegisterType(QUrl("qrc:/controls/PatientTab.qml"), "guiutils.qml.controls", 1, 0, "PatientTab");
//#ifdef CONFIG_CONNECTED
    qmlRegisterType(QUrl("qrc:/controls/RequestList.qml"), "guiutils.qml.controls", 1, 0, "RequestList");
//#endif // CONFIG_CONNECTED
    qmlRegisterType(QUrl("qrc:/controls/TargetTab.qml"), "guiutils.qml.controls", 1, 0, "TargetTab");
    qmlRegisterType(QUrl("qrc:/controls/TimePicker.qml"), "guiutils.qml.controls", 1, 0, "TimePicker");
    qmlRegisterType(QUrl("qrc:/controls/ValidationTab.qml"), "guiutils.qml.controls", 1, 0, "ValidationTab");	


    Q_INIT_RESOURCE(core);
    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Initialized Core.");
//    Q_INIT_RESOURCE(report);
//    EXLOG(QtDebugMsg, Tucuxi::gui::NOEZERROR, "Initialized Report.");
    Q_INIT_RESOURCE(rest);
    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Initialized Rest.");
    Q_INIT_RESOURCE(qml);
    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Initialized Qml.");
    Q_INIT_RESOURCE(guiutils);
    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Initialized Guiutils.");
//    Q_INIT_RESOURCE(report);
//    EXLOG(QtDebugMsg, Tucuxi::gui::NOEZERROR, "Initialized Report.");

    // Let's create CORE, to modify paths from the options
    Tucuxi::Gui::Core::Core::setInstance(new Tucuxi::Gui::AppUtils::AppCore());
    parseOptions();
    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Initialized Options.");
    initResources();
    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Initialized Resources.");
#ifdef CONFIG_CONNECTED
    initRestConfig();
    EXLOG(QtDebugMsg, Tucuxi::Gui::NOEZERROR, "Initialized REST configuration.");
#endif // CONFIG_CONNECTED

    //QtWebEngine::initialize();

    std::string path = QCoreApplication::applicationDirPath().toLatin1().data();
#ifndef NOLICENSE
    path += "/license.txt";
    Tucuxi::Common::LicenseError licResult = Tucuxi::Common::LicenseManager::checkLicenseFile(path);
    if (licResult != Tucuxi::Common::LicenseError::VALID_LICENSE) {
        std::string request;
        Tucuxi::Common::LicenseError reqResult = Tucuxi::Common::LicenseManager::generateRequestString(request, "1.0");
        if (reqResult != Tucuxi::Common::LicenseError::REQUEST_SUCCESSFUL) {
            return -1;
        }
        LicenseDialog dlg(request.c_str());
        int dlgResult = dlg.exec();
        if (QDialog::Accepted != dlgResult) {
            return -2;
        }
        std::string license = dlg.getLicense();
        if (license.size() == 0) {
            return -3;
        }
        Tucuxi::Common::LicenseManager::installLicense(license, path);
        Tucuxi::Common::LicenseError licResult = Tucuxi::Common::LicenseManager::checkLicenseFile(path);
        if (licResult != Tucuxi::Common::LicenseError::VALID_LICENSE) {
            return -4;
        }
    }
#endif

#ifndef NOSTARTUPSCREEN
    Tucuxi::Gui::GuiUtils::StartupScreen startupScreen;
    if (startupScreen.exec() != QDialog::Accepted)
        return 0;
#endif // NOSTARTUPSCREEN

    Tucuxi::Gui::GuiUtils::MainWindowController* mainWindow = new Tucuxi::Gui::GuiUtils::MainWindowController(&app);
#ifndef CONFIG_GUITEST
    mainWindow->setResizeMode(QQuickView::SizeRootObjectToView);
    mainWindow->setMinimumSize(QSize(800,680));
    mainWindow->show();
#endif // CONFIG_GUITEST

//    InterpretationController* irc = new InterpretationController(&app);

//    irc->startUpScreen();

//    irc->setResizeMode(QQuickView::SizeRootObjectToView);
//    irc->show();

/*

    RestRequestsClient *client = new RestRequestsClient();


    client->queryList();
*/

#ifdef CONFIG_GUITEST
    // Instantiate and run tests
    //GuiTests tests(mainWindow->getWindow());
    SpixGTest tests(mainWindow, mainWindow->getWindow(), argc, argv);
    auto bot = new spix::QtQmlBot();
    bot->runTestServer(tests);
#endif // CONFIG_GUITEST

    int result = app.exec();

#ifdef CONFIG_GUITEST
    return tests.testResult();
#else
    // delete mainWindow;
    return result;
#endif
}

void parseOptions()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Tucuxi is a TDM software for pharmacologists");

    const QCommandLineOption standaloneOption(QStringList() << "a" << "standalone",
                                    QCoreApplication::translate("main", "Run Tucuxi as a standalone software, effectively disabling any feature relying on retrieving medical data from a server."));
    const QCommandLineOption basePathOption(QStringList() << "b" << "base-path",
                                    QCoreApplication::translate("main", "Set the base path of the medical institution's server from which interpretion request can be retrieved."),
                                    "path",
                                    "/services");
    const QCommandLineOption listFileOption(QStringList() << "f" << "listfile",
                                    QCoreApplication::translate("main", "Set the file containing a list of pending requests. This option allows to try pending requests stored locally instead of by connecting to a REST server."),
                                    "listfile",
                                    "");
    const QCommandLineOption requestFileOption(QStringList() << "r" << "requestfile",
                                    QCoreApplication::translate("main", "Set the file containing single request. This option allows to start the software with a single request."),
                                    "requestfile",
                                    "");
    const QCommandLineOption iccaFileOption(QStringList() << "i" << "iccafile",
                                    QCoreApplication::translate("main", "Set the file containing flat request. This option allows to start the software with flat requests"),
                                    "iccafile",
                                    "");
    const QCommandLineOption portOption(QStringList() << "p" << "port",
                                    QCoreApplication::translate("main", "Set the port number of the medical institution's server from which interpretion request can be retrieved."),
                                    "port",
                                    QString::number(8889));
    const QCommandLineOption schemeOption(QStringList() << "s" << "scheme",
                                    QCoreApplication::translate("main", "Set the scheme of the medical institution's server from which interpretion request can be retrieved."),
                                    "scheme",
                                    "https");
    const QCommandLineOption hostOption(QStringList() << "t" << "host",
                                    QCoreApplication::translate("main", "Set the host name of the medical institution's server from which interpretion request can be retrieved."),
                                    "host",
                                    "ezecs1");
    const QCommandLineOption logRestOption(QStringList() << "l" << "logrest",
                                    QCoreApplication::translate("main", "Set the path in which the REST logs will be stored."),
                                    "logrest",
                                    QApplication::applicationDirPath());
    const QCommandLineOption logMessagesInFileOption("logfile",
                                    QCoreApplication::translate("main", "Save application's log messages into a file"));
    const QCommandLineOption certificateFileOption(QStringList() << "certificate",
                                    QCoreApplication::translate("main", "Set the file containing the SSL certificate."),
                                    "certificate",
                                    "");
    const QCommandLineOption drugsPathOption(QStringList() << "d" << "drugspath",
                                             QCoreApplication::translate("main", "Drug files path."),
                                             "drugspath",
                                             CORE->path(Tucuxi::Gui::Core::Core::Drugs2));
    const QCommandLineOption tqfLoggerOption(QStringList() << "logtqf",
                                    QCoreApplication::translate("main", "Sets a folder to log the computing requests files."),
                                    "logtqf",
                                    "log");
    const QCommandLineOption noGroupIntakeOption(QStringList() << "nogroupintake",
                                                 QCoreApplication::translate("main", "Do not group intake when imported from flat request file"));

    parser.addOption(basePathOption);
    const QCommandLineOption helpOption = parser.addHelpOption();
    parser.addOption(listFileOption);
    parser.addOption(requestFileOption);
    parser.addOption(iccaFileOption);
    parser.addOption(hostOption);
    parser.addOption(portOption);
    parser.addOption(schemeOption);
    parser.addOption(logRestOption);
    parser.addOption(logMessagesInFileOption);
    parser.addOption(certificateFileOption);
    parser.addOption(drugsPathOption);
    parser.addOption(tqfLoggerOption);
    parser.addOption(noGroupIntakeOption);
    const QCommandLineOption versionOption = parser.addVersionOption();

    parser.parse(QCoreApplication::arguments());
    if (parser.isSet(helpOption))
        parser.showHelp();
    if (parser.isSet(versionOption))
        parser.showVersion();
    if (parser.isSet(logMessagesInFileOption))
        Tucuxi::logging::Logger::initLogFile(QCoreApplication::applicationDirPath());
    if (parser.isSet(certificateFileOption)) {
#ifdef CONFIG_CONNECTED
        Tucuxi::Gui::Rest::NetworkAccessManager::setType(Tucuxi::Gui::Rest::NetworkAccessManager::ManagerType::Unique);
        Tucuxi::Gui::Rest::NetworkAccessManager::getInstance().setUniqueDefaultCaCertificate(parser.value(certificateFileOption));
#endif // CONFIG_CONNECTED
    }
    if (parser.isSet(drugsPathOption)) {
#ifndef CONFIG_DEMO
        CORE->setPath(Tucuxi::Gui::Core::Core::Drugs2, parser.value(drugsPathOption));
#endif // CONFIG_DEMO
    }
    if (parser.isSet(tqfLoggerOption)) {
        TqfLogger::getInstance()->setEnable(true);
        TqfLogger::getInstance()->setPath(parser.value(tqfLoggerOption));
    }

    //ToDo: set the standalone option in the settings, and disable request list
    //and other network related features
    Q_UNUSED(standaloneOption);

    Tucuxi::Gui::GuiUtils::AppGlobals *appGlobals = Tucuxi::Gui::GuiUtils::AppGlobals::getInstance();

    appGlobals->setListFile(parser.value(listFileOption));
    appGlobals->setRequestFile(parser.value(requestFileOption));
    appGlobals->setIccaImport(parser.isSet(iccaFileOption));
    appGlobals->setIccaFile(parser.value(iccaFileOption));

    if (parser.isSet(noGroupIntakeOption)) {
        appGlobals->setGroupIntake(false);
    }


#ifdef CONFIG_CONNECTED
    Tucuxi::Gui::Rest::MirthRequest::updateDefaultBasePath(parser.value(basePathOption));
    Tucuxi::Gui::Rest::MirthRequest::updateDefaultPort(parser.value(portOption).toInt());
    Tucuxi::Gui::Rest::MirthRequest::updateDefaultScheme(parser.value(schemeOption));
    Tucuxi::Gui::Rest::MirthRequest::updateDefaultHost(parser.value(hostOption));
    Tucuxi::Gui::Rest::RestLogger::getInstance()->setPath(parser.value(logRestOption));
#endif // CONFIG_CONNECTED

}


#ifdef COMPILE_WITH_TUCUCORE

#include "processingtucucore/src/drugs2manager.h"
#include "apputils/src/drugmanager.h"
#include "core/dal/drug/drug.h"

#endif // COMPILE_WITH_TUCUCORE

void initResources()
{
//    We do this in main to be able to access Core in parsing
//    Tucuxi::Gui::Core::setInstance(new Tucuxi::Gui::AppUtils::AppCore());

    CORE->setProcessingInterface(ProcessingFactory::createProcessing());
    APPCORE->drugManager();

    // We try to load the new drug models

#ifdef COMPILE_WITH_TUCUCORE
    Tucuxi::Gui::Processing::Drugs2Manager* manager = Tucuxi::Gui::Processing::Drugs2Manager::getInstance();
#ifdef CONFIG_DEMO
    std::string basePath = ":/drugs/";
#else
    std::string basePath = CORE->path(Tucuxi::Gui::Core::Core::Drugs2).toStdString() + "/";
#endif // CONFIG_DEMO
    manager->buildAllDrugModels(basePath);
    std::vector<Tucuxi::Gui::Core::DrugModel *> models = manager->getAllGuiDrugModels();
    for(auto model : models) {
        if (model != nullptr) {
            APPCORE->drugManager()->tryToAddDrugModelToRepo(model);
        }
    }
#endif // COMPILE_WITH_TUCUCORE



}


#ifdef CONFIG_CONNECTED
void initRestConfig() {

    // ToDo-REST //////////////////////////////////////////////////////////////
    // Add an option to use REST and use it to configure the URL
    // ////////////////////////////////////////////////////////////////////////

    if (true)
        return;

    RestConfigDialog *restConfig = new RestConfigDialog;
    restConfig->exec();

    Tucuxi::Gui::Rest::MirthRequest::updateDefaultPort(restConfig->getPort().toInt());
    Tucuxi::Gui::Rest::MirthRequest::updateDefaultScheme(restConfig->getScheme());
    Tucuxi::Gui::Rest::MirthRequest::updateDefaultHost(restConfig->getIp());
}
#endif // CONFIG_CONNECTED
