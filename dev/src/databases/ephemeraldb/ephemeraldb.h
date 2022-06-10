//@@license@@

#ifndef EPHEMERALDB_H
#define EPHEMERALDB_H

#include <QtAlgorithms>
#include <QObject>

#include "core/utils/errorenums.h"
//#include "../admin/src/dal/clinical.h"
//#include "curve.h"
#include "core/dal/drug/drug.h"
#include "core/dal/prediction.h"
#include "core/dal/drug/adme.h"
#include "core/dal/covariate.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/doses.h"
//#include "../admin/src/dal/email.h"
#include "core/dal/drug/halflife.h"
#include "core/dal/drug/infusions.h"
//#include "../admin/src/dal/institute.h"
#include "core/dal/drug/intervals.h"
//#include "../admin/src/dal/location.h"
#include "core/dal/coremeasure.h"
//#include "../admin/src/dal/measure.h"
#include "core/dal/drug/parameters.h"
//#include "../admin/src/dal/patient.h"
//#include "../admin/src/dal/person.h"
#include "core/dal/percentile.h"
//#include "../admin/src/dal/phone.h"
//#include "../admin/src/dal/practician.h"
#include "core/dal/residual.h"
#include "core/dal/drug/target.h"
#include "core/corerepository.h"
#include "apputils/src/apputilsrepository.h"
#include "admin/src/stdadminrepository.h"
#include "core/dal/drug/translatablestring.h"
#include "core/utils/ezutils.h"
#include "core/dal/drug/doses.h"

// YTA: The methods instanciations should be in a cpp file

#include "core/dal/drugresponseanalysis.h"


class User;
namespace Tucuxi {

using namespace GuiCore;

template<class T>
class EntityList: public QList<T>
{
    int currentId;

public:
    EntityList() {
        currentId = 1;
    }

    Response getById(int id, T& value) {
        Response r;
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->id() == id) {
                value = this->at(i);
                r.error = NoError;
                r.results.append(value);
                return r;
            }
        }
        r.error = NotFound;
        return r;
    }

    Response getByProperty(const char * propname, QVariant query, T& value) {
        Response r;
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->property(propname) == query) {
                value = this->at(i);
                r.error = NoError;
                r.results.append(value);
                return r;
            }
        }
        r.error = NotFound;
        return r;
    }

    Response getByName(QString name, T& value) {
        Response r;
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->getName()->value().toLower() == name.toLower()) {
                value = this->at(i);
                r.error = NoError;
                r.results.append(value);
                return r;
            }
        }
        r.error = NotFound;
        return r;
    }

    Response addEntity(T entity) {
        Response r;
        for(int i=0;i<this->size();i++) {
            if (this->at(i) == entity) {
                r.error = NoError;
                return r;
            }
        }
        this->append(entity);
        entity->setId(currentId);
        currentId++;
        r.error = NoError;
        return r;
    }

    Response deleteEntity(T& entity) {
        Response r;
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->id() == entity->id()) {
                this->removeAt(i);
                r.error = NoError;
                return r;
            }
        }
        r.error = NotFound;
        return r;
    }

    Response deleteEntityById(int _id) {
        T entity;
        Response r;
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->id() == _id) {
                this->removeAt(i);
                r.error = NoError;
                return r;
            }
        }
        r.error = NotFound;
        return r;
    }

    Response getList(QList<T> &list) {
        for(int i=0;i<this->size();i++) {
            list.append(this->at(i));
        }
        Response r;
        r.error = NoError;
        return r;
    }


    static bool sortDate(const T &s1, const T &s2)
    {
        return s1->date() < s2->date();
    }


    Response getListByDate(QList<T> &list) {
        for(int i=0;i<this->size();i++) {
            list.append(this->at(i));
        }
        qSort(list.begin(),list.end(),sortDate);

        Response r;
        r.error = NoError;
        return r;
    }


    Response getListByPatient(int _pid,QList<T> &list) {
        // YTA: Commented this
        /*
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->patient()->id() == _pid)
                list.append(this->at(i));
        }
        */
        Response r;
        r.error = NoError;

        return r;
    }

    Response getListByAnalysis(int _pid,QList<T> &list) {
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->getAnalysis()->id() == _pid)
                list.append(this->at(i));
        }
        Response r;
        r.error = NoError;

        return r;
    }

    Response getListByNames(QStringList &_names,QList<T> &list) {
        for(int i=0;i<this->size();i++) {
            if (_names.contains(this->at(i)->getName()))
                list.append(this->at(i));
        }
        Response r;
        r.error = NoError;

        return r;
    }

    Response getListByPatientAndDrug(int _pid,QString _did, QList<T> &list) {
        // YTA: Commented this
        /*
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->patient()->id() == _pid && this->at(i)->drug()->descriptor().id == _did)
                list.append(this->at(i));
        }
        */
        Response r;
        r.error = NoError;
        return r;
    }

    Response getListByPatientAndDrug(int _pid, int _did, QList<T> &list) {
    // YTA: Commented this
        /*for(int i=0;i<this->size();i++) {
            if (this->at(i)->patient()->id() == _pid && this->at(i)->drug()->id() == _did)
                list.append(this->at(i));
        }
        */
        Response r;
        r.error = NoError;
        return r;
    }

    Response getListByPatientDate(int _pid,QList<T> &list) {
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->patientId() == _pid)
                list.append(this->at(i));
        }
        qSort(list.begin(),list.end(),sortDate);

        Response r;
        r.error = NoError;
        return r;
    }

    /*
    Response getListByDrug(int _did,QList<T> &list) {
        for(int i=0;i<this->size();i++) {
            if (this->at(i)->drug()->id() == _did)
                list.append(this->at(i));
        }
        Response r;
        r.error = NoError;

        return r;
    }
*/
};

//! \brief The local database backend plugin
/** \ingroup databaseplugins
  This plugin use SQL to store information in a local SQLite file.
  The database file used is the one given in the URL part in connect().
  If no database is given (empty url), the default one is used (in the plugins data directory).

  \section sqlite SQLite
    SQLite is a software library that implements a self-contained, serverless, zero-configuration, transactional SQL database engine.
    SQLite is the most widely deployed SQL database engine in the world.
    The source code for SQLite is in the public domain.

    Website : http://www.sqlite.org/

  */

class EphemeralDB : public QObject, public CoreRepository, public AppUtilsRepository, public StdAdminDbInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.heig-vd.ezechiel.ephemeralDBInterface/0.1")
    Q_INTERFACES(AppUtilsRepository StdAdminDbInterface)
    Q_INTERFACES(Tucuxi::GuiCore::CoreRepository)
public:

EphemeralDB() : _encryption(Unknown)
{
    _id = "ch.heig-vd.ezechiel.ephemeraldb";
//    init(CORE->instance());
}

//If not already done, close the database
~EphemeralDB()
{}

//EphemeralDB();
//~EphemeralDB();

//void visit(CurveManager &cm) {}

Descriptor getDescriptor() {return descriptor();}
Descriptor descriptor();
QString databaseName() const;

bool init(GuiCore::Core *core);

bool tryRequest(const Response&, const QString&, QString&) {return false;}

QString errorString();
bool connect(DBAccount& account);
bool disconnect();

void needs(bool &url, bool &parent, bool &password);

Response getUser (int, User&) {return Response();}
Response getUserKey(const QString&, QString&) {return Response();}
Response setUserKey(QString, const QString&) {return Response();}
Response getUser(const QString&, User&) {return Response();}
Response getUsersList(QList<User>&) {return Response();}
Response setUser(const User&) {return Response();}
Response deleteUser(QString) {return Response();}
Response isEncrypted(bool) {return Response();}

// Concenience ////////////////////////////////////////////////////////////////////////////////////

Response getPatientFromId(const int &_id, SharedPatient& _p);

Response getCorePatient(const int &_id, CorePatient* corepatient) {
    SharedPatient _p;
    Response r = patients.getById(_id, _p);
    if (r.error == NoError) {
        corepatient = static_cast<CorePatient*>(_p);
    }
    return r;
}

Response setPatient(SharedPatient& _p);
Response getPatientsList(QList<SharedPatient>& _in);
Response deletePatient(SharedPatient & _p);
Response getPracticianFromId(const int &_id, SharedPractician& _p);
Response setPractician(SharedPractician& _p);
Response getPracticiansList(QList<SharedPractician>& _in);
Response deletePractician(SharedPractician & _p);
Response deletePractician(const int &_id);
Response getInstituteFromId(const int &_id, SharedInstitute& _p);
Response setInstitute(SharedInstitute& _p);
Response getInstitutesList(QList<SharedInstitute>& _in);
Response deleteInstitute(SharedInstitute & _p);
Response deleteInstitute(const int &_id);

Response getCoreMeasure(const int &_id, CoreMeasure*_m) {
    return coremeasures.getById(_id, _m);
}

Response setCoreMeasure(CoreMeasure*_cm) {
    return coremeasures.addEntity(_cm);
}

Response deleteCoreMeasure(const int &_id) {
    return coremeasures.deleteEntityById(_id);
}

Response deleteCoreMeasure(CoreMeasure*_cm) {
    return coremeasures.deleteEntity(_cm);
}

Response getCoreMeasuresList(QList<CoreMeasure*> &_cm) {
    return coremeasures.getList(_cm);
}

Response getCoreMeasuresFromPatientAndDrug(const int&_id, const QString&_drugid, QList<CoreMeasure*>&_in) {
    //FIX!!!
    Response r;
    r.error = NoError;
    return r;
}

Response getCoreMeasuresFromPatientAndDrug(const int& _id, const int& _drugid, QList<CoreMeasure*>& _in) {
    _in.clear();
    QList<Measure *> _measures;
    Response r = measures.getListByPatientAndDrug(_id, _drugid, _measures);
    if (r.error != NoError){
       //throw
    }
    foreach (Measure * _m, _measures) {
        _in.append(static_cast<CoreMeasure*>(_m));
    }

    return r;
}

Response getCoreMeasuresByDate(const int&_id, QList<CoreMeasure*>&_in) {
    //FIX!!!
    Response r;
    r.error = NoError;
    return r;
}

Response getClinicalFromId(const int &_id, Clinical* _p);
Response setClinical(Clinical* _p);
Response getClinicalsList(QList<Clinical*>& _in);
Response getClinicalsByDate(const int &_id, QList<Clinical*>& _in);
Response deleteClinical(Clinical*  _p);
Response deleteClinical(const int &_id);

Response getPartialRequestFromId(const int &_id, PartialRequest* _p);
Response getPartialRequestFromRequestId(QString &_id, PartialRequest*& _p);
Response setPartialRequest(PartialRequest* _p);
Response getPartialRequestsList(QList<PartialRequest*>& _in);
Response getPartialRequestsByDate(const int &_id, QList<PartialRequest*>& _in);
Response deletePartialRequest(PartialRequest*  _p);
Response deletePartialRequest(const int &_id);

Response getPatientVariateFromId(const int &_id, PatientVariate* _p);
Response setPatientVariate(PatientVariate* _p);
Response setPatientVariate(const int &_pid, PatientVariate* _p);
Response getPatientVariatesList(QList<PatientVariate*>& _in);
Response getPatientVariatesofPatient(const int &_id, QList<PatientVariate*>& _in) {
    _in.clear();
    return patientVariates.getListByPatient(_id, _in);
}

Response getPatientVariatesOfNames(const int &_id, QStringList &_names, QList<PatientVariate*>& _in) {
    _in.clear();
    Response r = patientVariates.getListByPatient(_id, _in);
    if (r.error != NoError) {
        return r;
    }
    return patientVariates.getListByNames(_names, _in);
}

Response getPatientVariatesByDate(const int &_id, QList<PatientVariate*>& _in);
Response deletePatientVariate(PatientVariate* _p);
Response deletePatientVariate(const int &_id);
Response getDrugVariateFromId(const int &_id, DrugVariate* _p);
Response setDrugVariate(DrugVariate* _in);
Response setDrugVariate(const int &_id, DrugVariate* _in);
Response getDrugVariatesOfDrug(const int &_id, QList<DrugVariate*>& _in) {
    _in.clear();
    // YTA: Check if that's not used somewhere
    Response r;
    return r;
    //return drugVariates.getListByDrug(_id, _in);
}
Response getDrugVariatesList(QList<DrugVariate*>& _in);
Response getDrugVariatesByDate(const int &_id, QList<DrugVariate*>& _in);
Response deleteDrugVariate(DrugVariate* _p);
Response deleteDrugVariate(const int &_id);
Response getPredictionFromId(const int &_id, Prediction* _p);
Response setPrediction(Prediction* _p);
Response getPredictionsList(QList<Prediction*>& _in);
Response getPredictionsList(const int& pid, QList<Prediction*>& _in);
Response getPredictionsList(QList<int>& _in, QStringList& _slist);
Response getPredictionsList(QList<int>& _in, const ident &_id, const QString &_drugid);
Response getPredictionsList(QList<Prediction*>& _in, const ident &_id, const QString &_drugid);
Response deletePrediction(Prediction*  _p);
Response deletePrediction(const int &_id);
Response getDosageFromId(const int &_id, Dosage*& _p);
Response setDosage(Dosage*& _p);
Response getDosagesList(DosageHistory* _in);
//Response getDosagesOfPatientAndDrug(const int &_pid, const int &_did, DosageHistory* _in);
//Response getDosagesTakenOfPatientAndDrug(const int &_pid, const int &_did, DosageHistory* _in);
//Response getDosagesTakenOfPatientAndDrugAndOfPrediction(const int &_predid, const int &_patid, const int &_drugid, DosageHistory* _in);
//Response getDosagesOfPred(const int &_prid, DosageHistory* _in);
Response getDosagesByDate(const int &_id, DosageHistory* _in);
Response deleteDosage(Dosage* & _p);
Response deleteDosage(const int &_id);
Response getValidDosesOfDrug(const int &_prid, ValidDoses * _in);
Response getValidIntervalsOfDrug(const int &_prid, ValidIntervals* _in);
Response getValidInfusionsOfDrug(const int &_prid, ValidInfusions* _in);
Response getDrugFromId(const int &_id, DrugModel* _p);
Response getDrugOfName(const QString &_name, DrugModel*& _p) {
    return drugs.getByName(_name, _p);
}

Response getActiveSubstanceOfId(const QString & id, ActiveSubstance*& _p) {
    Response r;
    foreach (DrugModel *model, drugs) {
        if (model->getActiveSubstance()->getSubstanceId().compare(id) == 0) {
            _p = model->getActiveSubstance();
            r.error = NoError;
        }
    }
    return r;
}

Response getDrugOfDrugId(const QString &_id, DrugModel*& _p) {
    return drugs.getByProperty("drugId",QVariant::fromValue(_id), _p);
}

Response setDrug(DrugModel* _p);
Response getDrugsList(QList<DrugModel*>& _in);
Response deleteDrug(DrugModel* _p);
Response setValidDoses(const int &_id, ValidDoses *_in);
Response setValidInfusions(const int &_id, ValidInfusions*_in);
Response setValidIntervals(const int &_id, ValidIntervals *_in);
Response getEmailFromId(const int &_id, Email* _p);
Response setEmail(Email* _p);
Response getEmailsList(QList<Email*>& _in);
Response deleteEmail(Email*  _p);
Response getMeasureFromId(const int &_id, Measure * _p);
Response setMeasure(Measure * _p);
Response setMeasure(const int &_pid, Measure * _p);
Response getMeasuresList(QList<Measure *>& _in);
Response deleteMeasure(Measure *  _p);
Response deleteMeasure(const int &_id);
Response getMeasuresByDate(const int &_id, QList<Measure *>& _in);
Response getMeasuresFromPatientAndDrug(const int& _id, const QString& _drugid, QList<Measure *>& _in);
Response getMeasuresFromPatientAndDrug(const int& _id, const int& _drugid, QList<Measure *>& _in);
Response getOperationFromId(const int &_id, Operation* _p);
Response setOperation(Operation* _p);
Response getOperationsList(QList<Operation*>& _in);
Response deleteOperation(Operation*  _p);
Response getDrugResponseAnalysisFromId(const int &_id, DrugResponseAnalysis*& _resp) {
    return drugResponseAnalyses.getById(_id, _resp);
}

Response setDrugResponseAnalysis(DrugResponseAnalysis* _p) {
    return drugResponseAnalyses.addEntity(_p);
}

Response getDrugResponseAnalysissList(QList<DrugResponseAnalysis*>& _in) {
    return drugResponseAnalyses.getList(_in);
}

Response deleteDrugResponseAnalysis(DrugResponseAnalysis* _p) {
    return drugResponseAnalyses.deleteEntity(_p);
}

Response getParameterFromId(const int &_id, SharedParameter& _p);
Response setParameter(SharedParameter& _p);
Response getParametersList(QList<SharedParameter>& _in);
Response deleteParameter(SharedParameter & _p);
Response getPhoneFromId(const int &_id, SharedPhone& _p);
Response setPhone(SharedPhone& _p);
Response getPhonesList(QList<SharedPhone>& _in);
Response deletePhone(SharedPhone & _p);
Response getTargetFromId(const int &_id, Target* _p);
Response setTarget(Target* _p);
Response getTargetsList(QList<Target*>& _in);
Response getTargetsOfDrug(const int & did, TargetList* _in) {
    // YTA : To be checked
    return Response();
//    return targets.getListByDrug(did, *_in);
}

Response deleteTarget(Target*  _p);
Response getLocationFromId(const int &_id, Location* _p);
Response setLocation(Location* _p);
Response getLocationsList(QList<Location*>& _in);
Response deleteLocation(const int &_id);
Response deleteLocation(Location*  _p);
// Encryption /////////////////////////////////////////////////////////////////////////////////////

Response isEncrypted(const bool &encrypted);
Response setEncrypted(const bool &encrypted);
Response encryptDatabase(const bool &encrypt);

// Utils //////////////////////////////////////////////////////////////////////////////////////////

//Enable the foreign key support for SQLite
bool enableFkSupport();
int findOrCreate (const QString &id, const QString &table);
//Response queryError(/*const QSqlQuery &query*/) const;
Response databaseError(/*const QSqlDatabase &database,*/ const QString &message) const;

private:

    //The encrypted status enum
    enum Encryption {
        Encrypted,
        Clear,
        Unknown
    };

    //Functions that help with the transactions
    Response startTransaction(const QString &id);
    Response rollbackTransaction(const QString &id);
    Response commitTransaction(const QString &id);

    //Functions that check if an ID already exists
    bool exists(const QString &table, const QString &pk, const QString &value);
    bool exists(const QString &table, const QString &pk, int value);

    //Functions that encrypt or decrypt whole tables
    Response encryptPersons(bool encrypt);
    Response encryptPatients(bool encrypt);
    Response encryptPracticians(bool encrypt);
    Response encryptLocations(bool encrypt);
    Response encryptInstitutes(bool encrypt);
    Response encryptReports(bool encrypt);
    Response encryptCovariates(bool encrypt);
    Response encryptClinicals(bool encrypt);
    Response encryptMeasures(bool encrypt);
    Response encryptCurves(bool encrypt);
    Response encryptDosages(bool encrypt);

    //The database connexion
//    QSqlDatabase _db;

    //The database ID
    QString _id;

    //The encryption status
    Encryption _encryption;

    //The transaction ID
    QString _transactionId;

private:

    EntityList<SharedPractician> practicians;
    EntityList<SharedPatient> patients;
    EntityList<PatientVariate*> patientVariates;
    EntityList<DrugVariate*> drugVariates;
//    EntityList<SharedCurve> curves;
    EntityList<Prediction*> predictions;
    EntityList<Dosage*> dosages;
    EntityList<DrugModel*> drugs;
    EntityList<ValidDose*> validDoses;
    EntityList<ValidInterval*> validIntervals;
    EntityList<ValidInfusion*> validInfusions;
    EntityList<Email*> emails;
    EntityList<Measure *> measures;
    EntityList<CoreMeasure*> coremeasures;
    EntityList<Operation*> operations;
    EntityList<SharedParameter> parameters;
    EntityList<SharedPhone> phones;
    EntityList<Target*> targets;
    EntityList<DosageHistory*> dosageHistories;
    EntityList<Location*> locations;
    EntityList<SharedInstitute> institutes;
    EntityList<Clinical*> clinicals;
    EntityList<DrugResponseAnalysis*> drugResponseAnalyses;
    EntityList<PartialRequest*> partialrequests;
};

}

#endif // EPHEMERALDB_H
