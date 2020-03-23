//#include "core/interfaces/genericdatabase.h"
#include "patientaction.h"

#include <QRegExp>

#include "admin/src/dal/measure.h"
#include "admin/src/dal/patient.h"
#include "admin/src/stdadminrepository.h"
#include "admin/src/adminfactory.h"
#include "apputils/src/appcore.h"
#include "apputils/src/drugmanager.h"
#include "core/pluginmanager.h"
#include "core/core.h"
#include "core/dal/drug/drug.h"
#include "core/dal/covariate.h"
#include "core/dal/drug/concentrations.h"
#include "core/definitionmanager.h"
#include "core/utils/errorenums.h"
#include "core/utils/logging.h"
#include "databases/ephemeraldb/ephemeraldb.h"
#include "errors_cliutils.h"

namespace ezechiel {
namespace cliutils {
PatientAction::PatientAction()
{
}

QString PatientAction::help()
{
    QString str;
    str = tr("   NAME") + "\n" +
          tr("       EzeCHieL Patient Command") + "\n" + "\n" +
          tr("   SYNOPSIS") + "\n" +
             "       " + QString("ezechiel-cli [OPTIONS] patient %1USERNAME %2PASSWORD SUBCOMMAND").arg(_CMD_PARAM_USERNAME, _CMD_PARAM_PASSWORD) + "\n" + "\n" +
          tr("   DESCRIPTION") + "\n" +
          tr("       The EzeCHieL Patient Command can be used to manage patients and their associated data, such as the measures and covariates.") + "\n" + "\n" +
          tr("   SUBCOMMANDS") + "\n" +
             "       list" + "\n" +
          tr("              Display the list of patients.")  + "\n" + "\n" +
             "       create FIRSTNAME LASTNAME BIRTHDATE" + "\n" +
          tr("              Create a new patient.")  + "\n" + "\n" +
             "       ID remove" + "\n" +
          tr("              Remove the patient with the given ID.") + "\n" + "\n" +
             "       ID measure list" + "\n" +
          tr("              Display the list of measure for the patient with the given ID.") + "\n" + "\n" +
             "       ID measure set TIME VALUE DRUG [UNIT]" + "\n" +
          tr("              Create a measure at the given TIME, for the given DRUG and the patient with the given ID, with the gievn UNIT.") + "\n" + "\n" +
             "       ID measure set MEASUREID VALUE" + "\n" +
          tr("              Modify the measure corresponding to MEASUREID for the patient with the given ID.") + "\n" + "\n" +
             "       ID measure remove MEASUREID" + "\n" +
          tr("              Remove the measure corresponding to MEASUREID for the patient with the given ID.") + "\n" + "\n" +
             "       ID covariate list" + "\n" +
          tr("              Display the list of covariates for the patient with the given ID.") + "\n" + "\n" +
             "       ID covariate set TIME COVARIATENAME VALUE" + "\n" +
          tr("              Create a covariate of type COVARIATENAME, at the given TIME, for the patient with the given ID.") + "\n" + "\n" +
             "       ID covariate set COVARIATEID VALUE" + "\n" +
          tr("              Modify the covariate corresponding to COVARIATEID for the patient with the given ID.") + "\n" + "\n" +
             "       ID covariate remove COVARIATEID" + "\n" +
          tr("              Remove the covariate corresponding to COVARIATEID for the patient with the given ID.") + "\n" + "\n" +
          "\n";

    return str;
}

bool PatientAction::run(const QStringList &iargs)
{
    QStringList args = iargs;

    //We need at least a command
    if (args.size() < 1) {
        _errors = tr("The 'patient' command requires at least a sub-command");
        return false;
    }

    QString comm = args.at(0);
    args.removeFirst();

    //List and create patient
    if (comm == "list")
        return listPatients();
    else if (comm == "create")
        return createPatient(args);

    //Test the argument count (at least the ID and a sub-command)
    if (args.size() < 1) {
        _errors = tr("The patient ID and a sub-command is required");
        return false;
    }

    //Get the ID
    bool ok;
    ident id = comm.toInt(&ok);
    if (!ok) {
        _errors = tr("The value '%1' is not an ID, which is numerical").arg(comm);
        return false;
    }

    //We need to fetch the patient
    SharedPatient p;
    ezechiel::core::Response r = ADMINREPO->getPatientFromId(id, p);
    if (r.error != NoError) {
        return false;
    }
//    if (!tryRequest(DB->getPatient(id,p),
//                    tr("Error while retrieving the patient '%1' from the database :").arg(id)) )
//        return false;

    comm = args.at(0);
    args.removeFirst();

    //Launch the sub-action
    if (comm == "measure")
        return measure(p,args);
    else if (comm == "covariate")
        return covariate(p,args);
    else if (comm == "remove")
        return removePatient(p,args);
    else {
        _errors = tr("Invalid sub-command : %1").arg(comm);
        return false;
    }
}

//If the response isn't good, set the error message and return false
bool PatientAction::tryRequest(const Response &r, const QString &msg)
{
   return ADMINREPO->tryRequest(r,msg,_errors);
}

//----------------------------- PATIENTS

bool PatientAction::listPatients()
{
    //Fetch it
    QList<SharedPatient> list;
//    if (!tryRequest(DB->getPatientsList(list),tr("Error while retrieving the patient list from the database :")))
//        return false;
    ezechiel::core::Response r = ADMINREPO->getPatientsList(list);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }

    //Print it
    LOG(QtDebugMsg, NOEZERROR, (QString("ID \t STAT_OK \t FIRSTNAME \t LASTNAME")));
    foreach (SharedPatient p, list)
    LOG(QtDebugMsg, NOEZERROR, (tr("%1 \t %2 \t %3 \t %4 \t 5").arg(p->id()).arg(static_cast<Patient*>(p)->statOk() ? tr("true"):tr("false"))
                .arg(static_cast<Patient*>(p)->person()->firstname())
                .arg(static_cast<Patient*>(p)->person()->name())));

    return true;
}

bool PatientAction::createPatient(const QStringList &args)
{
    SharedPatient patient = AdminFactory::createEntity<Patient>(ADMINREPO);

    static_cast<Patient*>(patient)->person()->firstname(args[0]);
    static_cast<Patient*>(patient)->person()->name(args[1]);

    QDateTime t = QDateTime::fromString(args[2], Action::dateTimeFormat());
    if (!t.isValid()) {
        _errors = tr("The 'birthdate' is invalid or not corretly formatted. Please use the format '%1'.").arg(Action::dateTimeFormat());
        return false;
    }
    QDate date = t.date();
    static_cast<Patient*>(patient)->person()->birthday(date);


    //Save the patient's data to the database and reset the 'edited' flag
//    if (!tryRequest(DB->setPatient(patient),QString(tr("Impossible to save patient [id%1] to the database")).arg(patient->id())))
//        return false;

    ezechiel::core::Response r = ADMINREPO->setPatient(patient);
    if (r.error != ezechiel::core::NoError) {
        return false;
    }

    LOG(QtDebugMsg, NOEZERROR, (tr("Patient ID: %1").arg(patient->id())));

    return true;
}

bool PatientAction::removePatient(SharedPatient &patient, QStringList &)
{
//    if (! tryRequest(DB->deletePatient(patient),
//                     tr("Error while removing the patient %1 from the database :").arg(patient->id())) )
//        return false;
    if (ADMINREPO->deletePatient(patient).error != NoError) {
        return false;
    }
    return true;
}

//----------------------------- MEASURES

//Check the arguments and launch the sub-value
bool PatientAction::measure(const SharedPatient &patient, QStringList &args)
{
    //At least a sub-command
    if (args.size() < 1) {
        _errors = tr("The a sub-command is required for the 'measure' command");
        return false;
    }

    QString first = args.at(0);
    args.removeFirst();

    //Sub-command
    if (first == "set")
        return setMeasure(patient,args);
    else if (first == "list")
        return listMeasures(patient);
    else  if (first == "remove")
        return removeMeasure(patient, args);
    else {
        _errors = tr("Invalid sub-command : %1").arg(first);
        return false;
    }
}

bool PatientAction::listMeasures(const SharedPatient &patient)
{
    //Fetch it
    QList<Measure *> list;
//    if (! tryRequest(DB->getMeasuresByDate(patient->id(),list),
//                     tr("Error while retrieving the patient's measure list from the database :")) )
//        return false;
    ezechiel::core::Response r = ADMINREPO->getMeasuresByDate(patient->id(), list);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }
    //Print it
    LOG(QtDebugMsg, NOEZERROR, (tr("ID \t DRUG \t TIME \t VALUE")));
    foreach (Measure * m, list)

    LOG(QtDebugMsg, NOEZERROR, (tr("%1 \t %2 \t %3 \t %4")
                .arg(m->id())
                .arg(m->getSdrug())
                .arg(m->getMoment().toString(Action::dateTimeFormat()))
                .arg(m->getConcentration()->toString()) ));

    return true;
}

bool PatientAction::setMeasure(const SharedPatient &patient, QStringList &args)
{
    //Two arguments = modify, three = create, other = ANGRY!
    if (args.size() < 2) {
        _errors = tr("To modify a measure, the measure ID and the new value are required, while the measure date, value and corresponding drug are required to create a new one.");
        return false;
    }

    //Create a new value
    if (args.size() > 2) {
        //Fetch the values
        Measure * m = AdminFactory::createEntity<Measure>(ADMINREPO);
        m->setMoment(QDateTime::fromString(args.at(0), Action::dateTimeFormat()));
        m->setSdrug(args.at(2));
//        m->setDrug(APPCORE->drugManager()->drug(m->sdrug()));
//        m->patient(patient);

        QString amountstring = args.at(1);
        if (args.size() == 4) {
            amountstring = amountstring + " " + args.at(3);
        }
        else {
            amountstring = amountstring + " " + APPCORE->drugManager()->drug(m->getSdrug())->getConcentrations()->getQuantity().unit().name();
        }

        m->getConcentration()->fromString(amountstring);


        //Test the date
        if (!m->getMoment().isValid()) {
            _errors = tr("Invalid date given : %1").arg(args.at(0));
            return false;
        }

        //Test the amount
        if (m->getConcentration()->value() == -1.0) {
            _errors = tr("Invalid value given : %1").arg(args.at(1));
            return false;
        }

        //Test the drug
        bool found = false;
        foreach (Descriptor d, APPCORE->drugManager()->descriptions())
            if (d.id == m->getSdrug()) {
                found = true;
                break;
            }
        //Found?
        if (!found) {
            _errors = tr("Invalid drug given : %1").arg(m->getSdrug());
            return false;
        }

        //Add in the DB
//        if ( ! tryRequest( DB->setMeasure(patient->id(),m),
//                           tr("Error while trying to add a new measure in the database :")))
//            return false;
        if (ADMINREPO->setMeasure(m).error != NoError) {
            return false;
        }
    }

    //Modify an existing value
    else if (args.size() == 2) {
        //Fetch the ID
        Measure * m;
        bool ok;
        m->setId(args.at(0).toInt(&ok));

        //Test the id
        if (!ok) {
            _errors = tr("Invalid ID given (must be numerical) : %1").arg(args.at(0));
            return false;
        }

        //Fetch the given measure
        QList<Measure *> list;
//        if (! tryRequest(DB->getMeasuresByDate(patient->id(),list),
//                         tr("Error while retrieving the patient's measure list from the database :")) )
//            return false;
        ezechiel::core::Response r = ADMINREPO->getMeasuresByDate(patient->id(),list);
        if (r.error != NoError && r.error != NotFound) {
            return false;
        }

        //Search for the ID
        ok = false;
        foreach (Measure * mm, list)
            if (mm->id() == m->id()) {
                m = mm;
                ok = true;
                break;
            }
        //Not found
        if (!ok) {
            _errors = tr("ID of the measure not found in the database : %1").arg(m->id());
            return false;
        }

        //Fetch the new value
        if (! m->getConcentration()->fromString(args.at(1)) ) {
            _errors = tr("Invalid value given : %1").arg(args.at(1));
            return false;
        }

        //Set it
//        if ( ! tryRequest( DB->setMeasure(patient->id(),m),
//                           tr("Error while setting the new value for measure %1 to %2 :").arg(m->id()).arg(m->amount().toString())))
//            return false;
        if (ADMINREPO->setMeasure(m).error != NoError) {
            return false;
        }
    }

    return true;
}

bool PatientAction::removeMeasure(const SharedPatient &, QStringList &args)
{
    //Fetch the ID
    bool ok;
    ident id = args.at(0).toInt(&ok);

    //Test the id
    if (!ok) {
        _errors = tr("Invalid ID given (must be numerical) : %1").arg(args.at(0));
        return false;
    }

    //Try to remove it
//    if ( ! tryRequest(DB->deleteMeasure(id),
//                      tr("Error while removing measure %1 from the database :").arg(id)))
//        return false;
    if (ADMINREPO->deleteMeasure(id).error != NoError) {
        return false;
    }

    return true;
}

//----------------------------- COVARIATES

bool PatientAction::covariate(const SharedPatient &patient, QStringList &args)
{
    //At least a sub-command
    if (args.size() < 1) {
        _errors = tr("The a sub-command is required for the 'covariate' command");
        return false;
    }

    QString first = args.at(0);
    args.removeFirst();

    //Sub-command
    if (first == "set")
        return setCovariate(patient,args);
    else if (first == "list")
        return listCovariates(patient);
    else  if (first == "remove")
        return removeCovariate(patient, args);
    else {
        _errors = tr("Invalid sub-command : %1").arg(first);
        return false;
    }
}

bool PatientAction::listCovariates(const SharedPatient &patient)
{
    //Fetch it
    QList<PatientVariate*> list;
//    if (! tryRequest(DB->getCovariatesByDate(patient->id(),list),
//                     tr("Error while retrieving the patient's covariate list from the database :")) )
    ezechiel::core::Response r = ADMINREPO->getPatientVariatesByDate(patient->id(),list);
    if (r.error != NoError && r.error != NotFound) {
        return false;
    }

    //Print it
    ;
    LOG(QtDebugMsg, NOEZERROR, (tr("ID \t DATE \t MID \t VALUE")));
    foreach (PatientVariate* c, list)

    LOG(QtDebugMsg, NOEZERROR, (tr("%1 \t %2 \t %3 \t %4 %5")
                .arg(c->id())
                .arg(c->getDate().toString(Action::dateTimeFormat()))
                .arg(c->getCovariateId())
                .arg(c->getQuantity()->value())
                .arg(c->getQuantity()->unit().name())));

    return true;
}

bool PatientAction::setCovariate(const SharedPatient &patient, QStringList &args)
{

    //Two arguments = modify, three = create, other = ANGRY!
    if (args.size() < 2 || args.size() > 3) {
        _errors = tr("To modify a covariate, the covariate ID and the new value are required, while the covariate date, value and corresponding MID are required to create a new one.");
        return false;
    }

    PatientVariate* c;
    //Create a new value
    if (args.size() == 3) {

        //Check duplicate
        QList<PatientVariate*> list;
//        if (! tryRequest(DB->getCovariatesByDate(patient->id(),list),
//                         tr("Error while retrieving the patient's covariate list from the database :")) )
//            return false;
        ezechiel::core::Response r = ADMINREPO->getPatientVariatesByDate(patient->id(),list);
        if (r.error != NoError && r.error != NotFound) {
            return false;
        }

        bool ok, existing;
        existing = false;
        ok = true;
        foreach (PatientVariate* cc, list) {
            if (cc->getCovariateId() == args.at(1)) {
                existing = true;
                if (cc->getDate() == QDateTime::fromString(args.at(0), Action::dateTimeFormat())) {
                    ok = false;
                    c = cc;
                    break;
                }
            }
        }

        //Not found
        if (!ok) {
            _errors = tr("A covariate with the id %1 exists for patient %2 at time %3,\n to update it perform the set command with arguments COVARIATEID VALUE instead of TIME COVARIATENAME VALUE.").arg(args.at(1)).arg(patient->id()).arg(args.at(0));
            return false;
        }

        QFileInfo fi(args.at(1));
        QString drugid = fi.completeBaseName();
        if (existing == false) {
            if (!CORE->definitionManager()->covariateExists(args.at(1)) && APPCORE->drugManager()->drug(drugid) == 0) {
                _errors = tr("Invalid drug id of the covariate id, or invalid global covariate: %1").arg(args.at(1));
                return false;
            }
            else {
                    c = ezechiel::core::CoreFactory::createEntity<PatientVariate>( ADMINREPO);
                    c->setCovariateId(args.at(1));
                    c->setDate(QDateTime::fromString(args.at(0), Action::dateTimeFormat()));
                    c->getQuantity()->setValue(args.at(2).toDouble(&ok));
                }
        }
        else {
            c->getQuantity()->setValue(args.at(2).toDouble(&ok));
        }

        //Test the date
        if (!c->getDate().isValid()) {
            _errors = tr("Invalid date given : %1").arg(args.at(0));
            return false;
        }

        //Test the value
        if (!ok) {
            _errors = tr("Invalid value given : %1").arg(args.at(2));
            return false;
        }

        //Test the MID
//        if (!c->isValid()) {
//            _errors = tr("Invalid MID given : %1").arg(args.at(1));
//            return false;
//        }

        //Add in the DB
//        if ( ! tryRequest( DB->setCovariate(patient->id(),c),
        if ( ADMINREPO->setPatientVariate(c).error != NoError) {
            return false;
        }
    }
    //Modify an existing value
    else if (args.size() == 2) {
        //Fetch the ID
        PatientVariate* c;
        bool ok;
        c->setId(args.at(0).toInt(&ok));

        //Test the id
        if (!ok) {
            _errors = tr("Invalid ID given (must be numerical) : %1").arg(args.at(0));
            return false;
        }

        //Fetch the given covariate
        QList<PatientVariate*> list;
//        if (! tryRequest(DB->getCovariatesByDate(patient->id(),list),
//                         tr("Error while retrieving the patient's covariate list from the database :")) )
//            return false;
        ezechiel::core::Response r = ADMINREPO->getPatientVariatesByDate(patient->id(),list);
        if (r.error != NoError && r.error != NotFound) {
            return false;
        }
        //Search for the ID
        ok = false;
        foreach (PatientVariate* cc, list)
            if (cc->id() == c->id()) {
                c = cc;
                ok = true;
                break;
            }
        //Not found
        if (!ok) {
            _errors = tr("ID of the covariate not found in the database : %1").arg(c->id());
            return false;
        }

        //Fetch the new value
        c->getQuantity()->setValue(args.at(1).toDouble(&ok));
        if (!ok) {
            _errors = tr("Invalid value given : %1").arg(args.at(1));
            return false;
        }

        //Set it
        if ( ADMINREPO->setPatientVariate(c).error != NoError) {
            return false;
        }
    }

    return true;
}

bool PatientAction::removeCovariate(const SharedPatient &, QStringList &args)
{
    //Fetch the ID
    bool ok;
    ident id = args.at(0).toInt(&ok);

    //Test the id
    if (!ok) {
        _errors = tr("Invalid ID given (must be numerical) : %1").arg(args.at(0));
        return false;
    }

    //Try to remove it
//    if ( ! tryRequest(DB->deleteCovariate(id),
//                      tr("Error while removing covariate %1 from the database :").arg(id)))
//        return false;
    if (ADMINREPO->deletePatientVariate(id).error != NoError) {
        return false;
    }

    return true;
}

} //namespace cliutils
} //namespace ezechiel
