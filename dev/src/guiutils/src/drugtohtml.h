//@@license@@

#ifndef DRUGTOHTML_H
#define DRUGTOHTML_H

#include <QString>

namespace Tucuxi {
namespace GuiCore {
class DrugModel;
class ActiveSubstance;
}
}

class DrugToHtml
{
public:
    DrugToHtml();

    QString drugToHtml(const Tucuxi::GuiCore::DrugModel* drug);
    QString activeSubstanceToHtml(const Tucuxi::GuiCore::ActiveSubstance* drug);
};

#endif // DRUGTOHTML_H
