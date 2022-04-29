//@@license@@

#include <QStringList>

#include "ephemeraldb.h"

#include "core/dal/drug/drugvariate.h"
#include "core/dal/drug/operation.h"
#include "core/dal/drugtreatment.h"

class Patient;

namespace ezechiel {


//void EphemeralDB::visit(CurveManager &cm) {}

Descriptor EphemeralDB::descriptor()
{
    return Descriptor(_id, "Ephemeral DB", "Local in-RAM memory");
}

QString EphemeralDB::databaseName() const
{
    return "";
}

bool EphemeralDB::init(Core *core)
{
//    Core::setInstance(core);


    return true;
}

QString EphemeralDB::errorString()
{
    return "";
}
//Open the database file
bool EphemeralDB::connect(DBAccount &account)
{
    return true;
}

//Close the database
bool EphemeralDB::disconnect()
{
    return true;
}

//We might use the url, but we do not request it
void EphemeralDB::needs(bool &url, bool &parent, bool &password)
{
    url = false;
    parent = false;
    password = false;
}


// Concenience ////////////////////////////////////////////////////////////////////////////////////

Response EphemeralDB::getPatientFromId(const int& _id, SharedPatient& _p) {
    return patients.getById(_id, _p);
}

Response EphemeralDB::setPatient(SharedPatient& _p) {
    return patients.addEntity(_p);
}

Response EphemeralDB::getPatientsList(QList<SharedPatient>& _in) {
    return patients.getList(_in);
}

Response EphemeralDB::deletePatient(SharedPatient & _p) {
    return patients.deleteEntity(_p);
}

Response EphemeralDB::getPracticianFromId(const int &_id, SharedPractician& _p) {
    return practicians.getById(_id, _p);
}

Response EphemeralDB::setPractician(SharedPractician& _p) {
    return practicians.addEntity(_p);
}

Response EphemeralDB::getPracticiansList(QList<SharedPractician>& _in) {
    return practicians.getList(_in);
}

Response EphemeralDB::deletePractician(SharedPractician & _p) {
    return practicians.deleteEntity(_p);
}

Response EphemeralDB::deletePractician(const int &_id) {
    return practicians.deleteEntityById(_id);
}

Response EphemeralDB::getInstituteFromId(const int &_id, SharedInstitute& _p) {
    return institutes.getById(_id, _p);
}

Response EphemeralDB::setInstitute(SharedInstitute& _p) {
    return institutes.addEntity(_p);
}

Response EphemeralDB::getInstitutesList(QList<SharedInstitute>& _in) {
    return institutes.getList(_in);
}

Response EphemeralDB::deleteInstitute(SharedInstitute & _p) {
    return institutes.deleteEntity(_p);
}

Response EphemeralDB::deleteInstitute(const int &_id) {
    return institutes.deleteEntityById(_id);
}

Response EphemeralDB::getClinicalFromId(const int &_id, Clinical* _p) {
    clinicals.getById(_id, _p);
    return Response();
}

Response EphemeralDB::setClinical(Clinical* _p) {
    return clinicals.addEntity(_p);
}

Response EphemeralDB::getClinicalsList(QList<Clinical*>& _in) {
    return clinicals.getList(_in);
}

Response EphemeralDB::getClinicalsByDate(const int &_id, QList<Clinical*>& _in) {
    // YTA: To be changed
    //return clinicals.getListByDate(_in);
    Response r;
    r.error = NoError;
    return r;
}

Response EphemeralDB::deleteClinical(Clinical*  _p) {
    return clinicals.deleteEntity(_p);
}

Response EphemeralDB::deleteClinical(const int &_id) {
    return clinicals.deleteEntityById(_id);
}

Response EphemeralDB::getPartialRequestFromId(const int &_id, PartialRequest* _p) {
    partialrequests.getById(_id, _p);
    return Response();
}

Response EphemeralDB::getPartialRequestFromRequestId(QString &_id, PartialRequest*& _p) {
    partialrequests.getByProperty("requestId", QVariant::fromValue(_id), _p);
    return Response();
}

Response EphemeralDB::setPartialRequest(PartialRequest* _p) {
    return partialrequests.addEntity(_p);
}

Response EphemeralDB::getPartialRequestsList(QList<PartialRequest*>& _in) {
    return partialrequests.getList(_in);
}

Response EphemeralDB::getPartialRequestsByDate(const int &_id, QList<PartialRequest*>& _in) {
    // YTA: To be changed
    //return partialrequests.getListByDate(_in);
    Response r;
    r.error = NoError;
    return r;
}

Response EphemeralDB::deletePartialRequest(PartialRequest*  _p) {
    return partialrequests.deleteEntity(_p);
}

Response EphemeralDB::deletePartialRequest(const int &_id) {
    return partialrequests.deleteEntityById(_id);
}

Response EphemeralDB::getPatientVariateFromId(const int &_id, PatientVariate* _p) {
    return patientVariates.getById(_id,_p);
}

Response EphemeralDB::setPatientVariate(PatientVariate* _p) {
    return patientVariates.addEntity(_p);
}

Response EphemeralDB::setPatientVariate(const int &_pid, PatientVariate* _p) {
    return patientVariates.addEntity(_p);
}

Response EphemeralDB::getPatientVariatesList(QList<PatientVariate*>& _in) {
    _in.clear();
    return patientVariates.getList(_in);
}

Response EphemeralDB::getPatientVariatesByDate(const int &_id, QList<PatientVariate*>& _in) {
    Response r;
    r.error = NoError;
    return r;
    // YTA: To be changed
    /*
    WhereClauseMap _where;
    _where.insert(PatientVariate::staticMetaObject.indexOfProperty("patient_id"), Comparison(AND_Equals, _id));
    OrderByClauseMap _order;
    _order.insert(PatientVariate::staticMetaObject.indexOfProperty("date"), Ascending);
    Response r = NEWDB->getDBObjects(PatientVariate::staticMetaObject,  0, _where, _order);
    if (r.error != NoError) {
        return r;
    }
    foreach (QObject* qobj, r.results) {
        _in.append(SharedPatientVariate(qobj));
    }
    return r;
    */
}

Response EphemeralDB::deletePatientVariate(PatientVariate* _p) {
    return patientVariates.deleteEntity(_p);
}

Response EphemeralDB::deletePatientVariate(const int &_id) {
    return patientVariates.deleteEntityById(_id);
}

Response EphemeralDB::getDrugVariateFromId(const int &_id, DrugVariate* _p) {
    return drugVariates.getById(_id,_p);
}

Response EphemeralDB::setDrugVariate(DrugVariate* _in) {
    return drugVariates.addEntity(_in);
}

Response EphemeralDB::setDrugVariate(const int &_id, DrugVariate* _in) {
    Response r;
    r.error = NoError;
    return r;
// YTA: To be changed
    /*    return drugVariates.addEntity()
    WhereClauseMap _where;
    _where.insert(Drug::staticMetaObject.indexOfProperty("id"), Comparison(AND_Equals, _id));
    Response r = getDBObjects(Drug::staticMetaObject, 0, _where);
    if (r.error != NoError) {
        return r;
    }
    _in->setDrug(SharedDrug(r.castresults<Drug*>().last()));
    return setDBObject(*_in, _in->parent());
*/
}

Response EphemeralDB::getDrugVariatesList(QList<DrugVariate*>& _in) {
    _in.clear();
    return drugVariates.getList(_in);
}

Response EphemeralDB::getDrugVariatesByDate(const int &_id, QList<DrugVariate*>& _in) {
    Response r = drugVariates.getList(_in);
    QList<DrugVariate*> dvars = r.castresults<DrugVariate*>();
    QList<DrugVariate*>::iterator it, it_end, next;
    it = dvars.begin();
    it_end = dvars.end();

    // YTA: DrugVariate should not be timed. PatientVariate yes
//    std::sort(dvars.begin(), dvars.end(), [](DrugVariate* dvar1, DrugVariate* dvar2){return dvar1->getDate() < dvar2->getDate();});
    return r;
}

Response EphemeralDB::deleteDrugVariate(DrugVariate* _p) {
    return drugVariates.deleteEntity(_p);
}

Response EphemeralDB::deleteDrugVariate(const int &_id) {
    return drugVariates.deleteEntityById(_id);
}

Response EphemeralDB::getPredictionFromId(const int &_id, Prediction* _p) {
    predictions.getById(_id,_p);
    return Response();
}

Response EphemeralDB::setPrediction(Prediction* _p) {
    return predictions.addEntity(_p);
}

Response EphemeralDB::getPredictionsList(QList<Prediction*>& _in) {
    return predictions.getList(_in);
}

Response EphemeralDB::getPredictionsList(const int &pid, QList<Prediction*>& _in) {
    return predictions.getListByAnalysis(pid, _in);
}

Response EphemeralDB::getPredictionsList(QList<int>& _in, QStringList& _slist) {
    return Response();
}

Response EphemeralDB::getPredictionsList(QList<int>& _in, const ident &_id, const QString &_drugid) {
    return Response();
}

Response EphemeralDB::getPredictionsList(QList<Prediction*>& _in, const ident &_id, const QString &_drugid) {
    Response r = predictions.getList(_in);
    QList<Prediction*> preds = r.castresults<Prediction*>();
    QList<Prediction*>::iterator it, it_end, next;
    it = preds.begin();
    it_end = preds.end();

    for (next = it; it != it_end; it = next) {
        if ((*it)->getAnalysis()->getTreatment()->getPatient()->id() != _id) {
            preds.removeOne(*it);
        }
        if ((*it)->getAnalysis()->getTreatment()->getActiveSubstanceId() != _drugid) {
            preds.removeOne(*it);
        }
    }
    return r;
}

Response EphemeralDB::deletePrediction(Prediction*  _p) {
    return predictions.deleteEntity(_p);
}

Response EphemeralDB::deletePrediction(const int &_id) {
    return predictions.deleteEntityById(_id);
}

Response EphemeralDB::getDosageFromId(const int &_id, Dosage*& _p) {
    return dosages.getById(_id,_p);
}

Response EphemeralDB::setDosage(Dosage*& _p) {
    return dosages.addEntity(_p);
}

Response EphemeralDB::getDosagesList(DosageHistory* _in) {
    QList<Dosage*> list;
    Response r = dosages.getList(list);
    foreach(Dosage* dosage, list)
        _in->append(dosage);
    return r;
}


//Response EphemeralDB::getDosagesOfPatientAndDrug(const int &_pid, const int &_did, DosageHistory* _in) {
//    Response r = dosages.getList(*_in);
///* YTA commented this
// *     if (_in->empty()) return r;
//    auto it = _in->constBegin();
//    auto it_next = ++_in->constBegin();
//    for (; it != _in->constEnd(); it = it_next, ++it_next) {
//        if ((*it)->prediction()->getTreatment()->patient()->id() != _pid || (*it)->prediction()->getTreatment()->drug()->id() != _did)
//            _in->removeOne(*it);
//    }
//    */
//    return r;

//}

//Response EphemeralDB::getDosagesTakenOfPatientAndDrug(const int &_pid, const int &_did, DosageHistory* _in) {
//    Response r = dosages.getList(*_in);
///* YTA commented this
// *     if (_in->empty()) return r;
//    auto it = _in->constBegin();
//    auto it_next = ++_in->constBegin();
//    for (; it != _in->constEnd(); it = it_next, ++it_next) {
//        if ((*it)->prediction()->getTreatment()->patient()->id() != _pid || (*it)->prediction()->getTreatment()->drug()->id() != _did)
//            _in->removeOne(*it);
//    }
//    */
//    return r;
//}

//Response EphemeralDB::getDosagesTakenOfPatientAndDrugAndOfPrediction(const int &_predid, const int &_patid, const int &_drugid, DosageHistory* _in) {
//    Response r = dosages.getList(*_in);
//    /* YTA commented this
//    if (_in->empty()) return r;
//    auto it = _in->constBegin();
//    auto it_next = ++_in->constBegin();
//    for (; it != _in->constEnd(); it = it_next, ++it_next) {
//        if ((*it)->prediction()->id() != _predid  || (*it)->prediction()->getTreatment()->patient()->id() != _patid || (*it)->prediction()->getTreatment()->drug()->id() != _drugid)
//            _in->removeOne(*it);
//    }
//    */
//    return r;
//}

//Response EphemeralDB::getDosagesOfPred(const int &_prid, DosageHistory* _in) {
//    Response r = dosages.getList(*_in);
//    /* YTA commented this
//    if (_in->empty()) return r;
//    auto it = _in->constBegin();
//    auto it_next = ++_in->constBegin();
//    for (; it != _in->constEnd(); it = it_next, ++it_next) {
//        if ((*it)->prediction()->id() != _prid)
//            _in->removeOne(*it);
//    }
//    */
//    return r;
//}

Response EphemeralDB::getDosagesByDate(const int &_id, DosageHistory* _in) {
    QList<Dosage*> list = _in->getList();
    Response r = dosages.getList(list);
    if (list.empty()) return r;
    std::sort(list.begin(), list.end(), [](Dosage* _d1, Dosage* _d2){return _d1->getApplied() < _d2->getApplied();});
    return r;
}

Response EphemeralDB::getValidDosesOfDrug(const int &_prid, ValidDoses *_in) {
/*    Response r = validDoses.getList(*_in);
    if (_in->empty()) return r;
    auto it = _in->constBegin();
    auto it_next = ++_in->constBegin();
    for (; it != _in->constEnd(); it = it_next, ++it_next) {
        if ((*it)->drug()->id() != _prid)
            _in->removeOne(*it);
    }
    return r;
*/
    return Response();
}

Response EphemeralDB::getValidIntervalsOfDrug(const int &_prid, ValidIntervals* _in) {
/*    Response r = validIntervals.getList(*_in);
    if (_in->empty()) return r;
    auto it = _in->constBegin();
    auto it_next = ++_in->constBegin();
    for (; it != _in->constEnd(); it = it_next, ++it_next) {
        if ((*it)->drug()->id() != _prid)
            _in->removeOne(*it);
    }
    return r;
*/
    return Response();
}

Response EphemeralDB::getValidInfusionsOfDrug(const int &_prid, ValidInfusions* _in) {
/*
 *     Response r = validInfusions.getList(*_in);
    if (_in->empty()) return r;
    auto it = _in->constBegin();
    auto it_next = ++_in->constBegin();
    for (; it != _in->constEnd(); it = it_next, ++it_next) {
        if ((*it)->drug()->id() != _prid)
            _in->removeOne(*it);
    }
    return r;
*/
    return Response();
}

Response EphemeralDB::deleteDosage(Dosage* & _p) {
    return dosages.deleteEntity(_p);
}

Response EphemeralDB::deleteDosage(const int &_id) {
    return dosages.deleteEntityById(_id);
}

Response EphemeralDB::getDrugFromId(const int &_id, DrugModel* _p) {
    return drugs.getById(_id,_p);
}

Response EphemeralDB::setDrug(DrugModel* _p) {
    return drugs.addEntity(_p);
}

Response EphemeralDB::getDrugsList(QList<DrugModel*>& _in) {
    _in.clear();
    return drugs.getList(_in);
}

Response EphemeralDB::deleteDrug(DrugModel* _p) {
    return drugs.deleteEntity(_p);
}

Response EphemeralDB::setValidDoses(const int &_id, ValidDoses *_in) {
    Response r;
    for (ValidDoses::iterator it = _in->begin(); it != _in->end(); ++it) {
        r = validDoses.addEntity(*it);
    }
    return r;
}

Response EphemeralDB::setValidIntervals(const int &_id, ValidIntervals* _in) {
    Response r;
    for (ValidIntervals::iterator it = _in->begin(); it != _in->end(); ++it) {
        r = validIntervals.addEntity(*it);
    }
    return r;
}

Response EphemeralDB::setValidInfusions(const int &_id, ValidInfusions*_in) {
    Response r;
    for(int i=0;i<_in->getList()->size(); i++)
        r = validInfusions.addEntity(_in->getList()->at(i));
    return r;
}

Response EphemeralDB::getEmailFromId(const int &_id, Email* _p) {
    return emails.getById(_id,_p);
}

Response EphemeralDB::setEmail(Email* _p) {
    return emails.addEntity(_p);
}

Response EphemeralDB::getEmailsList(QList<Email*>& _in) {
    return emails.getList(_in);
}

Response EphemeralDB::deleteEmail(Email*  _p) {
    return emails.deleteEntity(_p);
}


Response EphemeralDB::getMeasureFromId(const int &_id, Measure * _p) {
    return measures.getById(_id,_p);
}

Response EphemeralDB::setMeasure(Measure * _p) {
    return measures.addEntity(_p);
}

Response EphemeralDB::setMeasure(const int &_pid, Measure * _p) {

    SharedPatient patient;
    Response r = getPatientFromId(_pid,patient);
    if (r.error != NoError)
        return r;
    return measures.addEntity(_p);
}

Response EphemeralDB::getMeasuresList(QList<Measure *>& _in) {
    return measures.getList(_in);
}

Response EphemeralDB::deleteMeasure(Measure *  _p) {
    return measures.deleteEntity(_p);
}

Response EphemeralDB::deleteMeasure(const int &_id) {
    return measures.deleteEntityById(_id);
}

bool sortMeasureDate(const Measure * s1, const Measure * s2)
{
    return s1->getMoment() < s2->getMoment();
}


Response EphemeralDB::getMeasuresByDate(const int &_id, QList<Measure *>& _in) {
    Response r = measures.getListByPatient(_id, _in);
    qSort(_in.begin(),_in.end(),sortMeasureDate);
    return r;
}

Response EphemeralDB::getMeasuresFromPatientAndDrug(const int& _id, const QString& _drugid, QList<Measure *>& _in) {
    _in.clear();
    Response r = measures.getListByPatientAndDrug(_id, _drugid, _in);
    return r;
}

Response EphemeralDB::getMeasuresFromPatientAndDrug(const int& _id, const int& _drugid, QList<Measure *>& _in) {
    _in.clear();
    Response r = measures.getListByPatientAndDrug(_id, _drugid, _in);
    return r;
}

Response EphemeralDB::getOperationFromId(const int &_id, Operation* _p) {
    return operations.getById(_id,_p);
}

Response EphemeralDB::setOperation(Operation* _p) {
    return operations.addEntity(_p);
}

Response EphemeralDB::getOperationsList(QList<Operation*>& _in) {
    return operations.getList(_in);
}

Response EphemeralDB::deleteOperation(Operation*  _p) {
    return operations.deleteEntity(_p);
}

Response EphemeralDB::getParameterFromId(const int &_id, SharedParameter& _p) {
    return parameters.getById(_id,_p);
}

Response EphemeralDB::setParameter(SharedParameter& _p) {
    return parameters.addEntity(_p);
}

Response EphemeralDB::getParametersList(QList<SharedParameter>& _in) {
    return parameters.getList(_in);
}

Response EphemeralDB::deleteParameter(SharedParameter & _p) {
    return parameters.deleteEntity(_p);
}

Response EphemeralDB::getTargetFromId(const int &_id, Target* _p) {
    return targets.getById(_id,_p);
}

Response EphemeralDB::setTarget(Target* _p) {
    return targets.addEntity(_p);
}

Response EphemeralDB::getTargetsList(QList<Target*>& _in) {
    return targets.getList(_in);
}

Response EphemeralDB::deleteTarget(Target*  _p) {
    return targets.deleteEntity(_p);
}

Response EphemeralDB::getLocationFromId(const int &_id, Location* _p) {
    return locations.getById(_id,_p);
}

Response EphemeralDB::setLocation(Location* _p) {
    return locations.addEntity(_p);
}

Response EphemeralDB::getLocationsList(QList<Location*>& _in) {
    return locations.getList(_in);
}

Response EphemeralDB::deleteLocation(const int &_id) {
    return locations.deleteEntityById(_id);
}

Response EphemeralDB::deleteLocation(Location*  _p) {
    return locations.deleteEntity(_p);
}
// Encryption /////////////////////////////////////////////////////////////////////////////////////

Response EphemeralDB::isEncrypted(const bool &encrypted)
{
    return Response();
}

Response EphemeralDB::setEncrypted(const bool &encrypted)
{
    return Response();
}

Response EphemeralDB::encryptDatabase(const bool &encrypt)
{
    return Response();
}

// Utils //////////////////////////////////////////////////////////////////////////////////////////

//Enable the foreign key support for SQLite
bool EphemeralDB::enableFkSupport()
{
    return false;
}

//Find the ID or create this element
int EphemeralDB::findOrCreate (const QString &id, const QString &table)
{
    return EXIT_FAILURE;
}

//Return an error message on a query failure
//Response EphemeralDB::queryError(const QSqlQuery &query) const
//{
//    Response response(_id);

//    return response;
//}

//Return an error message on a database failure
Response EphemeralDB::databaseError(const QString &message) const
{
    Response response(_id);

    return response;
}

}
