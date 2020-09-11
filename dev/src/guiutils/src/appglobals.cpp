#include "appglobals.h"

#include "core/settings.h"

#include "admin/src/dal/practician.h"

AppGlobals* AppGlobals::m_instance = nullptr;

AppGlobals::AppGlobals() :
    m_listFile(),
    m_requestFile(),
    m_analyst(nullptr)
{

    m_showProcessingTime = SETTINGS.get(ezechiel::core::Module::GUI, "showProcessingTime" ,true).toBool();

}

bool AppGlobals::showProcessingTime()
{
    return m_showProcessingTime;
}

void AppGlobals::setShowProcessingTime(bool show)
{
    if (show != m_showProcessingTime) {
        m_showProcessingTime = show;
        SETTINGS.set(ezechiel::core::Module::GUI, "showProcessingTime" ,show);
    }
}

const QString AppGlobals::getListFile(){
    return m_listFile;
}

const QString AppGlobals::getRequestFile(){
    return m_requestFile;
}

void AppGlobals::setListFile(QString _listFile){
    m_listFile = _listFile;
}

void AppGlobals::setRequestFile(QString _requestFile){
    m_requestFile = _requestFile;
}

Practician* AppGlobals::getAnalyst()
{
    if (m_analyst == nullptr) {
        m_analyst = new Practician();
        loadAnalystSettings();
    }
    return m_analyst;
}

void AppGlobals::loadAnalystSettings()
{
    m_analyst->title(SETTINGS.get(ezechiel::core::Module::GUI, "analyst/title" ,"").toString());
    m_analyst->role(SETTINGS.get(ezechiel::core::Module::GUI, "analyst/role" ,"").toString());
    m_analyst->person()->name(SETTINGS.get(ezechiel::core::Module::GUI, "analyst/name" ,"").toString());
    m_analyst->person()->firstname(SETTINGS.get(ezechiel::core::Module::GUI, "analyst/firstname" ,"").toString());

    m_analyst->person()->location()->city(SETTINGS.get(ezechiel::core::Module::GUI, "analyst/address/city" ,"").toString());
    m_analyst->person()->location()->postcode(SETTINGS.get(ezechiel::core::Module::GUI, "analyst/address/postcode" ,"").toString());
    m_analyst->person()->location()->state(SETTINGS.get(ezechiel::core::Module::GUI, "analyst/address/state" ,"").toString());
    m_analyst->person()->location()->country(SETTINGS.get(ezechiel::core::Module::GUI, "analyst/address/country" ,"").toString());

    auto phoneNumber = SETTINGS.get(ezechiel::core::Module::GUI, "analyst/phones/default" ,"").toString();
    if (!phoneNumber.isEmpty()) {
        auto phone = ezechiel::core::CoreFactory::createEntity<Phone>(ABSTRACTREPO);
        phone->setNumber(phoneNumber);
        m_analyst->person()->getPhones()->append(phone);
    }

    m_analyst->institute()->name(SETTINGS.get(ezechiel::core::Module::GUI, "analyst/institute/name" ,"").toString());
}

void AppGlobals::saveAnalystSettings()
{
    SETTINGS.set(ezechiel::core::Module::GUI, "analyst/title" ,m_analyst->title());
    SETTINGS.set(ezechiel::core::Module::GUI, "analyst/role" ,m_analyst->role());
    SETTINGS.set(ezechiel::core::Module::GUI, "analyst/name" ,m_analyst->person()->name());
    SETTINGS.set(ezechiel::core::Module::GUI, "analyst/firstname" ,m_analyst->person()->firstname());

    SETTINGS.set(ezechiel::core::Module::GUI, "analyst/address/city" ,m_analyst->person()->location()->city());
    SETTINGS.set(ezechiel::core::Module::GUI, "analyst/address/postcode" ,m_analyst->person()->location()->postcode());
    SETTINGS.set(ezechiel::core::Module::GUI, "analyst/address/state" ,m_analyst->person()->location()->state());
    SETTINGS.set(ezechiel::core::Module::GUI, "analyst/address/country" ,m_analyst->person()->location()->country());


    if (m_analyst->person()->getPhones()->size() > 0) {
        SETTINGS.set(ezechiel::core::Module::GUI, "analyst/phones/default" ,m_analyst->person()->getPhones()->at(0)->getNumber());
    }

    SETTINGS.set(ezechiel::core::Module::GUI, "analyst/institute/name" ,m_analyst->institute()->name());
}

