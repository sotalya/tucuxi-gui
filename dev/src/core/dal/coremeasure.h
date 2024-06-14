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


#ifndef COREMEASURE_H
#define COREMEASURE_H

#include <QObject>

#include "core/dal/drug/operableamount.h"
#include "core/utils/ezutils.h"
#include "core/dal/entity.h"

Q_MOC_INCLUDE("core/dal/uncastedvalue.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class UncastedValueList;

class CoreMeasure : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(CoreMeasure)

    AUTO_PROPERTY_DECL(IdentifiableAmount*, concentration, Concentration)
    AUTO_PROPERTY_DECL(QDateTime, moment, Moment)
    AUTO_PROPERTY_DECL(QString, sdrug, Sdrug)
    AUTO_PROPERTY_DECL(bool, enable, Enable)

    AUTO_PROPERTY_DECL(UncastedValueList*, uncastedValues, UncastedValues)

protected:

    Q_INVOKABLE explicit CoreMeasure(AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE CoreMeasure(AbstractRepository *repository, QDateTime moment, IdentifiableAmount* amount, QString getSdrug, bool enable, ident dbid = -1);

public:
    void convert(const Unit &getUnit);
//    bool isValid();
    bool operator== ( CoreMeasure* other );
    bool operator!= ( CoreMeasure* other );
};


QML_POINTERLIST_CLASS_DECL(CoreMeasureList, CoreMeasure)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CoreMeasure*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::CoreMeasure*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CoreMeasureList*)


#endif // MEASURE_H
