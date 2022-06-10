//@@license@@

#ifndef GDATABASE
#define GDATABASE

#include <QString>
#include <QList>
#include <QVariant>

#include "plugin.h"
#include "apputils/src/user.h"
#include "core/utils/genericuser.h"

#include "core/core.h"
#include "core/utils/ezutils.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/halflife.h"
#include "core/dal/drug/infusions.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/coremeasure.h"
#include "core/dal/drug/parameters.h"
#include "core/dal/drug/target.h"
#include "core/dal/prediction.h"
#include "core/utils/version.h"
#include "core/utils/duration.h"
#include "core/utils/errorenums.h"

namespace Tucuxi {
namespace GuiCore {

class PatientVariate;


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

    /*
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
*/
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

    /*
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
*/
//    virtual Response getClinicalFromId(int, SharedClinical&) = 0;
//    virtual Response setClinical(SharedClinical&) = 0;
//    virtual Response getClinicalsList(QList<SharedClinical>&) = 0;
//    virtual Response getClinicalsByDate(int, QList<SharedClinical>&) = 0;
//    virtual Response deleteClinical(SharedClinical&) = 0;
//    virtual Response deleteClinical(int) = 0;

    virtual Response getPatientVariateFromId(int, PatientVariate* &) = 0;
    virtual Response setPatientVariate(PatientVariate* &) = 0;
    virtual Response setPatientVariate(int, PatientVariate* &) = 0;
    virtual Response getPatientVariatesList(QList<PatientVariate*>&) = 0;
    virtual Response getPatientVariatesByDate(int, QList<PatientVariate*>&) = 0;
    virtual Response deletePatientVariate(PatientVariate*&) = 0;
    virtual Response deletePatientVariate(int) = 0;

    virtual Response getDrugVariateFromId(int, DrugVariate* &) = 0;
    virtual Response setDrugVariate(int, DrugVariate* &) = 0;
    virtual Response getDrugVariatesList(QList<DrugVariate*>&) = 0;
    virtual Response getDrugVariatesByDate(int, QList<DrugVariate*>&) = 0;
    virtual Response deleteDrugVariate(DrugVariate*&) = 0;
    virtual Response deleteDrugVariate(int) = 0;

//    virtual Response getCurveFromId(int, SharedCurve &) = 0;
//    virtual Response setCurve(SharedCurve &) = 0;
//    virtual Response getCurvesList(QList<SharedCurve>&) = 0;
//    virtual Response getCurvesList(QList<int>&, QStringList&) = 0;
//    virtual Response getCurvesList(QList<int>&, const ident&, const QString&) = 0;
//    virtual Response getCurvesList(QList<SharedCurve>&, const ident&, const QString&) = 0;
//    virtual Response deleteCurve(SharedCurve&) = 0;
//    virtual Response deleteCurve(int) = 0;

    virtual Response getPredictionFromId(int, Prediction* ) = 0;
    virtual Response setPrediction(Prediction* ) = 0;
    virtual Response getPredictionsList(QList<Prediction*>&) = 0;
    virtual Response getPredictionsList(QList<int>&, QStringList&) = 0;
    virtual Response getPredictionsList(QList<int>&, const ident&, const QString&) = 0;
    virtual Response getPredictionsList(QList<Prediction*>&, const ident&, const QString&) = 0;
    virtual Response deletePrediction(Prediction*) = 0;
    virtual Response deletePrediction(int) = 0;

    virtual Response getDosageFromId(int, Dosage* &) = 0;
    virtual Response setDosage(Dosage* &) = 0;
    virtual Response getDosagesList(DosageHistory*&) = 0;
    virtual Response getDosagesByDate(int, DosageHistory*&) = 0;
    virtual Response deleteDosage(Dosage*&) = 0;
    virtual Response deleteDosage(int) = 0;

    virtual Response getDrugFromId(int, DrugModel* &) = 0;
    virtual Response setDrug(DrugModel* &) = 0;
    virtual Response getDrugsList(QList<DrugModel*>&) = 0;
    virtual Response deleteDrug(DrugModel*&) = 0;

    virtual Response setValidDoses(int, ValidDoses *&) = 0;

    virtual Response setValidIntervals(int, ValidIntervals*) = 0;

    virtual Response setValidInfusions(int, ValidInfusions*&) = 0;

    /*
    virtual Response getEmailFromId(int, SharedEmail &) = 0;
    virtual Response setEmail(SharedEmail &) = 0;
    virtual Response getEmailsList(QList<SharedEmail>&) = 0;
    virtual Response deleteEmail(SharedEmail&) = 0;

    virtual Response getPhoneFromId(int, SharedPhone &) = 0;
    virtual Response setPhone(SharedPhone &) = 0;
    virtual Response getPhonesList(QList<SharedPhone>&) = 0;
    virtual Response deletePhone(SharedPhone&) = 0;

    virtual Response getLocationFromId(int, SharedLocation &) = 0;
    virtual Response setLocation(SharedLocation &) = 0;
    virtual Response getLocationsList(QList<SharedLocation>&) = 0;
    virtual Response deleteLocation(SharedLocation&) = 0;
*/
//    virtual Response getInfusionsFromId(int, SharedValidInfusions &) = 0;
//    virtual Response setInfusions(SharedValidInfusions &) = 0;
//    virtual Response getInfusionssList(QList<SharedValidInfusions>&) = 0;
//    virtual Response deleteInfusions(SharedValidInfusions&) = 0;

//    virtual Response getIntervalsFromId(int, SharedValidIntervals &) = 0;
//    virtual Response setIntervals(SharedValidIntervals &) = 0;
//    virtual Response getIntervalssList(QList<SharedValidIntervals>&) = 0;
//    virtual Response deleteIntervals(SharedValidIntervals&) = 0;

    virtual Response getMeasureFromId(int, CoreMeasure* &) = 0;
    virtual Response setMeasure(CoreMeasure* &) = 0;
    virtual Response setMeasure(int, CoreMeasure* &) = 0;
    virtual Response getMeasuresList(QList<CoreMeasure*>&) = 0;
    virtual Response deleteMeasure(CoreMeasure*&) = 0;
    virtual Response deleteMeasure(int) = 0;
    virtual Response getMeasuresFromPatientAndDrug(const int&, const QString&, QList<CoreMeasure*>&) = 0;
    virtual Response getMeasuresByDate(int, QList<CoreMeasure*>&) = 0;

    virtual Response getParameterFromId(int, SharedParameter &) = 0;
    virtual Response setParameter(SharedParameter &) = 0;
    virtual Response getParametersList(QList<SharedParameter>&) = 0;
    virtual Response deleteParameter(SharedParameter&) = 0;

    virtual Response getTargetFromId(int, Target* ) = 0;
    virtual Response setTarget(Target* ) = 0;
    virtual Response getTargetsList(QList<Target*>&) = 0;
    virtual Response deleteTarget(Target*) = 0;

//protected:

//    QString _errors;

//    QList<Option> _options;

//    int _priority;

};


} // namespace core
} // namespace Tucuxi

//enabling WhereClauseMap as QVariant value means we can have nesting with parentheses (e.g. 'AND (X OR Y)')
//To nest, use '-1' as key value
Q_DECLARE_METATYPE(Tucuxi::GuiCore::WhereClauseMap)
Q_DECLARE_INTERFACE(Tucuxi::GuiCore::IDatabase, "ch.heig-vd.ezechiel.IDatabase/0.1")
#endif // GDATABASE

