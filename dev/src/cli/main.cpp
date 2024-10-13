/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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

    QCoreApplication::setApplicationName("Tucuxi-CLI");
    QCoreApplication::setOrganizationName("REDS");
    QCoreApplication::setOrganizationDomain("http://www.ezechiel.ch/");

    EXLOG(QtDebugMsg, Tucuxi::cli::NOEZERROR, "Registering DAL objects.");
//    qRegisterMetaType<InterpretationRequest*>("InterpretationRequest*");
    qRegisterMetaType<Tucuxi::Gui::Core::Duration>("Duration");
//    qRegisterMetaType<Patient*>("Patient*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Descriptor>("Descriptor");
//    qRegisterMetaType<QMetaType::Type>("QMetaType::Type");
////    qRegisterMetaType<Tucuxi::Gui::Core::QStringTranslation>("QStringTranslation");
//    qRegisterMetaType<Person*>("Person*");
//    qRegisterMetaType<GenderType>("GenderType");
//    qRegisterMetaType<Location*>("Location*");
//    qRegisterMetaType<Practician*>("Practician*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Drug*>("Drug*");
//    qRegisterMetaType<InterpretationAnalysis*>("InterpretationAnalysis*");
//    qRegisterMetaType<Tucuxi::Gui::Core::TargetList*>("TargetList*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Unit>("Unit");
//    qRegisterMetaType<Tucuxi::Gui::Core::DrugList*>("DrugList*");
//    qRegisterMetaType<Tucuxi::Gui::Core::CoreMeasure*>("CoreMeasure*");
//    qRegisterMetaType<Tucuxi::Gui::Core::CoreMeasureList*>("CoreMeasureList*");
//    qRegisterMetaType<Tucuxi::Gui::Core::PatientVariate*>("PatientVariate*");
//    qRegisterMetaType<Tucuxi::Gui::Core::DrugVariate*>("DrugVariate*");
//    qRegisterMetaType<Tucuxi::Gui::Core::DrugTreatment*>("DrugTreatment*");
//    qRegisterMetaType<Tucuxi::Gui::Core::PredictionSpec*>("PredictionSpec*");
//    qRegisterMetaType<Tucuxi::Gui::Core::PatientVariateList*>("PatientVariateList*");
//    qRegisterMetaType<Tucuxi::Gui::Core::DosageHistory*>("DosageHistory*");
//    qRegisterMetaType<Tucuxi::Gui::Core::CorePatient*>("CorePatient*");
//    qRegisterMetaType<Tucuxi::Gui::Core::CorePatientList*>("CorePatientList*");
//    qRegisterMetaType<Tucuxi::Gui::Core::ADME*>("ADME*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Admin::Route>("ADME::Route");
//    qRegisterMetaType<Tucuxi::Gui::Core::Dosage*>("Dosage*");
//    qRegisterMetaType<Tucuxi::Gui::Core::IdentifiableAmount*>("IdentifiableAmount*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Halflife*>("Halflife*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Concentrations*>("Concentrations*");
//    qRegisterMetaType<Tucuxi::Gui::Core::ValidDoses*>("ValidDoses*");
//    qRegisterMetaType<Tucuxi::Gui::Core::ValidIntervals*>("ValidIntervals*");
//    qRegisterMetaType<Tucuxi::Gui::Core::ValidInfusions*>("ValidInfusions*");
//    qRegisterMetaType<Tucuxi::Gui::Core::DrugVariateList*>("DrugVariateList*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Target*>("Target*");
//    qRegisterMetaType<Tucuxi::Gui::Core::TargetMethod::TargetType>("TargetMethod::TargetType");
//    qRegisterMetaType<Tucuxi::Gui::Core::Operation*>("Operation*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Bsv*>("Bsv*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Parameter*>("Parameter*");
//    qRegisterMetaType<Tucuxi::Gui::Core::ParameterSet*>("ParameterSet*");
//    qRegisterMetaType<Tucuxi::Gui::Core::Parameters*>("ParameterList*");
//    qRegisterMetaType<Tucuxi::Gui::Core::FancyPoint*>("FancyPoint*");
//    qRegisterMetaType<Tucuxi::Gui::Core::FancyPoints*>("FancyPoints*");
//    qRegisterMetaType<Tucuxi::Gui::Core::ErrorModel*>("ErrorModel*");
//    qRegisterMetaType<Tucuxi::Gui::Core::MetaData*>("MetaData*");
////    qRegisterMetaType<Tucuxi::Gui::Core::Prediction*>("Prediction*");
//    qRegisterMetaType<Tucuxi::Gui::Core::DrugResponseAnalysis*>("Analysis*");
//    qRegisterMetaType<Tucuxi::Gui::Core::OperableAmount*>("OperableAmount*");
//    qRegisterMetaType<Tucuxi::Gui::Core::OperationType>("OperationType");
//    qRegisterMetaType<Tucuxi::Gui::Core::OperableValue*>("OperableValue*");
//    qRegisterMetaType<Tucuxi::Gui::Core::OperableValueList*>("OperableValueList*");
//    qRegisterMetaType<Tucuxi::Gui::Core::OperationList*>("OperationList*");
//    qRegisterMetaType<Tucuxi::Gui::Core::UncastedValue*>("UncastedValue*");
//    qRegisterMetaType<Tucuxi::Gui::Core::UncastedValueList*>("UncastedValueList*");
//    qRegisterMetaType<Interpretation*>("Interpretation*");


    CORE->setRunningMode(Tucuxi::Gui::Core::Core::CLI);

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
    Tucuxi::Gui::Core::Core::setInstance(new Tucuxi::Gui::AppUtils::AppCore());

    CORE->setProcessingInterface(ProcessingFactory::createProcessing());
    APPCORE->drugManager();
}
