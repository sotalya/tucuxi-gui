#include "predictionaction.h"


#include <iostream>

#include <QRegExp>
#include <QVector>
#include <QFile>
#include <QMainWindow>
#include <QApplication>
#include <QList>
#include <QTextStream>

#include "apputils/src/appcore.h"
//#include "coremessagehandler.h"
#include "core/pluginmanager.h"
//#include "core/interfaces/database.h"
#include "core/utils/dataset.h"
//#include "chart.h"
//#include "chartitemgroup.h"
//#include "percentilecurve.h"
#include "core/dal/percentile.h"
//#include "curve.h"
#include "admin/src/dal/patient.h"
//#include "dot.h"
//#include "targetinterval.h"
#include "apputils/src/drugmanager.h"
#include "core/corefactory.h"
#include "admin/src/stdadminrepository.h"
#include "admin/src/adminfactory.h"
#include "core/corerepository.h"
#include "core/interfaces/processinginterface.h"
#include "core/interfaces/processingtraits.h"
#include "core/utils/logging.h"
#include "errors_cliutils.h"
#include "core/dal/predictionspec.h"
#include "core/dal/drug/drug.h"
#include "apputils/src/apputilsrepository.h"

namespace ezechiel {
namespace cliutils {
//The error messages
const char *const PredictionAction::_INVALID_COMMAND = QT_TR_NOOP_UTF8("Invalid or incomplete '%1' command, please check the help and try again.");

//The shared messages
const char *const PredictionAction::_ENGINE_OPTIONS_DISPLAY = QT_TR_NOOP_UTF8("The %1 options are as follows:");
const char *const PredictionAction::_MO = QT_TR_NOOP_UTF8("model");
const char *const PredictionAction::_PE = QT_TR_NOOP_UTF8("percentiles engine");
const char *const PredictionAction::_PO = QT_TR_NOOP_UTF8("post engine");
const char *const PredictionAction::_RE = QT_TR_NOOP_UTF8("reverse engine");

//The commands keywords
const char *const PredictionAction::_CMD_LIST    = "list";
const char *const PredictionAction::_CMD_OPTION  = "option";
const char *const PredictionAction::_CMD_MO_ABRV = "model";
const char *const PredictionAction::_CMD_PE_ABRV = "pe";
const char *const PredictionAction::_CMD_PO_ABRV = "po";
const char *const PredictionAction::_CMD_RE_ABRV = "re";
const char *const PredictionAction::_CMD_CURVE_CONTINUOUS  = "continuous";
const char *const PredictionAction::_CMD_CURVE_STEADYSTATE = "steadyState";
const char *const PredictionAction::_CMD_PARAM_TYPICAL     = "typical";
const char *const PredictionAction::_CMD_PARAM_APRIORI     = "apriori";
const char *const PredictionAction::_CMD_PARAM_APOSTERIORI = "aposteriori";

PredictionAction::PredictionAction()
{

}

QString PredictionAction::help()
{
        QString str;
        str = tr("   NAME") + "\n" +
              tr("       EzeCHieL Prediction Command") + "\n" + "\n" +
              tr("   SYNOPSIS") + "\n" +
                 "       " + QString("ezechiel-cli [OPTIONS] curve %1USERNAME %2PASSWORD SUBCOMMAND").arg(_CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD) + "\n" + "\n" +
              tr("   DESCRIPTION") + "\n" +
              tr("       The EzeCHieL Prediction Command can be used to manage curves and their associated data, such as dosages, engines parameters and more. The 'compare' option in the 'percentile' and the 'points' sub-commands can be used to compare the resulting data with the values in a file. All dates must be specified using the format '%1'.").arg(Action::dateTimeFormat()) + "\n" + "\n" +
              tr("   SUBCOMMANDS") + "\n" +
                 "       list" + "\n" +
              tr("              Display the list of curves.")  + "\n" + "\n" +
                 "       create NAME DRUG PATIENT MODEL FIRSTTAKE [pe=PERCENTILEENGINE] [po=POSTENGINE] [re=REVERSEENGINE]" + "\n" +
              tr("              Create a new curve for the given PATIENT with the given NAME, DRUG, MODEL and FIRSTTAKE. All engines not specified will be set to the highest priority engines.")  + "\n" + "\n" +
                 "       ID remove" + "\n" +
              tr("              Remove the curve with the given ID.") + "\n" + "\n" +
                 "       " + QString("ID curveType [%1|%2]").arg(_CMD_CURVE_CONTINUOUS, _CMD_CURVE_STEADYSTATE) + "\n" +
              tr("              Get or set the curve's' parameter type.") + "\n" + "\n" +
                 "       " + QString("ID type [%1|%2|%3]").arg(_CMD_PARAM_TYPICAL, _CMD_PARAM_APRIORI, _CMD_PARAM_APOSTERIORI) + "\n" +
              tr("              Get or set the curve's' parameter type.") + "\n" + "\n" +
                 "       ID name [NAME]" + "\n" +
              tr("              Get or set the curve's' name.") + "\n" + "\n" +
                 "       ID" + _CMD_OPTION + " " + QString("%1 [%2|%3|%4|%5]").arg(_CMD_LIST, _CMD_MO_ABRV, _CMD_PE_ABRV, _CMD_PO_ABRV, _CMD_RE_ABRV) + "\n" +
              tr("              List the current options of the model and engines.") + "\n" + "\n" +
                 "       ID" + _CMD_OPTION + " " + QString("%1|%2|%3|%4 OPTIONID [VALUE]").arg(_CMD_MO_ABRV, _CMD_PE_ABRV, _CMD_PO_ABRV, _CMD_RE_ABRV) + "\n" +
              tr("              Get or set the value of a model or engine's specific option.") + "\n" + "\n" +
                 "       ID" + QString(" parameters DATE [%1|%2|%3]").arg(_CMD_PARAM_TYPICAL, _CMD_PARAM_APRIORI, _CMD_PARAM_APOSTERIORI) + "\n" +
              tr("              Get or set the curve's reverse engine.") + "\n" + "\n" +
                 "       ID dosage list" + "\n" +
              tr("              List the curve's dosages.") + "\n" + "\n" +
                 "       ID dosage add DATE DOSE INTERVAL INFUSIONTIME DOSAGEUNIT" + "\n" +
              tr("              Add a dosage applied at this DATE with this DOSE, this INTERVAL, this INFUSIONTIME, and this DOSAGEUNIT.") + "\n" + "\n" +
                 "       ID dosage set DOSAGEID [applied=DATE] [dose=DOSE] [interval=INTERVAL] [tinf=INFUSIONTIME] [unit=UNIT]" + "\n" +
              tr("              Modify the dosage DOSAGEID.") + "\n" + "\n" +
                 "       ID dosage remove DOSAGEID" + "\n" +
              tr("              Remove the dosage DOSAGEID.") + "\n" + "\n" +
                 "       " + QString("ID point DATE [%1|%2|%3]").arg(_CMD_PARAM_TYPICAL, _CMD_PARAM_APRIORI, _CMD_PARAM_APOSTERIORI) + "\n" +
              tr("              Get the concentration value at the given DATE, for the given type. If the type is not given, the curve's current parameters type will be used.") + "\n" + "\n" +
                 "       ID points FROM TO DELTA [time[=FORMAT]] [header] [precision=PRECISION] [percentiles=X-Y[,...]] [file=FILE] [separator=SEPARATOR]" + "\n" +
              tr("              Extracts the curve points from the date FROM to the date TO, with DELTA minutes between each points. If 'time' is specified, the date of each point will be added to the output using the specified optional FORMAT. If 'header' is specified, an header will be added at the first line of the output. If 'precision' is specified, the number of digits after the decimal point will be set to PRECISION. If 'percentiles' is specified, the percentiles X-Y will also be exported. If 'file' is specified, the data will be exported in the specified FILE. If 'separator' is specified, the SEPARATOR will be used to separate data of each line.") + "\n" + "\n" +
                 "       ID percentile FROM TO POINTS VALUE [type=TYPE] [compare=FILE[,prec=PRECISION] [,unit=UNIT] [,quiet]]" + "\n" +
              tr("              Calculate percentile VALUE of POINTS number of points between FROM and TO using this TYPE. FROM and TO are duration after the first take, VALUE is in percent.") + "\n" + "\n" +
                 "       ID chart FROM TO POINTS [percentile=X-Y[,...]] [type=TYPE] [save=FILE, WIDTH, HEAGHT, DPI]" + "\n" +
              tr("              Calculate a curve of POINTS number of points between FROM and TO using this TYPE and with these percentile. FROM and TO are duration after the first take.") + "\n" + "\n" +
              "\n";

        return str;
}

//Redirect to the other functions
bool PredictionAction::run(const QStringList &iargs)
{
    QStringList args = iargs;

    _errors.clear();

    //We need at least a command
    if (args.size() < 1) {
        _errors = tr("The 'curve' command requires at least a sub-command");
        return false;
    }

    QString comm = args.at(0);
    args.removeFirst();

    //List and create curve
    if (comm == "list")
        return list();
    else if (comm == "create")
        return create(args);

    //Get the ID
    bool ok;
    ident id = comm.toInt(&ok);
    if (!ok) {
        _errors = tr("The value '%1' is not an ID, which is numerical").arg(comm);
        return false;
    }

    //Test the argument count (at least the ID and a sub-command)
    if (args.size() < 1) {
        _errors = tr("The curve ID and a sub-command is required");
        return false;
    }

    comm = args.at(0);
    args.removeFirst();

    //Remove action
    if (comm == "remove")
        return remove(id);

    //We need to fetch the curve
    DrugResponseAnalysis* _resp = ezechiel::core::CoreFactory::createEntity<DrugResponseAnalysis>(REPO);
    Response r = REPO->getDrugResponseAnalysisFromId(id, _resp);
    if (r.error != NoError) {
        LOG(QtFatalMsg, OBJECTNOTFOUND, tr("Drug response analysis not found from id %1").arg(id));
        return false;
    }
    //no given architecture that can do this yet, so I assume a predction object is supplied each time, otherwise make a default one
    QList<Prediction*> _preds;
    r = REPO->getPredictionsList(id, _preds);
    Prediction* pred;
    if (_preds.empty()) {
         pred = CoreFactory::createEntity<Prediction>(ABSTRACTREPO, this);
         pred->setParamsType(APRIORI);
         pred->setAnalysis(_resp);
    } else {
        pred = _preds.first();
    }
    if (args.size() > 0) {
        if (pred->getParamsType() == APOSTERIORI) {
            pred->getSpec()->setStartDate(pred->getTreatment()->getDosages()->first()->getApplied());
        } else {
            pred->getSpec()->setStartDate(QDateTime::fromString(args.at(0), _dateTimeFormat));
            if (args.size() > 1) {
                pred->getSpec()->setEndDate(QDateTime::fromString(args.at(1), _dateTimeFormat));
            }
        }
    }
    PatientVariateList *_pvars = CoreFactory::createEntity<PatientVariateList>(REPO);
    QList<PatientVariate*> _pvlist = _pvars->getList();
    r = ADMINREPO->getPatientVariatesofPatient(pred->getTreatment()->getPatient()->id(), _pvlist);
    pred->getTreatment()->setCovariates(_pvars);

    //Launch the sub-action
    bool _ret = false;
    if (comm == "dosage")
        _ret = dosage(pred, args);
    else if (comm == "curveType")
        _ret = curveType(pred,args);
    else if (comm == "type")
        _ret = type(pred,args);
    else if (comm == "parameters")
        _ret = parameters(pred,args);
    else if (comm == "points")
        _ret = points(pred,args);
    else if (comm == "point")
        _ret = point(pred,args);
    else if (comm == "percentile")
        _ret = percentile(pred,args);
    else if (comm == "reverse")
        _ret = reverse(pred,args);
    else if (comm == "option")
        _ret = option(pred,args);
    else {
        _errors = tr("Invalid sub-command : %1").arg(comm);
        _ret = false;
    }

    return _ret;
}

//Create the curve
bool PredictionAction::create(const QStringList &args)
{
    //We need at least 5 arguments
    if (args.size() < 5) {
        _errors = tr("The 'create' command requires at least 5 arguments : {name} {drug} {patient} {model} {first take}");
        return false;
    }

    Prediction* _pred = ezechiel::core::CoreFactory::createEntity<Prediction>(REPO);

    //Travel through the optional parameters
    QString pe,po,re;
    foreach (QString arg, args.mid(5)) {
        if (arg.startsWith("pe="))
            pe = arg.mid(4);
        else if (arg.startsWith("po="))
            po = arg.mid(4);
        else if (arg.startsWith("re="))
            re = arg.mid(4);
        else {
            _errors = tr("Invalid argument given : %1").arg(arg);
            return false;
        }
    }

    //Fetch the first take
    QDateTime date = QDateTime::fromString(args.at(4), Action::dateTimeFormat());
    if (!date.isValid()) {
        _errors = tr("Invalid first drug take date given : %1").arg(args.at(4));
        return false;
    }

    SharedPatient _patient = AdminFactory::createEntity<Patient>(ADMINREPO);
    ezechiel::core::Response r = ADMINREPO->getPatientFromId(args.at(2).toInt(), _patient);
    if (r.error != NoError) {
        return false;
    }

    //Set the values in the curve
    _pred->setName(args.at(0));
    _pred->getTreatment()->getDosages()->setFirsttake(date);
    _pred->getSpec()->setStartDate(date);
//    _pred->getTreatment()->getDrugModel(SharedDrugModel(CORE->pluginManager()->newDrugModelEngine(args.at(3))));
//    _pred->getTreatment()->getDrugModel()->setDrug(APPCORE->drugManager()->drug(args.at(1)));
//    _pred->getAnalysis()->setDrugModel(SharedDrugModel(CORE->pluginManager()->newDrugModelEngine(args.at(3))));
//    _pred->getAnalysis()->getDrugModel()->setDrug(APPCORE->drugManager()->drug(args.at(1)));

    // TODO : With refactoring, the next line is no longer valid:
    // _pred->getTreatment()->setDrug(APPCORE->drugManager()->drug(args.at(1)));
    LOG(QtFatalMsg, CANTSAVE, "Refactoring needed!");

    //    _pred->getAnalysis()->setPercentileEngine(SharedPercentileEngine(CORE->pluginManager()->newPercentileEngine(pe)));
//    _pred->getAnalysis()->setPostEngine(SharedPostEngine(CORE->pluginManager()->newPostEngine(po)));
//    _pred->getAnalysis()->setReverseEngine(SharedReverseEngine(CORE->pluginManager()->newReverseEngine(re)));
    _pred->getTreatment()->setPatient(_patient);
    _pred->setParamsType(ParameterType::POPULATION);

    //Save the curve
    r = REPO->setPrediction(_pred);
    if (r.error != NoError) {
        LOG(QtFatalMsg, CANTSAVE, "Couldnt save prediction.");
        return false;
    }

    //Display its ID
    LOG(QtDebugMsg, NOEZERROR, QString::number(_pred->id()));
    _pred->deleteLater();
    return true;
}

//Display the list of available curves
bool PredictionAction::list()
{
    QList<Prediction*> curves;

    //Fetch the list
//    if (!DATABASE->tryRequest(DB->getCurvesList(curves), tr("Error while loading the curve list"), _errors))
    ezechiel::core::Response r = REPO->getPredictionsList(curves);
    if (r.error != NoError && r.error != NotFound)
        return false;

    //Display it
    LOG(QtDebugMsg, NOEZERROR, tr("ID \t: NAME"));
    for (int i = 0; i < curves.size(); i++)
        LOG(QtDebugMsg, NOEZERROR, QString(tr("%1 \t: %2").arg(QString::number(curves.at(i)->id()), curves.at(i)->getName())));

    return true;
}

//Remove this curve
bool PredictionAction::remove(ident id)
{
//    return DATABASE->tryRequest( DB->deleteCurve(id),tr("Error while removing the curve %1").arg(id), _errors);
    if (REPO->deletePrediction(id).error != NoError) {
        return false;
    }
    return true;
}

bool PredictionAction::curveType(Prediction* curve, const QStringList &args)
{
    //Display the type if args is empty
    CurveType cType;
    if (args.isEmpty()) {
        LOG(QtDebugMsg, NOEZERROR, "No curvetype arguments set.");
    }
    //Set the curve's type otherwise
    else {
        if (args.at(0) == _CMD_CURVE_CONTINUOUS) {
            cType = Continous;
        } else if (args.at(0) == _CMD_CURVE_STEADYSTATE) {
            cType = Partial;
        } else {
            _errors = tr("Invalid or unknown curve type '%1'").arg(args.at(0));
            return false;
        }

        curve->setCurveType(cType);
        curve->save();
    }

    return true;
}

//Get or set the type
bool PredictionAction::type(Prediction* curve, const QStringList &args)
{
    //Without arguments, show the current value
    if (args.isEmpty()) {
        LOG(QtDebugMsg, NOEZERROR, typeToString(curve->getParamsType()));
    }
    //If there is a argument, set the value
    else {
        bool ok;
        ParameterType ptype = stringToType(args.at(0),ok);
        if (!ok)
            return false;
        curve->setParamsType(ptype);
        curve->save();
    }

    return true;
}

//Print the parameters at this time
bool PredictionAction::parameters(Prediction* curve, const QStringList &args)
{
    QDateTime date;

    //Check if enough parameters
    if (args.size() < 1) {
        _errors = tr("Missing parameter 'DATE'.");
        return false;
    }

    //Check if too many parameters
    if (args.size() > 2) {
        _errors = tr("Too many parameters.");
        return false;
    }

    //Get the date
    if (!(date = QDateTime::fromString(args.at(0), Action::dateTimeFormat())).isValid()) {
        _errors = tr("The given date is invalid or not corretly formatted. Please use the format '%1'.").arg(Action::dateTimeFormat());
        return false;
    }

    //Refresh the curve data
    Amount _amt;
    ParamTraits* ptraits;
    curve->getParamsType() == APRIORI ? ptraits = new AprioriTraits() : ptraits = new AposterioriTraits();
    PredictionResult* pred = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);
    ProcessingResult result = CORE->getProcessingInterface()->point(curve->getAnalysis(),
             PointTraits(curve->getSpec()->getStartDate(), curve->getSpec()->getEndDate(), date, ptraits, std::vector<double>(0)),_amt, *pred);

    //Display the parameters
//    LOG(QtDebugMsg, NOEZERROR, tr("PID \t VALUE \t\t UNIT \t STANDARD \t PROPORTIONAL"));
//    foreach (QString pid, parameters->pids()) {

        //Get the parameter ID
//        SharedParameter parameter = parameters->get(pid);

        //Disaply its values
//        LOG(QtDebugMsg, NOEZERROR, QString(tr("%1 \t %2 \t %3 \t %4 \t %5").arg(pid, QString::number(parameter->value(), 'f', 5), parameter->unit().toString(), QString::number(parameter->standard(), 'f', 5), QString::number(parameter->proportional(), 'f', 5))));
//    }

    return true;
}

//Manages the option commands
bool PredictionAction::option(Prediction* curve, QStringList &args)
{
    //Check if there is at least one command
    if (args.isEmpty()) {
        _errors = tr(_INVALID_COMMAND).arg(_CMD_OPTION);
        return false;
    }

    //Run the subcommand
    if (args[0] == _CMD_LIST)
        return optionList(curve, args);
//    if (args[0] == _CMD_MO_ABRV && args.size() > 1)
//        return optionManage(curve, args, curve->getAnalysis()->getDrugModel());
//    if (args[0] == _CMD_PE_ABRV && args.size() > 1)
//        return optionManage(curve, args, curve->getAnalysis()->getPercentileEngine());
//    if (args[0] == _CMD_PO_ABRV && args.size() > 1)
//        return optionManage(curve, args, curve->getAnalysis()->getPostEngine());
//    if (args[0] == _CMD_RE_ABRV && args.size() > 1)
//        return optionManage(curve, args, curve->getAnalysis()->getReverseEngine());

    //Display an error message
    _errors = tr(_INVALID_COMMAND).arg("option");
    return false;
}

//List the engines options
bool PredictionAction::optionList(Prediction* curve, QStringList &args)
{
    //Remove the "list" argument
    args.removeFirst();

    //Display the model options
//    if (args.isEmpty() || args[0] == _CMD_MO_ABRV) {
//        LOG(QtDebugMsg, NOEZERROR, QString(tr(_ENGINE_OPTIONS_DISPLAY).arg(tr(_MO))));
//        optionMultipleDisplay(curve->getAnalysis()->getDrugModel());
//    }

//    //Display the PE options
//    if (args.isEmpty() || args[0] == _CMD_PE_ABRV) {
//        LOG(QtDebugMsg, NOEZERROR, QString(tr(_ENGINE_OPTIONS_DISPLAY).arg(tr(_PE))));
//        optionMultipleDisplay(curve->getAnalysis()->getPercentileEngine());
//    }

//    //Display the PO options
//    if (args.isEmpty() || args[0] == _CMD_PO_ABRV) {
//        LOG(QtDebugMsg, NOEZERROR, QString(tr(_ENGINE_OPTIONS_DISPLAY).arg(tr(_PO))));
//        optionMultipleDisplay(curve->getAnalysis()->getPostEngine());
//    }

    //Display the RE options
//    if (args.isEmpty() || args[0] == _CMD_RE_ABRV) {
//        LOG(QtDebugMsg, NOEZERROR, QString(tr(_ENGINE_OPTIONS_DISPLAY).arg(tr(_RE))));
//        optionMultipleDisplay(curve->getAnalysis()->getReverseEngine());
//    }

    return true;
}

//Get or set a specific option
bool PredictionAction::optionManage(Prediction* curve, QStringList &args, Optionable *engine)
{
    //Remove the first argument
    args.removeFirst();

    //Check if the option exists
    if (!engine->optionExists(args[0])) {
        _errors = tr("The option ID '%1' could not be found for the specified engine.").arg(args[0]);
        return false;
    }

    //Check if display only
    if (args.size() == 1) {
        optionSingleDisplay(engine->option(args[0]));
        return true;
    }

    //Set the option value
    if (!engine->setOptionValue(args[0], args[1])) {
        _errors = tr("Failed to assign the value '%1' to the option ID '%2' in the sepcified engine.").arg(args[1], args[0]);
        return false;
    }

    //Save the curve
    Response r = REPO->setPrediction(curve);
    if (r.error != NoError) {
        LOG(QtFatalMsg, CANTSAVE, "Couldnt save prediction.");
        return false;
    }

    //Display the new value
    LOG(QtDebugMsg, NOEZERROR, tr("The value was successfully assigned:"));
    optionSingleDisplay(engine->option(args[0]));

    return true;
}

//Display an engine options
void PredictionAction::optionMultipleDisplay(const Optionable *engine)
{
    foreach (Option option, engine->getOptions())
        optionSingleDisplay(option);
}

//Display a single option value
void PredictionAction::optionSingleDisplay(const Option &option)
{
    LOG(QtDebugMsg, NOEZERROR, QString(tr("Option ID '%1': %2").arg(option.oid, option.value.toString())));
}

//Check the arguments and launch the sub-command
bool PredictionAction::dosage(Prediction* curve, QStringList &args)
{
    //At least a sub-command
    if (args.size() < 1) {
        _errors = tr("A sub-command is required for the 'dosage' command");
        return false;
    }

    QString comm = args.at(0);
    args.removeFirst();

    //Remove sub-command
    if (comm == "list")
            return dosageList(curve);
    else if (comm == "add")
            return dosageAdd(curve,args);

    //Need an ID
    if (args.size() < 1) {
        _errors = tr("A dosage ID is required to set or remove a dosage");
        return false;
    }

    //Get the ID
    bool ok;
    ident id = comm.toInt(&ok);
    if (!ok) {
        _errors = tr("The value '%1' is not an ID, which is numerical").arg(comm);
        return false;
    }

    comm = args.at(0);
    args.removeFirst();

    //Launch the sub-command
    if (comm == "set")
        return dosageSet(curve,id,args);
    else if (comm == "remove")
        return dosageRemove(id);
    else {
        _errors = tr("Invalid 'dosage' sub-command : %1").arg(comm);
        return false;
    }

    return true;
}

bool PredictionAction::dosageList(Prediction* curve)
{
    //Fetch it
    DosageHistory* list = ezechiel::core::CoreFactory::createEntity<DosageHistory>(REPO);
//    if (! DATABASE->tryRequest(DB->getDosagesByDate(curve->id(),list),
//                               tr("Error while retrieving the curve's dosage list from the database"), _errors) )
    ezechiel::core::Response r = REPO->getDosagesByDate(curve->id(),list);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }

    //Print it
    LOG(QtDebugMsg, NOEZERROR, QString(tr("ID \t TIME \t DOSE \t INTERVAL \t TINF")));
    foreach (Dosage* d, list->getList())
        LOG(QtDebugMsg, NOEZERROR, QString(tr("%1 \t %2 \t %3 \t %4 \t %5")
                .arg(d->id())
                .arg(d->getApplied().toString(Action::dateTimeFormat()))
                .arg(d->getQuantity()->toString())
                .arg(d->getInterval().toString())
                .arg(d->getTinf().toString())));

    return true;
}

//Add the dosage in the DB
bool PredictionAction::dosageAdd(Prediction* curve, QStringList &args)
{
    //We need 4 arguments
    if (args.size() < 4) {
        _errors = tr("The 'add' command for 'dosage' requires 4 arguments : {date} {dose} {interval} {tinf} [dosage unit]");
        return false;
    }

    //Create the dosage
    Dosage* d = ezechiel::core::CoreFactory::createEntity<Dosage>(REPO);
    // YTA commented this
    // d->prediction(curve);

    //Applied
    d->setApplied(QDateTime::fromString(args.at(0), Action::dateTimeFormat()));
    if (!d->getApplied().isValid())
        _errors = tr("Invalid date given : %1").arg(args.at(0));

    //Dose
    QString dosestring = args.at(1);
    if (args.size() == 5) {
            dosestring = dosestring + " " + args.at(4);
    }
    else {
        // TODO : With refactoring we removed this doseUnit...
        LOG(QtFatalMsg, CANTSAVE, "Please refactor");
//        dosestring = dosestring + " " + curve->getTreatment()->doseUnit().name();
    }

    if (!d->getQuantity()->fromString(dosestring))
        _errors = tr("Invalid dose given : %1").arg(args.at(1));
    d->getQuantity()->setValue(d->getQuantity()->value());
    d->getQuantity()->setUnit(d->getQuantity()->unit().toString());

    //Interval
    if (!d->getInterval().fromString(args.at(2)))
        _errors = tr("Invalid interval given : %1").arg(args.at(2));
    d->setInterval(Duration(args.at(2)));

    //Tinf
    if (!d->getTinf().fromString(args.at(3)))
        _errors = tr("Invalid infusion time given : %1").arg(args.at(3));
    d->setTinf(Duration(args.at(3)));

    //On error, quit
    if (!_errors.isEmpty())
        return false;

    //Add it in the database
//    return DATABASE->tryRequest( DB->setDosage(curve->id(),d), tr("Error while adding the new dosage"), _errors );
    if (REPO->setDosage(d).error != NoError) {
            return false;
    }
    return true;
}

//Modify the dosage
bool PredictionAction::dosageSet(Prediction* curve, ident id, QStringList &args)
{
    //We need at least 1 arguments
    if (args.size() < 1) {
        _errors = tr("The 'set' command for 'dosage' requires at least one arguments : [applied=date] [dose=dose] [interval=time] [tinf=time]");
        return false;
    }

    //Fetch the dosage in the database
    Dosage* dosage;
    DosageHistory* list;
//    if (! DATABASE->tryRequest(DB->getDosagesByDate(curve->id(),list),
//                               tr("Error while retrieving the curve's dosage list from the database"), _errors) )
//        return false;
    ezechiel::core::Response r = REPO->getDosagesByDate(curve->id(),list);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }
    //Search in the given list
    foreach (Dosage* d, list->getList())
        if (d->id() == id) {
            dosage = d;
            break;
        }

    //Go through the arguments
    foreach (QString arg, args) {

        //Separate the variable from the value
        QRegExp reg("([a-z]+)=(.*)");
        reg.indexIn(arg);
        QString var = reg.cap(1);
        QString value = reg.cap(2);

        //Applied
        if (var == "applied") {
            dosage->setApplied(QDateTime::fromString(value, Action::dateTimeFormat()));
            if (!dosage->getApplied().isValid())
                _errors = tr("Invalid date given : %1").arg(value);
        }

        //Dose
        else if (var == "dose") {
            if (!dosage->getQuantity()->fromString(value))
                _errors = tr("Invalid dose given : %1").arg(value);
        }

        //Interval
        else if (var == "interval") {
            if (!dosage->getInterval().fromString(value))
                _errors = tr("Invalid interval given : %1").arg(value);
        }

        //Tinf
        else if (var == "tinf") {
            if (!dosage->getTinf().fromString(value))
                _errors = tr("Invalid infusion time given : %1").arg(value);
        }

        //Invalid
        else
            _errors = tr("Invalid variable given : %1").arg(var);

        //On error, quit
        if (!_errors.isEmpty())
            return false;
    }

    //Set it in the database
//    return DATABASE->tryRequest( DB->setDosage(curve->id(),dosage), tr("Error while setting the dosage %1").arg(id), _errors );
    if (REPO->setDosage(dosage).error != NoError) {
            return false;
    }
}

bool PredictionAction::dosageRemove(ident id)
{
    //Try to remove it
//    if ( ! DATABASE->tryRequest(DB->deleteDosage(id),
//                                tr("Error while removing dosage %1 from the database :").arg(id), _errors))
//        return false;
    if (REPO->deleteDosage(id).error != NoError) {
        return false;
    }

    return true;
}


//Calculate a bunch of points
bool PredictionAction::points(Prediction* pred, QStringList &args)
{
    //The exportation data
    QDateTime from, to;
    Duration delta;
    PercentileList percentiles;
//    QString fileName, format = Action::dateTimeFormat(), separator = " ; ";
    QString fileName, format = Action::dateTimeFormat(), separator = " ";
    bool cast, time = true, header = false;
    int precision = -1;

    //Check if enough paramters
    if (args.size() < 3) {
        _errors = tr("The 'points' command requires at least 3 arguments : {from} {to} {points} [type]");
        return false;
    }

    //Get the interval
    if (!(from = QDateTime::fromString(args.at(0), Action::dateTimeFormat())).isValid()) {
        _errors = tr("The 'from' date is invalid or not corretly formatted. Please use the format '%1'.").arg(Action::dateTimeFormat());
        return false;
    }
//    if (from < pred->getTreatment()->getDosages()->firsttake()) {
//        _errors = tr("The 'from' date '%1' is earlier than the first take date '%2'.").arg(from.toString(Action::dateTimeFormat()), pred->getTreatment()->getDosages()->firsttake().toString(Action::dateTimeFormat()));
//        return false;
//    }
    args.removeFirst();

    if (!(to   = QDateTime::fromString(args.at(0), Action::dateTimeFormat())).isValid()) {
        _errors = tr("The 'to' date is invalid or not corretly formatted. Please use the format '%1'.").arg(Action::dateTimeFormat());
        return false;
    }
    if (to <= from) {
        _errors = tr("The 'to' date '%1' is earlier or identical to the 'from' date '%2'.").arg(to.toString(Action::dateTimeFormat()), from.toString(Action::dateTimeFormat()));
        return false;
    }
    args.removeFirst();

    //Get the delta
    delta = Duration(0, args.at(0).toInt(&cast));
    if (!cast || delta.isEmpty() || delta.isNegative()) {
        _errors = tr("Invalid, null or negative delta '%1' given.").arg(args.at(0));
        return false;
    }
    args.removeFirst();

    //Get the other parameters
    for (int i = 0; i < args.size(); i++) {

        if (args.at(i) == "header") {

            //Set the header boolean
//            header = true;

        } else if (args.at(i).startsWith("time")) {

            //Set the time boolean
            time = true;

            //Set the optional format
            if (!args[i].remove("time").isEmpty()) {
                format = args[i].remove("=");

                //Check the format
                if (QDateTime::currentDateTime().toString(format).isEmpty()) {
                    _errors = tr("Invalid time format '%1' given.").arg(args.at(i));
                    return false;
                }
            }

        } else if (args.at(i).startsWith("precision=")) {

            //Set the precision
            precision = args[i].remove("precision=").toInt(&cast);

            //Check if valid
            if (!cast) {
                _errors = tr("Invalid precision '%1' given.").arg(args.at(i));
                return false;
            }

        } else if (args.at(i).startsWith("percentiles=")) {

            //Set the percentiles
            QStringList rawPercentiles = args[i].remove("percentiles=").split(",");

            //Check if empty
            if (rawPercentiles.isEmpty()) {
                _errors = tr("Missing percentiles '%1' values.").arg(args.at(i));
                return false;
            }

            //Extract the percentiles
            foreach (QString current, rawPercentiles) {

                //Get the current values
                QStringList values = current.split("-");
                if (values.size() != 2) {
                    _errors = tr("Invalid percentiles '%1' given.").arg(current);
                    return false;
                }

                int low = values.at(0).toInt(&cast);
                if (!cast) {
                    _errors = tr("Invalid percentile value '%1' given.").arg(values.at(0));
                    return false;
                }

                int high = values.at(1).toInt(&cast);
                if (!cast) {
                    _errors = tr("Invalid percentile value '%1' given.").arg(values.at(1));
                    return false;
                }

                //Add the percentiles
                Percentile p((low < high ? low : high), (low < high ? high : low));
                percentiles.append(p);
            }

        } else if (args.at(i).startsWith("file=")) {

            //Set the file
            fileName = args[i].remove("file=");

            //Check if empty
            if (fileName.isEmpty()) {
                _errors = tr("Empty file specified.").arg(args.at(i));
                return false;
            }

        } /*else if (args.at(i).startsWith("separator=")) {

            //Set the separator
            separator = args[i].remove("separator=");

            //Check if empty
            if (separator.isEmpty()) {
                _errors = tr("Empty separator specified.").arg(args.at(i));
                return false;
            }

        }*/ else {

            //Invalid argument
            _errors = tr("Invalid argument '%1' given.").arg(args.at(i));
            return false;

        }
    }

    //The output string

//    DosageHistory* _dosages = ezechiel::core::CoreFactory::createEntity<DosageHistory>(REPO);
//    ezechiel::core::Response r = REPO->getDosagesByDate(pred->getTreatment()->getPatient()->id(), _dosages);
//    if (r.error != NoError && r.error != NotFound) {
//        return false;
//    }
//    pred->getTreatment()->setDosages(_dosages);
    //Export the data
    DataSet _ds;
    ParamTraits* ptraits;
    pred->getParamsType() == APRIORI ? ptraits = new AprioriTraits() : ptraits = new AposterioriTraits();
    PredictionResult* pred1 = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);
    ProcessingResult _result = CORE->getProcessingInterface()->points(pred->getAnalysis(),
             PointsTraits(pred->getSpec()->getStartDate(), pred->getSpec()->getEndDate(), pred->getSpec()->getNbPoints(), ptraits, std::vector<double>(0)),
                                                         /*_ds,*/ *pred1); //QVector< QVector<double> > data = pred->exportPoints(from, to, delta, percentiles, time);

    delete ptraits;
    //Check the result
//    if (data.isEmpty() || (data.size() == 1 && time)) {
//        _errors = tr("Extraction failed.");
//        return false;
//    }

    //Set the separator
//    separator.replace("\\t","\t");

    //Set the header
//    if (header) {

//        //Add the time
//        if (time)
//            result += tr("Time") + separator;

//        //Add the name
//        result += pred->getName();

//        //Add the percentiles
//        if (percentiles.size()) {
//            QList<int> values;

//            foreach (Percentile perc, percentiles)
//                values << perc.low() << perc.high();

//            qSort(values.begin(), values.end());

//            foreach (int perc, values)
//                result += separator + QString("P%1").arg(QString::number(perc));
//        }
//        result += QChar::LineFeed;
//    }

//    QVector< QVector<double> > data;
//    QVector<double> subdata, subdatatimes;
//    if (time) {
//        subdatatimes = _ds.getTimes();
//        data.push_back(subdatatimes);
//    }
//    if (!_ds.getData().empty()) {
//        subdata = _ds.getData();
//        data.push_back(subdata);
//    }

    //Set up the data
//    for (int i = 0; i < data.begin()->size(); i++) {
    QString result;
    FancyPoints* fpts = pred1->getPredictive()->getPredictionData()->getPoints();
    for (int i = 0; i < fpts->size(); i++) {

        //Add the time
//        if (time)
        result += QString::number((fpts->at(i)->getTime() - fpts->at(0)->getTime()) / 3600.0, 'f', 3) + ",";
//            result += QDateTime::fromTime_t(data.first().at(i)).toString(format) + separator;

        //Add the point
        result += QString::number(fpts->at(i)->getValue(), 'f', 3);
//        result += precision < 0 ? QString::number(data.at(time ? 1 : 0).at(i)) : QString::number(data.at(time ? 1 : 0).at(i), 'f', precision);

        //Add the percentiles
        PercentileDataList* pprs = pred1->getPredictive()->getPercentileDataList();
        for (int j = 0; j < pprs->size(); j++)
            result += "," + QString::number(pprs->at(j)->getPredictionData()->getPoints()->at(i)->getValue(), 'f', 3);

        if (pprs->size() > 0) {
            result += QChar::LineFeed;
        } else {
            result += ' ';
        }
    }

    std::cout << qPrintable(result) << std::endl << std::endl;
        LOG(QtCriticalMsg, NOEZERROR, tr("Calculated %1 points for curve from %2 until %3 with %4 parameters :\n\n%5\n\n")
            .arg(QString::number(fpts->size()), pred->getParamsType() == APRIORI ? "apriori": "aposteriori", pred->getSpec()->getStartDate().toString(), pred->getSpec()->getEndDate().toString(), result));
    //Export in terminal
//    if (fileName.isEmpty()) {
//        LOG(QtCriticalMsg, NOEZERROR, tr("Calculated %1 points for curve from %2 until %3 with %4 parameters :\n\n%5\n\n")
//            .arg(QString::number(data.begin()->size()), pred->getParamsType() == APRIORI ? "apriori": "aposteriori", pred->getSpec()->getStartDate().toString(), pred->getSpec()->getEndDate().toString(), result));
//        return true;
//    }

//    //Open the file
//    QFile file(fileName);

//    //Check if the file already exists
//    if (file.exists()) {
//        QTextStream qin(stdin);

//        //Prompt the user
//        LOG(QtDebugMsg, NOEZERROR, QString(tr("The file '%1' already exists. Are you sure you want to replace it with this data? (y/n)")).arg(QFileInfo(file).fileName()));

//        //Process the answer
//        QString answer;
//        do {
//            answer = qin.readLine();
//        } while (answer != "y" && answer != "yes" && answer != "n" && answer != "no");

//        //Cancel if refused
//        if (answer == "n" || answer == "no")
//            return true;
//    }

//    //Open the file and check for errors
//    if (!file.open(QIODevice::WriteOnly)) {
//        _errors = tr("The file '%1' could not be created. Please check if the path is valid or if you have the rights and try again.").arg(QFileInfo(file).fileName());
//        return false;
//    }

    //Write the data in the file
//    QTextStream out(stdout);
//    out << result;
//    file.close();

    return true;
}

//Only one point to calculate
bool PredictionAction::point(Prediction* pred, const QStringList &args)
{
    QDateTime date;

    //Check if enough parameters
    if (args.size() < 1) {
        _errors = tr("Missing parameter 'DATE'.");
        return false;
    }

    //Check if too many parameters
//    if (args.size() > 2) {
//        _errors = tr("Too many parameters.");
//        return false;
//    }

    //Get the date
    if (!(date = QDateTime::fromString(args.at(0), Action::dateTimeFormat())).isValid()) {
        _errors = tr("The given date is invalid or not corretly formatted. Please use the format '%1'.").arg(Action::dateTimeFormat());
        return false;
    }

    //Get the duration
    Duration at = Duration(0, 0, 0, pred->getTreatment()->getDosages()->firsttake().msecsTo(date));
    if (at.isEmpty() || at.isNegative()) {
        _errors = tr("The date must be after the first take.");
        return false;
    }

    //Get the parameters type
//    ParameterType type = curve->paramsType();
//    ParameterType type = ParameterType::APRIORI;
//    if (args.size() == 2)
//        if ((type = parameterType(args.at(1))) == LAST_PT)
//            return false;

//    SharedDrugVariateSet _covs = ezechiel::core::CoreFactory::createEntity<DrugVariateSet>(REPO);
//    ezechiel::core::Response r = REPO->getDrugVariatesByDate(pred->getTreatment()->drug()->id(), *_covs.data());
//    if (r.error != NoError && r.error != NotFound) {
//        return false;
//    }
//    pred->getTreatment()->drug()->setCovariates(_covs);

    QString result;

//    DosageHistory* _dosages = ezechiel::core::CoreFactory::createEntity<DosageHistory>(REPO);
//    Response r = REPO->getDosagesByDate(pred->getTreatment()->getPatient()->id(), _dosages);
//    if (r.error != NoError && r.error != NotFound) {
//        LOG(QtCriticalMsg, ezechiel::cliutils::OBJECTNOTFOUND, "Error retreiving dosages.");
//        return false;
//    }
//    pred->getTreatment()->setDosages(_dosages);

//    QList<Measure *> _measures;
//    r = ADMINREPO->getMeasuresFromPatientAndDrug(pred->getTreatment()->getPatient()->id(), pred->getTreatment()->getDrug()->id(), _measures);
//    if (r.error != NoError && r.error != NotFound) {
//        LOG(QtCriticalMsg, ezechiel::cliutils::OBJECTNOTFOUND, "Error retreiving measures.");
//        return false;
//    }
//    QList<CoreMeasure*> ms;
//    foreach (Measure * cm, _measures) {
//       pred->getTreatment()->getMeasures()->append(cm);
//    }

    //Export the data
    Amount data;
    ParamTraits* ptraits;
    if (pred->getParamsType() == APRIORI) {
        ptraits = new AprioriTraits();
    }
    else {
        ptraits = new AposterioriTraits();
    }

    PredictionResult* pred1 = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);
    ProcessingResult _result = CORE->getProcessingInterface()->point(pred->getAnalysis(),
                                                        PointTraits(pred->getSpec()->getStartDate(), pred->getSpec()->getEndDate(), date, ptraits),
                                                        data, *pred1);

    delete ptraits;
    //Get the concentration
//    Amount data = curve->getAnalysis()->point(curve, at, type);

    //Display the concentration
    LOG(QtCriticalMsg, NOEZERROR, tr("Calculated %1 points for curve with %1 parameters :\n\n%2\n\n")
        .arg(pred->getParamsType() == APRIORI ? "apriori": "aposteriori", data.toString(3)));
    return true;
}

bool PredictionAction::reverse(Prediction* pred, const QStringList &args) {

    //We need at least 4 arguments
    if (args.size() < 4) {
        _errors = tr("The 'reverse' command requires at least 4 arguments : {from} {to} {branchtime} {points} [type]");
        return false;
    }

    //Start and stop
    QDateTime start,stop,branchtime;
    start = QDateTime::fromString(args.at(0), QString(_dateTimeFormat));
    stop = QDateTime::fromString(args.at(1), QString(_dateTimeFormat));
    branchtime = QDateTime::fromString(args.at(2), QString(_dateTimeFormat));
    if (! start.isValid() )
        _errors = tr("Invalid 'from' duration given : %1").arg(args.at(0));
    if (! stop.isValid() )
        _errors = tr("Invalid 'to' duration given : %1").arg(args.at(1));
    if (! branchtime.isValid() )
        _errors = tr("Invalid 'branchtime' duration given : %1").arg(args.at(2));

    //Points
    bool ok;
    int points = args.at(3).toInt(&ok);
    if (!ok)
        _errors = tr("Invalid number of points given : %1").arg(args.at(3));

    //Default type
    ParameterType type = pred->getParamsType();

    //Optional parameters
    QString compareArgs;
    foreach (QString arg, args.mid(4)) {

        //Separate the variable from the value
        QRegExp reg("([a-z]+)=(.*)");
        reg.indexIn(arg);
        QString var = reg.cap(1);
        QString value = reg.cap(2);

        //Type
        if (var == "type")
            type = stringToType(value,ok);

        //Compare
        if (var == "compare")
            compareArgs = value;
    }

    //On error, quit
    if (!_errors.isEmpty())
        return false;

    QMap<Dosage*, DataSet> results;

    ParamTraits* ptraits = new AprioriTraits();
    ProcessingResult _result;
    PredictionResult* pred1 = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);
    _result = CORE->getProcessingInterface()->computeSuggestedAdjustments(pred->getAnalysis(),
                                                                          ReverseTraits(false, false, start, stop, branchtime, stop, pred->getSpec()->getNbPoints(), ptraits), *pred1);

    /*****************************************
     * results were previously modified in the reverse method. Now all results are stored in pred1.
     ****************************************/
    QTextStream out(stdout);
    for (auto it = results.begin(); it != results.end(); ++it) {
        out << "Dose: " << it.key()->getQuantity()->value() << " " ;
        out << "Interval: " << it.key()->getDbinterval() << " " ;
        out << "Infusion: " << it.key()->getDbtinf() << " " ;
        out << "Values: " << "\n" ;
        QVector<double> data = it->getData();
        QVector<double> times = it->getTimes();
        auto dit = data.begin();
        auto tit = times.begin();
        auto dit_end = data.end();
        auto tit_end = times.end();
        while (dit != dit_end && tit != tit_end) {
            out << *tit << " " << *dit << " ";
            dit++; tit++;
        }
        out.flush();
    }
    return true;
}

//Do the percentile dance? Yeah yeah yeah!
bool PredictionAction::percentile(Prediction* pred, const QStringList &args)
{
    //We need at least 4 arguments
    if (args.size() < 4) {
        _errors = tr("The 'percentile' command requires at least 4 arguments : {from} {to} {points} {value} [type] [method]");
        return false;
    }

    //Start and stop
    QDateTime start,stop;
    start = QDateTime::fromString(args.at(0), QString(_dateTimeFormat));
    stop = QDateTime::fromString(args.at(1), QString(_dateTimeFormat));
    if (! start.isValid() )
        _errors = tr("Invalid 'from' duration given : %1").arg(args.at(0));
    if (! stop.isValid() )
        _errors = tr("Invalid 'to' duration given : %1").arg(args.at(1));

    //Points
    bool ok;
    int points = args.at(2).toInt(&ok);
    if (!ok)
        _errors = tr("Invalid number of points given : %1").arg(args.at(2));

    //Percentile
    std::vector<double> percent;
    QMap<double, DataSet> results;
    QStringList _percentsstrl = args.at(3).split(" ");
    foreach (QString _s, _percentsstrl) {
        percent.push_back(_s.toDouble(&ok));
        if (!ok)
            _errors = tr("Invalid percentile value given : %1").arg(args.at(3));
    }
    results.insert(percent[0], DataSet());

    //Default type
    ParameterType type = pred->getParamsType();

    //Optional parameters
    QString compareArgs;
   PercentilesTraits* perctraits;

    foreach (QString arg, args.mid(4)) {

        //Separate the variable from the value
        QRegExp reg("([a-z]+)=(.*)");
        reg.indexIn(arg);
        QString var = reg.cap(1);
        QString value = reg.cap(2);

        //Type
        if (arg == "apriori") {
            type = APRIORI;
        }
        if (arg == "aposteriori") {
            type = APOSTERIORI;
        }

        if (type == APOSTERIORI) {
            perctraits = new PercentilesTraits(pred->getSpec()->getNbPoints(), percent, start, stop, new AposterioriTraits());
        } else {
            perctraits = new PercentilesTraits(pred->getSpec()->getNbPoints(), percent, start, stop, new AprioriTraits());
        }
        //Compare
        if (var == "compare")
            compareArgs = value;
        if (var == "method") {
            perctraits->options[QString("cornishfisher")] = QVariant::fromValue(0);

            if (value == "shash") {
                perctraits->options[QString("percentile_method")] = QVariant::fromValue(1);
            }
            if (value == "montecarlo") {
                perctraits->options[QString("percentile_method")] = QVariant::fromValue(2);
            }
        }
    }

    //On error, quit
    if (!_errors.isEmpty())
        return false;

    ProcessingResult _result;
    PredictionResult* pred1 = CoreFactory::createEntity<PredictionResult>(ABSTRACTREPO);
    if (type == APOSTERIORI) {
        _result = CORE->getProcessingInterface()->calculateAposterioriPercentiles(pred->getAnalysis(), *perctraits, /*results,*/ *pred1);
    } else {
//        CORE->getProcessingInterface()->enqueueAprPerc();
        _result = CORE->getProcessingInterface()->percentiles(pred->getAnalysis(), *perctraits, /*results,*/ *pred1);
    }

    QString result;
    PercentileDataList* pprs = pred1->getPredictive()->getPercentileDataList();
    if (pprs->size() <= 0) {
       return false;
    }
    FancyPoints* fpts = pprs->at(0)->getPredictionData()->getPoints();
    for (int i = 0; i < fpts->size(); ++i) {
        result += QString::number((fpts->at(i)->getTime() - fpts->at(0)->getTime()) / 3600.0, 'f', 3);
        for (int j = 0; j < pprs->size(); j++) {
            result += "," + QString::number(pprs->at(j)->getPredictionData()->getPoints()->at(i)->getValue(), 'f', 3);
        }
        result += QChar::LineFeed;
    }

    std::cout << qPrintable(result) << std::endl << std::endl;

    LOG(QtCriticalMsg, NOEZERROR, tr("Calculated %1 points for curve from %2 until %3 with %4 parameters :\n\n%5\n\n")
        .arg(QString::number(fpts->size()), pred->getParamsType() == APRIORI ? "apriori": "aposteriori", pred->getSpec()->getStartDate().toString(), pred->getSpec()->getEndDate().toString(), result));

    //If the compare is asked, go to it
    if (!compareArgs.isEmpty())
        return compare(results[percent[0]],compareArgs);

    return true;
}

//Calculate the points and percentiles and display the corresponding chart (or save to a file)
//bool PredictionAction::chart(SharedPrediction curve, const QStringList &args)
//{
//    //We need at least 3 arguments
//    if (args.size() < 3) {
//        _errors = tr("The 'chart' command requires at least 4 arguments : chart {from} {to} {points} [percentile=value-value]* [type=type] [save=file,width,height,dpi]");
//        return false;
//    }

//    //Start and stop
//    Duration start,stop;
//    if (! start.fromString(args.at(0)) )
//        _errors = tr("Invalid 'from' duration given : %1").arg(args.at(0));
//    if (! stop.fromString(args.at(1)) )
//        _errors = tr("Invalid 'to' duration given : %1").arg(args.at(1));

//    //Points
//    bool ok;
//    int points = args.at(2).toInt(&ok);
//    if (!ok)
//        _errors = tr("Invalid number of points given : %1").arg(args.at(2));

//    //Default type
//    ParameterType type = curve->paramsType();

//    //Optional parameters
//    QList<int> percentiles;
//    QString file;
//    int width,height,dpi;
//    foreach (QString arg, args.mid(3)) {

//        //Separate the variable from the value
//        QRegExp reg("([a-z]+)=(.*)");
//        reg.indexIn(arg);
//        QString var = reg.cap(1);
//        QString value = reg.cap(2);

//        //Type
//        if (var == "type")
//            type = stringToType(value,ok);

//        //File
//        else if (var == "save") {
//            QRegExp fas("([^,]+),([0-9]+),([0-9]+),([0-9]+)");
//            if (fas.indexIn(value) == -1) {
//                _errors = tr("Invalid file given, expecting in 'file,width,heigh,dpi' form, found : %1").arg(value);
//                break;
//            }
//            file = fas.cap(1);
//            width = fas.cap(2).toInt();
//            height = fas.cap(3).toInt();
//            dpi = fas.cap(4).toInt();
//        }

//        //Percentile
//        else if (var == "percentile") {
//            QRegExp ints("([0-9]+)-([0-9]+)");
//            if (ints.indexIn(value) == -1) {
//                _errors = tr("Invalid percentile range given, expecting in 'value-value' form, found : %1").arg(value);
//                break;
//            }
//            percentiles << ints.cap(1).toInt() << ints.cap(2).toInt();
//        }
//    }

//    //On error, quit
//    if (!_errors.isEmpty())
//        return false;

//    //Creating the chart object
//    Chart *chart = new Chart();
//    ChartItemGroup *group = new ChartItemGroup();
//    chart->attachGroup(group);

//    //Calculate the start and stop date
//    QDateTime startDate = curve->firsttake().addSecs(start.seconds());
//    QDateTime stopDate = curve->firsttake().addSecs(stop.seconds());

//    //Calculate the curve
//    DataSet data = curve->getAnalysis()->points(curve, start,stop,points,type);
//    Curve *c_data = new Curve();
//    c_data->setDataset(startDate,stopDate,data);
//    group->attachItem(c_data);

//    //Calculate the percentiles
//    for (int i=0; i<percentiles.size()/2; i+=2) {
//        DataSet low = curve->getAnalysis()->percentiles(curve, percentiles.mid(i,1),start,stop,points,type).constBegin().value();
//        DataSet high = curve->getAnalysis()->percentiles(curve, percentiles.mid(i+1,1),start,stop,points,type).constBegin().value();
//        PercentileCurve *pc = new PercentileCurve();
//        pc->setDataset(startDate,stopDate,low,high);
//        group->attachItem(pc);
//    }

//    //Show the measures
//    foreach (SharedCoreMeasure m, *curve->getTreatment()->measures().data()) {
//        if (curve->firsttake().secsTo(m->moment()) > start.toSeconds() && curve->firsttake().secsTo(m->moment()) < stop.toSeconds()) {
//            Dot *dot = new Dot();
//            dot->setPos(m->moment(), m->amount().valueIn(data.unit()));
//            group->attachItem(dot);
//        }
//    }

//    //Show the final wanted concentration
//    foreach (Target* t, *curve->getTreatment()->targets().data()) {
//        TargetInterval *ti = new TargetInterval(t->type());
//        ti->setTarget(t);
//        group->attachItem(ti);
//    }

//    //If a file is given, save to a file
//    if ( ! file.isEmpty() )
//        return chart->save(file,width,height,dpi);

//    //Else, open a window and show the chart
//    QMainWindow *w = new QMainWindow();
//    chart->setBackgroundRole(QPalette::NoRole);
//    w->setCentralWidget(chart);
//    w->show();

//    return QApplication::instance()->exec();
//}

//Type to string
QString PredictionAction::typeToString(ParameterType type) const
{
    switch (type) {
        case POPULATION: return tr("population");
        case APRIORI: return tr("apriori");
        case APOSTERIORI: return tr("aposteriori");
        case MANUAL: return tr("manual");
        default : break;
    }
    return "";
}

//String to type
ParameterType PredictionAction::stringToType(const QString &str, bool &ok)
{
    ok = true;
    if (str == tr("population"))
        return POPULATION;
    else if (str == tr("apriori"))
        return APRIORI;
    else if (str == tr("aposteriori"))
        return APOSTERIORI;
    else if (str == tr("manual"))
        return MANUAL;

    //GNIARK!
    ok = false;
    _errors = tr("Invalid parameter type given : %1").arg(str);
    return POPULATION;
}

ParameterType PredictionAction::parameterType(const QString &typeStr)
{
    //Return the type
    if      (typeStr == _CMD_PARAM_TYPICAL)
        return POPULATION;
    else if (typeStr == _CMD_PARAM_APRIORI)
        return APRIORI;
    else if (typeStr == _CMD_PARAM_APOSTERIORI)
        return APOSTERIORI;

    //Set the error message
    _errors = tr("The given type is invalid. It must be either '%1', '%2' or '%3'.").arg(_CMD_PARAM_TYPICAL, _CMD_PARAM_APRIORI, _CMD_PARAM_APOSTERIORI);

    //Return an invalid type
    return LAST_PT;
}

//Fetch the arguments and compare the value to the file
bool PredictionAction::compare(DataSet data, QString arg)
{
    //Separate the arguments
    QStringList args = arg.split(',');

    //The first one must be the file
    QString filename = args.at(0);
    args.removeFirst();

    //By default, quiet is off
    bool quiet = false;

    //Default precision of 0
    double prec = 0.0;

    //Optional parameters
    foreach (QString str, args) {

        //Separate the variable from the value
        QRegExp reg("([a-z]+)(=(.*))?");
        reg.indexIn(str);
        QString var = reg.cap(1);
        QString value = reg.cap(3);

        //Change the dataset unit
//        if (var == "unit") {
//            Unit u;
//            if (!u.fromString(value)) {
//                _errors = tr("Invalid unit given : %1").arg(value);
//                return false;
//            }
//            data.convert(u);
//        }

        //Precision
//        else if (var == "prec") {
//            bool ok;
//            prec = value.toDouble(&ok);
//            if (!ok) {
//                _errors = tr("Invalid precision value given : %1").arg(value);
//                return false;
//            }
//        }

        //Quiet
//        else if (var == "quiet")
//            quiet = true;
    }

    //Read the dataset
    DataSet base;
//    if ( ! base.read(filename) ) {
//        _errors = tr("Error while loading the dataset %1").arg(filename);
//        return false;
//    }

    //Compare the size
    if ( data.size() != base.size() ) {
        _errors = tr("Number of points in the data (%1) is not the same as in the file (%2)").arg(data.size()).arg(base.size());
        return false;
    }

    //Compare each value
    bool allOk = true;
    for (uint i=0; i<data.getData().size(); i++) {

        //Fetch
        double d = data.getData()[i];
        double b = base.getData()[i];

        //Compare
        if ( d < (b-prec) ||
             d > (b+prec) ) {
            LOG(QtDebugMsg, NOEZERROR, QString(tr("Invalid value at %1 : %2 != %3 +- %4").arg(i).arg(d).arg(b).arg(prec)));
            allOk = false;
        }

        //Display the right values only if quiet is not asked
        else if (!quiet)
            LOG(QtDebugMsg, NOEZERROR, QString(tr("%1 : %2 == %3 +- %4").arg(i).arg(d).arg(b).arg(prec)));
    }

    //If one or more values did fails, explain why
    if (!allOk)
        _errors = tr("Some value were incorrect");

    return allOk;
}

} //namespace cliutils
} //namespace ezechiel
