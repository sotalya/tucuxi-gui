//@@license@@

#ifndef GDATABASE
#define GDATABASE

#include <QString>
#include <QList>

#include "core.h"
#include "plugin.h"
#include "ezutils.h"
#include "user.h"
#include "version.h"
#include "duration.h"
#include "dal/genericdbobjects.h"
#include <QVariant>
#include "errorenums.h"
#include "genericuser.h"

// Convenience //
#include "patient.h"
#include "practician.h"
#include "clinical.h"
#include "curve.h"
#include "dosage.h"
#include "doses.h"
#include "drug.h"
#include "email.h"
#include "halflife.h"
#include "infusions.h"
#include "intervals.h"
#include "location.h"
#include "measure.h"
#include "operations.h"
#include "parameters.h"
#include "phone.h"
#include "posology.h"
#include "target.h"
#include "prediction.h"

namespace ezechiel {
//#define MODELUTILITIES(CLASSNAME) \

enum ComparisonOperator { AND_Equals, AND_GreaterThanOrEquals, AND_LessThanOrEquals, AND_GreaterThan, AND_LessThan, AND_NotEqual
                        , OR_Equals, OR_GreaterThanOrEquals, OR_LessThanOrEquals, OR_GreaterThan, OR_LessThan, OR_NotEqual };

typedef QHash<int, QPair<ComparisonOperator, QVariant>> WhereClauseMap;


typedef QPair<ComparisonOperator, QVariant> Comparison;

enum OrderOperator { Ascending, Descending };
typedef QHash<int, OrderOperator> OrderByClauseMap;

typedef QList<int> SelectClauseMap;

class IDatabase : public Plugin<IDatabase>
{
public:

    virtual ~IDatabase() = 0;

//    virtual Database* clone () = 0;

    virtual Descriptor descriptor () = 0;

//    int priority (const QString&);

//    QString errorString () = 0;

    virtual QList<QString> getOperatorString(const ComparisonOperator&) = 0;

    virtual Response getDBObjects (const QMetaObject&, QObject* parent) = 0;
    virtual Response getDBObjects (const QMetaObject&, QObject* parent, const SelectClauseMap&) = 0;
    virtual Response getDBObjects (const QMetaObject&, QObject* parent, const WhereClauseMap&) = 0;
    virtual Response getDBObjects (const QMetaObject&, QObject* parent, const SelectClauseMap&, const WhereClauseMap&) = 0;
    virtual Response getDBObjects (const QMetaObject&, QObject* parent, const WhereClauseMap&, const OrderByClauseMap&) = 0;
    virtual Response getDBObjects (const QMetaObject&, QObject* parent, const SelectClauseMap&, const WhereClauseMap&, const OrderByClauseMap&) = 0;

    virtual Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&) = 0;
    virtual Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&, const OrderByClauseMap&) = 0;
    virtual Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&, const WhereClauseMap&) = 0;
    virtual Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&, const WhereClauseMap&, const OrderByClauseMap&) = 0;
    virtual Response getDBObjectsOfDBObject(const QMetaObject&, const QMetaObject&, QObject* parent, const WhereClauseMap&, const WhereClauseMap&, const OrderByClauseMap&, const OrderByClauseMap&) = 0;

    virtual QList<QVariant> getDBData(const QMetaObject&, QObject* parent, const SelectClauseMap&) = 0;
    virtual QList<QVariant> getDBData(const QMetaObject&, QObject* parent, const SelectClauseMap&, const WhereClauseMap&) = 0;
    virtual QList<QVariant> getDBData(const QMetaObject&, QObject* parent, const SelectClauseMap&, const WhereClauseMap&, const OrderByClauseMap&) = 0;

    virtual Response setDBObjects (QList<QObject*>&, QObject* parent) = 0;
    virtual Response setDBObject (QObject&, QObject* parent) = 0;
    virtual Response updateDBObject (QObject&, QObject* parent, QString&, bool) = 0;
    virtual Response insertDBObject (QObject&, QObject* parent, QString&, bool, int) = 0;
    virtual Response deleteDBObjects (QList<QObject*>&, QObject* parent) = 0;
    virtual Response deleteDBObject (QObject&, QObject* parent) = 0;

//    virtual void setNext (DBInterface *next) = 0;
//    virtual void setList (QList<DBInterface*> plist) = 0;
//    virtual Descriptor<class Database> descriptor () = 0;
    virtual bool init (Core *core) = 0;
//    virtual QString errorString () = 0;
    virtual bool connect (DBAccount &account) = 0;
    virtual bool disconnect () = 0;
    virtual void needs (bool &url, bool &user, bool &password) = 0;

//    virtual Response isCompatible(bool &compatible) = 0;
    virtual Response isEncrypted(bool &encrypted) = 0;
    virtual Response setEncrypted(bool encrypted) = 0;
    virtual Response encryptDatabase(bool encrypt) = 0;

    // Convenience //

    virtual Response getPatientFromId(int, SharedPatient&) = 0;
    virtual Response setPatient(SharedPatient&) = 0;
    virtual Response getPatientsList(QList<SharedPatient>&) = 0;
    virtual Response deletePatient(SharedPatient&) = 0;

    virtual Response getPracticianFromId(int, SharedPractician&) = 0;
    virtual Response setPractician(SharedPractician&) = 0;
    virtual Response getPracticiansList(QList<SharedPractician>&) = 0;
    virtual Response deletePractician(SharedPractician&) = 0;
    virtual Response deletePractician(int) = 0;

    virtual Response getInstituteFromId(int, SharedInstitute&) = 0;
    virtual Response setInstitute(SharedInstitute&) = 0;
    virtual Response getInstitutesList(QList<SharedInstitute>&) = 0;
    virtual Response deleteInstitute(SharedInstitute&) = 0;
    virtual Response deleteInstitute(int) = 0;

    virtual Response getClinicalFromId(int, SharedClinical&) = 0;
    virtual Response setClinical(SharedClinical&) = 0;
    virtual Response getClinicalsList(QList<SharedClinical>&) = 0;
    virtual Response getClinicalsByDate(int, QList<SharedClinical>&) = 0;
    virtual Response deleteClinical(SharedClinical&) = 0;
    virtual Response deleteClinical(int) = 0;

    virtual Response getPatientVariateFromId(int, SharedPatientVariate &) = 0;
    virtual Response setPatientVariate(SharedPatientVariate &) = 0;
    virtual Response setPatientVariate(int, SharedPatientVariate &) = 0;
    virtual Response getPatientVariatesList(QList<SharedPatientVariate>&) = 0;
    virtual Response getPatientVariatesByDate(int, QList<SharedPatientVariate>&) = 0;
    virtual Response deletePatientVariate(SharedPatientVariate&) = 0;
    virtual Response deletePatientVariate(int) = 0;

    virtual Response setDrugVariate(int, SharedDrugVariate &) = 0;

    virtual Response getCurveFromId(int, SharedCurve &) = 0;
    virtual Response setCurve(SharedCurve &) = 0;
    virtual Response getCurvesList(QList<SharedCurve>&) = 0;
    virtual Response getCurvesList(QList<int>&, QStringList&) = 0;
    virtual Response getCurvesList(QList<int>&, const ident&, const QString&) = 0;
    virtual Response getCurvesList(QList<SharedCurve>&, const ident&, const QString&) = 0;
    virtual Response deleteCurve(SharedCurve&) = 0;
    virtual Response deleteCurve(int) = 0;

    virtual Response getPredictionFromId(int, SharedPrediction &) = 0;
    virtual Response setPrediction(SharedPrediction &) = 0;
    virtual Response getPredictionsList(QList<SharedPrediction>&) = 0;
    virtual Response getPredictionsList(QList<int>&, QStringList&) = 0;
    virtual Response getPredictionsList(QList<int>&, const ident&, const QString&) = 0;
    virtual Response getPredictionsList(QList<SharedPrediction>&, const ident&, const QString&) = 0;
    virtual Response deletePrediction(SharedPrediction&) = 0;
    virtual Response deletePrediction(int) = 0;

    virtual Response getDosageFromId(int, SharedDosage &) = 0;
    virtual Response setDosage(SharedDosage &) = 0;
    virtual Response getDosagesList(SharedDosageHistory&) = 0;
    virtual Response getDosagesByDate(int, SharedDosageHistory&) = 0;
    virtual Response deleteDosage(SharedDosage&) = 0;
    virtual Response deleteDosage(int) = 0;

    virtual Response getDrugFromId(int, SharedDrug &) = 0;
    virtual Response setDrug(SharedDrug &) = 0;
    virtual Response getDrugsList(QList<SharedDrug>&) = 0;
    virtual Response deleteDrug(SharedDrug&) = 0;

    virtual Response setValidDoses(int, SharedValidDoses&) = 0;

    virtual Response setValidIntervals(int, SharedValidIntervals&) = 0;

    virtual Response setValidInfusions(int, SharedValidInfusions&) = 0;

    virtual Response getEmailFromId(int, SharedEmail &) = 0;
    virtual Response setEmail(SharedEmail &) = 0;
    virtual Response getEmailsList(QList<SharedEmail>&) = 0;
    virtual Response deleteEmail(SharedEmail&) = 0;

//    virtual Response getInfusionsFromId(int, SharedValidInfusions &) = 0;
//    virtual Response setInfusions(SharedValidInfusions &) = 0;
//    virtual Response getInfusionssList(QList<SharedValidInfusions>&) = 0;
//    virtual Response deleteInfusions(SharedValidInfusions&) = 0;

//    virtual Response getIntervalsFromId(int, SharedValidIntervals &) = 0;
//    virtual Response setIntervals(SharedValidIntervals &) = 0;
//    virtual Response getIntervalssList(QList<SharedValidIntervals>&) = 0;
//    virtual Response deleteIntervals(SharedValidIntervals&) = 0;

    virtual Response getMeasureFromId(int, SharedMeasure &) = 0;
    virtual Response setMeasure(SharedMeasure &) = 0;
    virtual Response setMeasure(int, SharedMeasure &) = 0;
    virtual Response getMeasuresList(QList<SharedMeasure>&) = 0;
    virtual Response deleteMeasure(SharedMeasure&) = 0;
    virtual Response deleteMeasure(int) = 0;
    virtual Response getMeasuresFromPatientAndDrug(const int&, const QString&, QList<SharedMeasure>&) = 0;
    virtual Response getMeasuresByDate(int, QList<SharedMeasure>&) = 0;    

    virtual Response getOperationFromId(int, SharedOperation &) = 0;
    virtual Response setOperation(SharedOperation &) = 0;
    virtual Response getOperationsList(QList<SharedOperation>&) = 0;
    virtual Response deleteOperation(SharedOperation&) = 0;

    virtual Response getParameterFromId(int, SharedParameter &) = 0;
    virtual Response setParameter(SharedParameter &) = 0;
    virtual Response getParametersList(QList<SharedParameter>&) = 0;
    virtual Response deleteParameter(SharedParameter&) = 0;

    virtual Response getPhoneFromId(int, SharedPhone &) = 0;
    virtual Response setPhone(SharedPhone &) = 0;
    virtual Response getPhonesList(QList<SharedPhone>&) = 0;
    virtual Response deletePhone(SharedPhone&) = 0;

    virtual Response getPosologyFromId(int, SharedPosology &) = 0;
    virtual Response setPosology(SharedPosology &) = 0;
    virtual Response getPosologysList(QList<SharedPosology>&) = 0;
    virtual Response deletePosology(SharedPosology&) = 0;

    virtual Response getTargetFromId(int, SharedTarget &) = 0;
    virtual Response setTarget(SharedTarget &) = 0;
    virtual Response getTargetsList(QList<SharedTarget>&) = 0;
    virtual Response deleteTarget(SharedTarget&) = 0;

    virtual Response getLocationFromId(int, SharedLocation &) = 0;
    virtual Response setLocation(SharedLocation &) = 0;
    virtual Response getLocationsList(QList<SharedLocation>&) = 0;
    virtual Response deleteLocation(SharedLocation&) = 0;
//protected:

//    QString _errors;

//    QList<Option> _options;

//    int _priority;

};
}

//enabling WhereClauseMap as QVariant value means we can have nesting with parentheses (e.g. 'AND (X OR Y)')
//To nest, use '-1' as key value
Q_DECLARE_METATYPE(ezechiel::WhereClauseMap)
Q_DECLARE_INTERFACE(ezechiel::IDatabase, "ch.heig-vd.ezechiel.IDatabase/0.1")
#endif // GDATABASE

