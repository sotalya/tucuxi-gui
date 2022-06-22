//@@license@@

#ifndef MEASURETABCONTROLLER_H
#define MEASURETABCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/autoproperty.h"
#include "abstractviewcontroller.h"

namespace Tucuxi {
namespace Gui {
namespace Core {

class CoreMeasureList;

}
}
}

class MeasureTabController : public AbstractViewController
{
    Q_OBJECT
public:
    explicit MeasureTabController(QObject *parent = nullptr);

    //! This property is used by MeasureTab.qml to access the list of measures
    STD_PROPERTY_DECL(Tucuxi::Gui::Core::CoreMeasureList*, measures, Measures)

public:
    Q_INVOKABLE void addMeasure();
    Q_INVOKABLE void removeMeasure(int index);
    Q_INVOKABLE void setSampleId(int index, const QString &id);
    Q_INVOKABLE void setDbValue(int index, double value);
    Q_INVOKABLE void setMoment(int index, QDateTime time);
    Q_INVOKABLE void setArrivalDate(int index, QDateTime time);

    Q_INVOKABLE void forceRefresh();

private:
    bool isIndexValid(int index);
};

#endif // MEASURETABCONTROLLER_H
