#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QList>

#include "core/dal/covariate.h"
#include "core/dal/prediction.h"
#include "core/interfaces/abstractrepository.h"
#include "core/utils/errorenums.h"


namespace ezechiel {
namespace core {

typedef int ident;
// This class should be a superclass of all the aggregates listed below
class Aggregate;

class DrugModel;
class MedicalTreatment;
class TreatmentAnalysis;

class DrugVariate;
class Dosage;
class ValidDoses;
class ValidInfusions;
class ValidIntervals;
class CorePatient;
class CoreMeasure;
class DosageHistory;
class Operation;
class Target;
class TargetList;

class ShortPatient
{
public:
    int id;
    QString lastname;
    QString firstname;
};

class CoreRepository : /*public QObject,*/ public AbstractRepository
{
public:
//    explicit Repository(QObject *parent = nullptr);

    //! \brief Error string
    /** Will always responds that DBPlugin cannot connect or disconnect as it is a dead-end.
      */
    virtual QString errorString () = 0;

//    virtual Response getPatientVariate(int, SharedPatientVariate &) = 0;
//    virtual Response setPatientVariate(SharedPatientVariate &) = 0;
//    virtual Response setPatientVariate(int, SharedPatientVariate &) = 0;
//    virtual Response getPatientVariatesList(QList<SharedPatientVariate>&) = 0;
//    virtual Response getPatientVariatesByDate(int, QList<SharedPatientVariate>&) = 0;
//    virtual Response deletePatientVariate(SharedPatientVariate&) = 0;
//    virtual Response deletePatientVariate(int) = 0;

    virtual Response getDrugVariateFromId(const int&, DrugVariate*) = 0;
    virtual Response setDrugVariate(const int&, DrugVariate*) = 0;
    virtual Response getDrugVariatesList(QList<DrugVariate*>&) = 0;
    virtual Response getDrugVariatesOfDrug(const int&, QList<DrugVariate*>&) = 0;
    virtual Response getDrugVariatesByDate(const int&, QList<DrugVariate*>&) = 0;
    virtual Response deleteDrugVariate(DrugVariate*) = 0;
    virtual Response deleteDrugVariate(const int&) = 0;

//    virtual Response getCurve(int, SharedCurve &) = 0;
//    virtual Response setCurve(SharedCurve &) = 0;
//    virtual Response getCurvesList(QList<SharedCurve>&) = 0;
//    virtual Response getCurvesList(QList<int>&, QStringList&) = 0;
//    virtual Response getCurvesList(QList<int>&, const ident&, const QString&) = 0;
//    virtual Response getCurvesList(QList<SharedCurve>&, const ident&, const QString&) = 0;
//    virtual Response deleteCurve(SharedCurve&) = 0;
//    virtual Response deleteCurve(int) = 0;

    virtual Response getPredictionFromId(const int&, Prediction* ) = 0;
    virtual Response setPrediction(Prediction* ) = 0;
    virtual Response getPredictionsList(QList<Prediction*>&) = 0;
    virtual Response getPredictionsList(const int&, QList<Prediction*>&) = 0;
    virtual Response getPredictionsList(QList<int>&, QStringList&) = 0;
    virtual Response getPredictionsList(QList<int>&, const ident&, const QString&) = 0;
    virtual Response getPredictionsList(QList<Prediction*>&, const ident&, const QString&) = 0;
    virtual Response deletePrediction(Prediction*) = 0;
    virtual Response deletePrediction(const int&) = 0;

    virtual Response getDrugResponseAnalysisFromId(const int&, DrugResponseAnalysis*&) = 0;
    virtual Response setDrugResponseAnalysis(DrugResponseAnalysis*) = 0;
//    virtual Response getDrugResponseAnalysesList(QList<SharedDrugResponseAnalysis>&) = 0;
//    virtual Response getDrugResponseAnalysesList(const int&, QList<SharedDrugResponseAnalysis>&) = 0;
//    virtual Response getDrugResponseAnalysesList(QList<int>&, QStringList&) = 0;
//    virtual Response getDrugResponseAnalysesList(QList<int>&, const ident&, const QString&) = 0;
//    virtual Response getDrugResponseAnalysesList(QList<SharedDrugResponseAnalysis>&, const ident&, const QString&) = 0;
//    virtual Response deleteDrugResponseAnalysis(SharedDrugResponseAnalysis&) = 0;
//    virtual Response deleteDrugResponseAnalysis(const int&) = 0;

    virtual Response getDosageFromId(const int&, Dosage* &) = 0;
    virtual Response setDosage(Dosage* &) = 0;
    virtual Response getDosagesList(DosageHistory*) = 0;
    virtual Response getDosagesByDate(const int&, DosageHistory*) = 0;
    virtual Response deleteDosage(Dosage*&) = 0;
    virtual Response deleteDosage(const int&) = 0;
//    virtual Response getDosagesOfPatientAndDrug(const int&, const int&, DosageHistory*) = 0;
//    virtual Response getDosagesTakenOfPatientAndDrug(const int&, const int&, DosageHistory*) = 0;
//    virtual Response getDosagesTakenOfPatientAndDrugAndOfPrediction(const int&, const int&, const int&, DosageHistory*) = 0;
//    virtual Response getDosagesOfPred(const int&, DosageHistory*) = 0;

    virtual Response getValidDosesOfDrug(const int &, ValidDoses* ) = 0;
    virtual Response getValidInfusionsOfDrug(const int &, ValidInfusions* ) = 0;
    virtual Response getValidIntervalsOfDrug(const int &, ValidIntervals* ) = 0;

//    virtual Response getDrug(int, SharedDrug &) = 0;
//    virtual Response setDrug(SharedDrug &) = 0;
//    virtual Response getDrugsList(QList<SharedDrug>&) = 0;
//    virtual Response deleteDrug(SharedDrug&) = 0;

//    virtual Response setValidDoses(int, SharedValidDoses&) = 0;

//    virtual Response setValidIntervals(int, SharedValidIntervals&) = 0;

//    virtual Response setValidInfusions(int, SharedValidInfusions&) = 0;

    /*
    virtual Response getEmail(int, SharedEmail &) = 0;
    virtual Response setEmail(SharedEmail &) = 0;
    virtual Response getEmailsList(QList<SharedEmail>&) = 0;
    virtual Response deleteEmail(SharedEmail&) = 0;

    virtual Response getPhone(int, SharedPhone &) = 0;
    virtual Response setPhone(SharedPhone &) = 0;
    virtual Response getPhonesList(QList<SharedPhone>&) = 0;
    virtual Response deletePhone(SharedPhone&) = 0;

    virtual Response getLocation(int, SharedLocation &) = 0;
    virtual Response setLocation(SharedLocation &) = 0;
    virtual Response getLocationsList(QList<SharedLocation>&) = 0;
    virtual Response deleteLocation(SharedLocation&) = 0;
*/
//    virtual Response getInfusions(int, SharedValidInfusions &) = 0;
//    virtual Response setInfusions(SharedValidInfusions &) = 0;
//    virtual Response getInfusionssList(QList<SharedValidInfusions>&) = 0;
//    virtual Response deleteInfusions(SharedValidInfusions&) = 0;

//    virtual Response getIntervals(int, SharedValidIntervals &) = 0;
//    virtual Response setIntervals(SharedValidIntervals &) = 0;
//    virtual Response getIntervalssList(QList<SharedValidIntervals>&) = 0;
//    virtual Response deleteIntervals(SharedValidIntervals&) = 0;

    virtual Response getCorePatient(const int&, CorePatient*) = 0;

    virtual Response getCoreMeasure(const int&, CoreMeasure*) = 0;
    virtual Response setCoreMeasure(CoreMeasure*) = 0;
//    virtual Response setCoreMeasure(const int&, SharedCoreMeasure &) = 0;
    virtual Response getCoreMeasuresList(QList<CoreMeasure*>&) = 0;
    virtual Response deleteCoreMeasure(CoreMeasure*) = 0;
    virtual Response deleteCoreMeasure(const int&) = 0;
    virtual Response getCoreMeasuresFromPatientAndDrug(const int&, const QString&, QList<CoreMeasure*>&) = 0;
    virtual Response getCoreMeasuresFromPatientAndDrug(const int&, const int&, QList<CoreMeasure*>&) = 0;
    virtual Response getCoreMeasuresByDate(const int&, QList<CoreMeasure*>&) = 0;

    virtual Response getOperationFromId(const int&, Operation* ) = 0;
    virtual Response setOperation(Operation* ) = 0;
    virtual Response getOperationsList(QList<Operation*>&) = 0;
    virtual Response deleteOperation(Operation*) = 0;

    virtual Response getParameterFromId(const int&, SharedParameter &) = 0;
    virtual Response setParameter(SharedParameter &) = 0;
    virtual Response getParametersList(QList<SharedParameter>&) = 0;
    virtual Response deleteParameter(SharedParameter&) = 0;

//    virtual Response getPosology(int, SharedPosology &) = 0;
//    virtual Response setPosology(SharedPosology &) = 0;
//    virtual Response getPosologysList(QList<SharedPosology>&) = 0;
//    virtual Response deletePosology(SharedPosology&) = 0;

    virtual Response getTargetFromId(const int&, Target* ) = 0;
    virtual Response setTarget(Target* ) = 0;
    virtual Response getTargetsList(QList<Target*>&) = 0;
    virtual Response deleteTarget(Target*) = 0;

    virtual Response getTargetsOfDrug(const int & , TargetList* ) = 0;

/*
 *
    // The following methods represent a subset of the old database interface
    // I think some things are missing, for sure, but that presents the idea

    virtual Response save(Patient *patient) = 0;
    virtual Response save(Practician *practician) = 0;
    virtual Response save(Institute *institute) = 0;
    virtual Response save(Drug *drug) = 0;
    virtual Response save(MedicalTreatment *treatment) = 0;
    virtual Response save(TreatmentAnalysis *analysis) = 0;
    virtual Response save(ReportData *report) = 0;

    virtual Response deleteObject(Aggregate *aggregate) = 0;

    // We can us pointers to pointers so as to let the repository allocate the object
    virtual Response getPatientById(int id, Patient **patient) = 0;
    virtual Response getPracticianById(int id, Practician **practician) = 0;
    virtual Response getInstituteById(int id, Institute **institute) = 0;
    virtual Response getDrugById(int id, Drug **drug) = 0;
    virtual Response getMedicalTreatmentById(int id, MedicalTreatment **treatment) = 0;
    virtual Response getTreatmentAnalysisById(int id, TreatmentAnalysis **analysis) = 0;
    virtual Response getReportById(int id, ReportData **report) = 0;
    // returns a list of patients (not all their information, but only first name and last name
    virtual Response getPatientList(QString from,QString to, QList<ShortPatient> *list) = 0;
*/

};

} // namespace core
} // namespace ezechiel
Q_DECLARE_INTERFACE(ezechiel::core::CoreRepository, "ch.heig-vd.ezechiel.CoreInterface/0.1")

#endif // REPOSITORY_H
