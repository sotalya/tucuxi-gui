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


#include "covariatetabcontroller.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/covariate.h"
#include "interpretationcontroller.h"
#include "core/dal/drug/translatablestring.h"

using namespace Tucuxi::Gui::GuiUtils;

DrugVariateInfo::DrugVariateInfo(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _actualValue(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(repository, this)),
    _defaultValue(Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::OperableAmount>(repository, this)),
    _type(QMetaType::Int),
    _automatic(false)
{
}

AUTO_PROPERTY_IMPL(DrugVariateInfo, QString, covariateId, CovariateId)
AUTO_PROPERTY_IMPL(DrugVariateInfo, QString, name, Name)
AUTO_PROPERTY_IMPL(DrugVariateInfo, QDateTime, date, Date)
AUTO_PROPERTY_IMPL(DrugVariateInfo, Tucuxi::Gui::Core::OperableAmount*, actualValue, ActualValue)
AUTO_PROPERTY_IMPL(DrugVariateInfo, Tucuxi::Gui::Core::OperableAmount*, defaultValue, DefaultValue)
AUTO_PROPERTY_IMPL(DrugVariateInfo, QMetaType::Type, type, Type)
AUTO_PROPERTY_IMPL(DrugVariateInfo, bool, automatic, Automatic)
AUTO_PROPERTY_IMPL(DrugVariateInfo, Tucuxi::Gui::Core::CovariateType, covariateType, CovariateType)
QML_POINTERLIST_CLASS_IMPL(DrugVariateInfoList, DrugVariateInfo)

STD_PROPERTY_IMPL(Tucuxi::Gui::GuiUtils::CovariateTabController, DrugVariateInfoList*, drugVariateInfos, DrugVariateInfos)
STD_PROPERTY_IMPL(Tucuxi::Gui::GuiUtils::CovariateTabController, Tucuxi::Gui::Core::PatientVariateList*, fileredVariates, FileredVariates)
STD_PROPERTY_IMPL(Tucuxi::Gui::GuiUtils::CovariateTabController, Tucuxi::Gui::Core::PatientVariateList*, patientVariates, PatientVariates)



Tucuxi::Gui::GuiUtils::CovariateTabController::CovariateTabController(QObject *parent) : AbstractViewController(parent)
{
    _drugVariateInfos = Tucuxi::Gui::Core::CoreFactory::createEntity<DrugVariateInfoList>(ABSTRACTREPO, this);
    _fileredVariates = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariateList>(ABSTRACTREPO, this);
    _patientVariates = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariateList>(ABSTRACTREPO, this);
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::reset(Tucuxi::Gui::Core::DrugVariateList* drugVariates)
{
    _drugVariateInfos->clear();
    _fileredVariates->clearWithoutDeletion();

    if (drugVariates == nullptr)
        return;

    // Create a copy of all drug variates
    foreach (Tucuxi::Gui::Core::DrugVariate* dv, drugVariates->getList())
    {
        DrugVariateInfo *copy = Tucuxi::Gui::Core::CoreFactory::createEntity<DrugVariateInfo>(ABSTRACTREPO, _drugVariateInfos);
        // copy->setName(dv->getCovariateId());
        copy->setCovariateId(dv->getCovariateId());
        copy->setName(dv->getVisualNameTranslation()->value());
        copy->setType(dv->getType());
        copy->getActualValue()->setDbvalue(dv->getQuantity()->getDbvalue());
        copy->getActualValue()->setUnit(dv->getQuantity()->unit());
        copy->getDefaultValue()->setDbvalue(dv->getQuantity()->getDbvalue());
        copy->getDefaultValue()->setUnit(dv->getQuantity()->unit());
        copy->setAutomatic(dv->getAutomatic());
        copy->setCovariateType(dv->getCovariateType());
        _drugVariateInfos->append(copy);
    }
    updateAllActualValues();
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::selectDrugVariate(int drugVariateFromIndex)
{
    if (!isDrugIndexValid(drugVariateFromIndex)) return;

    DrugVariateInfo *drugVariate = _drugVariateInfos->at(drugVariateFromIndex);

    // Refresh our list containing the history for the currently selected drug variate
    _fileredVariates->clear();
    foreach (Tucuxi::Gui::Core::PatientVariate* patientVariate, _patientVariates->getList())
    {
        if (patientVariate->getCovariateId() == drugVariate->getCovariateId())
        {
            _fileredVariates->append(patientVariate);
        }
    }

    _fileredVariates->sort(compareVariate);
    emit fileredVariatesChanged(_fileredVariates);
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::addPatientVariate(const int drugVariateFromIndex)
{
    if (!isDrugIndexValid(drugVariateFromIndex)) return;

    DrugVariateInfo *drugVariate = _drugVariateInfos->at(drugVariateFromIndex);
    QString variateName = drugVariate->getCovariateId();

    Tucuxi::Gui::Core::PatientVariate* pv = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO, _patientVariates);
    pv->setDate(QDateTime::currentDateTime());
    pv->setCovariateId(variateName);
    pv->setName(drugVariate->getName());
    pv->setType(drugVariate->getType());
    pv->getQuantity()->setDbvalue(drugVariate->getDefaultValue()->value());
    pv->getQuantity()->setUnit(drugVariate->getDefaultValue()->unit());

    _patientVariates->append(pv);
    _fileredVariates->append(pv);

    updateActualValue(variateName);

    emit fileredVariatesChanged(_fileredVariates);
    emit patientVariatesChanged(_patientVariates);
}


void Tucuxi::Gui::GuiUtils::CovariateTabController::setBirthdate(QDateTime date)
{
    // Remove the birthdate if it exists
    for(int i = 0; i < _patientVariates->size() ; i++) {
        if (_patientVariates->at(i)->getCovariateId() == "birthdate") {
            _patientVariates->remove(i);
            i = _patientVariates->size() + 1;
        }
    }

    Tucuxi::Gui::Core::PatientVariate* pv = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO, _patientVariates);
    pv->setDate(QDateTime::currentDateTime());
    pv->setValueAsString(date.toString(Qt::ISODate));
    pv->setCovariateId("birthdate");
    pv->setName("birthdate");
    pv->setType(QMetaType::Type::Int);
    pv->getQuantity()->setDbvalue(date.toMSecsSinceEpoch() / (qint64)1000);
    pv->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit("s"));
    _patientVariates->append(pv);


    // Remove the AgeInYears, AgeInMonths, AgeInWeeks, and AgeInDays
    for(int i = 0; i < _patientVariates->size() ; i++) {
        auto covarType = findCovariateTypeFromPatientVariate(_patientVariates->at(i));
        if (covarType == Core::CovariateType::AgeInYears || covarType == Core::CovariateType::AgeInDays ||
            covarType == Core::CovariateType::AgeInWeeks || covarType == Core::CovariateType::AgeInMonths)
        {
            _patientVariates->remove(i);
        }
    }

    QDate birthday = date.date();
    bool oneChanged = false;

    foreach (DrugVariateInfo* drugVariate, _drugVariateInfos->getList())
    {
        auto covarType = drugVariate->getCovariateType();
        if (covarType == Core::CovariateType::AgeInYears || covarType == Core::CovariateType::AgeInDays ||
            covarType == Core::CovariateType::AgeInWeeks || covarType == Core::CovariateType::AgeInMonths)
        {
            Tucuxi::Gui::Core::PatientVariate* pv = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO, _patientVariates);
            pv->setDate(QDateTime::currentDateTime());
            pv->setCovariateId(drugVariate->getCovariateId());
            pv->setName(drugVariate->getName());
            pv->setType(drugVariate->getType());
            pv->getQuantity()->setDbvalue(computeTimeBasedOnCovariateType(birthday, covarType));
            pv->getQuantity()->setUnit(drugVariate->getDefaultValue()->unit());

            _patientVariates->append(pv);
            //_fileredVariates->append(pv);

            oneChanged = true;

            //updateActualValue(variateName);
        }
    }

    if (oneChanged) {
        updateAllActualValues();
        emit fileredVariatesChanged(_fileredVariates);
    }
    emit patientVariatesChanged(_patientVariates);
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::setSinglePatientVariate(QString id, double value)
{
    bool specialSet = false;
    // Remove the AgeInYears, AgeInMonths, AgeInWeeks, AgeInDays, and sex
    for(int i = 0; i < _patientVariates->size() ; i++) {
        auto covarType = findCovariateTypeFromPatientVariate(_patientVariates->at(i));
        if (covarType == Core::CovariateType::AgeInYears || covarType == Core::CovariateType::AgeInDays ||
            covarType == Core::CovariateType::AgeInWeeks || covarType == Core::CovariateType::AgeInMonths ||
            covarType == Core::CovariateType::Sex)
        {
            _patientVariates->remove(i);
            specialSet = true;
        }
    }


    QString variateName = id;
    foreach (DrugVariateInfo* drugVariate, _drugVariateInfos->getList())
    {
        if (drugVariate->getCovariateId() == id) {
            Tucuxi::Gui::Core::PatientVariate* pv = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO, _patientVariates);
            pv->setDate(QDateTime::currentDateTime());
            pv->setCovariateId(variateName);
            pv->setName(drugVariate->getName());
            pv->setType(drugVariate->getType());
            pv->getQuantity()->setDbvalue(value);
            pv->getQuantity()->setUnit(drugVariate->getDefaultValue()->unit());

            _patientVariates->append(pv);
            if (!specialSet) {
                _fileredVariates->append(pv);
            }
            updateActualValue(variateName);

            if (!specialSet) {
                emit fileredVariatesChanged(_fileredVariates);
            }
            emit patientVariatesChanged(_patientVariates);

            selectDrugVariate(0);
            return;
        }
    }
/*
    if (id == "birthdate") {
        Tucuxi::Gui::Core::PatientVariate* pv = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Core::PatientVariate>(ABSTRACTREPO, _patientVariates);
        pv->setDate(QDateTime::currentDateTime());
        pv->setCovariateId("birthdate");
        pv->setName("birthdate");
        pv->setType(QMetaType::Type::Int);
        pv->getQuantity()->setDbvalue(value);
        pv->getQuantity()->setUnit(drugVariate->getDefaultValue()->unit());
        _patientVariates->append(pv);

    }
*/
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::removePatientVariate(int index)
{
    if (!isPatientIndexValid(index)) return;

    // Remove the variate from the list of displayed variates
    Tucuxi::Gui::Core::PatientVariate* pv = _fileredVariates->at(index);
    _fileredVariates->remove(index);

    QString variateName = pv->getCovariateId();

    // Remove the variate from the "real" list
    for (int i=0; i<_patientVariates->size(); i++)
    {
        if (_patientVariates->at(i) == pv)
        {
            _patientVariates->remove(i);
            break;
        }
    }

    updateActualValue(variateName);
    emit fileredVariatesChanged(_fileredVariates);
    emit patientVariatesChanged(_patientVariates);
    masterController->covariateUpdated();
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::setDbValue(int index, double value)
{
    if (!isPatientIndexValid(index)) return;

    Tucuxi::Gui::Core::PatientVariate *pv = _fileredVariates->at(index);
    if (pv->getQuantity()->getDbvalue() != value)
    {
        pv->getQuantity()->setDbvalue(value);
        updateActualValue(pv->getCovariateId());
    }
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::setDate(int index, QDateTime time)
{
    if (!isPatientIndexValid(index)) return;

    Tucuxi::Gui::Core::PatientVariate *pv = _fileredVariates->at(index);
    if (pv->getDate() != time)
    {
        pv->setDate(time);
        updateActualValue(pv->getCovariateId());
    }
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::updateAllActualValues()
{
    foreach (DrugVariateInfo *dv, _drugVariateInfos->getList())
    {
         // Update the "latest value" field with the value of the latest variate
        qint64 latestDate = std::numeric_limits < qint64 >::min();
        foreach (Tucuxi::Gui::Core::PatientVariate *pv, _patientVariates->getList())
        {
            if (pv->getCovariateId() == dv->getCovariateId())
            {
                pv->setName(dv->getName());
                qint64 date = pv->getDate().toMSecsSinceEpoch();
                if ((latestDate < date) && (pv->getDate() <= QDateTime::currentDateTime()))
                {
                    latestDate = date;
                    dv->getActualValue()->setDbvalue(pv->getQuantity()->getDbvalue());
                    dv->getActualValue()->setUnitstring(pv->getQuantity()->getUnitstring());
                    dv->setDate(pv->getDate());
                }
            }
        }

        // If history is empty then set then display the default value
        if (latestDate == 0)
        {
            dv->getActualValue()->setDbvalue(dv->getDefaultValue()->value());
            dv->getActualValue()->setUnitstring(dv->getDefaultValue()->getUnitstring());
        }
    }

    _fileredVariates->sort(compareVariate);
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::updateActualValue(QString variateName)
{
    // Find the currently selected drug
    foreach (DrugVariateInfo *dv, _drugVariateInfos->getList())
    {
        if (dv->getCovariateId() == variateName)
        {
            // Update the "latest value" field with the value of the latest variate
            qint64 latestDate = 0;
            foreach (Tucuxi::Gui::Core::PatientVariate *pv, _patientVariates->getList())
            {
                if (pv->getCovariateId() == variateName)
                {
                    qint64 date = pv->getDate().toMSecsSinceEpoch();
                    if ((latestDate < date) && (pv->getDate() <= QDateTime::currentDateTime()))
                    {
                        latestDate = date;
                        dv->getActualValue()->setDbvalue(pv->getQuantity()->getDbvalue());
                        dv->getActualValue()->setUnitstring(pv->getQuantity()->getUnitstring());
                        dv->setDate(pv->getDate());
                    }
                }
            }

            // If history is empty then set then display the default value
            if (latestDate == 0)
            {
                dv->getActualValue()->setDbvalue(dv->getDefaultValue()->value());
                dv->getActualValue()->setUnitstring(dv->getDefaultValue()->getUnitstring());
            }
            break;
        }
    }

    _fileredVariates->sort(compareVariate);
}

void Tucuxi::Gui::GuiUtils::CovariateTabController::forceRefresh()
{
    masterController->covariateUpdated();
}

bool Tucuxi::Gui::GuiUtils::CovariateTabController::isPatientIndexValid(int index)
{
    return (index >= 0) && (index < _fileredVariates->size());
}

bool Tucuxi::Gui::GuiUtils::CovariateTabController::isDrugIndexValid(int index)
{
    return (index >= 0) && (index < _drugVariateInfos->size());
}

bool Tucuxi::Gui::GuiUtils::CovariateTabController::compareVariate(const Tucuxi::Gui::Core::PatientVariate* a, const Tucuxi::Gui::Core::PatientVariate* b)
{
    return (a->getDate() < b->getDate());
}

Tucuxi::Gui::Core::CovariateType Tucuxi::Gui::GuiUtils::CovariateTabController::findCovariateTypeFromPatientVariate(const Tucuxi::Gui::Core::PatientVariate* patientVariate)
{
    foreach (DrugVariateInfo* drugVariate, _drugVariateInfos->getList()){
        if(patientVariate->getCovariateId() == drugVariate->getCovariateId()){
            return drugVariate->getCovariateType();
        }
    }
    return Tucuxi::Gui::Core::CovariateType::Standard;
}

double Tucuxi::Gui::GuiUtils::CovariateTabController::computeTimeBasedOnCovariateType(const QDate date, const Tucuxi::Gui::Core::CovariateType covarType){
    if (covarType == Tucuxi::Gui::Core::CovariateType::AgeInYears){
        return date.daysTo(QDate::currentDate()) / 365;
    }
    else if (covarType == Tucuxi::Gui::Core::CovariateType::AgeInDays){
        return date.daysTo(QDate::currentDate());
    }
    else if (covarType == Tucuxi::Gui::Core::CovariateType::AgeInWeeks){
        return date.daysTo(QDate::currentDate()) / 7;
    }
    else if (covarType == Tucuxi::Gui::Core::CovariateType::AgeInMonths){
        return date.daysTo(QDate::currentDate()) / 30;
    }
    return 0;
}
