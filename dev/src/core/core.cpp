#include <QCoreApplication>
#include <QDir>
#include <QLocale>
#include <QString>
#include <QTranslator>

#include "core/core.h"
//#include "coremessagehandler.h"
#include "core/corerepository.h"
#include "core/utils/version.h"
#include "core/pluginmanager.h"
#include "distributor.h"
#include "core/definitionmanager.h"
#include "core/utils/units.h"
#include "core/utils/xmlvalidator.h"
#include "settings.h"
#include "../current_version.h"
#include "core/interfaces/database.h"

#include "databases/ephemeraldb/ephemeraldb.h"
#include "core/errors_core.h"
#include "core/utils/logging.h"


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

namespace ezechiel {
//    class EphemeralDB;
namespace core {

//The settings names
const char *const Core::_stgLanguage       = "language";
const char *const Core::_stgCustomDrugPath = "customDrugPath";

//The core instance
Core *Core::_instance = nullptr;

//If the local path doesn't exists, copy the global one into it
void Core::init()
{
    QDir data (path(ApplicationData));
    if (!data.exists()) {
        //Use a path relative to the executable if it's relative
        QString global = QFileInfo(APPDATA).isAbsolute() ? APPDATA : path(Executable) + "/" + APPDATA;
        LOG(QtDebugMsg, ezechiel::core::NOEZERROR, QObject::tr("Copying the global directory to the local one (%1 -> %2)").arg(global,data.path()));
        Core::cpDir(global,data.absolutePath());
    }
}

//Delete the instance
void Core::quit()
{
    if (_instance) {
        delete _instance;
        //Unset our instance
        _instance = nullptr;
    }
}

//Standard singleton function
Core * Core::instance()
{
    if (!_instance)
        _instance = new Core();
    return _instance;
}

void Core::setInstance(Core *core)
{
    _instance = core;
}

CoreMessageHandler* Core::messageHandler ()
{
    return _messageHandler;
}

void Core::setMessageHandler (CoreMessageHandler* mh)
{
    _messageHandler = mh;
}

DefinitionManager *Core::definitionManager()
{
    if (!_definitionManager)
        _definitionManager = new DefinitionManager();
    return _definitionManager;
}

ProcessingInterface *Core::getProcessingInterface()
{
    Q_ASSERT(_processingInterface != nullptr);
    return _processingInterface;
}


void Core::setProcessingInterface(ProcessingInterface *interface)
{
    _processingInterface = interface;
}




void Core::createPluginManager()
{
    if (!this->_pluginManager)
        this->_pluginManager = new PluginManager(this);
}

void Core::preloadPlugins ()
{
    pluginloader->core = this;
    pluginPreloadMutex->lock();
    static bool first = true;
    if (first) {
        pluginloader->start(QThread::LowPriority);
        first = false;
    }
    pluginPreloadMutex->unlock();
}

void ThreadPluginLoader::run() {
    core->pluginMutex->lock();
    core->createPluginManager();
    core->pluginMutex->unlock();
}


PluginManager* Core::pluginManager ()
{
    if (!_pluginManager)
    {
        pluginMutex->lock();
        createPluginManager();
        pluginMutex->unlock();
    }
    return _pluginManager;
}


//Returns the language
QString Core::language() const
{
    return SETTINGS.get(Module::Core, _stgLanguage).toString();
}

//Sets the language
void Core::setLanguage(QString language)
{
    SETTINGS.set(Module::Core, _stgLanguage, language);
}

//Sets a translator
bool Core::setTranslators(const QStringList &components, const QString &given)
{
    bool result = true;

    //Get the language
    QString language = given.isEmpty() ? this->language() : given;

    //Check the language
    if (language == "en")
        return true;

    //Check the modules
    if (components.isEmpty())
        return true;

    //Translate each component
    foreach(QString component, components) {
        QTranslator *translator = new QTranslator();

        //The current component translation file
        QString translationFile = CORE->path(Core::Translations) + "/" + component + "_" + language;

        //Load the component translation file
        if (!translator->load(translationFile)) {
            LOG(QtWarningMsg, NOEZERROR, QObject::tr("The translation file '%1' could not be found").arg(translationFile));
            result = false;
            continue;
        }

        //Install the compoment translation file
        if (!_application->installTranslator(translator)) {
            LOG(QtWarningMsg, NOEZERROR, QObject::tr("The translation file '%1' could not be installed").arg(translationFile));
            result = false;
            continue;
        }

        //Display a message upon success
        qDebug() << QObject::tr("The translation file '%1' was successfully installed for the component '%2'").arg(translationFile, component);
    }

    return result;
}

//Returns a path
QString Core::path(Core::PathType type)
{
    return _paths[type];
}

//Sets the paths
void Core::setPath(Core::PathType type, const QString &p)
{
    QString path = QDir::cleanPath(p);

    //Ignore if empty
    if (path.isEmpty())
        return;

    //Remove trailing
    if (path.endsWith("/") && path != QDir::rootPath())
        path.remove(path.size() - 1, 1);

    //Update the path
    _paths[type] = path;

    //Update the settings
    if (type == CustomDrugs)
        SETTINGS.set(Module::Core, _stgCustomDrugPath, path);
}

ezechiel::core::AbstractRepository *Core::repository()
{
    if (_repository == nullptr) {
        LOG(QtDebugMsg, ezechiel::core::NOEZERROR, QObject::tr("loading db"));
        _repository = pluginManager()->loadDatabase(SQLITE3);
    }
    return _repository;
}
/*
Database * Core::database()
{
    if (!_database)
        _database = new Database();
    return _database;
}

DBInterface * Core::db(QString db)
{
    return database()->getDB(db);
}

ezechiel::IDatabase * Core::newdb(QString newdb) {
    return database()->getNewDB(newdb);
}
*/

QString Core::buildID()
{
    return BUILD_ID;
}

Version Core::version(VersionType vType)
{
    switch (vType) {
    case Tucuxi: return Version(EZECHIEL_VERSION);
    case Editor:   return Version(EDITOR_VERSION);
    case Data:     return Version(DATABASE_VERSION);
    default:       return Version();
    }
}

QCoreApplication * Core::application()
{
    return _application;
}

Distributor * Core::distributor()
{
    if (!_distributor)
        _distributor = new Distributor();
    return _distributor;
}

UnitsRegister *Core::unitsRegister()
{
    if (!_unitsRegister)
        _unitsRegister = new UnitsRegister();
    return _unitsRegister;
}

XmlValidator *Core::xmlValidator()
{
    if (!_xmlValidator)
        _xmlValidator = new XmlValidator();
    return _xmlValidator;
}

Settings &Core::settings() const
{
    return Settings::getInstance();
}

//------------------------- PRIVATE --------------------------

//Recursively copy a directory, as Qt doesn't seems to do it
//This piece of code comes from http://stackoverflow.com/questions/2536524/copy-directory-using-qt
//All thanks to roop (http://stackoverflow.com/users/536368/roop)
bool Core::cpDir(const QString &srcPath, const QString &dstPath)
{
    QDir parentDstDir(QFileInfo(dstPath).path());
    if (!parentDstDir.mkdir(QFileInfo(dstPath).fileName()))
        return false;

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath))
                return false;
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath))
                return false;
        } else {
            LOG(QtWarningMsg, NOEZERROR, QString("Unhandled item : %1").arg(info.filePath()));
        }
    }
    return true;
}

//Constructor
Core::Core() : _runningMode(Core::CLI), _messageHandler(nullptr), _definitionManager(nullptr),
    _pluginManager(nullptr), _repository(nullptr),  /*_database(nullptr), */_application(nullptr),
    _distributor(nullptr), _unitsRegister(nullptr), _xmlValidator(nullptr),
    _processingInterface(nullptr)
{
    //Initialize the instance
    _instance = this;

    //Initialize the QApplication
    _application = QCoreApplication::instance();

    //Initialize the mutexes
    pluginMutex = new QMutex();
    pluginPreloadMutex = new QMutex();

    //Initialize the loaders
    pluginloader = new ThreadPluginLoader();

    //Initialize the paths
    initPaths();

    //Initialize the language
    initLanguage();
}

//Destructs the pseudo-singletons
Core::~Core ()
{
    if (_repository)
        delete _repository;
//    if (_database)
//        delete _database;
    if (_definitionManager)
        delete _definitionManager;
    if (_pluginManager)
        delete _pluginManager;
    if (pluginMutex)
        delete pluginMutex;
    if (pluginPreloadMutex)
        delete pluginPreloadMutex;
    if (pluginloader)
        delete pluginloader;
}


Core::RunningMode Core::runningMode() const
{
    return _runningMode;
}
void Core::setRunningMode(Core::RunningMode mode)
{
    _runningMode = mode;
}


//Initializes the paths
void Core::initPaths()
{
    //Set the executable path
    _paths[Executable] = _application->applicationDirPath();

    //Set the modules paths
    _paths[ApplicationData] = _paths[Executable];
#ifdef Q_OS_MACX
    _paths[ApplicationData] = _paths[Executable] + "/../Resources";
#endif
#ifdef Q_OS_ANDROID
    _paths[ApplicationData] = "assets:";
#endif
    _paths[Databases]    = _paths[ApplicationData] + "/dbs";
    _paths[Drugs]        = _paths[ApplicationData] + "/drugs";
    _paths[Drugs2]       = _paths[ApplicationData] + "/drugfiles";
    _paths[Plugins]      = _paths[ApplicationData] + "/plugins";
    _paths[PluginsData]  = _paths[ApplicationData] + "/data";
    _paths[Translations] = _paths[ApplicationData] + "/trans";
    _paths[Report]       = _paths[ApplicationData] + "/report";

    //Set the report module paths
    _paths[ReportTemplates]  = _paths[Report] + "/templates";

    //Set the temporary paths
    _paths[Temporary] = QDir::tempPath();

    //Set the settings dependent paths
    _paths[CustomDrugs] = SETTINGS.get(Module::Core, _stgCustomDrugPath, QVariant(QDir::homePath() + "/drugs")).toString();

    //Clean all the paths
    for (int i = 0; i < LAST; i++)
        _paths[i] = QDir::cleanPath(_paths[i]);
}

//Inializes the language
void Core::initLanguage()
{
    QString language;

    //Get the setting and locale languages
    QString configLanguage = this->language();
    QString localeLanguage = QLocale::system().name().toLower();

    //Truncate the locale language
    localeLanguage.truncate(2);

    //Set the current language
    if (configLanguage.isEmpty() || configLanguage == localeLanguage)
        language = localeLanguage;
    else
        language = configLanguage;

    //Store the current language
    setLanguage(language);
}

} // namespace core
} // namespace ezechiel

