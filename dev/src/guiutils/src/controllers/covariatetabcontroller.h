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


#ifndef COVARIATETABCONTROLLER_H
#define COVARIATETABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"
#include "core/dal/entity.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/covariate.h"

Q_MOC_INCLUDE("core/dal/drug/operableamount.h")

namespace Tucuxi {
namespace Gui {
namespace Core {

class PatientVariateList;
class DrugVariateList;
class OperableAmount;
class AbstractRepository;

}
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class DrugVariateInfo : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(Tucuxi::Gui::GuiUtils::DrugVariateInfo)

    AUTO_PROPERTY_DECL(QString, covariateId, CovariateId)
    AUTO_PROPERTY_DECL(QString, name, Name)
    AUTO_PROPERTY_DECL(QDateTime, date, Date)
    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::OperableAmount*, actualValue, ActualValue)
    AUTO_PROPERTY_DECL(Tucuxi::Gui::Core::OperableAmount*, defaultValue, DefaultValue)
    AUTO_PROPERTY_DECL(QMetaType::Type, type, Type)
    AUTO_PROPERTY_DECL(bool, automatic, Automatic)
    AUTO_PROPERTY_DECL(Core::CovariateType, covariateType, CovariateType)

public:
    Q_INVOKABLE DrugVariateInfo(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr);

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

};

QML_POINTERLIST_CLASS_DECL(DrugVariateInfoList, DrugVariateInfo)


}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::GuiUtils::DrugVariateInfo*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::GuiUtils::DrugVariateInfo*>)

Q_DECLARE_METATYPE(Tucuxi::Gui::GuiUtils::DrugVariateInfoList*)


namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class CovariateTabController : public AbstractViewController
{
    Q_OBJECT

public:
    explicit CovariateTabController(QObject *parent = nullptr);

    //! This property is used by CovariateTab.qml to display the list of drug variates
    //! It is not meant to be modified during an interpretation
    STD_PROPERTY_DECL(DrugVariateInfoList*, drugVariateInfos, DrugVariateInfos)

    //! This property is used by CovariateTab.qml to display the subset of patient
    //! variates corresponding to the currently selected drug variate
    STD_PROPERTY_DECL(Tucuxi::Gui::Core::PatientVariateList*, fileredVariates, FileredVariates)

    //! This property is used internally and contains the patient history for all drug variates
    STD_PROPERTY_DECL(Tucuxi::Gui::Core::PatientVariateList*, patientVariates, PatientVariates)

public:
    void reset(Tucuxi::Gui::Core::DrugVariateList* drugVariates);

    Q_INVOKABLE void selectDrugVariate(int drugVariateFromIndex);
    Q_INVOKABLE void addPatientVariate(int drugVariateFromIndex);
    Q_INVOKABLE void removePatientVariate(int index);
    Q_INVOKABLE void setDbValue(int index, double value);
    Q_INVOKABLE void setDate(int index, QDateTime time);    

    Q_INVOKABLE void forceRefresh();

    void updateAllActualValues();

    void setSinglePatientVariate(QString id, double value);

    void setBirthdate(QDateTime date);

protected:
    bool isDrugIndexValid(int index);
    bool isPatientIndexValid(int index);
    void updateActualValue(QString variateName);

private:
    static bool compareVariate(const Tucuxi::Gui::Core::PatientVariate* a, const Tucuxi::Gui::Core::PatientVariate* b);
    Core::CovariateType findCovariateTypeFromPatientVariate(const Core::PatientVariate* patientVariate);
    double computeTimeBasedOnCovariateType(const QDate date, const Core::CovariateType covarType);
};

}
}
}

#endif // COVARIATETABCONTROLLER_H
