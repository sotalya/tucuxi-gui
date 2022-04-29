//@@license@@

#ifndef COREPATIENT_H
#define COREPATIENT_H

#include "core/dal/entity.h"

namespace ezechiel {
namespace core {

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
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::CorePatient*)
Q_DECLARE_METATYPE(QList<ezechiel::core::CorePatient*>)
Q_DECLARE_METATYPE(ezechiel::core::CorePatientList*)
Q_DECLARE_METATYPE(ezechiel::core::CorePatientSet*)


#endif // COREPATIENT_H
