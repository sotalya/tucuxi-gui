//@@license@@

#include "core/dal/corepatient.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


Tucuxi::Gui::Core::CorePatientSet::CorePatientSet(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent, const Tucuxi::Gui::Core::CorePatientSet* &other)
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
}
