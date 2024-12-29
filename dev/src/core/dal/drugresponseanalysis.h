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


#ifndef DRUGRESPONSEANALYSIS_H
#define DRUGRESPONSEANALYSIS_H

#include "core/dal/entity.h"

Q_MOC_INCLUDE("core/dal/drug/drug.h")
Q_MOC_INCLUDE("core/dal/drugtreatment.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class DrugTreatment;
class DrugModel;

class DrugResponseAnalysis : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(DrugResponseAnalysis)

    AUTO_PROPERTY_DECL(DrugTreatment*, treatment, Treatment)
    AUTO_PROPERTY_DECL(DrugModel*, drugModel, DrugModel)


protected:

    Q_INVOKABLE DrugResponseAnalysis(AbstractRepository *repository = nullptr, QObject* parent = nullptr);

protected:

};


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DrugResponseAnalysis*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::DrugResponseAnalysis*>)

#endif // DRUGRESPONSEANALYSIS_H
