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


#ifndef GUITOTUCUCORETRANSLATOR_H
#define GUITOTUCUCORETRANSLATOR_H

#include <QDateTime>
#include <QString>

#include "core/interfaces/processingtraits.h"

#include "tucucommon/datetime.h"

#include "tucucore/drugdefinitions.h"
#include "tucucore/computingservice/computingtrait.h"

namespace Tucuxi {
namespace Core {
class DrugTreatment;
class DrugModel;
class DosageTimeRange;
}
}

namespace Tucuxi {
namespace Gui {
namespace Core {

class DrugTreatment;
class DrugModel;
class ParamTraits;
class Dosage;
}
}
}


class GuiToTucucoreTranslator
{
public:
    GuiToTucucoreTranslator() = default;

    Tucuxi::Common::DateTime buildDateTime(const QDateTime &qDate);


    Tucuxi::Common::TucuUnit buildUnit(const QString &_strUnit);

    Tucuxi::Core::PredictionParameterType buildParameterType(const Tucuxi::Gui::Core::ParamTraits *traits);

    Tucuxi::Core::DosageTimeRange *buildTimeRange(const Tucuxi::Gui::Core::Dosage *guiDosage);

    Tucuxi::Core::DrugTreatment *buildTreatment(const Tucuxi::Gui::Core::DrugTreatment *guiTreatment, QDateTime adjTime = QDateTime());
    Tucuxi::Core::DrugModel *buildDrugModel(const Tucuxi::Gui::Core::DrugModel *guiDrugModel);

};



#endif // GUITOTUCUCORETRANSLATOR_H
