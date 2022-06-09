//@@license@@

#include "addaction.h"
#include "core/core.h"
#include "apputils/src/drugmanager.h"
#include "core/pluginmanager.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "apputils/src/appcore.h"
#include "admin/src/dal/measure.h"
#include "admin/src/dal/patient.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/drug.h"
#include "admin/src/stdadminrepository.h"
#include "admin/src/adminfactory.h"
#include "core/corerepository.h"
#include "core/corefactory.h"
#include "apputils/src/apputilsrepository.h"
#include "core/utils/logging.h"
#include "errors_cliutils.h"

using namespace ezechiel::GuiCore;

namespace ezechiel {
namespace cliutils {
const char *const AddAction::_separator = "_";

AddAction::AddAction()
{

}

QString AddAction::name()
{
    return "addAction";
}

QString AddAction::help()
{
    QString str;
    str = tr("   NAME") + "\n" +
          tr("       EzeCHieL Add Command") + "\n" + "\n" +
          tr("   SYNOPSIS") + "\n" +
             "       " + QString("       ezechiel-cli [OPTIONS] addAction %1USERNAME %2PASSWORD SUBCOMMAND").arg(_CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD) + "\n" + "\n" +
          tr("   DESCRIPTION") + "\n" +
          tr("       The EzeCHieL Add Command can be used to add data to existing curves and patients. The concentrations must be specified in [ug/l], the dose in [mg] and the intervals in seconds. All dates must be specified using the format '%1'. The command is obsolete and will be replaced by similar actions in other commands.").arg(Action::dateTimeFormat()) + "\n" + "\n" +
          tr("   SUBCOMMANDS") + "\n" +
             "       measure FIRSTNAME NAME DRUGNAME DATE CONCENTRATION" + "\n" +
          tr("              Add or replace a measure for this patient and this drug.")  + "\n" + "\n" +
             "       dose FIRSTNAME NAME DRUGNAME DATE DOSE INTERVAL" + "\n" +
          tr("              Add or replace a dose for this patient and this drug.")  + "\n" + "\n" +
          "\n";

    return str;
}

bool AddAction::run(const QStringList &args)
{
    //Clear the previous erros
    _errors.clear();

    //Get the sub-command arg
    if (args.size() < 1) {
        _errors = tr("The 'IsyPeM' command requires at least a sub-command\n");
        return false;
    }

    QString subCommand = args[0];

    //Run the measure sub-command
    if (subCommand == "measure") {

        //Check the number of arguments
        if (args.size() < 6) {
            _errors = tr("Insufficient number of parameters for the add measure action\n");
            return false;
        }

        return addMeasure(args[1], args[2], args[3], args[4], args[5]);
    }

    //Run the dosage sub-command
    if (subCommand == "dose") {

        //Check the number of arguments
        if (args.size() < 7) {
            _errors = tr("Insufficient number of parameters for the add dose action\n");
            return false;
        }

        return addDosage(args[1], args[2], args[3], args[4], args[5], args[6]);
    }

    //Return an error in case of invalid sub-command
    _errors = tr("Unknown command '%1'\n").arg(subCommand);
    return false;
}

bool AddAction::addMeasure(const QString &firstname, const QString &name, const QString &drugName, const QString &dateStr, const QString &concentrationStr)
{
    //Reset the previous data
    _lastMeasure = AdminFactory::createEntity<Measure>(REPO);
    _lastPatientId = -1;

    //Prepare the data
    SharedPatient patient;
    QString drugId;
    QDateTime date;
    double concentration;
    QList<Measure *> measures;

    //Load the patients
    if (!loadPatients())
        return false;

    //Get the patient
    if (!getPatient(firstname, name, patient))
        return false;

    //Get the drug ID
    if (!getDrugId(drugName, drugId))
        return false;

    //Get the date
    if (!getDate(dateStr, date))
        return false;

    //Get the concentration
    if (!getValue(concentrationStr, concentration))
        return false;

    //Contruct the measure
    Measure * measure = AdminFactory::createEntity<Measure>(REPO);
    measure->setMoment(date);
    measure->setConcentration(ezechiel::GuiCore::CoreFactory::createEntity<IdentifiableAmount>(REPO, measure));
    measure->getConcentration()->setValue(concentration);
    measure->getConcentration()->setUnit(Unit("ug/l"));

    //Get the patient measures for this drug
//    if (!tryRequest(DB->getMeasuresByDrug(patient->id(), drugId, measures), tr("Impossible to retrieve the measures of patient ID '%1' for the drug '%2' from the database").arg(QString::number(patient->id()), drugId)))
//        return false;
    ezechiel::GuiCore::Response r = ADMINREPO->getMeasuresFromPatientAndDrug(patient->id(), drugId, measures);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }


    //Check if the measure already exists
    foreach (Measure * m, measures) {
        if (m->getMoment() == measure->getMoment()) {
            if (m != measure) {
                measure->setId(m->id());
                break;
            } else
                return true;
        }
    }

    //If not, save the measure
    /*if (!tryRequest(DB->setMeasure(patient->id(), measure), tr("Impossible to save patient ID '%1' measure in the database").arg(QString::number(patient->id()))))
        return false*/;
    r = ADMINREPO->setMeasure(measure);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }

    //Register it
    _lastMeasure = measure;
    _lastPatientId = patient->id();

    return true;
}

bool AddAction::addDosage(const QString &firstname, const QString &name, const QString &drugName, const QString &dateStr, const QString &doseStr, const QString &intervalStr)
{
    //Reset the previous data
    _lastDosage = ezechiel::GuiCore::CoreFactory::createEntity<Dosage>(REPO);
    _lastCurveId = invalid_ident;

    //Prepare the data
    SharedPatient patient;
    QString drugId;
    QDateTime date;
    double dose;
    int interval;

    //Load the patients
    if (!loadPatients())
        return false;

    //Get the patient
    if (!getPatient(firstname, name, patient))
        return false;

    //Get the drug ID
    if (!getDrugId(drugName, drugId))
        return false;

    //Get the date
    if (!getDate(dateStr, date))
        return false;

    //Get the dosage
    if (!getValue(doseStr, dose))
        return false;

    //Get the interval
    if (!getValue(intervalStr, interval))
        return false;

    //Construct the dosage
    Dosage* dosage;
    dosage->setApplied(date);
    dosage->setQuantity(CoreFactory::createEntity<IdentifiableAmount>(ABSTRACTREPO, dosage));
    dosage->getQuantity()->setValue(dose);
    dosage->getQuantity()->setUnit(Unit("mg"));
    dosage->setInterval(Duration(0, 0, interval));
    dosage->setTinf(Duration(0, 30));
    dosage->setId(-1);

    //Set the curve name
    QString curveName(drugName + " (" + firstname + " " + name + ")");

    //Check if the curve already exists
    QList<Prediction*> curves;
//    if (!tryRequest(DB->getCurvesList(curves), tr("Impossible to retrieve the list of curves from the database")))
//        return false;
    ezechiel::GuiCore::Response r = REPO->getPredictionsList(curves);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }

    for (int i = 0; i < curves.size(); i++) {
        if (curves.at(i)->getName() == curveName)
            return setDosage(curves.at(i)->id(), dosage);
    }

    //If it does not, create it
    return setCurve(drugId, curveName, patient, dosage);
}

bool AddAction::lastMeasure(Measure * measure, ident &patientId)
{
    if (!_lastMeasure->isValid() || _lastPatientId == -1)
        return false;

    measure = _lastMeasure;
    patientId = _lastPatientId;

    return true;
}

bool AddAction::lastDosage(Dosage* &dosage, ident &curveId)
{
    if (!_lastDosage->isValid() || _lastCurveId == -1)
        return false;

    dosage = _lastDosage;
    curveId = _lastCurveId;

    return true;
}

bool AddAction::loadPatients()
{
    QList<SharedPatient> patients;

//    if (!tryRequest(DB->getPatientsList(patients), tr("Impossible to retrieve the list of patients from the database")))
//        return false;
    ezechiel::GuiCore::Response r = ADMINREPO->getPatientsList(patients);
    if (r.error != NoError && r.error != NotFound) {
//        EZERROR(CoreError::CliError, tr("Impossible to retrieve the list of patients from the database"));
        return false;
    }

    foreach (SharedPatient patient, patients) {
        _patients.insert(static_cast<Patient*>(patient)->person()->name().toLower() + _separator + static_cast<Patient*>(patient)->person()->firstname().toLower(), patient->id());
    }

    return true;
}

bool AddAction::patientExists(const QString &name, const QString &firstname) const
{
    return _patients.contains(name.toLower() + _separator + firstname.toLower());
}

ident AddAction::patientId(const QString &name, const QString &firstname) const
{
    return _patients.value(name.toLower() + _separator + firstname.toLower(), invalid_ident);
}

bool AddAction::getPatient(const QString &firstname, const QString &name, SharedPatient &patient)
{
    patient = AdminFactory::createEntity<Patient>(ADMINREPO);
    //Verify if the patient exists
    if (patientExists(name, firstname)) {
        patient->setId ( patientId(name, firstname));

        if (patient->id() == invalid_ident) {
            _errors = tr("Invalid patient ID\n");
            return false;
        }

        return true;
    }

    //Register the location in the database
    Location* location = AdminFactory::createEntity<Location>(ADMINREPO);
    location->country("Suisse");
    location->address("Grande Rue 23");
    location->city("Lausanne");
    location->postcode("1000");
    location->state("Vaud");

    static_cast<Patient*>(patient)->person()->firstname(firstname);
    static_cast<Patient*>(patient)->person()->name(name);
    static_cast<Patient*>(patient)->person()->location(location);
    static_cast<Patient*>(patient)->person()->birthday(QDate::fromString("01/02/1968", "dd/MM/yyyy"));
    Email* email = AdminFactory::createEntity<Email>(ADMINREPO);
    email->setEmail(firstname.toLower() + "." + name.toLower() + "@gmail.com");
    static_cast<Patient*>(patient)->person()->addEmail(email);
    SharedPhone phone = AdminFactory::createEntity<Phone>(ADMINREPO);
    phone->setNumber("021/222 22 22");
    phone->setType(PhoneType::Private);
    static_cast<Patient*>(patient)->person()->addPhone(phone);
    static_cast<Patient*>(patient)->person()->gender(Person::Male);

    static_cast<Patient*>(patient)->statOk (false);

    //Register the patient in the database
//    if (!tryRequest(DB->setPatient(patient), tr("Impossible to save patient '%1 %2' in the database").arg(firstname, name)))
//        return false;

    ezechiel::GuiCore::Response r = ADMINREPO->setPatient(patient);
    if (r.error != ezechiel::GuiCore::NoError && r.error != NotFound) {
        return false;
    }

    return true;
}

bool AddAction::getDrugId(const QString &drugName, QString &drugId)
{
    //Search the drug ID
    foreach (Descriptor d, APPCORE->drugManager()->descriptions()) {
        if (d.name == drugName) {
            drugId = d.id;
            return true;
        }
    }

    //Return an error if not found
    _errors = tr("Could not find any drug matching the name '%1'\n").arg(drugName);
    return false;
}

bool AddAction::getDate(const QString &dateStr, QDateTime &date)
{
    //Get the date
    date = QDateTime::fromString(dateStr, Action::dateTimeFormat());

    //Verify the date
    if (date.isValid())
        return true;

    //Return an error if invalid
    _errors = tr("Invalid date or date format '%1'\n").arg(dateStr);
    return false;
}

bool AddAction::getValue(const QString &valueStr, double &value)
{
    bool valid;

    //Get the value
    value = valueStr.toDouble(&valid);

    if (valid && value >= 0.0)
        return true;

    //Return an error if invalid
    _errors = tr("Invalid or negative value '%1'\n").arg(valueStr);
    return false;
}

bool AddAction::getValue(const QString &valueStr, int &value)
{
    bool valid;

    //Get the value
    value = valueStr.toInt(&valid);

    if (valid && value >= 0)
        return true;

    //Return an error if invalid
    _errors = tr("Invalid or negative value '%1'\n").arg(valueStr);
    return false;
}

bool AddAction::setDosage(const ident curveId, Dosage* &dosage)
{
    DrugResponseAnalysis* _resp = ezechiel::GuiCore::CoreFactory::createEntity<DrugResponseAnalysis>(REPO);

    //Load the curve
    Response r = REPO->getDrugResponseAnalysisFromId(curveId, _resp);
    if (r.error != NoError) {
        LOG(QtFatalMsg, OBJECTNOTFOUND, tr("Drug response analysis not found from id %1").arg(curveId));
        return false;
    }

    //Verify the initial date
    if (_resp->getTreatment()->getDosages()->firsttake() > dosage->getApplied()) {
        _errors = tr("The dosage date cannot be before the firsttake date\n");
        return false;
    }

    //Add or update the dosage
    Dosage* dosageAtTime = _resp->getTreatment()->getDosages()->dosage(dosage->getApplied());

    if (dosageAtTime == dosage)
        return true;

    if (dosageAtTime->isValid())
        dosage->setId(dosageAtTime->id());

//    if (!tryRequest(DB->setDosage(curve->id(), dosage), tr("Impossible to save dosage for cuvre id '%1' in the database").arg(QString::number(curve->id()))))
//        return false;
    r = REPO->setDosage(dosage);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }

    //Register it
    _lastDosage = dosage;
    _lastCurveId = _resp->id();

    return true;
}

bool AddAction::setCurve(const QString drugId, const QString &curveName, const SharedPatient &patient, Dosage* &dosage)
{
    DrugResponseAnalysis* curve = ezechiel::GuiCore::CoreFactory::createEntity<DrugResponseAnalysis>(REPO);

    //Find a compatible model for the drug
    QList<QString> drugModels(APPCORE->drugManager()->models(drugId));
    QList<QString> compatiblePlugins(CORE->pluginManager()->filter(drugModels));
    if (compatiblePlugins.isEmpty()) {
        _errors = tr("Could not find any compatible model for the drug\n");
        return false;
    }

    //Get the model and engines IDs
    QString modelId(compatiblePlugins.first());

    //Set the parameter type
    ParameterType parameterType = APOSTERIORI;

    //Set the model
//    SharedDrugModel _mdl = SharedDrugModel(CORE->pluginManager()->newDrugModelEngine(modelId));
//    curve->setDrugModel(_mdl);

    //Set the drug
    ezechiel::GuiCore::DrugModel* _drg = APPCORE->drugManager()->drug(drugId);
    if (!_drg) {
        return false;
    }
    curve->setDrugModel(_drg);
    curve->getTreatment()->setActiveSubstanceId(_drg->getActiveSubstance()->getSubstanceId());

    //Set the engines
//    SharedPercentileEngine _pe = CORE->pluginManager()->newPercentileEngine(peId);
//    if (!_pe) {
//        return false;
//    }
//    curve->setPercentileEngine(_pe);

//    SharedPostEngine _po = CORE->pluginManager()->newPostEngine(poId);
//    if (!_po) {
//        return false;
//    }
//    curve->setPostEngine(_po);

//    SharedReverseEngine _re = CORE->pluginManager()->newReverseEngine(reId);
//    if (!_re) {
//        return false;
//    }
//    curve->setReverseEngine(_re);

    //Set the patient
    curve->getTreatment()->setPatient(patient);
//    if (!curve->getTreatment()->setPatient(patient)) {
//        _errors = tr("An error occured while setting the curve's patient: ") + curve->errorString() + "\n";
//        return false;
//    }

    //Finalize the curve
    curve->getTreatment()->getDosages()->setFirsttake(dosage->getApplied());
    curve->getTreatment()->setName(curveName);
    curve->getTreatment()->getDosages()->append(dosage);

    //Save the curve
    Response r = REPO->setDrugResponseAnalysis(curve);
    if (r.error != NoError){
        //this error mgmt should be in the databases project
        LOG(QtFatalMsg, CANTSAVE, "Couldnt save drug response analysis.");
        return false;
    }

    return true;
}

bool AddAction::tryRequest(const Response &r, const QString &msg)
{
   return APPUTILSREPO->tryRequest(r, msg, _errors);
}

} //namespace cliutils
} //namespace ezechiel
