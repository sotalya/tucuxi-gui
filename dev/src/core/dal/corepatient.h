//@@license@@

#ifndef COREPATIENT_H
#define COREPATIENT_H

#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class CorePatient : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(CorePatient)

protected:
    Q_INVOKABLE CorePatient(AbstractRepository *repository = 0, QObject *parent = nullptr) : Entity(repository, parent) {}
    Q_INVOKABLE CorePatient(AbstractRepository *repository, const int &id, QObject *parent = nullptr) : Entity(repository, parent) {setId(id);}
};

class CorePatientSet : public QList<CorePatient*>
{
public :
    CorePatientSet(AbstractRepository *repository = 0, QObject* parent = 0) {}
    CorePatientSet(AbstractRepository *repository, QObject* parent, const CorePatientSet* &other);
};

QML_POINTERLIST_CLASS_DECL(CorePatientList, CorePatient)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CorePatient*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::CorePatient*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CorePatientList*)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CorePatientSet*)


#endif // COREPATIENT_H
