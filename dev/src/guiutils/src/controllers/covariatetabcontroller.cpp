//@@license@@

#include "covariatetabcontroller.h"
#include "core/dal/drug/drugvariate.h"
#include "core/dal/covariate.h"
#include "interpretationcontroller.h"
#include "core/utils/connect.h"
#include "core/dal/drug/translatablestring.h"



DrugVariateInfo::DrugVariateInfo(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent) :
    Entity(repository,parent),
    _actualValue(Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::OperableAmount>(repository, this)),
    _defaultValue(Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::OperableAmount>(repository, this)),
    _type(QMetaType::Int),
    _automatic(false)
{
}

AUTO_PROPERTY_IMPL(DrugVariateInfo, QString, covariateId, CovariateId)
AUTO_PROPERTY_IMPL(DrugVariateInfo, QString, name, Name)
AUTO_PROPERTY_IMPL(DrugVariateInfo, QDateTime, date, Date)
AUTO_PROPERTY_IMPL(DrugVariateInfo, Tucuxi::GuiCore::OperableAmount*, actualValue, ActualValue)
AUTO_PROPERTY_IMPL(DrugVariateInfo, Tucuxi::GuiCore::OperableAmount*, defaultValue, DefaultValue)
AUTO_PROPERTY_IMPL(DrugVariateInfo, QMetaType::Type, type, Type)
AUTO_PROPERTY_IMPL(DrugVariateInfo, bool, automatic, Automatic)
QML_POINTERLIST_CLASS_IMPL(DrugVariateInfoList, DrugVariateInfo)

STD_PROPERTY_IMPL(CovariateTabController, DrugVariateInfoList*, drugVariateInfos, DrugVariateInfos)
STD_PROPERTY_IMPL(CovariateTabController, Tucuxi::GuiCore::PatientVariateList*, fileredVariates, FileredVariates)
STD_PROPERTY_IMPL(CovariateTabController, Tucuxi::GuiCore::PatientVariateList*, patientVariates, PatientVariates)



CovariateTabController::CovariateTabController(QObject *parent) : AbstractViewController(parent)
{
    _drugVariateInfos = Tucuxi::GuiCore::CoreFactory::createEntity<DrugVariateInfoList>(ABSTRACTREPO, this);
    _fileredVariates = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariateList>(ABSTRACTREPO, this);
    _patientVariates = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariateList>(ABSTRACTREPO, this);
}

void CovariateTabController::reset(Tucuxi::GuiCore::DrugVariateList* drugVariates)
{
    _drugVariateInfos->clear();
    _fileredVariates->clearWithoutDeletion();

    if (drugVariates == nullptr)
        return;

    // Create a copy of all drug variates
    foreach (Tucuxi::GuiCore::DrugVariate* dv, drugVariates->getList())
    {
        DrugVariateInfo *copy = Tucuxi::GuiCore::CoreFactory::createEntity<DrugVariateInfo>(ABSTRACTREPO, _drugVariateInfos);
        // copy->setName(dv->getCovariateId());
        copy->setCovariateId(dv->getCovariateId());
        copy->setName(dv->getVisualNameTranslation()->value());
        copy->setType(dv->getType());
        copy->getActualValue()->setDbvalue(dv->getQuantity()->getDbvalue());
        copy->getActualValue()->setUnit(dv->getQuantity()->unit());
        copy->getDefaultValue()->setDbvalue(dv->getQuantity()->getDbvalue());
        copy->getDefaultValue()->setUnit(dv->getQuantity()->unit());
        copy->setAutomatic(dv->getAutomatic());
        _drugVariateInfos->append(copy);
    }
    updateAllActualValues();
}

void CovariateTabController::selectDrugVariate(int drugVariateFromIndex)
{
    if (!isDrugIndexValid(drugVariateFromIndex)) return;

    DrugVariateInfo *drugVariate = _drugVariateInfos->at(drugVariateFromIndex);

    // Refresh our list containing the history for the currently selected drug variate
    _fileredVariates->clear();
    foreach (Tucuxi::GuiCore::PatientVariate* patientVariate, _patientVariates->getList())
    {
        if (patientVariate->getCovariateId() == drugVariate->getCovariateId())
        {
            _fileredVariates->append(patientVariate);
        }
    }
    emit fileredVariatesChanged(_fileredVariates);
}

void CovariateTabController::addPatientVariate(const int drugVariateFromIndex)
{
    if (!isDrugIndexValid(drugVariateFromIndex)) return;

    DrugVariateInfo *drugVariate = _drugVariateInfos->at(drugVariateFromIndex);
    QString variateName = drugVariate->getCovariateId();

    Tucuxi::GuiCore::PatientVariate* pv = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariate>(ABSTRACTREPO, _patientVariates);
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


void CovariateTabController::setBirthdate(QDateTime date)
{
    // Remove the birthdate if it exists
    for(int i = 0; i < _patientVariates->size() ; i++) {
        if (_patientVariates->at(i)->getCovariateId() == "birthdate") {
            _patientVariates->remove(i);
            i = _patientVariates->size() + 1;
        }
    }

    Tucuxi::GuiCore::PatientVariate* pv = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariate>(ABSTRACTREPO, _patientVariates);
    pv->setDate(QDateTime::currentDateTime());
    pv->setValueAsString(date.toString(Qt::ISODate));
    pv->setCovariateId("birthdate");
    pv->setName("birthdate");
    pv->setType(QMetaType::Type::Int);
    pv->getQuantity()->setDbvalue(date.toMSecsSinceEpoch() / (qint64)1000);
    pv->getQuantity()->setUnit(Tucuxi::GuiCore::Unit("s"));
    _patientVariates->append(pv);



    // Remove the age if it exists
    for(int i = 0; i < _patientVariates->size() ; i++) {
        if (_patientVariates->at(i)->getCovariateId() == "age") {
            _patientVariates->remove(i);
            i = _patientVariates->size() + 1;
        }
    }
    // Remove the age if it exists
    for(int i = 0; i < _patientVariates->size() ; i++) {
        if (_patientVariates->at(i)->getCovariateId() == "pna") {
            _patientVariates->remove(i);
            i = _patientVariates->size() + 1;
        }
    }


    QDate birthday = date.date();
    double age = birthday.daysTo(QDate::currentDate()) / 365;
    double pna = birthday.daysTo(QDate::currentDate());

    bool oneChanged = false;

    QString variateName = "age";
    double value = age;
    foreach (DrugVariateInfo* drugVariate, _drugVariateInfos->getList())
    {
        if (drugVariate->getCovariateId() == variateName) {
            Tucuxi::GuiCore::PatientVariate* pv = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariate>(ABSTRACTREPO, _patientVariates);
            pv->setDate(QDateTime::currentDateTime());
            pv->setCovariateId(variateName);
            pv->setName(drugVariate->getName());
            pv->setType(drugVariate->getType());
            pv->getQuantity()->setDbvalue(value);
            pv->getQuantity()->setUnit(drugVariate->getDefaultValue()->unit());

            _patientVariates->append(pv);
            //_fileredVariates->append(pv);

            oneChanged = true;

            //updateActualValue(variateName);
        }
    }

    variateName = "pna";
    value = pna;
    foreach (DrugVariateInfo* drugVariate, _drugVariateInfos->getList())
    {
        if (drugVariate->getCovariateId() == variateName) {
            Tucuxi::GuiCore::PatientVariate* pv = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariate>(ABSTRACTREPO, _patientVariates);
            pv->setDate(QDateTime::currentDateTime());
            pv->setCovariateId(variateName);
            pv->setName(drugVariate->getName());
            pv->setType(drugVariate->getType());
            pv->getQuantity()->setDbvalue(value);
            pv->getQuantity()->setUnit(drugVariate->getDefaultValue()->unit());

            _patientVariates->append(pv);
            //_fileredVariates->append(pv);
            //updateActualValue(variateName);

            oneChanged = true;
            selectDrugVariate(0);
        }
    }

    if (oneChanged) {
        updateAllActualValues();
        emit fileredVariatesChanged(_fileredVariates);
    }
    emit patientVariatesChanged(_patientVariates);
}

void CovariateTabController::setSinglePatientVariate(QString id, double value)
{
    bool specialSet = false;
    if (id == "age") {
        // Remove the age if it exists
        for(int i = 0; i < _patientVariates->size() ; i++) {
            if (_patientVariates->at(i)->getCovariateId() == "age") {
                _patientVariates->remove(i);
                i = _patientVariates->size() + 1;
                specialSet = true;
            }
        }
    }

    if (id == "pna") {
        // Remove the pna if it exists
        for(int i = 0; i < _patientVariates->size() ; i++) {
            if (_patientVariates->at(i)->getCovariateId() == "pna") {
                _patientVariates->remove(i);
                i = _patientVariates->size() + 1;
                specialSet = true;
            }
        }
    }

    if (id == "sex") {
        // Remove the sex if it exists
        for(int i = 0; i < _patientVariates->size() ; i++) {
            if (_patientVariates->at(i)->getCovariateId() == "sex") {
                _patientVariates->remove(i);
                i = _patientVariates->size() + 1;
                specialSet = true;
            }
        }
    }


    QString variateName = id;
    foreach (DrugVariateInfo* drugVariate, _drugVariateInfos->getList())
    {
        if (drugVariate->getCovariateId() == id) {
            Tucuxi::GuiCore::PatientVariate* pv = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariate>(ABSTRACTREPO, _patientVariates);
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
        Tucuxi::GuiCore::PatientVariate* pv = Tucuxi::GuiCore::CoreFactory::createEntity<Tucuxi::GuiCore::PatientVariate>(ABSTRACTREPO, _patientVariates);
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

void CovariateTabController::removePatientVariate(int index)
{
    if (!isPatientIndexValid(index)) return;

    // Remove the variate from the list of displayed variates
    Tucuxi::GuiCore::PatientVariate* pv = _fileredVariates->at(index);
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

void CovariateTabController::setDbValue(int index, double value)
{
    if (!isPatientIndexValid(index)) return;

    Tucuxi::GuiCore::PatientVariate *pv = _fileredVariates->at(index);
    if (pv->getQuantity()->getDbvalue() != value)
    {
        pv->getQuantity()->setDbvalue(value);
        updateActualValue(pv->getCovariateId());
    }
}

void CovariateTabController::setDate(int index, QDateTime time)
{
    if (!isPatientIndexValid(index)) return;

    Tucuxi::GuiCore::PatientVariate *pv = _fileredVariates->at(index);
    if (pv->getDate() != time)
    {
        pv->setDate(time);
        updateActualValue(pv->getCovariateId());
    }
}

void CovariateTabController::updateAllActualValues()
{
    foreach (DrugVariateInfo *dv, _drugVariateInfos->getList())
    {
         // Update the "latest value" field with the value of the latest variate
        qint64 latestDate = std::numeric_limits < qint64 >::min();
        foreach (Tucuxi::GuiCore::PatientVariate *pv, _patientVariates->getList())
        {
            if (pv->getCovariateId() == dv->getCovariateId())
            {
                pv->setName(dv->getName());
                qint64 date = pv->getDate().toMSecsSinceEpoch();
                if ((latestDate < date) && (pv->getDate() <= QDateTime::currentDateTime()))
                {
                    latestDate = date;
                    dv->getActualValue()->setDbvalue(pv->getQuantity()->getDbvalue());
                    dv->setDate(pv->getDate());
                }
            }
        }

        // If history is empty then set then display the default value
        if (latestDate == 0)
        {
            dv->getActualValue()->setDbvalue(dv->getDefaultValue()->value());
        }
    }

    _fileredVariates->sort(compareVariate);
}

void CovariateTabController::updateActualValue(QString variateName)
{
    // Find the currently selected drug
    foreach (DrugVariateInfo *dv, _drugVariateInfos->getList())
    {
        if (dv->getCovariateId() == variateName)
        {
            // Update the "latest value" field with the value of the latest variate
            qint64 latestDate = 0;
            foreach (Tucuxi::GuiCore::PatientVariate *pv, _patientVariates->getList())
            {
                if (pv->getCovariateId() == variateName)
                {
                    qint64 date = pv->getDate().toMSecsSinceEpoch();
                    if ((latestDate < date) && (pv->getDate() <= QDateTime::currentDateTime()))
                    {
                        latestDate = date;
                        dv->getActualValue()->setDbvalue(pv->getQuantity()->getDbvalue());
                        dv->setDate(pv->getDate());
                    }
                }
            }

            // If history is empty then set then display the default value
            if (latestDate == 0)
            {
                dv->getActualValue()->setDbvalue(dv->getDefaultValue()->value());
            }
            break;
        }
    }

    _fileredVariates->sort(compareVariate);
}

void CovariateTabController::forceRefresh()
{
    masterController->covariateUpdated();
}

bool CovariateTabController::isPatientIndexValid(int index)
{
    return (index >= 0) && (index < _fileredVariates->size());
}

bool CovariateTabController::isDrugIndexValid(int index)
{
    return (index >= 0) && (index < _drugVariateInfos->size());
}

bool CovariateTabController::compareVariate(const Tucuxi::GuiCore::PatientVariate* a, const Tucuxi::GuiCore::PatientVariate* b)
{
    return (a->getDate() < b->getDate());
}
