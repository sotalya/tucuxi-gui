#include <QApplication>
#include <QObject>
#include <QtQml>
#include <QQuickView>
#include <QObject>
#include <QtQml>
#include <QtWebEngine/QtWebEngine>
#include <QSystemTrayIcon>

#include "errors_gui.h"
#include "restconfigdialog.h"
#include "rest/network/mirthrequest.h"
#include "rest/network/networkaccessmanager.h"

#include "core/settings.h"
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
#include "admin/src/dal/measure.h"
#include "admin/src/dal/interpretationrequest.h"
#include "admin/src/dal/steptypes.h"
#include "admin/src/dal/clinical.h"
#include "admin/src/requestsclient.h"

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

#include "guiutils/src/models/adjustmentlistmodel.h"
#include "guiutils/src/models/adjustmentsettingsmodel.h"
#include "guiutils/src/models/covariatelistmodel.h"
#include "guiutils/src/models/covariatemodel.h"
#include "guiutils/src/models/domainlistmodel.h"
#include "guiutils/src/models/dosagelistmodel.h"
#include "guiutils/src/models/druglistmodel.h"
#include "guiutils/src/models/drugvariatelistmodel.h"
#include "guiutils/src/models/institutelistmodel.h"
#include "guiutils/src/models/measurelistmodel.h"
#include "guiutils/src/models/partialrequestlistmodel.h"
#include "guiutils/src/models/patientlistmodel.h"
#include "guiutils/src/models/practicianlistmodel.h"
#include "guiutils/src/models/reversemodel.h"
#include "guiutils/src/models/sortfilterentitylistproxymodel.h"
#include "guiutils/src/models/studylistmodel.h"
#include "guiutils/src/models/targetlistmodel.h"
#include "guiutils/src/models/validationmodel.h"

#include "rest/restlogger.h"
#include "rest/restrequestsclient.h"

#include "guiutils/src/startupscreen.h"
#include "guiutils/src/startupwindow.h"


#include "core/utils/logging.h"

#ifdef COMPILE_WITH_TUCUCORE
#include "tucucrypto/licensemanager.h"
#else
#include "src/fromtucuxi/licensemanager.h"
#endif // COMPILE_WITH_TUCUCORE

#ifdef COMPILE_WITH_TUCUCORE
#include "tucucore/version.h"
#endif // COMPILE_WITH_TUCUCORE

#include "licensedialog.h"

void parseOptions();
void initResources();

#ifdef CONFIG_CONNECTED
void initRestConfig();
#endif // CONFIG_CONNECTED

#include <iostream>

#ifdef CONFIG_GUITEST
#include <QTest>

#include <Spix/QtQmlBot.h>

class GuiTests : public spix::TestServer {
public:
    GuiTests(QQuickWindow *window) : m_window(window) {}

protected:

    QQuickWindow *m_window;

    void executeTest() override
    {
        wait(std::chrono::milliseconds(1000));
        bool ok;

        spix::ItemPath path("mainWindow");
        std::string root = path.rootComponent();
        const std::vector<std::string> children = path.components();
        //        const std::vector<std::string>& components() const;
        //        size_t length() const { return m_components.size(); };
        //        std::string rootComponent() const;

        ok = existsAndVisible(spix::ItemPath("mainWindow"));
        ok = existsAndVisible(spix::ItemPath("mainWindow/launchView"));
        ok = existsAndVisible(spix::ItemPath("mainWindow/launchView/newPatient"));
        ok = existsAndVisible(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));
        mouseClick(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));

        spix::ItemPath path1("mainWindow/launchView/newPatient");
        std::string root1 = path.rootComponent();
        const std::vector<std::string> children1 = path.components();

        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/drugListItem_3"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
        wait(std::chrono::milliseconds(1500));
        mouseClick(spix::ItemPath("dosageDialog/okDosage"));
        wait(std::chrono::milliseconds(1500));
        this->takeScreenshot("mainWindow/flowView", "screen.png");

        wait(std::chrono::milliseconds(1500));
        takeScreenshot("mainWindow/flowView", "screen2.png");

        // We call this function to synchronize the thread before clicking
        existsAndVisible(spix::ItemPath("fake"));
        QTest::keyClick(m_window, Qt::Key_Right);
        wait(std::chrono::milliseconds(1500));
        takeScreenshot("mainWindow/flowView", "screen3.png");
        mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/selectAdjustment_0"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/validateInterpretation"));
        wait(std::chrono::milliseconds(500));

        existsAndVisible(spix::ItemPath("fake"));
//        std::cout << " dialog: " << qPrintable(qApp->activeWindow()->windowTitle()) << std::endl;
        QTest::keyClick(qApp->activeWindow(), Qt::Key_Enter);


        mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
        wait(std::chrono::milliseconds(20000));
        mouseClick(spix::ItemPath("mainWindow/flowView/printButton"));
        wait(std::chrono::milliseconds(2000));
        mouseClick(spix::ItemPath("mainWindow/applicationBarView/shutDownAction"));

/*
        QTest::keyClick(qApp->activeModalWidget(), Qt::Key_A);
        QTest::keyClick(qApp->activeModalWidget(), Qt::Key_B);
        QTest::keyClick(qApp->activeModalWidget(), Qt::Key_C);
        QTest::keyClick(qApp->activeModalWidget(), Qt::Key_Enter);

        QTest::keyClick(qApp->activePopupWidget(), Qt::Key_A);
        QTest::keyClick(qApp->activePopupWidget(), Qt::Key_B);
        QTest::keyClick(qApp->activePopupWidget(), Qt::Key_C);
        QTest::keyClick(qApp->activePopupWidget(), Qt::Key_Enter);
*/
    }
};
#endif // CONFIG_GUITEST

// YTA: Demo of a subclass outside core that uses the factory
/*
class NewBsvCreator;

class NewBsv: public ezechiel::core::Bsv
{
    SUBENTITY_UTILS(NewBsv, ezechiel::core::Bsv)

protected:
    NewBsv(AbstractRepository *repo, QObject *parent) : ezechiel::core::Bsv(repo, parent) { std::cout << "********************\nI am a new Bsv\n****************\n";}
};
*/

/*
#include <signal.h>
#include <iostream>
#include <cstdlib>

// Print a message when Ezechiel is stopped by a signal
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
//    ezechiel::core::Bsv::getFactory()->registerCreator(new NewBsv::NewBsvCreator());

    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, QString("Built from Gui Git revision %1").arg(GIT_REVISION));
#ifdef COMPILE_WITH_TUCUCORE
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, QString("Built from Tucucore Git revision %1").arg(QString::fromStdString(Tucuxi::Core::Version::getGitRevision())));
#endif // COMPILE_WITH_TUCUCORE


    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering DAL objects.");

    // Set settings info before CORE is called
    QCoreApplication::setOrganizationName("HEIG-VD");
//    QSettings::setDefaultFormat(QSettings::Format::IniFormat);
#ifdef CONFIG_DEMO
    QCoreApplication::setApplicationName("Tucuxi - Demo version - Not intended for medical use");
#else
    QCoreApplication::setApplicationName("Tucuxi");
#endif // CONFIG_DEMO
    QCoreApplication::setApplicationVersion("0.3.0");

    CORE->setRunningMode(ezechiel::core::Core::GUI);

    qRegisterMetaType<ezechiel::core::PredictionResult*>("PredictionResult*");
    qRegisterMetaType<ezechiel::core::PercentileData*>("PercentilePair*");
    qRegisterMetaType<ezechiel::core::PercentileDataList*>("PercentileDataList*");
    qRegisterMetaType<ezechiel::core::Predictive*>("Predictive*");
    qRegisterMetaType<ezechiel::core::PredictionData*>("PredictionData*");
    qRegisterMetaType<ezechiel::core::Points*>("Points*");
    qRegisterMetaType<ezechiel::core::ChartData*>("ChartData*");
    qRegisterMetaType<QList<QObject*>*>("QList<QObject*>*");
    qRegisterMetaType<InterpretationRequest*>("InterpretationRequest*");
    qRegisterMetaType<ezechiel::core::Duration>("Duration");
    qRegisterMetaType<Patient*>("Patient*");
    qRegisterMetaType<ezechiel::core::Descriptor>("Descriptor");
    qRegisterMetaType<QMetaType::Type>("QMetaType::Type");
//    qRegisterMetaType<ezechiel::core::QStringTranslation>("QStringTranslation");
    qRegisterMetaType<ezechiel::core::TranslatableString*>("TranslatableString*");
    qRegisterMetaType<Person*>("Person*");
    qRegisterMetaType<Location*>("Location*");
    qRegisterMetaType<Practician*>("Practician*");
    qRegisterMetaType<ezechiel::core::DrugModel*>("DrugModel*");
    qRegisterMetaType<InterpretationAnalysis*>("InterpretationAnalysis*");
    qRegisterMetaType<ezechiel::core::TargetList*>("TargetList*");
    qRegisterMetaType<ezechiel::core::Unit>("Unit");
    qRegisterMetaType<ezechiel::core::DrugModelList*>("DrugList*");
    qRegisterMetaType<ezechiel::core::ActiveSubstance*>("ActiveSubstance*");
    qRegisterMetaType<ezechiel::core::ActiveSubstanceList*>("ActiveSubstanceList*");
    qRegisterMetaType<ezechiel::core::LightActiveSubstance*>("LightActiveSubstance*");
    qRegisterMetaType<ezechiel::core::LightActiveSubstanceList*>("LightActiveSubstanceList*");
    qRegisterMetaType<ezechiel::core::CoreMeasure*>("CoreMeasure*");
    qRegisterMetaType<ezechiel::core::CoreMeasureList*>("CoreMeasureList*");
    qRegisterMetaType<ezechiel::core::PatientVariate*>("PatientVariate*");
    qRegisterMetaType<ezechiel::core::DrugVariate*>("DrugVariate*");
    qRegisterMetaType<ezechiel::core::DrugTreatment*>("DrugTreatment*");
    qRegisterMetaType<ezechiel::core::PredictionSpec*>("PredictionSpec*");
    qRegisterMetaType<ezechiel::core::PatientVariateList*>("PatientVariateList*");
    qRegisterMetaType<ezechiel::core::Dosage*>("Dosage*");
    qRegisterMetaType<ezechiel::core::AdjustmentDosage*>("AdjustmentDosage*");
    qRegisterMetaType<ezechiel::core::DosageHistory*>("DosageHistory*");
    qRegisterMetaType<ezechiel::core::CorePatient*>("CorePatient*");
    qRegisterMetaType<ezechiel::core::CorePatientList*>("CorePatientList*");
    qRegisterMetaType<ezechiel::core::ADME*>("ADME*");
    qRegisterMetaType<ezechiel::core::Admin*>("Admin*");
    qRegisterMetaType<ezechiel::core::AdminList*>("AdminList*");
    qRegisterMetaType<ezechiel::core::Admin::Route>("ADME::Route");
    qRegisterMetaType<ezechiel::core::Amount>("Amount");
    qRegisterMetaType<ezechiel::core::Reference*>("Reference*");
    qRegisterMetaType<ezechiel::core::ReferenceList*>("ReferenceList*");
    qRegisterMetaType<ezechiel::core::Editor*>("Editor*");
    qRegisterMetaType<ezechiel::core::EditorList*>("EditorList*");
    // qRegisterMetaType<ezechiel::core::EditorSet*>("EditorSet*");
    qRegisterMetaType<ezechiel::core::IdentifiableAmount*>("IdentifiableAmount*");
    qRegisterMetaType<ezechiel::core::Halflife*>("Halflife*");
    qRegisterMetaType<ezechiel::core::Concentrations*>("Concentrations*");
    qRegisterMetaType<ezechiel::core::Results*>("Results*");
    qRegisterMetaType<ezechiel::core::ValidDoses*>("ValidDoses*");
    qRegisterMetaType<ezechiel::core::ValidIntervals*>("ValidIntervals*");
    qRegisterMetaType<ezechiel::core::ValidInfusions*>("ValidInfusions*");
    qRegisterMetaType<ezechiel::core::DrugVariateList*>("DrugVariateList*");
    qRegisterMetaType<ezechiel::core::Target*>("Target*");
    qRegisterMetaType<ezechiel::core::TargetMethod*>("TargetMethod*");
    qRegisterMetaType<ezechiel::core::TargetMethod::TargetType>("TargetType");
    qRegisterMetaType<ezechiel::core::Operation*>("Operation*");
    qRegisterMetaType<QList<ezechiel::core::Operation*> >("QList<Operation*>");
    qRegisterMetaType<ezechiel::core::Bsv*>("Bsv*");
    qRegisterMetaType<ezechiel::core::Parameter*>("Parameter*");
    qRegisterMetaType<ezechiel::core::ParameterSet*>("ParameterSet*");
    qRegisterMetaType<ezechiel::core::Parameters*>("ParameterList*");
    qRegisterMetaType<ezechiel::core::DrugTreatment*>("DrugTreatment*");
    qRegisterMetaType<ezechiel::core::StandardTreatment*>("StandardTreatment*");
    qRegisterMetaType<DrugVariateInfo*>("DrugVariateInfo*");
    qRegisterMetaType<DrugVariateInfoList*>("DrugVariateInfoList*");
    qRegisterMetaType<ezechiel::core::TargetEvaluationResult*>("TargetEvaluationResult*");
    qRegisterMetaType<ezechiel::core::TargetEvaluationResults*>("TargetEvaluationResults*");
    qRegisterMetaType<ezechiel::core::Adjustment*>("Adjustment*");
    qRegisterMetaType<ezechiel::core::Adjustments*>("Adjustments*");
    qRegisterMetaType<ezechiel::core::FancyPoint*>("FancyPoint*");
    qRegisterMetaType<ezechiel::core::FancyPoints*>("FancyPoints*");
    qRegisterMetaType<ezechiel::core::ErrorModel*>("ErrorModel*");
    qRegisterMetaType<ezechiel::core::MetaData*>("MetaData*");
    qRegisterMetaType<ezechiel::core::Prediction*>("Prediction*");
    qRegisterMetaType<ezechiel::core::DrugResponseAnalysis*>("Analysis*");
    qRegisterMetaType<ezechiel::core::OperableAmount*>("OperableAmount*");
    qRegisterMetaType<ezechiel::core::OperationType>("OperationType");
    qRegisterMetaType<ezechiel::core::OperableValue*>("OperableValue*");
    qRegisterMetaType<ezechiel::core::OperableValueList*>("OperableValueList*");
    qRegisterMetaType<ezechiel::core::OperationList*>("OperationList*");
    qRegisterMetaType<ezechiel::core::UncastedValue*>("UncastedValue*");
    qRegisterMetaType<ezechiel::core::UncastedValueList*>("UncastedValueList*");
    qRegisterMetaType<ezechiel::core::Admin::Route>("Route");
    qRegisterMetaType<ValidationStatus::StatusType>("StatusType");
    qRegisterMetaType<ClinicalSet*>("ClinicalSet*");
    qRegisterMetaType<Interpretation*>("Interpretation*");
    qRegisterMetaType<Interpretation::InterpretationType>("InterpretationType");
    qRegisterMetaType<InterpretationController*>("InterpretationController*");
    qRegisterMetaType<MeasureTabController*>("MeasureTabController*");
    qRegisterMetaType<DosageTabController*>("DosageTabController*");
    qRegisterMetaType<TargetTabController*>("TargetTabController*");
    qRegisterMetaType<CovariateTabController*>("CovariateTabController*");
    qRegisterMetaType<AdjustmentTabController*>("AdjustmentTabController*");
    qRegisterMetaType<DrugTabController*>("DrugTabController*");
    qRegisterMetaType<MainWindowController*>("MainWindowController*");
    qRegisterMetaType<Person::GenderType>("GenderType");

    qRegisterMetaType< ezechiel::core::IntList >( "IntList" );
    qmlRegisterUncreatableType<InterpretationRequest>("ezechiel", 1, 0, "InterpretationRequest", QObject::tr("Cannot instantiate type 'InterpretationRequest'"));
    qmlRegisterUncreatableType<Patient>("ezechiel", 1, 0, "Patient", QObject::tr("Cannot instantiate type 'Patient'"));
    qmlRegisterUncreatableType<Person>("ezechiel", 1, 0, "Person", QObject::tr("Cannot instantiate type 'Person'"));
    qmlRegisterUncreatableType<Location>("ezechiel", 1, 0, "Location", QObject::tr("Cannot instantiate type 'Location'"));
    qmlRegisterUncreatableType<Practician>("ezechiel", 1, 0, "Practician", QObject::tr("Cannot instantiate type 'Practician'"));
    qmlRegisterUncreatableType<ezechiel::core::DrugModel>("ezechiel", 1, 0, "DrugModel", QObject::tr("Cannot instantiate type 'DrugModel'"));
    qmlRegisterUncreatableType<ezechiel::core::ActiveSubstance>("ezechiel", 1, 0, "ActiveSubstance", QObject::tr("Cannot instantiate type 'ActiveSubstance'"));
    qmlRegisterUncreatableType<ezechiel::core::Target>("ezechiel", 1, 0, "Target", QObject::tr("Cannot instantiate type 'Target'"));
    qmlRegisterUncreatableType<ezechiel::core::TargetEvaluationResult>("ezechiel", 1, 0, "TargetEvaluationResult", QObject::tr("Cannot instantiate type 'TargetEvaluationResult'"));
    qmlRegisterUncreatableType<ezechiel::core::TargetEvaluationResults>("ezechiel", 1, 0, "TargetEvaluationResults", QObject::tr("Cannot instantiate type 'TargetEvaluationResults'"));
    qmlRegisterUncreatableType<ezechiel::core::CoreMeasure>("ezechiel", 1, 0, "CoreMeasure", QObject::tr("Cannot instantiate type 'CoreMeasure'"));
    qmlRegisterUncreatableType<ezechiel::core::PatientVariate>("ezechiel", 1, 0, "PatientVariate", QObject::tr("Cannot instantiate type 'PatientVariate'"));
    qmlRegisterUncreatableType<ezechiel::core::DrugVariate>("ezechiel", 1, 0, "DrugVariate", QObject::tr("Cannot instantiate type 'DrugVariate'"));    
    qmlRegisterUncreatableType<DrugVariateInfo>("ezechiel", 1, 0, "DrugVariateInfo", QObject::tr("Cannot instantiate type 'DrugVariateInfo'"));
    qmlRegisterUncreatableType<ezechiel::core::Adjustment>("ezechiel", 1, 0, "Adjustment", QObject::tr("Cannot instantiate type 'Adjustment'"));
    qmlRegisterUncreatableType<ezechiel::core::DrugTreatment>("ezechiel", 1, 0, "DrugTreatment", QObject::tr("Cannot instantiate type 'Treatment'"));
    qmlRegisterUncreatableType<ezechiel::core::PredictionSpec>("ezechiel", 1, 0, "PredictionSpec", QObject::tr("Cannot instantiate type 'PredictionSpec'"));
    qmlRegisterUncreatableType<ezechiel::core::PatientVariateList>("ezechiel", 1, 0, "PatientVariateList", QObject::tr("Cannot instantiate type 'PatientVariateList'"));
    qmlRegisterUncreatableType<ezechiel::core::Dosage>("ezechiel", 1, 0, "Dosage", QObject::tr("Cannot instantiate type 'Dosage'"));
    qmlRegisterUncreatableType<ezechiel::core::DosageHistory>("ezechiel", 1, 0, "DosageHistory", QObject::tr("Cannot instantiate type 'DosageHistory'"));
    qmlRegisterUncreatableType<ezechiel::core::CorePatient>("ezechiel", 1, 0, "CorePatient", QObject::tr("Cannot instantiate type 'CorePatient'"));
    qmlRegisterUncreatableType<ezechiel::core::ADME>("ezechiel", 1, 0, "ADME", QObject::tr("Cannot instantiate type 'ADME'"));
    qmlRegisterUncreatableType<ezechiel::core::Halflife>("ezechiel", 1, 0, "Halflife", QObject::tr("Cannot instantiate type 'Halflife'"));
    qmlRegisterUncreatableType<ezechiel::core::Concentrations>("ezechiel", 1, 0, "Concentrations", QObject::tr("Cannot instantiate type 'Concentrations'"));
    qmlRegisterUncreatableType<ezechiel::core::Results>("ezechiel", 1, 0, "Results", QObject::tr("Cannot instantiate type 'Results'"));
    qmlRegisterUncreatableType<ezechiel::core::ValidDoses>("ezechiel", 1, 0, "ValidDoses", QObject::tr("Cannot instantiate type 'ValidDoses'"));
    qmlRegisterUncreatableType<ezechiel::core::ValidIntervals>("ezechiel", 1, 0, "ValidIntervals", QObject::tr("Cannot instantiate type 'ValidIntervals'"));
    qmlRegisterUncreatableType<ezechiel::core::ValidInfusions>("ezechiel", 1, 0, "ValidInfusions", QObject::tr("Cannot instantiate type 'ValidInfusions'"));
    qmlRegisterUncreatableType<ezechiel::core::DrugVariateList>("ezechiel", 1, 0, "DrugVariateList", QObject::tr("Cannot instantiate type 'DrugVariateList'"));
    qmlRegisterUncreatableType<ezechiel::core::Operation>("ezechiel", 1, 0, "Operation", QObject::tr("Cannot instantiate type 'Operation'"));
    qmlRegisterUncreatableType<ezechiel::core::ParameterSet>("ezechiel", 1, 0, "ParameterSet", QObject::tr("Cannot instantiate type 'ParameterSet'"));
    qmlRegisterUncreatableType<ezechiel::core::ErrorModel>("ezechiel", 1, 0, "ErrorModel", QObject::tr("Cannot instantiate type 'ErrorModel'"));
    qmlRegisterUncreatableType<ezechiel::core::MetaData>("ezechiel", 1, 0, "MetaData", QObject::tr("Cannot instantiate type 'MetaData'"));
    qmlRegisterUncreatableType<ezechiel::core::Prediction>("ezechiel", 1, 0, "Prediction", QObject::tr("Cannot instantiate type 'Prediction'"));
    qmlRegisterUncreatableType<ezechiel::core::DrugResponseAnalysis>("ezechiel", 1, 0, "Analysis", QObject::tr("Cannot instantiate type 'Analysis'"));
    qmlRegisterUncreatableType<ezechiel::core::OperableAmount>("ezechiel", 1, 0, "OperableAmount", QObject::tr("Cannot instantiate type 'OperableAmount'"));
    qmlRegisterUncreatableType<ezechiel::core::UncastedValue>("ezechiel", 1, 0, "UncastedValue", QObject::tr("Cannot instantiate type 'UncastedValue'"));
    qmlRegisterUncreatableType<ezechiel::core::UncastedValueList>("ezechiel", 1, 0, "UncastedValueList", QObject::tr("Cannot instantiate type 'UncastedValueList'"));
    qmlRegisterUncreatableType<Interpretation>("ezechiel", 1, 0, "Interpretation", QObject::tr("Cannot instantiate type 'Interpretation'"));
    qmlRegisterUncreatableType<InterpretationController>("ezechiel", 1, 0, "InterpretationController", QObject::tr("Cannot instantiate type 'InterpretationController'"));
    qmlRegisterUncreatableType<MeasureTabController>("ezechiel", 1, 0, "MeasureTabController", QObject::tr("Cannot instantiate type 'MeasureTabController'"));
    qmlRegisterUncreatableType<TargetTabController>("ezechiel", 1, 0, "TargetTabController", QObject::tr("Cannot instantiate type 'TargetTabController'"));
    qmlRegisterUncreatableType<DosageTabController>("ezechiel", 1, 0, "DosageTabController", QObject::tr("Cannot instantiate type 'DosageTabController'"));
    qmlRegisterUncreatableType<CovariateTabController>("ezechiel", 1, 0, "CovariateTabController", QObject::tr("Cannot instantiate type 'CovariateTabController'"));
    qmlRegisterUncreatableType<AdjustmentTabController>("ezechiel", 1, 0, "AdjustmentTabController", QObject::tr("Cannot instantiate type 'AdjustmentTabController'"));
    qmlRegisterUncreatableType<DrugTabController>("ezechiel", 1, 0, "DrugTabController", QObject::tr("Cannot instantiate type 'DrugTabController'"));
    qmlRegisterUncreatableType<MainWindowController>("ezechiel", 1, 0, "MainWindowController", QObject::tr("Cannot instantiate type 'MainWindowController'"));

    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering model objects.");
    /*
    qmlRegisterUncreatableType<AdjustmentListModel>("ezechiel", 1, 0, "AdjustmentListModel", QObject::tr("Cannot instantiate type 'AdjustmentListModel'"));
    qmlRegisterUncreatableType<AdjustmentSettingsModel>("ezechiel", 1, 0, "AdjustmentSettingsModel", QObject::tr("Cannot instantiate type 'AdjustmentSettingsModel'"));
    qmlRegisterUncreatableType<CovariateListModel>("ezechiel", 1, 0, "CovariateListModel", QObject::tr("Cannot instantiate type 'CovariateListModel'"));
    qmlRegisterUncreatableType<CovariateModel>("ezechiel", 1, 0, "CovariateModel", QObject::tr("Cannot instantiate type 'CovariateModel'"));
    qmlRegisterUncreatableType<DomainListModel>("ezechiel", 1, 0, "DomainListModel", QObject::tr("Cannot instantiate type 'DomainListModel'"));
    qmlRegisterUncreatableType<DosageListModel>("ezechiel", 1, 0, "DosageListModel", QObject::tr("Cannot instantiate type 'DosageListModel'"));
    qmlRegisterUncreatableType<DrugListModel>("ezechiel", 1, 0, "DrugListModel", QObject::tr("Cannot instantiate type 'DrugListModel'"));
    qmlRegisterUncreatableType<DrugVariateListModel>("ezechiel", 1, 0, "DrugVariateListModel", QObject::tr("Cannot instantiate type 'DrugVariateListModel'"));
    qmlRegisterUncreatableType<InstituteListModel>("ezechiel", 1, 0, "InstituteListModel", QObject::tr("Cannot instantiate type 'InstituteListModel'"));
    qmlRegisterUncreatableType<MeasureListModel>("ezechiel", 1, 0, "MeasureListModel", QObject::tr("Cannot instantiate type 'MeasureListModel'"));
    qmlRegisterUncreatableType<PatientListModel>("ezechiel", 1, 0, "PatientListModel", QObject::tr("Cannot instantiate type 'PatientListModel'"));
    qmlRegisterUncreatableType<ValidationModel>("ezechiel", 1, 0, "ValidationModel", QObject::tr("Cannot instantiate type 'ValidationModel'"));
*/
//#ifdef CONFIG_CONNECTED
    qmlRegisterUncreatableType<PartialRequestListModel>("ezechiel", 1, 0, "PartialRequestListModel", QObject::tr("Cannot instantiate type 'PartialRequestListModel'"));
//#endif // CONFIG_CONNECTED
//    qmlRegisterUncreatableType<PracticianListModel>("ezechiel", 1, 0, "PracticianListModel", QObject::tr("Cannot instantiate type 'PracticianListModel'"));
#ifdef CONFIG_CONNECTED
    qmlRegisterUncreatableType<RequestsController>("ezechiel", 1, 0, "RequestsController", QObject::tr("Cannot instantiate type 'RequestsController'"));
#endif // CONFIG_CONNECTED
//    qmlRegisterUncreatableType<ReverseModel>("ezechiel", 1, 0, "ReverseModel", QObject::tr("Cannot instantiate type 'ReverseModel'"));
    qmlRegisterUncreatableType<SortFilterEntityListProxyModel>("ezechiel", 1, 0, "SortFilterEntityListProxyModel", QObject::tr("Cannot instantiate type 'SortFilterEntityListProxyModel'"));
//    qmlRegisterUncreatableType<StudyListModel>("ezechiel", 1, 0, "StudyListModel", QObject::tr("Cannot instantiate type 'StudyListModel'"));
//    qmlRegisterUncreatableType<TargetListModel>("ezechiel", 1, 0, "TargetListModel", QObject::tr("Cannot instantiate type 'TargetListModel'"));
    qmlRegisterUncreatableType<StartupWindow>("ezechiel", 1, 0, "StartupWindow", QObject::tr("Cannot instantiate type 'StartupWindow'"));

    qmlRegisterUncreatableType<ezechiel::core::PredictionResult>("ezechiel", 1, 0, "PredictionResult", QObject::tr("Cannot instantiate type 'PredictionResult'"));
    qmlRegisterUncreatableType<ezechiel::core::PercentileData>("ezechiel", 1, 0, "PercentilePair", QObject::tr("Cannot instantiate type 'PercentilePair'"));
    qmlRegisterUncreatableType<ezechiel::core::PercentileDataList>("ezechiel", 1, 0, "PercentileDataList", QObject::tr("Cannot instantiate type 'PecentilePairs'"));
    qmlRegisterUncreatableType<ezechiel::core::Predictive>("ezechiel", 1, 0, "Predictive", QObject::tr("Cannot instantiate type 'Predictive'"));
    qmlRegisterUncreatableType<ezechiel::core::PredictionData>("ezechiel", 1, 0, "PredictionData", QObject::tr("Cannot instantiate type 'PredictionData'"));
    qmlRegisterUncreatableType<ezechiel::core::Points>("ezechiel", 1, 0, "Points", QObject::tr("Cannot instantiate type 'Points'"));
    qmlRegisterUncreatableType<ezechiel::core::ChartData>("ezechiel", 1, 0, "ChartData", QObject::tr("Cannot instantiate type 'ChartData'"));
    qmlRegisterUncreatableType<ezechiel::core::CorePatient>("ezechiel", 1, 0, "CorePatient", QObject::tr("Cannot instantiate type 'CorePatient'"));
    qmlRegisterUncreatableType<ValidationStatus>("ezechiel", 1, 0, "Status", QObject::tr("Cannot instantiate type 'Status'"));
	qmlRegisterUncreatableType<StepType>("ezechiel", 1, 0, "StepType", QObject::tr("Cannot instantiate type 'StepType'"));

    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering other types.");
    qmlRegisterType<ProxyModelFilter>("ezechiel", 1, 0, "ProxyModelFilter");

    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering QML singletons.");
    qmlRegisterSingletonType(QUrl("qrc:/controls/Status.qml"), "guiutils.qml.controls", 1, 0, "Status");
    qmlRegisterSingletonType(QUrl("qrc:/controls/ToolTips.qml"), "guiutils.qml.controls", 1, 0, "ToolTips");
    qmlRegisterSingletonType(QUrl("qrc:/styles/Style.qml"), "guiutils.qml.styles", 1, 0, "Style");

    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Registering QML components.");
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
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Core.");
//    Q_INIT_RESOURCE(report);
//    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Report.");
    Q_INIT_RESOURCE(rest);
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Rest.");
    Q_INIT_RESOURCE(qml);
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Qml.");
    Q_INIT_RESOURCE(guiutils);
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Guiutils.");
//    Q_INIT_RESOURCE(report);
//    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Report.");

    parseOptions();
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Options.");
    initResources();
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized Resources.");
#ifdef CONFIG_CONNECTED
    initRestConfig();
    EXLOG(QtDebugMsg, ezechiel::gui::NOEZERROR, "Initialized REST configuration.");
#endif // CONFIG_CONNECTED

    QtWebEngine::initialize();

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
    StartupScreen startupScreen;
    if (startupScreen.exec() != QDialog::Accepted)
        return 0;
#endif // NOSTARTUPSCREEN

    MainWindowController* mainWindow = new MainWindowController(&app);
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
    GuiTests tests(mainWindow->getWindow());
    auto bot = new spix::QtQmlBot();
    bot->runTestServer(tests);
#endif // CONFIG_GUITEST

    int result = app.exec();

    // delete mainWindow;
    return result;
}

void parseOptions()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Tucuxi is a TDM software for pharmacologists");

    const QCommandLineOption standaloneOption(QStringList() << "a" << "standalone",
                                    QCoreApplication::translate("main", "Run EzeCHieL as a standalone software, effectively disabling any feature relying on retrieving medical data from a server."));
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
                                    QCoreApplication::translate("main", "Save applicaiotn's log messages into a file"));
    const QCommandLineOption certificateFileOption(QStringList() << "certificate",
                                    QCoreApplication::translate("main", "Set the file containing the SSL certificate."),
                                    "certificate",
                                    "");

    parser.addOption(basePathOption);
    const QCommandLineOption helpOption = parser.addHelpOption();
    parser.addOption(listFileOption);
    parser.addOption(requestFileOption);
    parser.addOption(hostOption);
    parser.addOption(portOption);
    parser.addOption(schemeOption);
    parser.addOption(logRestOption);
    parser.addOption(logMessagesInFileOption);
    parser.addOption(certificateFileOption);
    const QCommandLineOption versionOption = parser.addVersionOption();

    parser.parse(QCoreApplication::arguments());
    if (parser.isSet(helpOption))
        parser.showHelp();
    if (parser.isSet(versionOption))
        parser.showVersion();
    if (parser.isSet(logMessagesInFileOption))
        ezechiel::logging::Logger::initLogFile(QCoreApplication::applicationDirPath());
    if (parser.isSet(certificateFileOption)) {
#ifdef CONFIG_CONNECTED
        NetworkAccessManager::setType(NetworkAccessManager::ManagerType::Unique);
        NetworkAccessManager::getInstance().setUniqueDefaultCaCertificate(parser.value(certificateFileOption));
#endif // CONFIG_CONNECTED
    }

    //ToDo: set the standalone option in the settings, and disable request list
    //and other network related features
    Q_UNUSED(standaloneOption);

    SETTINGS.set(ezechiel::core::Module::GUI,"listFile", parser.value(listFileOption));
    SETTINGS.set(ezechiel::core::Module::GUI,"requestFile", parser.value(requestFileOption));
#ifdef CONFIG_CONNECTED
    MirthRequest::updateDefaultBasePath(parser.value(basePathOption));
    MirthRequest::updateDefaultPort(parser.value(portOption).toInt());
    MirthRequest::updateDefaultScheme(parser.value(schemeOption));
    MirthRequest::updateDefaultHost(parser.value(hostOption));
    RestLogger::getInstance()->setPath(parser.value(logRestOption));
#endif // CONFIG_CONNECTED

}


#ifdef COMPILE_WITH_TUCUCORE

#include "processingtucucore/src/drugs2manager.h"
#include "apputils/src/drugmanager.h"
#include "core/dal/drug/drug.h"

#endif // COMPILE_WITH_TUCUCORE

void initResources()
{
    ezechiel::core::Core::setInstance(new ezechiel::apputils::AppCore());

    CORE->setProcessingInterface(ProcessingFactory::createProcessing());
    APPCORE->drugManager();

    // We try to load the new drug models

#ifdef COMPILE_WITH_TUCUCORE
    Drugs2Manager* manager = Drugs2Manager::getInstance();
#ifdef CONFIG_DEMO
    std::string basePath = ":/drugs/";
#else
    std::string basePath = CORE->path(ezechiel::core::Core::Drugs2).toStdString() + "/";
#endif // CONFIG_DEMO
    manager->buildAllDrugModels(basePath);
    std::vector<ezechiel::core::DrugModel *> models = manager->getAllEzechielDrugModels();
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

    MirthRequest::updateDefaultPort(restConfig->getPort().toInt());
    MirthRequest::updateDefaultScheme(restConfig->getScheme());
    MirthRequest::updateDefaultHost(restConfig->getIp());
}
#endif // CONFIG_CONNECTED
