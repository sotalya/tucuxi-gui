//@@license@@

/** \mainpage Tucuxi code documentation
 *
 * Tucuxi is a medical software designed to predict the drug concentration in a patient's body.
 * It is an add-on to the ISyPeM project. It will allow to better adjust drug posology of a patient that needs to take medicine every day.
 * For now practitioners use empirical methods to find the right posology, for which results might depends on the practitioner own knowledge and experiences.
 * They also usually ask pharmacologists about the analysis of a measure result.
 * With Tucuxi they will possess a software tool able to predict the chronological evolution of drug concentration, calculated with strong models built on the population statistical data.
 * An adaptation to the current posology will also be proposed.
 *
 * This documentation is for developpers only. For general purpose documentation, see the specifications in ezechiel/dev/doc/specification/ezechiel_spec.pdf
 *
 * Internal groups :
 * \li \ref core "Core" : Base object to access database, settings, managers, etc.
 * \li \ref gui "GUI" : The Graphical User Interface part
 * \li \ref cli "CLI" : The Command Line Interface part
 */

/** \defgroup core The Tucuxi's Core library
 * The Tucuxi's Core library is a reunion of libraries that do not depend on
 * an interface and depends only on QtCore library enabling it to be build for many platforms.
 * The Core object is a super-singleton : a singleton that can be set across multiple threads and library.
 * Some objects in the Core library are meant to be unique (such as PluginManager, SecurityManager, Settings, etc).
 * Instead of using the singleton patterns multiple times, the Core object create and save on instance of each, and provide public getters for them.
 *
 * \section list List of parts
 * \li \ref database "Database" : Contains an unified access to the different database backends and handle the plugins that give an access to the real storing method.
 * \li \ref databaseplugins "Database plugins" : The backends for the database system.
 * \li \ref drugmanager "Drug manager" : Handles the xml drug files and provides access to the corresponding drug objects.
 * \li \ref pluginsmanager "Plugins manager" : Handles the different models and engines.
 * \li \ref plugins "Models and engines plugins" : Models and percentile/post/reverse engines are plugins, as different methods could be used for the same result.
 * \li \ref settings "Settings" : Get and set the user settings, and more
 * \li \ref security "Security" : Manage the users, the login system and the data encryption
 * \li \ref utils "Utilitaries" : A set of objects used as utilitaries by the others
 *
 * Moreover, an message/error system is implemented project-wide, see \ref errors
 */

/** \defgroup utils Utilitaries
 * These object are not some big-bulldozer object, but offers some background helper functions for the rest of the \ref core
 */

#ifndef CORE_H
#define CORE_H



#include <QString>
#include <typeinfo>

#include <QThread>
#include <QSemaphore>
#include <QMutex>

class QCoreApplication;

namespace ezechiel {

class IDatabase;

namespace core {

class DefinitionManager;
class PluginManager;
class CoreMessageHandler;
class CoreRepository;
class AbstractRepository;
//class DBInterface;
//class Database;
class Version;
class Distributor;
class UnitsRegister;
class XmlValidator;
class Settings;
class ProcessingInterface;

//------------- Shortcuts for objects ----------------
#define CORE ezechiel::core::Core::instance()
#define DEFINITIONS ezechiel::core::Core::instance()->definitionManager()
#define ABSTRACTREPO ezechiel::core::Core::instance()->repository()
#define REPO dynamic_cast<ezechiel::core::CoreRepository*>(ABSTRACTREPO)
//#define DB Core::instance()->db()
//#define NEWDB Core::instance()->newdb()
#define FILEPATH(ARG) ezechiel::core::Core::instance()->path(ARG)
#define DATABASE ezechiel::core::Core::instance()->database()
#define PLUGINS ezechiel::core::Core::instance()->pluginManager()
#define XMLVALIDATOR ezechiel::core::Core::instance()->xmlValidator()
#define SETTINGS ezechiel::core::Core::instance()->settings()

//! Use this macro to send an error
//#define EZERROR(e,m) CORE->messageHandler()->error(e,__LINE__,__FILE__,typeid(this).name(),QString(m))
////! Use this macro to send a warning message
//#define WARNING(m) CORE->messageHandler()->warning(__LINE__,__FILE__,typeid(this).name(),QString(m))
////! Use this macro to send a debug message
//#define DEBUG(m) CORE->messageHandler()->debug(__LINE__,__FILE__,typeid(this).name(),QString(m))

////! Use this macro to send an error (static version)
//#define SEZERROR(n,e,m) CORE->messageHandler()->error(e,__LINE__,__FILE__,n,QString(m))
////! Use this macro to send a warning message (static version)
//#define SWARNING(n,m) CORE->messageHandler()->warning(__LINE__,__FILE__,n,QString(m))
////! Use this macro to send a debug message (static version)
//#define SDEBUG(n,m) CORE->messageHandler()->debug(__LINE__,__FILE__,n,QString(m))


class ThreadPluginLoader;
class ThreadDrugLoader;

//! \brief Core main object
/** \ingroup core
   This singleton contains all the core objects.
   An instance of each of these objects can be retrieved and set here.
   The difference between a normal singleton is that an instance of core can be given, thus enabling a library to load the main core and then use it
  */
class Core
{
public:
    //! Paths types
    enum PathType {
        Executable,
        Drugs,
        Drugs2,
        CustomDrugs,
        Plugins,
        PluginsData,
        Databases,
        ApplicationData,
        Temporary,
        Translations,
        Report,
        ReportTemplates,
        LAST //Do not use
    };

    //! Version types
    enum VersionType {
        Tucuxi,
        Editor,
        Data
    };

    enum RunningMode {
        CLI,
        GUI
    };

    // This mode can be used to act differently depending on the mode.
    // To be set in cli/main.cpp and gui/main.cpp
    RunningMode runningMode() const;
    void setRunningMode(RunningMode mode);

    RunningMode _runningMode;

    //! Initialise the directories
    //! Create the ezechiel's directory if it doesn't exists and copy the global content to it
    void init ();

    //! Delete the Core part
    /** Will delete all the contained objects.
    * Any call to getInstance() will create a new Core object.
    */
    static void quit ();

    //! Get the Core's instance
    static Core* instance ();
    //! Set the Core's instance
    static void setInstance (Core *core);

    //---------------- STATIC OBJECTS --------------------

    //! Get the message handler.
    CoreMessageHandler* messageHandler ();
    //! Set the message handler.
    void setMessageHandler (CoreMessageHandler* mh);

    //! Get the Definitions instance.
    DefinitionManager* definitionManager ();

    void preloadPlugins ();

    void createPluginManager();

    ThreadPluginLoader *pluginloader;

    QMutex *pluginMutex;
    QMutex *pluginPreloadMutex;


    QSemaphore *pluginsSem;

    QSemaphore *drugSem;

    //! Get the plugins manager.
    PluginManager *pluginManager ();


    AbstractRepository *repository ();

    //! Get the database object.
//    Database* database ();

//    ezechiel::IDatabase* newdb(QString bewdb = "");

    //! Directly get the database to use.
    //! \sa DB::getDB()
    //DBInterface* db (QString db = "");

    //! Get the QApplication running.
    QCoreApplication* application ();

    //! Get the distributor instance (channel already set).
    Distributor* distributor ();

    //! Get the unit register instance.
    UnitsRegister* unitsRegister ();

    //! Get the XML validator
    XmlValidator* xmlValidator();

    //! Get the settings.
    Settings &settings() const;

    //-------------- INFOS -------------------

    //! Get the Core library build ID.
    QString buildID ();

    //! Get the current Tucuxi's version.
    Version version (VersionType vType);

    //-------------- UTILITIES ---------------

    //! Get the absolute path of the given path type.
    /** Get the absolute path of the given path type. The path will not have a trailing file separator
     * unless it is the root path.
     * @param type The path type.
     * @return The absolute path.
     */
    QString path(PathType type);

    //! Set the path of the given path type.
    /** Set the path of the given path type. The path must be absolute. If it ends by a trailing
     * file separator, the separator will be removed unless it is the root path.
     * @param type The path type.
     * @param path The absolute path.
     */
    void setPath(PathType type, const QString &path);

    //! Get the current language.
    /** Get the current language from the application settings.
     * @return The current language.
     */
    QString language() const;

    //! Set the language.
    /** Set the language and store it in the application settings.
     * @param language The language.
     */
    void setLanguage(QString language);

    //! Set translators for the given components and language.
    /** Set translators for the given components and language. If the language is omitted, the
     * translator will be set for the current language.
     *
     * Note: The translators MUST be set for any language different than english BEFORE any widget
     * is created in order for the application to be translated correctly.
     * @param components The components list for which the translators must be set.
     * @param The language for which the translators must be set. If omitted, the translators will
     * be set for the current language.
     * @return True if the translators were successfully set for each component.
     */
    bool setTranslators(const QStringList &components, const QString &language = QString());

    ProcessingInterface *getProcessingInterface();
    void setProcessingInterface( ProcessingInterface *);

protected:
    //Constructor
    Core();

    //Destructor
    virtual ~Core();

private:
    //Copies a directory
    static bool cpDir(const QString &srcPath, const QString &dstPath);

    //The settings names
    static const char *const _stgLanguage;
    static const char *const _stgCustomDrugPath;

    //The core instance
    static Core *_instance;

    //Initializes the paths
    void initPaths();

    //Initializes the language
    void initLanguage();

    //The pseudo-singletons
    CoreMessageHandler *_messageHandler;
    DefinitionManager *_definitionManager;
    PluginManager *_pluginManager;
    AbstractRepository *_repository;
//    Database *_database;
    QCoreApplication *_application;
    Distributor *_distributor;
    UnitsRegister *_unitsRegister;
    XmlValidator *_xmlValidator;
    ProcessingInterface *_processingInterface;

    //The application paths
    QString _paths[LAST];
};


class ThreadPluginLoader : public QThread
{
    Q_OBJECT
public:

    Core *core;

    void run();
};

} // namespace core
} // namespace ezechiel


#endif //CORE_H
