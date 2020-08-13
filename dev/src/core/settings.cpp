#include "settings.h"

#include <QMutexLocker>

namespace ezechiel {
namespace core {


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

Settings::~Settings()
{

}

//ParameterGraphSettings::ParameterGraphSettings()
//{}

//SingleGraphSettings::SingleGraphSettings()
//{}

//GraphDisplaySettings::GraphDisplaySettings()
//{}

} // namespace core
} // namespace ezechiel

