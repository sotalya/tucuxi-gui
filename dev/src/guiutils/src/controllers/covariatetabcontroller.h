//@@license@@

#ifndef COVARIATETABCONTROLLER_H
#define COVARIATETABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"
#include "core/dal/entity.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/covariate.h"

namespace Tucuxi {
namespace GuiCore {

class PatientVariateList;
class DrugVariateList;
class OperableAmount;
class AbstractRepository;

}
}

class DrugVariateInfo : public Tucuxi::GuiCore::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(DrugVariateInfo)

    AUTO_PROPERTY_DECL(QString, covariateId, CovariateId)
    AUTO_PROPERTY_DECL(QString, name, Name)
    AUTO_PROPERTY_DECL(QDateTime, date, Date)
    AUTO_PROPERTY_DECL(Tucuxi::GuiCore::OperableAmount*, actualValue, ActualValue)
    AUTO_PROPERTY_DECL(Tucuxi::GuiCore::OperableAmount*, defaultValue, DefaultValue)
    AUTO_PROPERTY_DECL(QMetaType::Type, type, Type)
    AUTO_PROPERTY_DECL(bool, automatic, Automatic)

public:
    Q_INVOKABLE DrugVariateInfo(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent = nullptr);

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

Q_DECLARE_METATYPE(DrugVariateInfo*)
Q_DECLARE_METATYPE(QList<DrugVariateInfo*>)

QML_POINTERLIST_CLASS_DECL(DrugVariateInfoList, DrugVariateInfo)
Q_DECLARE_METATYPE(DrugVariateInfoList*)

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
    STD_PROPERTY_DECL(Tucuxi::GuiCore::PatientVariateList*, fileredVariates, FileredVariates)

    //! This property is used internally and contains the patient history for all drug variates
    STD_PROPERTY_DECL(Tucuxi::GuiCore::PatientVariateList*, patientVariates, PatientVariates)

public:
    void reset(Tucuxi::GuiCore::DrugVariateList* drugVaraites);

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
    static bool compareVariate(const Tucuxi::GuiCore::PatientVariate* a, const Tucuxi::GuiCore::PatientVariate* b);
};

#endif // COVARIATETABCONTROLLER_H
