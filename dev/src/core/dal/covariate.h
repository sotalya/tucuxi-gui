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


#ifndef COVARIATE_H
#define COVARIATE_H

#include <QString>
#include <QMetaType>
#include <QDateTime>
#include <QMetaType>

#include "core/utils/ezutils.h"
#include "core/utils/units.h"
//#include "dbobjects.h"
#include "drug/operableamount.h"

#include "core/dal/entity.h"

Q_MOC_INCLUDE("core/dal/uncastedvalue.h")

namespace Tucuxi {
namespace Gui {
namespace Core {


class UncastedValueList;

class PatientVariate : public Entity {
    Q_OBJECT

    ENTITY_UTILS(PatientVariate)

    Q_PROPERTY(QString covariateId READ getCovariateId WRITE setCovariateId NOTIFY covariateIdChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    AUTO_PROPERTY_DECL(QDateTime, date, Date)
    //! Now only used for storing the birthdate
    AUTO_PROPERTY_DECL(QString, valueAsString, ValueAsString)
    AUTO_PROPERTY_DECL(OperableAmount*, quantity, Quantity)
    AUTO_PROPERTY_DECL(QString, description, Description)
    AUTO_PROPERTY_DECL(Descriptor, descriptor, Descriptor)
    AUTO_PROPERTY_DECL(QMetaType::Type, type, Type)
    //! To indicate if it was automatically calculated or forced by the user
    AUTO_PROPERTY_DECL(bool, forced, Forced)

    AUTO_PROPERTY_DECL(UncastedValueList*, uncastedValues, UncastedValues)

protected:

    Q_INVOKABLE PatientVariate(AbstractRepository *repository = 0, QObject *parent = nullptr, const QString &id = "", const QDateTime &date = QDateTime());
    Q_INVOKABLE PatientVariate(AbstractRepository *repository, QObject *parent, const QString &id, const QDateTime &date, double value);

public:

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

    Q_INVOKABLE QString typeToQString()
    {
        QString stringType;

        if(_type == QMetaType::Type::Double){
            stringType = "double";
        } else if(_type == QMetaType::Type::Int){
            stringType = "int";
        } else if (_type ==  QMetaType::Type::Bool){
            stringType = "bool";
        }else{
            stringType = "noType";
        }

        return stringType;
    }

    QString getCovariateId() const {return _covariateId;}
    bool setCovariateId(const QString covariateId);
    QString getName() const {return _name;}
    bool setName(const QString name);

    QString toString() const;
    bool isValid () const;
    void operator= (double value);
    bool operator== (const PatientVariate &) const;
    bool operator!= (const PatientVariate &) const;

signals:
    void nameChanged(QString);
    void covariateIdChanged(QString);

private:
    QString _name;
    QString _covariateId;
    bool _valid;
};

QML_POINTERLIST_CLASS_DECL(PatientVariateList, PatientVariate)

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::PatientVariate*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::PatientVariate*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::PatientVariateList*)

#endif // COVARIATE_H
