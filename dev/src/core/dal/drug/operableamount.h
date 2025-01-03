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


#ifndef OPERABLEAMOUNT_H
#define OPERABLEAMOUNT_H

#include "core/dal/entity.h"
#include "core/dal/amount.h"
#include "core/utils/ezutils.h"
#include "core/dal/drug/operation.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class IdentifiableAmount : public Entity, public Amount
{
    Q_OBJECT

    ENTITY_UTILS(IdentifiableAmount)
    Q_PROPERTY(double dbvalue READ getDbvalue WRITE setDbvalue NOTIFY dbvalueChanged)
    Q_PROPERTY(QString unitstring READ getUnitstring WRITE setUnitstring NOTIFY unitstringChanged)
    AUTO_PROPERTY_DECL(QString, amountId, AmountId)
public:
    QString getUnitstring() {
        return this->unit().name();
    }
    void setUnitstring(const QString& unit) {
        Unit u;
        u.fromString("["+unit+"]");
        if (u != this->unit()) {
            _unit.fromString("["+unit+"]");
            emit unitstringChanged(unit);
            emit dbvalueChanged(value());
        }
    }
    double getDbvalue() const { return this->value(); }
    void setDbvalue(double val) {
        if (this->value()!= val) {
            this->setValue(val);
            emit dbvalueChanged(val);
        }
    }

    Q_INVOKABLE double valueInUnit(QString toUnit) {
        if (Unit::isCompatible(unit(), Unit(toUnit))) {
            return Unit::convertToUnit(value(), unit(), Unit(toUnit));
        }
        QString mess = QString("Error in unit conversion. Initial: %1. Final: %2").arg(unit().name()).arg(toUnit);
        EXLOG(QtWarningMsg, Tucuxi::Gui::Core::DATAERROR, mess);
        return value();
    }

signals:
    void dbvalueChanged(double);
    void unitstringChanged(QString);

protected:
    Q_INVOKABLE IdentifiableAmount(AbstractRepository *repository, QObject *parent = nullptr) : Entity(repository, parent), _amountId("default"), Amount() {}
};

//class OperationList;

class OperableAmount : public IdentifiableAmount
{
    Q_OBJECT

    ENTITY_UTILS(OperableAmount)

    AUTO_PROPERTY_DECL(OperationList*, operations, Operations)

    // This field should be used for PatientVariate, for instance.
    // It will allow to define if the user did choose a value or did let the operation be applied
    // Should not be used for DrugVariate
    AUTO_PROPERTY_DECL(bool, fixed, Fixed)

    protected:
        Q_INVOKABLE OperableAmount(AbstractRepository *repository, QObject *parent = nullptr)
      : IdentifiableAmount(repository, parent),
       _operations(CoreFactory::createEntity<OperationList>(repository, parent)),
       _fixed(false)
    {}
};

class OperableValue : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(OperableValue)

    AUTO_PROPERTY_DECL(QString, valueId, ValueId)
    AUTO_PROPERTY_DECL(QVariant, value, Value)
    AUTO_PROPERTY_DECL(OperationList*, operations, Operations)

    protected:
        Q_INVOKABLE OperableValue(AbstractRepository *repository, QObject *parent = nullptr)
      : Entity(repository, parent),
       _operations(CoreFactory::createEntity<OperationList>(repository, parent))
        {}
};

POINTERLIST_CLASS_DECL(OperableValueList, OperableValue)


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::IdentifiableAmount*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::IdentifiableAmount*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::OperableAmount*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::OperableAmount*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::OperableValue*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::OperableValue*>)

#endif // OPERABLEAMOUNT_H
