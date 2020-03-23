#include "dbplugin.h"

#include "core.h"

DBPlugin::DBPlugin():
   next(0)
{
   //Fill the default error request code
   error.error = NoBackend;
   error.responder = "none";
}

// Set the next variable
void DBPlugin::setNext (StdAdminDbInterface *next)
{
   this->next = next;
}

// Set the plist variable
void DBPlugin::setList (QList<StdAdminDbInterface*> plist)
{
   this->plist = plist;
}

// Returns default data
Descriptor DBPlugin::getDescriptor ()
{
    return Descriptor("ch.heig-vd.ezechiel.database.default","default","");
}

//Returns an empty string
QString DBPlugin::databaseName() const
{
    return QString();
}

// Set the instance in Core
bool DBPlugin::init(Core *core)
{
   Core::setInstance(core);
   return true;
}

QString DBPlugin::errorString()
{
   return tr("The DBPlugin backend cannot connect or disconnect nor store any information");
}

// Returns false
bool DBPlugin::connect (DBAccount)
{ return false; }

//Returns false
bool DBPlugin::disconnect ()
{ return false; }

void DBPlugin::needs(bool &url, bool &user, bool &password)
{
   url = false;
   user = false;
   password = false;
}

// REQUEST MACRO
// Base of linked list algorithm, basically : "if next exist, call the function on him, else return an error"
#define DEFAULT(func) return (next) ? next->func : error;

// REQUESTS

Response DBPlugin::getVersion(QString &version)
{
    DEFAULT(getVersion(version));
}

Response DBPlugin::isCompatible(bool &compatible)
{
    DEFAULT(isCompatible(compatible));
}

Response DBPlugin::getPractician(const ident &id, SharedPractician &practician)
{
    DEFAULT(getPractician(id,practician));
}

Response DBPlugin::getPractician(const QString &externalId, SharedPractician &practician)
{
    DEFAULT(getPractician(externalId, practician));
}

Response DBPlugin::setPractician(SharedPractician practician)
{
    DEFAULT(setPractician(practician));
}

Response DBPlugin::deletePractician(const ident &id)
{
    DEFAULT(deletePractician(id));
}

Response DBPlugin::getPracticiansList(QList<SharedPractician> &list)
{
    DEFAULT(getPracticiansList(list));
}

Response DBPlugin::getInstitute(const ident &id, SharedInstitute &institute)
{
    DEFAULT(getInstitute(id,institute));
}

Response DBPlugin::getInstitute(const QString &externalId, SharedInstitute &institute)
{
    DEFAULT(getInstitute(externalId, institute));
}

Response DBPlugin::setInstitute(const SharedInstitute &institute)
{
    DEFAULT(setInstitute(institute));
}

Response DBPlugin::deleteInstitute(const ident &id)
{
    DEFAULT(deleteInstitute(id));
}

Response DBPlugin::getPerson(const ident &id, SharedPerson &person)
{
    DEFAULT(getPerson(id, person));
}

Response DBPlugin::setPerson(SharedPerson &person)
{
    DEFAULT(setPerson(person));
}

Response DBPlugin::deletePerson(const ident &id)
{
    DEFAULT(deletePerson(id));
}

Response DBPlugin::getPersonsList(QList<SharedPerson> &persons)
{
    DEFAULT(getPersonsList(persons));
}

Response DBPlugin::getLocation(const ident &id, Location *&location)
{
    DEFAULT(getLocation(id,location));
}

Response DBPlugin::setLocation(Location *location)
{
    DEFAULT(setLocation(location));
}

Response DBPlugin::deleteLocation(const ident &id)
{
    DEFAULT(deleteLocation(id));
}

Response DBPlugin::getLocationsList(QList<Location> &locations)
{
    DEFAULT(getLocationsList(locations));
}

Response DBPlugin::getPatient (const ident &id, SharedPatient &patient)
{
    DEFAULT(getPatient(id, patient));
}

Response DBPlugin::getPatient(const QString &externalId, SharedPatient &patient)
{
    DEFAULT(getPatient(externalId, patient));
}

Response DBPlugin::setPatient (SharedPatient &patient)
{
    DEFAULT(setPatient(patient));
}

Response DBPlugin::deletePatient (SharedPatient &patient)
{
    DEFAULT(deletePatient(patient));
}

Response DBPlugin::getPatientsList (QList<SharedPatient> &list)
{
    DEFAULT(getPatientsList(list));
}

Response DBPlugin::getMeasuresByDate (const ident &patient, QList<SharedMeasure> &list)
{
    DEFAULT(getMeasuresByDate(patient, list));
}

Response DBPlugin::setMeasure (const ident &patient, SharedMeasure measure)
{
    DEFAULT(setMeasure(patient, measure));
}

Response DBPlugin::deleteMeasure (const ident &id)
{
    DEFAULT(deleteMeasure(id));
}

Response DBPlugin::getClinicalsByDate(const ident &patient, QList<SharedClinical> &list)
{
    DEFAULT(getClinicalsByDate(patient, list));
}

Response DBPlugin::setClinical(const ident &patient, SharedClinical &clinical)
{
    DEFAULT(setClinical(patient, clinical));
}

Response DBPlugin::deleteClinical(const ident &id)
{
    DEFAULT(deleteClinical(id));
}

Response DBPlugin::getCovariatesByDate (const ident &patient, QList<SharedPatientVariate> &list)
{
    DEFAULT(getCovariatesByDate(patient, list));
}

Response DBPlugin::getMeasuresByDrug(const ident &patient, const QString &drugId, QList<SharedMeasure> &list)
{
    DEFAULT(getMeasuresByDrug(patient, drugId, list));
}

Response DBPlugin::setCovariate (const ident &patient, SharedPatientVariate covariate)
{
    DEFAULT(setCovariate(patient, covariate));
}

Response DBPlugin::deleteCovariate(const ident &id)
{
    DEFAULT(deleteCovariate(id));
}

Response DBPlugin::getReport(const ident &id, ReportData &data)
{
    DEFAULT(getReport(id, data));
}

Response DBPlugin::getReport(const QString &field, const QVariant &value, ReportData &data)
{
    DEFAULT(getReport(field, value, data));
}

Response DBPlugin::setReport(ReportData &data)
{
    DEFAULT(setReport(data));
}

Response DBPlugin::deleteReport(ReportData &data)
{
    DEFAULT(deleteReport(data));
}

Response DBPlugin::getReportsList(QList<ReportData *> &dataList, const QString &where)
{
    DEFAULT(getReportsList(dataList, where));
}

Response DBPlugin::getCurve(const ident &id, DBCurve &curve)
{
    DEFAULT(getCurve(id, curve));
}

Response DBPlugin::setCurve(DBCurve &curve)
{
    DEFAULT(setCurve(curve));
}

Response DBPlugin::deleteCurve(ident id)
{
    DEFAULT(deleteCurve(id));
}

Response DBPlugin::getCurvesList(QList<DBCurve> &curves)
{
    DEFAULT(getCurvesList(curves));
}

Response DBPlugin::getCurvesList(QList<DBCurve> &curves, const ident patientId, const QString &drugId)
{
    DEFAULT(getCurvesList(curves, patientId, drugId));
}

Response DBPlugin::getCurvesList(QList<ident> &ids, QStringList &names)
{
    DEFAULT(getCurvesList(ids, names));
}

Response DBPlugin::getDosagesByDate (const ident &curve, QList<SharedDosage> &list)
{
    DEFAULT(getDosagesByDate(curve, list));
}

Response DBPlugin::setDosage (const ident &curve, SharedDosage &dosage)
{
    DEFAULT(setDosage(curve, dosage));
}

Response DBPlugin::deleteDosage(const ident &id)
{
    DEFAULT(deleteDosage(id));
}

Response DBPlugin::getModelsOptions(const ident &curveId, QList<Option> &options)
{
    DEFAULT(getModelsOptions(curveId, options));
}

Response DBPlugin::getModelOptions(const QString &modelId, const ident &curveId, QList<Option> &options)
{
    DEFAULT(getModelOptions(modelId, curveId, options));
}

Response DBPlugin::setModelOption(const QString &modelId, const ident &curveId, Option &option)
{
    DEFAULT(setModelOption(modelId, curveId, option));
}

Response DBPlugin::deleteModelsOptions(const ident &curveId)
{
    DEFAULT(deleteModelsOptions(curveId));
}

Response DBPlugin::deleteModelOption(const QString &modelId, const ident &curveId, const ident &optionId)
{
    DEFAULT(deleteModelOption(modelId, curveId, optionId));
}

Response DBPlugin::getPEsOptions(const ident &curveId, QList<Option> &options)
{
    DEFAULT(getPEsOptions(curveId, options));
}

Response DBPlugin::getPEOptions(const QString &peId, const ident &curveId, QList<Option> &options)
{
    DEFAULT(getPEOptions(peId, curveId, options));
}

Response DBPlugin::setPEOption(const QString &peId, const ident &curveId, Option &option)
{
    DEFAULT(setPEOption(peId, curveId, option));
}

Response DBPlugin::deletePEsOptions(const ident &curveId)
{
    DEFAULT(deletePEsOptions(curveId));
}

Response DBPlugin::deletePEOption(const QString &peId, const ident &curveId, const ident &optionId)
{
    DEFAULT(deletePEOption(peId, curveId, optionId));
}

Response DBPlugin::getPOsOptions(const ident &curveId, QList<Option> &options)
{
    DEFAULT(getPOsOptions(curveId, options));
}

Response DBPlugin::getPOOptions(const QString &poId, const ident &curveId, QList<Option> &options)
{
    DEFAULT(getPOOptions(poId, curveId, options));
}

Response DBPlugin::setPOOption(const QString &poId, const ident &curveId, Option &option)
{
    DEFAULT(setPOOption(poId, curveId, option));
}

Response DBPlugin::deletePOsOptions(const ident &curveId)
{
    DEFAULT(deletePOsOptions(curveId));
}

Response DBPlugin::deletePOOption(const QString &poId, const ident &curveId, const ident &optionId)
{
    DEFAULT(deletePOOption(poId, curveId, optionId));
}

Response DBPlugin::getREsOptions(const ident &curveId, QList<Option> &options)
{
    DEFAULT(getREsOptions(curveId, options));
}

Response DBPlugin::getREOptions(const QString &reId, const ident &curveId, QList<Option> &options)
{
    DEFAULT(getREOptions(reId, curveId, options));
}

Response DBPlugin::setREOption(const QString &reId, const ident &curveId, Option &option)
{
    DEFAULT(setREOption(reId, curveId, option));
}

Response DBPlugin::deleteREsOptions(const ident &curveId)
{
    DEFAULT(deleteREsOptions(curveId));
}

Response DBPlugin::deleteREOption(const QString &reId, const ident &curveId, const ident &optionId)
{
    DEFAULT(deleteREOption(reId, curveId, optionId));
}

Response DBPlugin::getOption(const ident &id, Option &option)
{
    DEFAULT(getOption(id, option));
}

Response DBPlugin::setOption(Option &option)
{
    DEFAULT(setOption(option));
}

Response DBPlugin::deleteOption(const ident &id)
{
    DEFAULT(deleteOption(id));
}

Response DBPlugin::isEncrypted(bool &encrypted)
{
    DEFAULT(isEncrypted(encrypted));
}

Response DBPlugin::setEncrypted(bool encrypted)
{
    DEFAULT(setEncrypted(encrypted));
}

Response DBPlugin::encryptDatabase(bool encrypt)
{
    DEFAULT(encryptDatabase(encrypt));
}
