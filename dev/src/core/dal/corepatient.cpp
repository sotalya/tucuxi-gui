//@@license@@

#include "core/dal/corepatient.h"

namespace Tucuxi {
namespace GuiCore {

Tucuxi::GuiCore::CorePatientSet::CorePatientSet(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent, const Tucuxi::GuiCore::CorePatientSet* &other)
{
    foreach (CorePatient* _p, *other) {
        CorePatient* _np = CoreFactory::createEntity<CorePatient>(repository, parent);
        _np->setId(_p->id());
        this->append(_np);
    }
}

QML_POINTERLIST_CLASS_IMPL(CorePatientList, CorePatient)
}
}
