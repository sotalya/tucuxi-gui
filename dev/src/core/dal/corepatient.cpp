//@@license@@

#include "core/dal/corepatient.h"

namespace ezechiel {
namespace GuiCore {

ezechiel::GuiCore::CorePatientSet::CorePatientSet(ezechiel::GuiCore::AbstractRepository *repository, QObject *parent, const ezechiel::GuiCore::CorePatientSet* &other)
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
