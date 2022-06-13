#include "apputils/src/appcore.h"
#include "apputils/src/securitymanager.h"
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
Tucuxi::Gui::Core::SecurityManager *AppCore::securityManager()
{
    if (!_securityManager)
        _securityManager = &Tucuxi::Gui::Core::SecurityManager::instance();
    return _securityManager;

 // TODO Check why this doesn't work (it creates 2 instances)
 //   return &SecurityManager::instance();
}


//Constructor
AppCore::AppCore() : _securityManager(0), _drugManager(0)
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


