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


#ifndef INTERPRETATIONTOREQUESTXMLEXPORT_H
#define INTERPRETATIONTOREQUESTXMLEXPORT_H

#include <QString>
#include <QXmlStreamWriter>

namespace Tucuxi {
namespace Gui {
namespace Core {
class ActiveSubstance;
class DosageHistory;
class CoreMeasureList;
class PatientVariateList;
}
}
}

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Interpretation;
class ClinicalSet;
class Patient;

class InterpretationToRequestXmlExport
{
public:
    InterpretationToRequestXmlExport();

    QString toXml(Interpretation *interpretation);

protected:

    bool saveActiveSubstance(Tucuxi::Gui::Core::ActiveSubstance *activeSubstance);
    bool saveDosages(Tucuxi::Gui::Core::DosageHistory *dosageHistory);
    bool saveSamples(Tucuxi::Gui::Core::CoreMeasureList *samples);
    bool saveCovariates(Tucuxi::Gui::Core::PatientVariateList *covariates);
    bool saveClinicals(ClinicalSet *clinicals);
    bool savePatient(Patient *patient);

    QXmlStreamWriter writer;

};

}
}
}

#endif // INTERPRETATIONTOREQUESTXMLEXPORT_H
