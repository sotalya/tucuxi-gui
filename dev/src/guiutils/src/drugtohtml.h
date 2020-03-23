#ifndef DRUGTOHTML_H
#define DRUGTOHTML_H

#include <QString>

namespace ezechiel {
namespace core {
class DrugModel;
class ActiveSubstance;
}
}

class DrugToHtml
{
public:
    DrugToHtml();

    QString drugToHtml(const ezechiel::core::DrugModel* drug);
    QString activeSubstanceToHtml(const ezechiel::core::ActiveSubstance* drug);
};

#endif // DRUGTOHTML_H
