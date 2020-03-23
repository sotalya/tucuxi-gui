#include "core/dal/corepatient.h"

namespace ezechiel {
namespace core {

ezechiel::core::CorePatientSet::CorePatientSet(ezechiel::core::AbstractRepository *repository, QObject *parent, const ezechiel::core::CorePatientSet* &other)
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
