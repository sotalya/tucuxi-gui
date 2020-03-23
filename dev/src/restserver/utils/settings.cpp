#include "settings.h"

Settings &Settings::getInstance()
{
    static Settings settings;
    return settings;
}

void Settings::set(const QString &key, const QVariant &value)
{
    settings.insert(key, value);
}

QVariant Settings::get(const QString &key, const QVariant &defaultValue) const
{
    return settings.value(key, defaultValue);
}

bool Settings::isSet(const QString &key) const
{
    return settings.contains(key);
}

Settings::Settings() :
    QObject(),
    settings()
{

}

Settings::~Settings()
{

}
