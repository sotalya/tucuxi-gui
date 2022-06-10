//@@license@@

#ifndef DRUGTOHTML_H
#define DRUGTOHTML_H

#include <QString>

namespace Tucuxi {
namespace Gui {
namespace Core {

class DrugModel;
class ActiveSubstance;
}
}
}

class DrugToHtml
{
public:
    DrugToHtml();

    QString drugToHtml(const Tucuxi::Gui::Core::DrugModel* drug);
    QString activeSubstanceToHtml(const Tucuxi::Gui::Core::ActiveSubstance* drug);
};

#endif // DRUGTOHTML_H
