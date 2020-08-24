#ifndef LOCATION_H
#define LOCATION_H

#include "core/dal/entity.h"
#include "core/utils/autoproperty.h"

class Location : public ezechiel::core::Entity
{
    Q_OBJECT
    ADMIN_ENTITY_UTILS(Location)
    AUTO_PROPERTY(QString, address)
    AUTO_PROPERTY(QString, postcode)
    AUTO_PROPERTY(QString, city)
    AUTO_PROPERTY(QString, state)
    AUTO_PROPERTY(QString, country)

public:
    Q_INVOKABLE Location(ezechiel::core::AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE Location(ezechiel::core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

    QVariant toQVariant(void) const;
};

Q_DECLARE_METATYPE(Location*)
Q_DECLARE_METATYPE(QList<Location*>)

#endif // LOCATION_H
