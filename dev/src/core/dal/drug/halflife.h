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


#ifndef HALFLIFE_H
#define HALFLIFE_H

#include "core/utils/units.h"
#include "core/utils/ezutils.h"
#include "core/dal/entity.h"
#include "core/utils/duration.h"

Q_MOC_INCLUDE("core/dal/drug/translatablestring.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class TranslatableString;

class Halflife : public Entity
{
    Q_OBJECT

    Q_PROPERTY(QString unitString READ getUnitString WRITE setUnitString NOTIFY unitStringChanged)


    ENTITY_UTILS(Halflife)

    AUTO_PROPERTY_DECL(Unit, unit, Unit)
    AUTO_PROPERTY_DECL(double, value, Value)
    AUTO_PROPERTY_DECL(int, multiplier, Multiplier)
//    AUTO_PROPERTY_DECL(CommentSet, commentSet, CommentSet)

    AUTO_PROPERTY_DECL(TranslatableString*, comments, Comments)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

protected:

    Q_INVOKABLE explicit Halflife(AbstractRepository *repository, QObject *parent = nullptr)
    : Entity(repository, parent), _unit(), _value(-1.0), _multiplier(-1), _comments(0) {}
    Q_INVOKABLE Halflife(AbstractRepository *repository, QObject *parent, const Unit &unit, double value, int multiplier)
     : Entity(repository,parent), _unit(unit), _value(value), _multiplier(multiplier) {}

public:
    bool isValid() const {return _unit.isValid() && _value > 0.0 && _multiplier > 0;}

    Duration halfLife() const {
        // ToDo ///////////////////////////////////////////////////////////////////////////////////////
        // Convert the duration using the half-life's unit.
        // ////////////////////////////////////////////////////////////////////////////////////////////
        return Duration(_value);
    }

    QString getUnitString() {return _unit.name();}
    void setUnitString(QString name) { _unit.fromString("["+name+"]");}

signals:
    void unitStringChanged(QString);

};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::Halflife*)
//Q_DECLARE_OPAQUE_POINTER(Tucuxi::Gui::Core::Halflife*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::Halflife*>)

#endif // HALFLIFE_H
