//@@license@@

#include "appglobals.h"

#include "core/settings.h"

#include "admin/src/dal/practician.h"

using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::GuiUtils;

AppGlobals* AppGlobals::m_instance = nullptr;

AppGlobals::AppGlobals() :
    m_listFile(),
    m_requestFile(),
    m_iccaFile(),
    m_analyst(nullptr)
{

    m_showProcessingTime = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "showProcessingTime" ,true).toBool();
    m_autoCalculation = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "autoCalculation" ,true).toBool();
    m_percentileCalculation = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "percentileCalculation" ,true).toBool();

}

bool AppGlobals::showProcessingTime()
{
    return m_showProcessingTime;
}

void AppGlobals::setShowProcessingTime(bool show)
{
    if (show != m_showProcessingTime) {
        m_showProcessingTime = show;
        SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "showProcessingTime" ,show);
    }
}

bool AppGlobals::autoCalculation()
{
    return m_autoCalculation;
}

void AppGlobals::setAutoCalculation(bool enable)
{
    if (enable != m_autoCalculation) {
        m_autoCalculation = enable;
        SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "autoCalculation" ,enable);
    }
}

bool AppGlobals::percentileCalculation()
{
    return m_percentileCalculation;
}

void AppGlobals::setPercentileCalculation(bool enable)
{
    if (enable != m_percentileCalculation) {
        m_percentileCalculation = enable;
        SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "percentileCalculation" ,enable);
    }
}

const QString AppGlobals::getListFile(){
    return m_listFile;
}

const QString AppGlobals::getRequestFile(){
    return m_requestFile;
}

const QString AppGlobals::getIccaFile(){
    return m_iccaFile;
}

void AppGlobals::setListFile(QString _listFile){
    m_listFile = _listFile;
}

void AppGlobals::setRequestFile(QString _requestFile){
    m_requestFile = _requestFile;
}

void AppGlobals::setIccaFile(QString _iccaFile){
    m_iccaFile = _iccaFile;
}

Practician* AppGlobals::getAnalyst()
{
    //TODO (JRP): To debug c.f. correspondig issue #14
    if (true || m_analyst == nullptr) {
        m_analyst = new Practician();
        loadAnalystSettings();
    }
    return m_analyst;
}

void AppGlobals::loadAnalystSettings()
{
    m_analyst->title(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/title" ,"").toString());
    m_analyst->role(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/role" ,"").toString());
    m_analyst->person()->name(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/name" ,"").toString());
    m_analyst->person()->firstname(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/firstname" ,"").toString());

    m_analyst->person()->location()->city(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/address/city" ,"").toString());
    m_analyst->person()->location()->postcode(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/address/postcode" ,"").toString());
    m_analyst->person()->location()->state(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/address/state" ,"").toString());
    m_analyst->person()->location()->country(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/address/country" ,"").toString());

    auto phoneNumber = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/phones/default" ,"").toString();
    if (!phoneNumber.isEmpty()) {
        auto phone = Tucuxi::Gui::Core::CoreFactory::createEntity<Tucuxi::Gui::Admin::Phone>(ABSTRACTREPO);
        phone->setNumber(phoneNumber);
        m_analyst->person()->getPhones()->append(phone);
    }

    m_analyst->institute()->name(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/institute/name" ,"").toString());
}

void AppGlobals::saveAnalystSettings()
{
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/title" ,m_analyst->title());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/role" ,m_analyst->role());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/name" ,m_analyst->person()->name());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/firstname" ,m_analyst->person()->firstname());

    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/address/city" ,m_analyst->person()->location()->city());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/address/postcode" ,m_analyst->person()->location()->postcode());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/address/state" ,m_analyst->person()->location()->state());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/address/country" ,m_analyst->person()->location()->country());


    if (m_analyst->person()->getPhones()->size() > 0) {
        SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/phones/default" ,m_analyst->person()->getPhones()->at(0)->getNumber());
    }

    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/institute/name" ,m_analyst->institute()->name());
}

