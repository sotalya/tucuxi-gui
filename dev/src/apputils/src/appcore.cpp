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


#include "apputils/src/appcore.h"
#ifndef CONFIG_NOBOTAN
#include "apputils/src/securitymanager.h"
#endif // CONFIG_NOBOTAN
#include "apputils/src/drugmanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QLocale>
#include <QString>
#include <QTranslator>

//! Declare some system-related data
//! A cmake and a qmake version are available, while the qmake version is static the cmake one is dynamic
#ifdef CMAKE
#include "core-cmake.h"
#else //Build without cmake (qmake)

//On mac OS search for the source in the ../Resource directory
#ifdef Q_OS_MACX
#define APPDATA "../Resources"
#define BUILD_ID "qmake-mac"
#else //On the others, just search under the executable
#define APPDATA "./"
#define BUILD_ID "qmake"
#endif

#endif

namespace Tucuxi {
namespace Gui {
namespace AppUtils {

#ifndef CONFIG_NOBOTAN
Tucuxi::Gui::Core::SecurityManager *AppCore::securityManager()
{
    if (!_securityManager)
        _securityManager = &Tucuxi::Gui::Core::SecurityManager::instance();
    return _securityManager;

 // TODO Check why this doesn't work (it creates 2 instances)
 //   return &SecurityManager::instance();
}
#endif // CONFIG_NOBOTAN


//Constructor
AppCore::AppCore() :
#ifndef CONFIG_NOBOTAN
    _securityManager(nullptr),
    #endif // CONFIG_NOBOTAN
    _drugManager(nullptr)
{
    drugMutex = new QMutex();
    drugPreloadMutex = new QMutex();

    drugloader = new Tucuxi::Gui::Core::ThreadDrugLoader();
}

//Destructs the pseudo-singletons
AppCore::~AppCore ()
{
    if (_drugManager)
        delete _drugManager;
    if (drugloader)
        delete drugloader;
    if (drugMutex)
        delete drugMutex;
    if (drugPreloadMutex)
        delete drugPreloadMutex;
}

DrugManager *AppCore::drugManager()
{
    if (!_drugManager) {
        drugMutex->lock();
        createDrugManager();
        drugMutex->unlock();
    }
    return _drugManager;
}

void AppCore::createDrugManager()
{
    if (!this->_drugManager)
        this->_drugManager = new DrugManager();
}

void AppCore::preloadDrugs ()
{
    drugloader->core = this;
    drugPreloadMutex->lock();
    static bool first = true;
    if (first) {
        drugloader->start(QThread::LowPriority);
        first = false;
    }
    drugPreloadMutex->unlock();
}

} //namespace AppUtils
} //namespace Gui
} //namespace Tucuxi

void Tucuxi::Gui::Core::ThreadDrugLoader::run() {
    core->drugMutex->lock();
    core->createDrugManager();
    core->drugMutex->unlock();
}


