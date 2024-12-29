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


#include "settings.h"

#include <QMutexLocker>

namespace Tucuxi {
namespace Gui {
namespace Core {



Settings &Settings::getInstance()
{
    static Settings settings;
    return settings;
}

void Settings::clearModule(Module module)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Clear the settings
    remove(root);
}

void Settings::clearClass(Module module, const QString &className)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Clear the settings
    remove(root.append("/").append(className));
}

void Settings::clearSetting(Module module, const QString &name)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Clear the setting
    remove(root.append("/").append(name));
}

void Settings::clearSetting(Module module, const QString &className, const QString &name)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Clear the setting
    remove(root.append("/").append(className).append("/").append(name));
}

QVariant Settings::get(Module module, const QString &name, const QVariant &defaultValue)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Return the value
    return value(root.append("/").append(name), defaultValue);
}

QVariant Settings::get(Module module, const QString &className, const QString &name, const QVariant &defaultValue)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Return the value
    return value(root.append("/").append(className).append("/").append(name), defaultValue);
}

QVariant Settings::get(Module module, const QString &className, const char *name, const QVariant &defaultValue)
{
    return get(module, className, QString(name), defaultValue);
}

QMap<QString, QVariant> Settings::getSettings(Module module, const QMap<QString, QVariant> &settings)
{
    QMap<QString, QVariant> result;

    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Return the values
    beginGroup(root);
    for (QMap<QString, QVariant>::const_iterator i = settings.constBegin(); i != settings.constEnd(); ++i)
        result.insert(i.key(), value(i.key(), i.value()));
    endGroup();

    return result;
}

QMap<QString, QVariant> Settings::getSettings(Module module, const QString &className, const QMap<QString, QVariant> &settings)
{
    QMap<QString, QVariant> result;

    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Return the values
    beginGroup(root);
    beginGroup(className);
    for (QMap<QString, QVariant>::const_iterator i = settings.constBegin(); i != settings.constEnd(); ++i)
        result.insert(i.key(), value(i.key(), i.value()));
    endGroup();
    endGroup();

    return result;
}

void Settings::set(Module module, const QString &name, const QVariant &value)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Set the value
    setValue(root.append("/").append(name), value);
}

void Settings::set(Module module, const QString &className, const QString &name, const QVariant &value)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Set the value
    setValue(root.append("/").append(className).append("/").append(name), value);
}

void Settings::setSettings(Module module, const QMap<QString, QVariant> &settings)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Set the values
    beginGroup(root);
    for (QMap<QString, QVariant>::const_iterator i = settings.constBegin(); i != settings.constEnd(); ++i)
        setValue(i.key(), i.value());
    endGroup();
}

void Settings::setSettings(Module module, const QString &className, const QMap<QString, QVariant> &settings)
{
    //Get the module name
    QString root = _modules.value(module);
    Q_ASSERT(!root.isEmpty());

    //Lock the mutex
    QMutexLocker locker(&_mutex);

    //Set the values
    beginGroup(root);
    beginGroup(className);
    for (QMap<QString, QVariant>::const_iterator i = settings.constBegin(); i != settings.constEnd(); ++i)
        setValue(i.key(), i.value());
    endGroup();
    endGroup();
}

Settings::Settings(QObject *parent) : QSettings("Sotalya", "Tucuxi", parent)
//Settings::Settings(QObject *parent) : QSettings("tucuxi_settings.ini", QSettings::IniFormat, parent)
{
    //Associate a name for each module
    _modules.insert(Module::Chart,    "chart");
    _modules.insert(Module::CLI,      "cli");
    _modules.insert(Module::Core,     "core");
    _modules.insert(Module::Database, "database");
    _modules.insert(Module::GUI,      "gui");
    _modules.insert(Module::Plugin,   "plugin");
    _modules.insert(Module::Report,   "report");
}

Settings::~Settings() = default;

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

