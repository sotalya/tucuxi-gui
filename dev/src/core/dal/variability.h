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


#ifndef VARIABILITY_H
#define VARIABILITY_H

#include "core/dal/entity.h"
#include "core/dal/amount.h"
#include "core/utils/ezutils.h"

Q_MOC_INCLUDE("core/dal/drug/operation.h")
Q_MOC_INCLUDE("core/dal/drug/translatablestring.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class OperationList;
class TranslatableString;

class OperableDouble : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(OperableDouble)

    AUTO_PROPERTY_DECL(QString, valueId, ValueId)
    AUTO_PROPERTY_DECL(double, value, Value)
    AUTO_PROPERTY_DECL(OperationList*, operations, Operations)

    protected:
        Q_INVOKABLE OperableDouble(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) {}
};

POINTERLIST_CLASS_DECL(OperableDoubleList, OperableDouble)

class Variability : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(Variability)

    AUTO_PROPERTY_DECL(QString, distributionId, DistributionId)
    AUTO_PROPERTY_DECL(OperableDoubleList*, stdDeviations, StdDeviations)
    AUTO_PROPERTY_DECL(TranslatableString*,comments,Comments)


protected:
    Q_INVOKABLE Variability(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) {}
};


class VariableValue : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(VariableValue)

    AUTO_PROPERTY_DECL(QVariant, value, Value)
    AUTO_PROPERTY_DECL(Variability*, variability, Variability)

protected:
    Q_INVOKABLE VariableValue(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent) {}
};


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Variability*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::VariableValue*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::OperableDouble*)

#endif // VARIABILITY_H
