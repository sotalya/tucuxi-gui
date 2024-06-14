/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef MANAGER_H
#define MANAGER_H

/** \defgroup plugins Model/Engines plugins
 * The model and engines are all plugins. Here is a list of the different plugins types:
 *
 * \li \ref models
 * \li \ref peengines
 * \li \ref poengines
 * \li \ref reengines
 * \li \ref factories
 *
 * \defgroup models Models plugins
 * \defgroup peengines Percentiles engines plugins
 * \defgroup poengines Post engines plugins
 * \defgroup reengines Reverse engines plugins
 * \defgroup factories Plugins factories
 */

/** \defgroup pluginsmanager Plugin manager
 * \ingroup core
 * The PluginManager loads and manages the \ref plugins used by the software.
 *
 * \section desc Detailed Description
 * The plugin manager provides an interface to the plugins used by the software.
 * Upon instantiation, the plugin manager will search for any plugins in the default plugins folders.
 * It will then store them in map which will allow the user to retrieve a description for a given ID, or a list of descriptions of all the plugins of a certain type.
 * It will also provide functions to build an instance of a plugin given its ID.
 *
 * Finally, it is possible to add or remove a folder from the search path, which will trigger a new scan of all the plugins.
 *
 * \section trans Note on translation
 * A plugin might contain translations for its descriptions and internal messages.
 * In any case, the software will search a translation file for the language currently used.
 * Therefore, the plugin manager will always try to load the corresponding .qm file from the default translation folder.
 * If not corresponding translation file exists, the default language will be used.
 * The translation file has to be named against the plugin ID, followed by an underscore and the corresponding language code.
 * For example: ch.heig-vd.ezechiel.example_en.qm
 *
 * By default, the current language of the software is used upon instantiation of the plugin manager.
 * It is also possible to set another language at any time, resulting in a new scan of the plugins.
 */

#include <QVariant>
#include <QString>
#include <QList>
#include <QMap>
#include <QPluginLoader>
#include <QDir>

#include "core/interfaces/database.h"
#include "core/interfaces/drugmodelengine.h"
#include "core/utils/ezutils.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


/** Plugin Manager
 * \ingroup manager
 * This object load and manage the plugins and build a list of available drugs.
 */
class PluginManager : public QObject {
   Q_OBJECT

public:

   /** Constructor.
    * This constructor will load each plugin available to the application.
    * The core object is given, because the models/engines part might be compiled as a standalone library.
    */
   PluginManager(Core *core);

   //! Destructor.
   ~PluginManager();

   AbstractRepository * loadDatabase(const DATABASEENGINE&);

   //! Returns the list of directories.
   /** Returns the list of directories in which the plugin manager will search for plugins.
    * By default, the list consists of only the Plugins path defined in the core.
    * @return The list of directories
    */
   QList<QDir> directories() const;

   //! Returns the current language.
   /** Returns the current language used by the plugins when available.
    * The default language is the one returned by the core at application startup.
    * @return The current language
    */
   QString language() const;

   //! Filters the models passed to the function and returns the available models' IDs.
   /** This function takes a list of models IDs, and checks for each model ID if a corresponding model plugin is available.
    * It then returns the list of models IDs that are available. The models IDs passed to the function may contain known
    * models IDs, in which case all the existing standard models compatible with those known models will be added to the
    * result. This function is particularly usefull to filter models IDs returned by the drug manager for a given drug.
    * @param models A list of models IDs and/or known models IDs.
    * @return The list of the available models' IDs.
    * \sa DrugManager
    */
   QList<QString> filter(const QList<QString> &models) const;

   //! Returns the compatible intakes IDs for this models' list.
   /** This function takes a list of models IDs, and retrieves for each one the compatible intake.
    * It then returns a list of intakes without any duplication.
    * @param models A list of models IDs.
    * @return The list of the available intakes' IDs.
    * \sa DefinitionManager
    */
   QList<QString> intakes(const QList<QString> &models) const;

   /** \brief Get a list of available models.
    * @return A list of model's name, description and id.
    */
   QList<ModelDescriptor> models() const;

   /** \brief Get a list of available models IDs.
    * @return A list of model IDs.
    */
   const QStringList modelIds() const;

   /** \brief Get the list of available models for a known model ID.
    * @param knownModelId A known model ID.
    * @return The list of available models' descriptors.
    */
   QList<ModelDescriptor> knownModels(const QString &knownModelId);

   /** \brief Get a model descriptor given it's ID.
    * @return The model descriptor.
    */
   ModelDescriptor model(const QString modelId) const;

   /** \brief Check if a model exists.
    * @param modelId The model ID.
    * @return True if it exists.
    */
   bool modelExists(const QString &modelId) const;

   /** \brief Get the priority of a model.
    * @param modelId The model ID.
    * @return The model priority.
    */
   int modelPriority(const QString &modelId) const;


   //! Add a directory to the scan list.
   /** All the directories will be scan again, and the models/engines maps updated accordingly.
    * @param directory Directory to add.
    * @return True if successfully added, otherwise false.
    */
   bool addDirectory(const QDir &directory);

   //! Remove a directory from the scan list.
   /** All the directories will be scan again, and the models/engines maps updated accordingly.
    * @param directory Directory to remove.
    * @return True if successfully removed, otherwise false.
    */
   bool removeDirectory(const QDir &directory);

   //! Change the plugins language.
   /** All the directories will be scan again, and the models/engines maps updated with the corresponding language if available.
    * @param language The new language.
    */
   void setLanguage(const QString language);

private:
   //Search all directories for plugins
   void scanPlugins();

   //Search for plugins in the given directory
   void scanDirectory(const QDir &directory);

   //Current language
   QString _language;

   //Directories to look into
   QList<QDir> _directories;

   //Map of <Known model ID, Model ID>
   QMultiHash<QString, QString> _km;

   //Map of <Model ID, Model descriptor>
   QMap<QString, ModelDescriptor> _ml;

   //Map of <PE ID, PE descriptor>
   QMap<QString, Descriptor> _el;

   //Map of <PO ID, PO descriptor>
   QMap<QString, Descriptor> _pl;

   //Map of <RE ID, RE descriptor>
   QMap<QString, Descriptor> _rl;

};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // MANAGER_H
