//@@license@@

#ifndef DRUGRESPONSEANALYSIS_H
#define DRUGRESPONSEANALYSIS_H

#include "core/dal/entity.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


class DrugTreatment;
class DrugModel;

class DrugResponseAnalysis : public Entity
{
    Q_OBJECT

    ENTITY_UTILS(DrugResponseAnalysis)

    AUTO_PROPERTY_DECL(DrugTreatment*, treatment, Treatment)
    AUTO_PROPERTY_DECL(DrugModel*, drugModel, DrugModel)


protected:

    Q_INVOKABLE DrugResponseAnalysis(AbstractRepository *repository = nullptr, QObject* parent = nullptr);

protected:

};


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

Q_DECLARE_METATYPE(Tucuxi::Gui::Core::DrugResponseAnalysis*)
Q_DECLARE_METATYPE(QList<Tucuxi::Gui::Core::DrugResponseAnalysis*>)

#endif // DRUGRESPONSEANALYSIS_H
