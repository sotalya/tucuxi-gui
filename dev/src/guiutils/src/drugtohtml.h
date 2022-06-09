//@@license@@

#ifndef DRUGTOHTML_H
#define DRUGTOHTML_H

#include <QString>

namespace ezechiel {
namespace GuiCore {
class DrugModel;
class ActiveSubstance;
}
}

class DrugToHtml
{
public:
    DrugToHtml();

    QString drugToHtml(const ezechiel::GuiCore::DrugModel* drug);
    QString activeSubstanceToHtml(const ezechiel::GuiCore::ActiveSubstance* drug);
};

#endif // DRUGTOHTML_H
