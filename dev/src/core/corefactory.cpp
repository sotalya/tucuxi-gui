//@@license@@

#include "core/corefactory.h"

namespace ezechiel {
namespace core {

CoreFactory* CoreFactory::_instance = 0;

CoreFactory::CoreFactory()
{
}

//SharedCorePatient CoreFactory::createPatient(AbstractRepository *repository, QObject *parent)
//{
//    return SharedCorePatient(new CorePatient(repository,parent));
//}

//SharedMeasure CoreFactory::createMeasure(AbstractRepository *repository, QObject *parent)
//{
//    return SharedMeasure(new Measure(repository,parent));
//}

//SharedPrediction CoreFactory::createPrediction(AbstractRepository *repository, QObject *parent)
//{
//    return SharedPrediction(new Prediction(repository));
//}

} // namespace core
} // namespace ezechiel
