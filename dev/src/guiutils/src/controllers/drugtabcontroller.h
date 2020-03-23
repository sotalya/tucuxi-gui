#ifndef DRUGTABCONTROLLER_H
#define DRUGTABCONTROLLER_H

#include <QObject>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"

namespace ezechiel {
namespace core {

class ActiveSubstance;
class DrugModel;

}
}

class DrugTabController : public AbstractViewController
{
    Q_OBJECT

    STD_PROPERTY_DECL(QString, drugHtmlDescription, DrugHtmlDescription)

public:
    DrugTabController(QObject *parent = nullptr);

public slots:

    void setDrugInfo(ezechiel::core::ActiveSubstance *substance);
    void setDrugModelInfo(ezechiel::core::DrugModel *drugModel);
};

#endif // DRUGTABCONTROLLER_H
