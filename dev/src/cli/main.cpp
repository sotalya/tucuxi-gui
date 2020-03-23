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

using namespace ezechiel;
void initResources();

int main(int argc, char *argv[])
{

    //Setup the application
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName("EzeCHieL-CLI");
    QCoreApplication::setOrganizationName("REDS");
    QCoreApplication::setOrganizationDomain("http://www.ezechiel.ch/");

    EXLOG(QtDebugMsg, ezechiel::cli::NOEZERROR, "Registering DAL objects.");
//    qRegisterMetaType<InterpretationRequest*>("InterpretationRequest*");
    qRegisterMetaType<ezechiel::core::Duration>("Duration");
//    qRegisterMetaType<Patient*>("Patient*");
//    qRegisterMetaType<ezechiel::core::Descriptor>("Descriptor");
//    qRegisterMetaType<QMetaType::Type>("QMetaType::Type");
////    qRegisterMetaType<ezechiel::core::QStringTranslation>("QStringTranslation");
//    qRegisterMetaType<Person*>("Person*");
//    qRegisterMetaType<GenderType>("GenderType");
//    qRegisterMetaType<Location*>("Location*");
//    qRegisterMetaType<Practician*>("Practician*");
//    qRegisterMetaType<ezechiel::core::Drug*>("Drug*");
//    qRegisterMetaType<InterpretationAnalysis*>("InterpretationAnalysis*");
//    qRegisterMetaType<ezechiel::core::TargetList*>("TargetList*");
//    qRegisterMetaType<ezechiel::core::Unit>("Unit");
//    qRegisterMetaType<ezechiel::core::DrugList*>("DrugList*");
//    qRegisterMetaType<ezechiel::core::CoreMeasure*>("CoreMeasure*");
//    qRegisterMetaType<ezechiel::core::CoreMeasureList*>("CoreMeasureList*");
//    qRegisterMetaType<ezechiel::core::PatientVariate*>("PatientVariate*");
//    qRegisterMetaType<ezechiel::core::DrugVariate*>("DrugVariate*");
//    qRegisterMetaType<ezechiel::core::DrugTreatment*>("DrugTreatment*");
//    qRegisterMetaType<ezechiel::core::PredictionSpec*>("PredictionSpec*");
//    qRegisterMetaType<ezechiel::core::PatientVariateList*>("PatientVariateList*");
//    qRegisterMetaType<ezechiel::core::DosageHistory*>("DosageHistory*");
//    qRegisterMetaType<ezechiel::core::CorePatient*>("CorePatient*");
//    qRegisterMetaType<ezechiel::core::CorePatientList*>("CorePatientList*");
//    qRegisterMetaType<ezechiel::core::ADME*>("ADME*");
//    qRegisterMetaType<ezechiel::core::Admin::Route>("ADME::Route");
//    qRegisterMetaType<ezechiel::core::Dosage*>("Dosage*");
//    qRegisterMetaType<ezechiel::core::IdentifiableAmount*>("IdentifiableAmount*");
//    qRegisterMetaType<ezechiel::core::Halflife*>("Halflife*");
//    qRegisterMetaType<ezechiel::core::Concentrations*>("Concentrations*");
//    qRegisterMetaType<ezechiel::core::ValidDoses*>("ValidDoses*");
//    qRegisterMetaType<ezechiel::core::ValidIntervals*>("ValidIntervals*");
//    qRegisterMetaType<ezechiel::core::ValidInfusions*>("ValidInfusions*");
//    qRegisterMetaType<ezechiel::core::DrugVariateList*>("DrugVariateList*");
//    qRegisterMetaType<ezechiel::core::Target*>("Target*");
//    qRegisterMetaType<ezechiel::core::TargetMethod::TargetType>("TargetMethod::TargetType");
//    qRegisterMetaType<ezechiel::core::Operation*>("Operation*");
//    qRegisterMetaType<ezechiel::core::Bsv*>("Bsv*");
//    qRegisterMetaType<ezechiel::core::Parameter*>("Parameter*");
//    qRegisterMetaType<ezechiel::core::ParameterSet*>("ParameterSet*");
//    qRegisterMetaType<ezechiel::core::Parameters*>("ParameterList*");
//    qRegisterMetaType<ezechiel::core::FancyPoint*>("FancyPoint*");
//    qRegisterMetaType<ezechiel::core::FancyPoints*>("FancyPoints*");
//    qRegisterMetaType<ezechiel::core::ErrorModel*>("ErrorModel*");
//    qRegisterMetaType<ezechiel::core::MetaData*>("MetaData*");
////    qRegisterMetaType<ezechiel::core::Prediction*>("Prediction*");
//    qRegisterMetaType<ezechiel::core::DrugResponseAnalysis*>("Analysis*");
//    qRegisterMetaType<ezechiel::core::OperableAmount*>("OperableAmount*");
//    qRegisterMetaType<ezechiel::core::OperationType>("OperationType");
//    qRegisterMetaType<ezechiel::core::OperableValue*>("OperableValue*");
//    qRegisterMetaType<ezechiel::core::OperableValueList*>("OperableValueList*");
//    qRegisterMetaType<ezechiel::core::OperationList*>("OperationList*");
//    qRegisterMetaType<ezechiel::core::UncastedValue*>("UncastedValue*");
//    qRegisterMetaType<ezechiel::core::UncastedValueList*>("UncastedValueList*");
//    qRegisterMetaType<Interpretation*>("Interpretation*");


    CORE->setRunningMode(ezechiel::core::Core::CLI);

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
    ezechiel::core::Core::setInstance(new ezechiel::apputils::AppCore());

    CORE->setProcessingInterface(ProcessingFactory::createProcessing());
    APPCORE->drugManager();
}
