#ifndef APPUTILSFACTORY_H
#define APPUTILSFACTORY_H

//#include "core/dal/corepatient.h"
//#include "admin/src/dal/measure.h"
//#include "core/dal/prediction.h"
#include "core/interfaces/abstractrepository.h"

class ApputilsFactory
{
public:
    ApputilsFactory();

    template<typename entity>
    static entity* createEntity(Tucuxi::Gui::Core::AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new entity(repository, parent);
    }

    template<typename entity>
    static entity* cloneEntity(Tucuxi::Gui::Core::AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return new entity(repository, parent, _target);
    }

//    virtual SharedCorePatient createPatient(AbstractRepository *repository, QObject *parent);
//    virtual SharedMeasure createMeasure(AbstractRepository *repository, QObject *parent);
//    virtual SharedPrediction createPrediction(AbstractRepository *repository, QObject *parent);
};

#endif // COREFACTORY_H
