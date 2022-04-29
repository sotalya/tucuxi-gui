//@@license@@

#ifndef DRUGRESPONSEANALYSIS_H
#define DRUGRESPONSEANALYSIS_H

#include "core/dal/entity.h"

namespace ezechiel {
namespace core {

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


} // namespace core
} // namespace ezechiel

Q_DECLARE_METATYPE(ezechiel::core::DrugResponseAnalysis*)
Q_DECLARE_METATYPE(QList<ezechiel::core::DrugResponseAnalysis*>)

#endif // DRUGRESPONSEANALYSIS_H
