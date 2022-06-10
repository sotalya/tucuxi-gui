//@@license@@

#ifndef COREPATIENT_H
#define COREPATIENT_H

#include "core/dal/entity.h"

namespace Tucuxi {
namespace GuiCore {

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
} // namespace core
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::GuiCore::CorePatient*)
Q_DECLARE_METATYPE(QList<Tucuxi::GuiCore::CorePatient*>)
Q_DECLARE_METATYPE(Tucuxi::GuiCore::CorePatientList*)
Q_DECLARE_METATYPE(Tucuxi::GuiCore::CorePatientSet*)


#endif // COREPATIENT_H
