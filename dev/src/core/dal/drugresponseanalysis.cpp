//@@license@@

#include "core/dal/drugresponseanalysis.h"

#include "core/dal/drugtreatment.h"
#include "core/dal/drug/drug.h"

namespace Tucuxi {
namespace Gui {
namespace Core {


//AUTO_PROPERTY_IMPL(DrugResponseAnalysis, PostEngine*, postEngine, PostEngine)
//AUTO_PROPERTY_IMPL(DrugResponseAnalysis, PercentileEngine*, percentileEngine, PercentileEngine)
//AUTO_PROPERTY_IMPL(DrugResponseAnalysis, ReverseEngine*, reverseEngine, ReverseEngine)
AUTO_PROPERTY_IMPL(DrugResponseAnalysis, DrugTreatment*, treatment, Treatment)
AUTO_PROPERTY_IMPL(DrugResponseAnalysis, DrugModel*, drugModel, DrugModel)

DrugResponseAnalysis::DrugResponseAnalysis(AbstractRepository *repository, QObject* parent)
    : Entity(repository, parent),
//      _postEngine(0),
//      _percentileEngine(0),
//      _reverseEngine(0),
      _treatment(Tucuxi::Gui::Core::CoreFactory::createEntity<DrugTreatment>(repository, this)),
      _drugModel(nullptr)
{
}

} // namespace Core
} // namespace Gui
} // namespace Tucuxi
