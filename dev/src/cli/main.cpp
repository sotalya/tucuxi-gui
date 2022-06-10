//@@license@@

#include <QApplication>
#include <QTimer>


#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/interpretationrequest.h"
#include "admin/src/dal/interpretationanalysis.h"
#include "admin/src/dal/measure.h"
#include "admin/src/dal/interpretation.h"

#include "cliutils/climanager.h"
#include "apputils/src/appcore.h"
#include "errors_cli.h"
#include "core/utils/logging.h"
#include "core/core.h"
#include "core/dal/drug/adme.h"
#include "core/dal/drug/drug.h"
#include "core/utils/duration.h"
#include "core/dal/predictionspec.h"
#include "core/dal/covariate.h"
#include "core/dal/drug/target.h"
#include "core/dal/prediction.h"
#include "core/dal/uncastedvalue.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/concentrations.h"
#include "core/dal/drug/halflife.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/bsv.h"
#include "core/dal/drug/errormodel.h"
#include "core/dal/drug/metadata.h"
#include "core/processingfactory.h"


//#define QT_MESSAGE_PATTERN="[%{time yyyyMMdd h:mm:ss.zzz t} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}"
/* Load the translator and launch the CLI
  */

using namespace Tucuxi;
void initResources();

int main(int argc, char *argv[])
{

    //Setup the application
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName("EzeCHieL-CLI");
    QCoreApplication::setOrganizationName("REDS");
    QCoreApplication::setOrganizationDomain("http://www.ezechiel.ch/");

    EXLOG(QtDebugMsg, Tucuxi::cli::NOEZERROR, "Registering DAL objects.");
//    qRegisterMetaType<InterpretationRequest*>("InterpretationRequest*");
    qRegisterMetaType<Tucuxi::GuiCore::Duration>("Duration");
//    qRegisterMetaType<Patient*>("Patient*");
//    qRegisterMetaType<Tucuxi::GuiCore::Descriptor>("Descriptor");
//    qRegisterMetaType<QMetaType::Type>("QMetaType::Type");
////    qRegisterMetaType<Tucuxi::GuiCore::QStringTranslation>("QStringTranslation");
//    qRegisterMetaType<Person*>("Person*");
//    qRegisterMetaType<GenderType>("GenderType");
//    qRegisterMetaType<Location*>("Location*");
//    qRegisterMetaType<Practician*>("Practician*");
//    qRegisterMetaType<Tucuxi::GuiCore::Drug*>("Drug*");
//    qRegisterMetaType<InterpretationAnalysis*>("InterpretationAnalysis*");
//    qRegisterMetaType<Tucuxi::GuiCore::TargetList*>("TargetList*");
//    qRegisterMetaType<Tucuxi::GuiCore::Unit>("Unit");
//    qRegisterMetaType<Tucuxi::GuiCore::DrugList*>("DrugList*");
//    qRegisterMetaType<Tucuxi::GuiCore::CoreMeasure*>("CoreMeasure*");
//    qRegisterMetaType<Tucuxi::GuiCore::CoreMeasureList*>("CoreMeasureList*");
//    qRegisterMetaType<Tucuxi::GuiCore::PatientVariate*>("PatientVariate*");
//    qRegisterMetaType<Tucuxi::GuiCore::DrugVariate*>("DrugVariate*");
//    qRegisterMetaType<Tucuxi::GuiCore::DrugTreatment*>("DrugTreatment*");
//    qRegisterMetaType<Tucuxi::GuiCore::PredictionSpec*>("PredictionSpec*");
//    qRegisterMetaType<Tucuxi::GuiCore::PatientVariateList*>("PatientVariateList*");
//    qRegisterMetaType<Tucuxi::GuiCore::DosageHistory*>("DosageHistory*");
//    qRegisterMetaType<Tucuxi::GuiCore::CorePatient*>("CorePatient*");
//    qRegisterMetaType<Tucuxi::GuiCore::CorePatientList*>("CorePatientList*");
//    qRegisterMetaType<Tucuxi::GuiCore::ADME*>("ADME*");
//    qRegisterMetaType<Tucuxi::GuiCore::Admin::Route>("ADME::Route");
//    qRegisterMetaType<Tucuxi::GuiCore::Dosage*>("Dosage*");
//    qRegisterMetaType<Tucuxi::GuiCore::IdentifiableAmount*>("IdentifiableAmount*");
//    qRegisterMetaType<Tucuxi::GuiCore::Halflife*>("Halflife*");
//    qRegisterMetaType<Tucuxi::GuiCore::Concentrations*>("Concentrations*");
//    qRegisterMetaType<Tucuxi::GuiCore::ValidDoses*>("ValidDoses*");
//    qRegisterMetaType<Tucuxi::GuiCore::ValidIntervals*>("ValidIntervals*");
//    qRegisterMetaType<Tucuxi::GuiCore::ValidInfusions*>("ValidInfusions*");
//    qRegisterMetaType<Tucuxi::GuiCore::DrugVariateList*>("DrugVariateList*");
//    qRegisterMetaType<Tucuxi::GuiCore::Target*>("Target*");
//    qRegisterMetaType<Tucuxi::GuiCore::TargetMethod::TargetType>("TargetMethod::TargetType");
//    qRegisterMetaType<Tucuxi::GuiCore::Operation*>("Operation*");
//    qRegisterMetaType<Tucuxi::GuiCore::Bsv*>("Bsv*");
//    qRegisterMetaType<Tucuxi::GuiCore::Parameter*>("Parameter*");
//    qRegisterMetaType<Tucuxi::GuiCore::ParameterSet*>("ParameterSet*");
//    qRegisterMetaType<Tucuxi::GuiCore::Parameters*>("ParameterList*");
//    qRegisterMetaType<Tucuxi::GuiCore::FancyPoint*>("FancyPoint*");
//    qRegisterMetaType<Tucuxi::GuiCore::FancyPoints*>("FancyPoints*");
//    qRegisterMetaType<Tucuxi::GuiCore::ErrorModel*>("ErrorModel*");
//    qRegisterMetaType<Tucuxi::GuiCore::MetaData*>("MetaData*");
////    qRegisterMetaType<Tucuxi::GuiCore::Prediction*>("Prediction*");
//    qRegisterMetaType<Tucuxi::GuiCore::DrugResponseAnalysis*>("Analysis*");
//    qRegisterMetaType<Tucuxi::GuiCore::OperableAmount*>("OperableAmount*");
//    qRegisterMetaType<Tucuxi::GuiCore::OperationType>("OperationType");
//    qRegisterMetaType<Tucuxi::GuiCore::OperableValue*>("OperableValue*");
//    qRegisterMetaType<Tucuxi::GuiCore::OperableValueList*>("OperableValueList*");
//    qRegisterMetaType<Tucuxi::GuiCore::OperationList*>("OperationList*");
//    qRegisterMetaType<Tucuxi::GuiCore::UncastedValue*>("UncastedValue*");
//    qRegisterMetaType<Tucuxi::GuiCore::UncastedValueList*>("UncastedValueList*");
//    qRegisterMetaType<Interpretation*>("Interpretation*");


    CORE->setRunningMode(Tucuxi::GuiCore::Core::CLI);

    //Initialize the Core
    Q_INIT_RESOURCE(core);
#ifdef CONFIG_CONNECTED
    Q_INIT_RESOURCE(rest);
    EXLOG(QtDebugMsg, cli::NOEZERROR, "Initialized Rest.");
#endif

    initResources();
    cliutils::CliManager* cli = new cliutils::CliManager(&app);
    QObject::connect(cli, SIGNAL(finished()), &app, SLOT(quit()));
    //this is required to make the lambda run while the event loop is active
    QTimer::singleShot(10, &app, [&](){
        cli->execute(app, argc, argv);
    });
    //ok instead of a socket and a QSocketNotifier, just wait 5 secs and then exit.
    QTimer::singleShot(25000, &app, [&](){
        cli->finished();
    });
    return app.exec();
}

void initResources()
{
    Tucuxi::GuiCore::Core::setInstance(new Tucuxi::GuiAppUtils::AppCore());

    CORE->setProcessingInterface(ProcessingFactory::createProcessing());
    APPCORE->drugManager();
}
