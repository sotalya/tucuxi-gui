//@@license@@

#ifndef ABSTRACTVIEWCONTROLLER_H
#define ABSTRACTVIEWCONTROLLER_H

#include <QObject>

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class InterpretationController;


class AbstractViewController : public QObject
{
    Q_OBJECT

public:
    explicit AbstractViewController(QObject *parent = nullptr);

    virtual void setMasterController(InterpretationController *controller);

protected:

    InterpretationController *masterController;
};

}
}
}

#endif // ABSTRACTVIEWCONTROLLER_H
