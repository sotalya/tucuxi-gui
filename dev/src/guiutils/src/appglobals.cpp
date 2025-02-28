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


#include "appglobals.h"

#include "core/settings.h"

#include "admin/src/dal/practician.h"

#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <sys/stat.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::GuiUtils;

AppGlobals* AppGlobals::m_instance = nullptr;

AppGlobals::AppGlobals() :
    m_listFile(),
    m_requestFile(),
    m_flatFile(),
    m_analyst(nullptr),
    m_groupIntake(true),
    m_flatImport(false),
    m_cdssOnly(false)
{

    m_showProcessingTime = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "showProcessingTime" ,true).toBool();
    m_autoCalculation = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "autoCalculation" ,true).toBool();
    m_percentileCalculation = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "percentileCalculation" ,true).toBool();
    m_cdssOnly = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "cdssOnly", false).toBool();
    m_cdssReportPath = loadCDSSReportPath();
}

bool AppGlobals::showProcessingTime()
{
    return m_showProcessingTime;
}

void AppGlobals::setShowProcessingTime(bool show)
{
    if (show != m_showProcessingTime) {
        m_showProcessingTime = show;
        SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "showProcessingTime", show);
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
        SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "percentileCalculation", enable);
    }
}

bool AppGlobals::cdssOnly()
{
    return m_cdssOnly;
}

void AppGlobals::setCdssOnly(bool cdssOnly)
{
    if (cdssOnly != m_cdssOnly) {
        m_cdssOnly = cdssOnly;
        SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "cdssOnly", cdssOnly);
        emit cdssOnlyChanged();
    }
}

const QString AppGlobals::getListFile(){
    return m_listFile;
}

const QString AppGlobals::getRequestFile(){
    return m_requestFile;
}

const QString AppGlobals::getFlatFile(){
    return m_flatFile;
}

bool AppGlobals::getFlatImport(){
    return m_flatImport;
}

bool AppGlobals::getGroupIntake(){
    return m_groupIntake;
}

void AppGlobals::setListFile(QString _listFile){
    m_listFile = _listFile;
}

void AppGlobals::setRequestFile(QString _requestFile){
    m_requestFile = _requestFile;
}

void AppGlobals::setFlatFile(QString _flatFile){
    m_flatFile = _flatFile;
}

void AppGlobals::setFlatImport(bool enable){
    m_flatImport = enable;
}

void AppGlobals::setGroupIntake(bool enable){
    m_groupIntake = enable;
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

void AppGlobals::saveCDSSReportPath(){
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "cdssReportPath" , m_cdssReportPath);
}

void AppGlobals::updateCDSSReportPath(QString path){
    m_cdssReportPath = path;
}

QString AppGlobals::getCDSSReportPath(){
    return m_cdssReportPath;
}

QString AppGlobals::loadCDSSReportPath(){
    QString cdssReportPath = SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "cdssReportPath").toString();

    return (cdssReportPath.isEmpty()) ? getDefaultPath() : cdssReportPath;
}

QString AppGlobals::getDefaultPath(){
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return QString::fromStdString(current_working_dir);
}

void AppGlobals::loadAnalystSettings()
{
    m_analyst->title(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/title" ,"").toString());
    m_analyst->role(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/role" ,"").toString());
    m_analyst->person()->name(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/name" ,"").toString());
    m_analyst->person()->firstname(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/firstname" ,"").toString());

    m_analyst->person()->location()->address(SETTINGS.get(Tucuxi::Gui::Core::Module::GUI, "analyst/address/address" ,"").toString());
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

    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/address/address" ,m_analyst->person()->location()->address());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/address/city" ,m_analyst->person()->location()->city());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/address/postcode" ,m_analyst->person()->location()->postcode());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/address/state" ,m_analyst->person()->location()->state());
    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/address/country" ,m_analyst->person()->location()->country());


    if (m_analyst->person()->getPhones()->size() > 0) {
        SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/phones/default" ,m_analyst->person()->getPhones()->at(0)->getNumber());
    }

    SETTINGS.set(Tucuxi::Gui::Core::Module::GUI, "analyst/institute/name" ,m_analyst->institute()->name());
}
