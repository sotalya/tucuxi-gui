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


#ifndef DATAXMLEXPORT_H
#define DATAXMLEXPORT_H


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

class DataXmlExport
{
public:
    DataXmlExport();

    QString toXml(Interpretation *interpretation);
    QString toCdssXml(Interpretation *interpretation);

protected:
    QString writeDate(QDateTime date);
    bool save(Tucuxi::Gui::Core::DrugResponseAnalysis *drugResponseAnalysis);
    bool save(Tucuxi::Gui::Core::DrugTreatment *treatment);
    bool save(Tucuxi::Gui::Core::PatientVariateList *list);
    bool save(Tucuxi::Gui::Core::CoreMeasureList *list);
    bool save(Tucuxi::Gui::Core::ActiveSubstance *substance);
    bool save(Patient *patient);
    bool save(Core::Dosage *dosage);
    bool save(Tucuxi::Gui::Core::DosageHistory *history);
    bool saveIdentifiableAmount(const QString &tagName, Tucuxi::Gui::Core::IdentifiableAmount *amount);
    bool saveAdminData(Interpretation *interpretation);

    QXmlStreamWriter writer;

};

}
}
}

#endif // DATAXMLEXPORT_H
