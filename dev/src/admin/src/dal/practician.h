#ifndef PRACTICIAN_H
#define PRACTICIAN_H

#include "core/dal/entity.h"
#include "person.h"
#include "institute.h"

namespace Tucuxi {
namespace Gui {
namespace Admin {

class Practician : public Tucuxi::Gui::Core::Entity
{
    Q_OBJECT
    ADMIN_ENTITY_UTILS(Practician)
    AUTO_PROPERTY(QString, externalId)
    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY(QString, role)
    AUTO_PROPERTY(Person*, person)
    AUTO_PROPERTY(int, person_id)
    AUTO_PROPERTY(Institute*, institute)
    AUTO_PROPERTY(int, institute_id)

    Q_INVOKABLE bool isValid() Q_DECL_OVERRIDE
    {
        return true;
    }

//    void copyTo(Practician *other);

public:
    Q_INVOKABLE Practician(Tucuxi::Gui::Core::AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE Practician(Tucuxi::Gui::Core::AbstractRepository *repository, const int &id, QObject *parent = nullptr);

};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::Practician*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Admin::Practician*>)

namespace Tucuxi {
namespace Gui {
namespace Admin {

class PracticianSet : public QList<Practician*>
{
public :
    PracticianSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent = 0) {}
    PracticianSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent, const PracticianSet* &other);
};

}
}
}

Q_DECLARE_METATYPE(Tucuxi::Gui::Admin::PracticianSet*)

typedef Tucuxi::Gui::Admin::Practician* SharedPractician;
typedef Tucuxi::Gui::Admin::PracticianSet* SharedPracticianSet;

#endif // PRACTICIAN_H
