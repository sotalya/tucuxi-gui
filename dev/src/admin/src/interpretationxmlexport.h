/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef INTERPRETATIONXMLEXPORT_H
#define INTERPRETATIONXMLEXPORT_H


#include "admin/src/dal/validationstatus.h"
#include <QString>
#include <QXmlStreamWriter>

namespace Tucuxi {
namespace Gui {
namespace Core {

class ActiveSubstance;
class Dosage;
class AdjustmentDosage;
class DosageHistory;
class CoreMeasureList;
class PatientVariateList;
class DrugResponseAnalysis;
class DrugTreatment;
class TargetList;
class UncastedValueList;
class ADME;
class ValidDoses;
class ValidInfusions;
class ValidIntervals;
class DrugVariateList;
class ParameterSet;
class OperationList;
class IdentifiableAmount;
class OperableAmount;
}
}
}

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Interpretation;
class InterpretationAnalysis;
class InterpretationRequest;
class Practician;
class ValidationStatus;
class PhoneList;

class ClinicalSet;
class Patient;
class Person;

class InterpretationXmlExport
{
public:
    InterpretationXmlExport();

    QString toXml(Interpretation *interpretation);
    QString stepToString(int step);
    QString toStringValidation(ValidationStatus::ValidationStatusType status);

protected:

    bool save(InterpretationAnalysis *analysis);
    bool save(InterpretationRequest *request);
    bool save(Tucuxi::Gui::Core::DrugResponseAnalysis *drugResponseAnalysis);
    bool save(Tucuxi::Gui::Core::DrugTreatment *drugResponseAnalysis);
    bool save(Practician *practician, QString name);
    bool save(ValidationStatus *validationStatus);
    bool save(Patient *patient);
    bool save(Person *person, QString name);
    bool save(ClinicalSet *clinicals);
    bool save(PhoneList *list);


    QString writeDate(QDateTime date);
    bool save(Tucuxi::Gui::Core::Dosage *dosage, const QString &tagName = "dosage");
    bool save(Tucuxi::Gui::Core::AdjustmentDosage *dosage, const QString &tagName = "dosage");
    bool save(Tucuxi::Gui::Core::DosageHistory *history, const QString &tagName = "dosageHistory");
    bool save(Tucuxi::Gui::Core::PatientVariateList *list);
    bool save(Tucuxi::Gui::Core::TargetList *list);
    bool save(Tucuxi::Gui::Core::CoreMeasureList *list);
    bool save(Tucuxi::Gui::Core::UncastedValueList *list);
    bool save(Tucuxi::Gui::Core::ADME *adme);
    bool save(Tucuxi::Gui::Core::ValidDoses *doses);
    bool save(Tucuxi::Gui::Core::ValidInfusions *infusions);
    bool save(Tucuxi::Gui::Core::ValidIntervals *intervals);
    bool save(Tucuxi::Gui::Core::DrugVariateList *list);
    bool save(Tucuxi::Gui::Core::ParameterSet *set);
    bool save(Tucuxi::Gui::Core::OperationList *list);
    bool saveIdentifiableAmount(const QString &tagName, Tucuxi::Gui::Core::IdentifiableAmount *amount);
    bool saveOperableAmount(const QString &tagName, Tucuxi::Gui::Core::OperableAmount *amount);

    QXmlStreamWriter writer;

};

}
}
}

#endif // INTERPRETATIONXMLEXPORT_H
