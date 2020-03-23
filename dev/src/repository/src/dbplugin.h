#ifndef DBPLUGIN_H
#define DBPLUGIN_H

#include <QString>
#include <QList>
#include <QObject>

//#include "dbinterface.h"
#include "stdadminrepository.h"
#include "apputilsrepository.h"
#include "errorenums.h"

#include "ezutils.h"
#include "medical.h"
#include "dbobjects.h"

class Core;

//! The default database backend plugin.
/*! \ingroup database databaseplugins
  This object is directly used by the DB object as the last on the preferred list.
  It implements all the functions and requests in the DBInterface interface and provides a default behavior for the requests.
  This default behavior consist of trying the next plugin and if none is present return an error code.
  Every database plugin should extend this object, not the DBInterface.
  */
class Measure;
class Clinical;
class PatientVariate;
class ReportData;
class Dosage;
typedef Measure* SharedMeasure;
typedef Clinical* SharedClinical;
typedef PatientVariate* SharedPatientVariate;
typedef Dosage* SharedDosage;

class DBPlugin : public QObject, public StdAdminDbInterface
{
   Q_OBJECT
   Q_INTERFACES(StdAdminDbInterface)
public:

   //! This constructor needs to be initialised
   DBPlugin();

   //! \brief Set the next plugin
   void setNext (StdAdminDbInterface *next);
   //! \brief Set the plugin list
   void setList (QList<StdAdminDbInterface *> plist);

   //! \brief Get the plugin's name
   /*! DBPlugin will return "default" as name, nothing for the description,
      and ch.heig-vd.ezechiel.database.default for ID.
     */
   virtual Descriptor getDescriptor ();
   virtual QString databaseName() const;

   //! \brief Initialise the plugin
   /*! The default behavior of this function is to set the Core instance in Core.
      Based on that, a plugin might or might not want to reimplement the function.
     */
   virtual bool init(Core *core);

   //! \brief Error string
   /*! Will always responds that DBPlugin cannot connect or disconnect as it is a dead-end.
     */
   virtual QString errorString ();

   //! \brief Connect to the backend.
   /*! Simply returns false.
     */
   virtual bool connect (DBAccount account);

   //! \brief Disconnect to the backend.
   /*! Simply returns false.
     */
   virtual bool disconnect ();

   //! \brief Returns the database needs, in this case, nothing.
   virtual void needs (bool &url, bool &user, bool &password);

   //REQUESTS
   virtual Response getVersion(QString &version);
   virtual Response isCompatible(bool &compatible);
   virtual Response getPractician (const ident &id, SharedPractician &practician);
   virtual Response getPractician (const QString &externalId, SharedPractician &practician);
   virtual Response setPractician (SharedPractician practician);
   virtual Response deletePractician (const ident &id);
   virtual Response getPracticiansList (QList<SharedPractician> &list);
   virtual Response getInstitute (const ident &id, SharedInstitute &institute);
   virtual Response getInstitute (const QString &externalId, SharedInstitute &institute);
   virtual Response setInstitute (const SharedInstitute &institute);
   virtual Response deleteInstitute (const ident &id);
protected:
   virtual Response getPerson (const ident &id, SharedPerson &person);
   virtual Response setPerson (SharedPerson &person);
   virtual Response deletePerson (const ident &id);
   virtual Response getPersonsList(QList<SharedPerson> &persons);
public:
   virtual Response getLocation (const ident &id, Location *&location);
   virtual Response setLocation (Location *location);
   virtual Response deleteLocation (const ident &id);
   virtual Response getLocationsList(QList<Location> &locations);
   virtual Response getPatient (const ident &id, SharedPatient &patient);
   virtual Response getPatient (const QString &externalId, SharedPatient &patient);
   virtual Response setPatient (SharedPatient &patient);
   virtual Response deletePatient (SharedPatient &patient);
   virtual Response getPatientsList (QList<SharedPatient > &list);
   virtual Response getMeasuresByDate (const ident &patient, QList<SharedMeasure> &list);
   virtual Response getMeasuresByDrug (const ident &patient, const QString &drugId, QList<SharedMeasure> &list);
   virtual Response setMeasure (const ident &patient, SharedMeasure measure);
   virtual Response deleteMeasure (const ident &id);
   virtual Response getClinicalsByDate (const ident &patient, QList<SharedClinical> &list);
   virtual Response setClinical(const ident &patient, SharedClinical &clinical);
   virtual Response deleteClinical (const ident &id);
   virtual Response getCovariatesByDate (const ident &patient, QList<SharedPatientVariate> &list);
   virtual Response setCovariate (const ident &patient, SharedPatientVariate covariate);
   virtual Response deleteCovariate(const ident &id);
   virtual Response getReport(const ident &id, ReportData &data);
   virtual Response getReport(const QString &field, const QVariant &value, ReportData &data);
   virtual Response setReport(ReportData &data);
   virtual Response deleteReport(ReportData &data);
   virtual Response getReportsList(QList<ReportData *> &dataList, const QString &where = QString());
   virtual Response getCurve(const ident &id, DBCurve &curve);
   virtual Response setCurve(DBCurve &curve);
   virtual Response deleteCurve(ident id);
   virtual Response getCurvesList(QList<DBCurve> &curves);
   virtual Response getCurvesList(QList<DBCurve> &curves, const ident patientId, const QString &drugId);
   virtual Response getCurvesList(QList<ident> &ids, QStringList &names);
   virtual Response getDosagesByDate (const ident &curve, QList<SharedDosage> &list);
   virtual Response setDosage (const ident &curve, SharedDosage &dosage);
   virtual Response deleteDosage(const ident &id);
   virtual Response getModelsOptions(const ident &curveId, QList<Option> &options);
   virtual Response getModelOptions(const QString &modelId, const ident &curveId, QList<Option> &options);
   virtual Response setModelOption(const QString &modelId, const ident &curveId, Option &option);
   virtual Response deleteModelsOptions(const ident &curveId);
   virtual Response deleteModelOption(const QString &modelId, const ident &curveId, const ident &optionId);
   virtual Response getPEsOptions(const ident &curveId, QList<Option> &options);
   virtual Response getPEOptions(const QString &peId, const ident &curveId, QList<Option> &options);
   virtual Response setPEOption(const QString &peId, const ident &curveId, Option &option);
   virtual Response deletePEsOptions(const ident &curveId);
   virtual Response deletePEOption(const QString &peId, const ident &curveId, const ident &optionId);
   virtual Response getPOsOptions(const ident &curveId, QList<Option> &options);
   virtual Response getPOOptions(const QString &poId, const ident &curveId, QList<Option> &options);
   virtual Response setPOOption(const QString &poId, const ident &curveId, Option &option);
   virtual Response deletePOsOptions(const ident &curveId);
   virtual Response deletePOOption(const QString &poId, const ident &curveId, const ident &optionId);
   virtual Response getREsOptions(const ident &curveId, QList<Option> &options);
   virtual Response getREOptions(const QString &reId, const ident &curveId, QList<Option> &options);
   virtual Response setREOption(const QString &reId, const ident &curveId, Option &option);
   virtual Response deleteREsOptions(const ident &curveId);
   virtual Response deleteREOption(const QString &reId, const ident &curveId, const ident &optionId);
   virtual Response getOption(const ident &id, Option &option);
   virtual Response setOption(Option &option);
   virtual Response deleteOption(const ident &id);
   virtual Response isEncrypted(bool &encrypted);
   virtual Response setEncrypted(bool encrypted);
   virtual Response encryptDatabase(bool encrypt);

   //ADD NEW REQUEST ABOVE THIS MESSAGE


private:
   // Plugins list (neighborhoods)
   QList<StdAdminDbInterface*> plist;
   // Next plugin on the linked list
   StdAdminDbInterface *next;
   //Default request with an error code
   Response error;
};

#endif // DBPLUGIN_H
