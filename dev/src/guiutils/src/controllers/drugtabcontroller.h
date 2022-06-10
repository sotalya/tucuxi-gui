//@@license@@

#ifndef DRUGTABCONTROLLER_H
#define DRUGTABCONTROLLER_H

#include <QObject>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"

namespace Tucuxi {
namespace GuiCore {

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

    void setDrugInfo(Tucuxi::GuiCore::ActiveSubstance *substance);
    void setDrugModelInfo(Tucuxi::GuiCore::DrugModel *drugModel);
};

#endif // DRUGTABCONTROLLER_H
