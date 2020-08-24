#ifndef PRACTICIAN_H
#define PRACTICIAN_H

#include "core/dal/entity.h"
#include "person.h"
#include "institute.h"


class Practician : public ezechiel::core::Entity
{
    Q_OBJECT
    ADMIN_ENTITY_UTILS(Practician)
    AUTO_PROPERTY(QString, externalId)
    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY(Person*, person)
    AUTO_PROPERTY(int, person_id)
    AUTO_PROPERTY(Institute*, institute)
    AUTO_PROPERTY(int, institute_id)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

public:
    Q_INVOKABLE Practician(ezechiel::core::AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE Practician(ezechiel::core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);
    Q_INVOKABLE void saveToSettings(void);
    Q_INVOKABLE QVariant loadSettings(void);


};
Q_DECLARE_METATYPE(Practician*)
Q_DECLARE_METATYPE(QList<Practician*>)

class PracticianSet : public QList<Practician*>
{
public :
    PracticianSet(ezechiel::core::AbstractRepository *repository, QObject* parent = 0) {}
    PracticianSet(ezechiel::core::AbstractRepository *repository, QObject* parent, const PracticianSet* &other);
};
Q_DECLARE_METATYPE(PracticianSet*)

typedef Practician* SharedPractician;
typedef PracticianSet* SharedPracticianSet;

#endif // PRACTICIAN_H
