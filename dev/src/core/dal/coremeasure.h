//@@license@@

#ifndef COREMEASURE_H
#define COREMEASURE_H

#include <QObject>

#include "core/dal/drug/operableamount.h"
#include "core/utils/ezutils.h"
#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class UncastedValueList;

class CoreMeasure : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(CoreMeasure)

    AUTO_PROPERTY_DECL(IdentifiableAmount*, concentration, Concentration)
    AUTO_PROPERTY_DECL(QDateTime, moment, Moment)
    AUTO_PROPERTY_DECL(QString, sdrug, Sdrug)

    AUTO_PROPERTY_DECL(UncastedValueList*, uncastedValues, UncastedValues)

protected:

    Q_INVOKABLE explicit CoreMeasure(AbstractRepository *repository = 0, QObject *parent = nullptr);
    Q_INVOKABLE CoreMeasure(AbstractRepository *repository, QDateTime moment, IdentifiableAmount* amount, QString getSdrug, ident dbid = -1);

public:
    void convert(const Unit &getUnit);
//    bool isValid();
    bool operator== ( CoreMeasure* other );
    bool operator!= ( CoreMeasure* other );
};


QML_POINTERLIST_CLASS_DECL(CoreMeasureList, CoreMeasure)
} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CoreMeasure*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::CoreMeasure*>)
Q_DECLARE_METATYPE(Tucuxi::Gui::Core::CoreMeasureList*)


#endif // MEASURE_H
