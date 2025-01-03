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


#ifndef DRUGTREATMENT_H
#define DRUGTREATMENT_H

#include <QDateTime>

#include "core/dal/entity.h"
#include "core/utils/units.h"

Q_MOC_INCLUDE("core/dal/coremeasure.h")
Q_MOC_INCLUDE("core/dal/corepatient.h")
Q_MOC_INCLUDE("core/dal/dosage.h")
Q_MOC_INCLUDE("core/dal/covariate.h")
Q_MOC_INCLUDE("core/dal/drug/target.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


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



} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DrugTreatment*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::DrugTreatment*>)
#endif // DRUGTREATMENT_H
