//@@license@@

#ifndef LOCALDB_H
#define LOCALDB_H

class ReportData;

#include <QSqlDatabase>

#include "dbplugin.h"
#include "dbobjects.h"

//! \brief The local database backend plugin
/*! \ingroup databaseplugins
  This plugin use SQL to store information in a local SQLite file.
  The database file used is the one given in the URL part in connect().
  If no database is given (empty url), the default one is used (in the plugins data directory).

  \section sqlite SQLite
    SQLite is a software library that implements a self-contained, serverless, zero-configuration, transactional SQL database engine.
    SQLite is the most widely deployed SQL database engine in the world.
    The source code for SQLite is in the public domain.

    Website : http://www.sqlite.org/

  */
class LocalDB : public DBPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "localdb")
    Q_INTERFACES(DBInterface)

public:

    LocalDB ();
    virtual ~LocalDB ();

    //! \brief Get the plugin's name
    /*! This plugin identifier is ch.heig-vd.ezechiel.localdb
     */
    Descriptor getDescriptor ();

    virtual QString databaseName() const;

    //! Set the Core instance in Core...
    bool init (Core *core);

    //! Will return the message given by the SQL backend (db.lastError())
    QString errorString ();

    //! \brief Connect to the backend.
    /*! Only uses the URL of the account, to find the database file.
     If the URL is empty, use the default location (in the plugin's data)
     */
    bool connect (DBAccount account);

    //! \brief Disconnect to the backend.
    /*! Simply returns true, nothing given from the backend.
     */
    bool disconnect ();

    //! Needs nothing right now
    void needs (bool &url, bool &user, bool &password);


    //REQUESTS

    virtual Response getVersion(QString &version);
    virtual Response isCompatible(bool &compatible);
    virtual Response getUser(const QString &username, User &user);
    virtual Response setUser(const User &user);
    virtual Response getUserKey(const QString &username, QString &key);
    virtual Response setUserKey(const QString &username, const QString &key);
    virtual Response deleteUser(const QString &username);
    virtual Response getUsersList(QList<User> &list);
    virtual Response getPractician (const ident &id, SharedPractician &practician);
    virtual Response getPractician (const QString &externalId, SharedPractician &practician);
    virtual Response setPractician (SharedPractician practician);
    virtual Response deletePractician (const ident &id);
    virtual Response getPracticiansList (QList<SharedPractician> &list);
    virtual Response getInstitute (const ident &id, SharedInstitute &institute);
    virtual Response getInstitute (const QString &externalId, SharedInstitute &institute);
    virtual Response setInstitute (const SharedInstitute &institute);
    virtual Response deleteInstitute (const ident &id);
    virtual Response getInstitutesList (QList<SharedInstitute> &list);
protected:
    virtual Response getPerson (const ident &id, SharedPerson &person);
    virtual Response setPerson (SharedPerson &person);
    virtual Response deletePerson (const ident &id);
    virtual Response getPersonsList(QList<SharedPerson> &persons);
public:
    virtual Response getLocation (const ident &id, SharedLocation &location);
    virtual Response setLocation (SharedLocation location);
    virtual Response getLocation (const ident &id, Location *&location);
    virtual Response setLocation (Location *location);
    virtual Response deleteLocation (const ident &id);
    virtual Response getLocationsList(QList<Location *> &locations);
    virtual Response getPatient (const ident &id, SharedPatient &patient);
    virtual Response getPatient (const QString &externalId, SharedPatient &patient);
    virtual Response setPatient (SharedPatient &patient);
    virtual Response deletePatient (const ident &id);
    virtual Response getPatientsList (QList<SharedPatient> &list);
    virtual Response getMeasuresByDate (const ident &patient, QList<SharedMeasure> &list);
    virtual Response getMeasuresByDrug (const ident &patient, const QString &drugId, QList<SharedMeasure> &list);
    virtual Response setMeasure (const ident &patient, SharedMeasure measure);
    virtual Response deleteMeasure (const ident &id);
    virtual Response getClinicalsByDate (const ident &patient, QList<SharedClinical> &list);
    virtual Response setClinical(const ident &patient, SharedClinical &clinical);
    virtual Response deleteClinical (const ident &id);
    virtual Response getCovariatesByDate (const ident &patient, QList<SharedPatientVariate> &list);
    virtual Response setCovariate (const ident &patient, SharedPatientVariate &covariate);
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

private:
    //The encrypted status enum
    enum Encryption {
        Encrypted,
        Clear,
        Unknown
    };

    //Enables the foreign key support for SQLite
    bool enableFkSupport();

    //Fetches or creates an ID from the database
    ident findOrCreate(const QString &id, const QString &table);

    //Functions that returns a formated error
    Response queryError(const QSqlQuery &query) const;
    Response databaseError(const QSqlDatabase &database, const QString &message) const;

    //Functions that help with the transactions
    Response startTransaction(const QString &id);
    Response rollbackTransaction(const QString &id);
    Response commitTransaction(const QString &id);

    //Functions that check if an ID already exists
    bool exists(const QString &table, const QString &pk, const QString &value);
    bool exists(const QString &table, const QString &pk, ident value);

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
    QSqlDatabase _db;

    //The database ID
    QString _id;

    //The encryption status
    Encryption _encryption;

    //The transaction ID
    QString _transactionId;
};

#endif // LOCALDB_H
