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

#ifndef APPCORE_H
#define APPCORE_H

#include "core/core.h"

namespace Tucuxi {
namespace Gui {
namespace Core {

    class SecurityManager;
}
}
}

#include <QString>
#include <typeinfo>


//------------- Shortcuts for objects ----------------
#define APPCORE (static_cast<Tucuxi::Gui::AppUtils::AppCore *>(Tucuxi::Gui::Core::Core::instance()))
#define SECURITY (static_cast<Tucuxi::Gui::AppUtils::AppCore *>(Tucuxi::Gui::Core::Core::instance())->securityManager())


namespace Tucuxi {
namespace Gui {
namespace AppUtils {
class DrugManager;

//! \brief Core main object
/** \ingroup core
   This singleton contains all the core objects.
   An instance of each of these objects can be retrieved and set here.
   The difference between a normal singleton is that an instance of core can be given, thus enabling a library to load the main core and then use it
  */

class AppCore : public Tucuxi::Gui::Core::Core
{
public:

    //! Get the security manager.
    Tucuxi::Gui::Core::SecurityManager *securityManager();

    //Constructor
    AppCore();

    Tucuxi::Gui::Core::ThreadDrugLoader *drugloader;

    void preloadDrugs ();
    void createDrugManager ();

    //! Get the drugs manager.
    DrugManager *drugManager ();

    QMutex *drugPreloadMutex;

    QMutex *drugMutex;
private:

    DrugManager *_drugManager;

    //Destructor
    ~AppCore();

    //The pseudo-singletons
    Tucuxi::Gui::Core::SecurityManager *_securityManager;

};
} //namespace AppUtils
} //namespace Gui
} //namespace Tucuxi

class Tucuxi::Gui::Core::ThreadDrugLoader : public QThread
{
    Q_OBJECT
public:

    Tucuxi::Gui::AppUtils::AppCore *core;

    void run();
};
#endif //APPCORE_H
