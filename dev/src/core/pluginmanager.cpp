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


#include "core/pluginmanager.h"

#include <QDir>
#include <QDebug>
#include <QTranslator>
#include <QCoreApplication>
#include <QString>

#include "core/interfaces/database.h"
//#include "coremessagehandler.h"
//#include "core_errors.h"
#include "core/core.h"
#include "core/definitionmanager.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"
#include "core/corerepository.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


//Constructor
PluginManager::PluginManager(Core *core)
{
    // ToDo /////////////////////////////////
    // Shouldn't be necessary to set the core
    // //////////////////////////////////////

    //Set the core object
    Core::setInstance(core);

    //Set the default language
    _language = CORE->language();

    //Set the directories to look into
    _directories.append(QDir(CORE->path(Core::Plugins)));

    //Scan all available plugins
    scanPlugins();
}

//Destructor
PluginManager::~PluginManager()
{

}

//Returns the directories list
QList<QDir> PluginManager::directories() const
{
    return _directories;
}

//Returns the currently used language
QString PluginManager::language() const
{
    return _language;
}

//Returns a list of models matching the given list of models IDs
QList<QString> PluginManager::filter(const QList<QString> &models) const
{
    QList<QString> mids;

    //Check if the given models contains known models IDs
    foreach (QString modelId, models)
        if (_km.contains(modelId))
            mids.append(_km.values(modelId));

    //Check if the given models contains standard models IDs
    foreach (QString modelId, models)
        if (_ml.contains(modelId) && !mids.contains(modelId))
            mids.append(modelId);

    return mids;
}

//Returns the compatible intakes for this models list.
QList<QString> PluginManager::intakes(const QList<QString> &models) const
{
    QList<QString> iids;

    //Add the corresponding intakes only once
    foreach (QString modelId, models) {
        Descriptor intake = CORE->definitionManager()->intake(model(modelId).intake);

        if (!intake.isValid())
            break;

        if (!iids.contains(intake.id))
            iids.append(intake.id);
    }

    return iids;
}

//-------------- GETTERS ----------------

//Returns the list of models
QList<ModelDescriptor> PluginManager::models() const
{
    return _ml.values();
}


const QStringList PluginManager::modelIds() const
{
    QStringList list;
    list = _km.keys();
    list.removeDuplicates();
    list.sort();
    return list;
}

//Returns the list of models of known model
QList<ModelDescriptor> PluginManager::knownModels(const QString &knownModelId)
{
    QList<ModelDescriptor> models;

    //Get the corresponding models descriptors
    foreach (QString modelId, _km.values(knownModelId))
        models.append(_ml.value(modelId));

    //Return the available models descriptors
    return models;
}

//Returns a model descriptor
ModelDescriptor PluginManager::model(const QString modelId) const
{
    return _ml.value(modelId);
}

//Checks if a model exists
bool PluginManager::modelExists(const QString &modelId) const
{
    return _ml.contains(modelId);
}



//-------------- NEW INSTANCES ----------------

//For the new instances, use the model factory to create the wanted object



//Add a directory to scan list
bool PluginManager::addDirectory(const QDir &directory)
{
    //Check if the directory is not already included
    if (_directories.contains(directory))
        return false;

    //Add the directory
    _directories.append(directory);

    //Scan all available drug files
    scanPlugins();

    return true;
}

//Remove a directory to scan list
bool PluginManager::removeDirectory(const QDir &directory)
{
    //Find the corresponding index
    int index = _directories.indexOf(directory);

    //Abort if not found
    if (index == -1)
        return false;

    //Remove the directory
    _directories.removeAt(index);

    //Scan all available drug files
    scanPlugins();

    return true;
}

//Sets the application language
void PluginManager::setLanguage(const QString language)
{
    //Set the new current language
    _language = language;

    //Scan all available drug files
    scanPlugins();
}

//-------------- PRIVATE ----------------

//Scan all plugins
void PluginManager::scanPlugins()
{
    //Remove the existing data
    _km.clear();
    _ml.clear();
    _el.clear();
    _pl.clear();
    _rl.clear();

    //Scan each one of the directories
    foreach (QDir dir, _directories)
        scanDirectory(dir);
}

AbstractRepository * PluginManager::loadDatabase(const DATABASEENGINE &engine) {
    QPluginLoader *loader;
    //Plugins ID list
    QList<QString> pluginsId;

    AbstractRepository* repo;
    LOG(QtDebugMsg, NOEZERROR, FILEPATH(Core::Databases));
    foreach (QFileInfo file, (QDir(FILEPATH(Core::Databases))).entryInfoList(QDir::Files)) {
//        //If the file is not readable, error & continue
        if ( ! file.isReadable() ) {
            LOG(QtFatalMsg, NOEZERROR, tr("The file %1 is not readable").arg(file.filePath()));
            continue;
        }
        //Load the plugin
//        qDebug() << "Loading " + file.fileName();
#ifdef Q_OS_ANDROID
        loader = new QPluginLoader("libephemeraldb.so");
#else
        loader = new QPluginLoader(file.filePath(),this);
#endif
        if (!loader->load()){
            LOG(QtWarningMsg, NOEZERROR, tr("Unable to load the db %1 with error %2").arg(file.filePath()).arg(loader->errorString()));
        } else {
            if ((repo = qobject_cast<CoreRepository*>(loader->instance()))) {
                LOG(QtDebugMsg, Tucuxi::Gui::Core::NOEZERROR, QObject::tr("casted db, loaded plugin"));
                return repo;
            }
            else {
                LOG(QtFatalMsg, Tucuxi::Gui::Core::NOEZERROR, QObject::tr("Could not cast db"));
            }
        }
    }
}

//For each plugins directories, search for plugins, load the factories and save them
void PluginManager::scanDirectory(const QDir &directory)
{
//    qDebug() << "Scanning " + directory.absolutePath();

    //Base objects
    QPluginLoader *loader;

    //Plugins ID list
    QList<QString> pluginsId;

    //For file inside
    foreach (QFileInfo file, directory.entryInfoList(QDir::Files)) {

        //If the file is not readable, error & continue
        if ( ! file.isReadable() ) {
            LOG(QtWarningMsg, NOEZERROR, tr("The file %1 is not readable").arg(file.filePath()));
            continue;
        }

        //Load the plugin
//        qDebug() << "Loading " + file.fileName();
        loader = new QPluginLoader(file.filePath());
        if (!loader->load()){
            LOG(QtWarningMsg, NOEZERROR, tr("Unable to load the model") + " " + file.filePath());
            qDebug() << "Error : " + loader->errorString();
        }

        //Now we try to cast the plugin to find out its type

        if (false) {

        }

        //Else, error (destroy the loader in any case)
        else {
            LOG(QtWarningMsg, NOEZERROR, tr("That's strange, I was able to load this plugin, but it doesn't corresponds to any of the type I accept. Alien plugin?"));
            delete loader;
            continue;
        }
        delete loader;
    }

    //Translations path
    QString transDir = CORE->path(Core::Translations);

    //For each engines and models
    foreach(QString id, pluginsId) {

        //Load the translator for the current language
        QFileInfo transFile(QString("%1/%2_%3.qm").arg(transDir, id, _language));

        //If it exists, load the translation
        if (transFile.isReadable()) {
            QTranslator *t = new QTranslator();
            if (t->load(transFile.filePath())) {
                CORE->application()->installTranslator(t);
                LOG(QtDebugMsg, NOEZERROR, "Loaded a translation file for " + id + " (" + transFile.filePath() + ")");
            } else {
                LOG(QtWarningMsg, NOEZERROR, tr("Unable to load the translation file for %1").arg(id));
            }
        }
    }
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

