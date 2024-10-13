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


#ifndef ERRORMODEL_H
#define ERRORMODEL_H

#include "core/dal/entity.h"
#include "core/utils/ezutils.h"

Q_MOC_INCLUDE("core/dal/drug/translatablestring.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class TranslatableString;

class ErrorModel : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(ErrorModel)

    //! The parameter's value standard variability
    AUTO_PROPERTY_DECL(double, additive, Additive)
    //! The parameter's value proportional variability
    AUTO_PROPERTY_DECL(double, proportional, Proportional)

    AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

protected:

    Q_INVOKABLE ErrorModel(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _additive(0.0), _proportional(0.0), _comments(0) {}
    Q_INVOKABLE ErrorModel(AbstractRepository *repository, QObject * parent, double additive, double proportional)
        : Entity(repository, parent), _additive(additive), _proportional(proportional) {}
};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::ErrorModel*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::ErrorModel*>)

#endif // ERRORMODEL_H
