#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>

class Settings : public QObject
{
    Q_OBJECT

public:
    static Settings &getInstance();

    void set(const QString &key, const QVariant &value);
    QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;
    bool isSet(const QString &key) const;

private:
    explicit Settings();
    virtual ~Settings() Q_DECL_OVERRIDE;

    QMap<QString, QVariant> settings;
};

#endif // SETTINGS_H
