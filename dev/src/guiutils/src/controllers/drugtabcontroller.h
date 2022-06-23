//@@license@@

#ifndef DRUGTABCONTROLLER_H
#define DRUGTABCONTROLLER_H

#include <QObject>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class ActiveSubstance;
class DrugModel;

}
}
}

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class DrugTabController : public AbstractViewController
{
    Q_OBJECT

    STD_PROPERTY_DECL(QString, drugHtmlDescription, DrugHtmlDescription)

public:
    DrugTabController(QObject *parent = nullptr);

public slots:

    void setDrugInfo(Tucuxi::Gui::Core::ActiveSubstance *substance);
    void setDrugModelInfo(Tucuxi::Gui::Core::DrugModel *drugModel);
};

}
}
}

#endif // DRUGTABCONTROLLER_H
