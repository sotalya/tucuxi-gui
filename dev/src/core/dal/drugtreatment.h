#ifndef DRUGTREATMENT_H
#define DRUGTREATMENT_H

#include <QDateTime>

#include "core/dal/entity.h"
#include "core/utils/units.h"

namespace ezechiel {
namespace core {

class CorePatient;
class ActiveSubstance;
class DosageHistory;
class PatientVariate;
class PatientVariateList;
class CoreMeasure;
class Target;
class TargetList;
class CoreMeasureList;

class DrugTreatment : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(DrugTreatment)

    AUTO_PROPERTY_DECL(QString, name, Name)
    AUTO_PROPERTY_DECL(QString, activeSubstanceId, ActiveSubstanceId)
    AUTO_PROPERTY_DECL(CorePatient*, patient, Patient)
//    AUTO_PROPERTY_DECL(ActiveSubstance*, drug, Drug)
    AUTO_PROPERTY_DECL(DosageHistory*, dosages, Dosages)
    AUTO_PROPERTY_DECL(DosageHistory*, adjustments, Adjustments)
    AUTO_PROPERTY_DECL(PatientVariateList*, covariates, Covariates)
    AUTO_PROPERTY_DECL(CoreMeasureList*, measures, Measures)
    AUTO_PROPERTY_DECL(TargetList*, targets, Targets)


    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

//    Q_PROPERTY(QList<PatientVariate*> *covariates READ covariates WRITE setCovariates NOTIFY covariatesChanged)

protected:

    Q_INVOKABLE DrugTreatment(AbstractRepository *repository = nullptr, QObject* parent = nullptr);

public:

//    QList<PatientVariate*> *covariates() {return &_covariates;}
//    void setCovariates(QList<PatientVariate*> *covariates) {_covariates = *covariates;}

//    QList<CoreMeasure*> *measures() {return &_measures;}
//    void setCoreMeasures(QList<CoreMeasure*> *measures) {_measures = *measures;}

//    QList<Target*> *targets() {return &_targets;}
//    void setTargets(QList<Target*> *targets) {_targets = *targets;}

    int defaultRouteOfIntake();

//    Unit doseUnit() const;

//    Unit concentrationUnit() const;

//signals:
//    void covariatesChanged(QList<PatientVariate*>*);
//    void measuresModified(QList<CoreMeasure*>*);

protected:
//    QList<PatientVariate*> _covariates;
//    QList<CoreMeasure*> _measures;
//    QList<Target*> _targets;

private:    
    QDateTime _firstTake;
};



} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::DrugTreatment*)
Q_DECLARE_METATYPE(QList<ezechiel::core::DrugTreatment*>)
#endif // DRUGTREATMENT_H
