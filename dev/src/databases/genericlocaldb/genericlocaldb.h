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


#ifndef LOCALDB_H
#define LOCALDB_H

#include <QSqlDatabase>

#include "genericdatabase.h"
#include "pluginfactory.h"
#include "genericuser.h"
#include "plugin.h"

#include <QObject>
#include <QMetaProperty>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Tucuxi {


//class GenericLocalDBFactory : public Factory
//{
//    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "genericlocaldb")
//    Q_INTERFACES(Tucuxi::Factory)
//public:
//    void init(Core *core);
//    PluginBase* newPlugin (QString& id);
//    Descriptor descriptor ();
//    PluginBase::PluginType ptype() {
//        return PluginBase::PluginType::database;
//    }
//    int priority(const QString &id) const;

//private:
//    Descriptor desc;
//};

class GenericLocalDB : public IDatabase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "genericlocaldb")
    Q_INTERFACES(Tucuxi::IDatabase)
public:

    GenericLocalDB ();
    virtual ~GenericLocalDB ();
//    int priority(const QString&) const;
//    Database* clone();
    //all the custom types we create
    QList<int> user_def_types;
    Descriptor descriptor ();
    virtual QString databaseName() const;    
    bool init (Core *core);
//    QString errorString ();
    bool connect (DBAccount &account);
    bool disconnect ();
    void needs (bool &url, bool &user, bool &password);    

    void visit(CurveManager &);

    QList<QString> getOperatorString(const ComparisonOperator &);

    Response getDBObjects (const QMetaObject&, QObject* parent);
    Response getDBObjects (const QMetaObject&, QObject* parent, const SelectClauseMap&);
    Response getDBObjects (const QMetaObject&, QObject* parent, const WhereClauseMap&);
    Response getDBObjects (const QMetaObject&, QObject* parent, const SelectClauseMap&, const WhereClauseMap&);
    Response getDBObjects(const QMetaObject&, QObject* parent, const WhereClauseMap&, const OrderByClauseMap&);
    Response getDBObjects (const QMetaObject&, QObject* parent, const SelectClauseMap&, const WhereClauseMap&, const OrderByClauseMap&);

    //Although generally not needed when we need to get specific data fields, we can use this method which will not create any dbobjects.
    //It basically does the same thing as getObjects except it doesnt create or return any qobjects and the User is unaffected.
    QList<QVariant> getDBData(const QMetaObject&, QObject* parent, const SelectClauseMap&);
    QList<QVariant> getDBData(const QMetaObject&, QObject* parent, const SelectClauseMap&, const WhereClauseMap&);
    QList<QVariant> getDBData(const QMetaObject&, QObject* parent, const SelectClauseMap&, const WhereClauseMap&, const OrderByClauseMap&);

    //After some testing, it seemed like it would help to be able to do n-1 gets (e.g. get all the Curves with Drug x)
    //This method does a join and

    Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&);
    Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&, const OrderByClauseMap&);
    Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&, const WhereClauseMap&);
    Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&, const WhereClauseMap&, const OrderByClauseMap&);
    Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&, const WhereClauseMap&, const OrderByClauseMap&, const OrderByClauseMap&);

    Response setDBObjects (QList<QObject*>&, QObject* parent);
    Response setDBObject (QObject&, QObject* parent, QString&);
    Response setDBObject (QObject&, QObject* parent);
    Response updateDBObject (QObject&, QObject* parent, QString&, bool);
    Response insertDBObject (QObject&, QObject* parent, QString&, bool, int);
    Response deleteDBObject (QObject&, QObject* parent);
    Response deleteDBObjects (QList<QObject*>&, QObject* parent);

    QString whereBuilder(const QMetaObject&, const WhereClauseMap&);
//    virtual Tucuxi::Response getVersion(QString &version);
//    virtual Tucuxi::Response isCompatible(bool &compatible);

    virtual Response isEncrypted(bool &encrypted);
    virtual Response setEncrypted(bool encrypted);
    virtual Response encryptDatabase(bool encrypt);

    /// Convenience Classes ///
    Response getPatientFromId(int _id, SharedPatient&);
    Response setPatient(SharedPatient& _p);
    Response getPatientsList(QList<SharedPatient>&);
    Response deletePatient(SharedPatient&);

    Response getPracticianFromId(int, SharedPractician &);
    Response setPractician(SharedPractician &);
    Response getPracticiansList(QList<SharedPractician>&);
    Response deletePractician(SharedPractician&);
    Response deletePractician(int);

    Response getInstituteFromId(int, SharedInstitute &);
    Response setInstitute(SharedInstitute &);
    Response getInstitutesList(QList<SharedInstitute>&);
    Response deleteInstitute(SharedInstitute&);
    Response deleteInstitute(int);

    Response getClinicalFromId(int, SharedClinical &);
    Response setClinical(SharedClinical &);
    Response getClinicalsList(QList<SharedClinical>&);
    Response getClinicalsByDate(int, QList<SharedClinical>&);
    Response deleteClinical(SharedClinical&);
    Response deleteClinical(int);

    Response getPatientVariateFromId(int, SharedPatientVariate &);
    Response setPatientVariate(SharedPatientVariate &);
    Response setPatientVariate(int, SharedPatientVariate &);
    Response getPatientVariatesList(QList<SharedPatientVariate>&);
    Response getPatientVariatesByDate(int, QList<SharedPatientVariate>&);
    Response deletePatientVariate(SharedPatientVariate&);
    Response deletePatientVariate(int);

    Response getDrugVariateFromId(int, SharedDrugVariate &);
    Response setDrugVariate(int, SharedDrugVariate &);
    Response getDrugVariatesList(QList<SharedDrugVariate>&);
    Response getDrugVariatesByDate(int, QList<SharedDrugVariate>&);
    Response deleteDrugVariate(SharedDrugVariate&);
    Response deleteDrugVariate(int);

    Response getCurveFromId(int, SharedCurve &);
    Response setCurve(SharedCurve &);
    Response getCurvesList(QList<SharedCurve>&);
    Response getCurvesList(QList<int>&, QStringList&);
    Response getCurvesList(QList<int>&, const ident&, const QString&);
    Response getCurvesList(QList<SharedCurve>&, const ident&, const QString&);
    Response deleteCurve(SharedCurve&);
    Response deleteCurve(int);

    Response getPredictionFromId(int, SharedPrediction &);
    Response setPrediction(SharedPrediction &);
    Response getPredictionsList(QList<SharedPrediction>&);
    Response getPredictionsList(QList<int>&, QStringList&);
    Response getPredictionsList(QList<int>&, const ident&, const QString&);
    Response getPredictionsList(QList<SharedPrediction>&, const ident&, const QString&);
    Response deletePrediction(SharedPrediction&);
    Response deletePrediction(int);

    Response getDosageFromId(int, SharedDosage &);
    Response setDosage(SharedDosage &);
    Response getDosagesList(SharedDosageHistory&);
    Response getDosagesByDate(int, SharedDosageHistory&);
    Response deleteDosage(SharedDosage&);
    Response deleteDosage(int);

    Response getDrugFromId(int, SharedDrug &);
    Response setDrug(SharedDrug &);
    Response getDrugsList(QList<SharedDrug>&);
    Response deleteDrug(SharedDrug&);

    Response setValidDoses(int, SharedValidDoses &);

    Response setValidIntervals(int, SharedValidIntervals&);

    Response setValidInfusions(int, SharedValidInfusions&);

    Response getEmailFromId(int, SharedEmail &);
    Response setEmail(SharedEmail &);
    Response getEmailsList(QList<SharedEmail>&);
    Response deleteEmail(SharedEmail&);

//    Response getInfusionsFromId(int, SharedValidInfusions &);
//    Response setInfusions(SharedValidInfusions &);
//    Response getInfusionssList(QList<SharedValidInfusions>&);
//    Response deleteInfusions(SharedValidInfusions&);

//    Response getIntervalsFromId(int, SharedValidIntervals &);
//    Response setIntervals(SharedValidIntervals &);
//    Response getIntervalssList(QList<SharedValidIntervals>&);
//    Response deleteIntervals(SharedValidIntervals&);

    Response getMeasureFromId(int, SharedMeasure &);
    Response setMeasure(SharedMeasure &);
    Response setMeasure(int, SharedMeasure &);
    Response getMeasuresList(QList<SharedMeasure>&);
    Response deleteMeasure(SharedMeasure&);
    Response deleteMeasure(int);
    Response getMeasuresFromPatientAndDrug(const int&, const QString&, QList<SharedMeasure>&);
    Response getMeasuresByDate(int, QList<SharedMeasure>&);

    Response getOperationFromId(int, SharedOperation &);
    Response setOperation(SharedOperation &);
    Response getOperationsList(QList<SharedOperation>&);
    Response deleteOperation(SharedOperation&);

    Response getParameterFromId(int, SharedParameter &);
    Response setParameter(SharedParameter &);
    Response getParametersList(QList<SharedParameter>&);
    Response deleteParameter(SharedParameter&);

    Response getPhoneFromId(int, SharedPhone &);
    Response setPhone(SharedPhone &);
    Response getPhonesList(QList<SharedPhone>&);
    Response deletePhone(SharedPhone&);

    Response getPosologyFromId(int, SharedPosology &);
    Response setPosology(SharedPosology &);
    Response getPosologysList(QList<SharedPosology>&);
    Response deletePosology(SharedPosology&);

    Response getTargetFromId(int, SharedTarget &);
    Response setTarget(SharedTarget &);
    Response getTargetsList(QList<SharedTarget>&);
    Response deleteTarget(SharedTarget&);


    Response getLocationFromId(int, SharedLocation &);
    Response setLocation(SharedLocation &);
    Response getLocationsList(QList<SharedLocation>&);
    Response deleteLocation(SharedLocation&);

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
    int findOrCreate(const QString &id, const QString &table);

    //Functions that returns a formated error
    Response queryError(const QSqlQuery &query) const;
    Response databaseError(const QSqlDatabase &database, const QString &message) const;

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
    QSqlDatabase _db;

    //The database ID
    QString _id;

    //The encryption status
    Encryption _encryption;

    //The transaction ID
    QString _transactionId;        
};

}

#endif // LOCALDB_H
