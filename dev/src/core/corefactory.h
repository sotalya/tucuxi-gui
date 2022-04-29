//@@license@@

#ifndef COREFACTORY_H
#define COREFACTORY_H

#include <QObject>
#include "core/interfaces/abstractrepository.h"

namespace ezechiel {
namespace core {

class CoreFactory;


class CoreFactory
{
public:
    CoreFactory();

    static CoreFactory *_instance;

    static CoreFactory *getInstance()
    {
        if (!_instance)
            _instance = new CoreFactory();
        return _instance;
    }

    static void setInstance(CoreFactory *factory)
    {
        if (_instance)
            delete _instance;
        _instance = factory;
    }

    template<typename entity>
    entity* instanceCreateEntity(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new entity(repository, parent);
    }
/*
    template<>
    static Drug* createEntity<Drug>(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new Drug(repository, parent);
    }
*/
    template<typename entity>
    entity* instanceCloneEntity(AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return new entity(repository, parent, _target);
    }


    template<typename entity>
    static entity* createEntity(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return entity::createEntity(repository, parent);

//        return getInstance()->instanceCreateEntity<entity>(repository, parent);
//        return new entity(repository, parent);
    }

    template<typename entity>
    static entity* cloneEntity(AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return getInstance()->instanceCloneEntity<entity>(repository, parent, _target);
//        return new entity(repository, parent, _target);
    }

};

/*
 *
class CoreFactory
{
public:
    CoreFactory();

    static CoreFactory *_instance;

    static CoreFactory *getInstance()
    {
        if (!_instance)
            _instance = new CoreFactory();
        return _instance;
    }

    static void setInstance(CoreFactory *factory)
    {
        if (_instance)
            delete _instance;
        _instance = factory;
    }

    template<typename entity>
    entity* instanceCreateEntity(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new entity(repository, parent);
    }

    template<typename entity>
    entity* instanceCloneEntity(AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return new entity(repository, parent, _target);
    }


    template<typename entity>
    static entity* createEntity(AbstractRepository *repository, QObject *parent = nullptr)
    {
        return getInstance()->instanceCreateEntity<entity>(repository, parent);
//        return new entity(repository, parent);
    }

    template<typename entity>
    static entity* cloneEntity(AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return getInstance()->instanceCloneEntity<entity>(repository, parent, _target);
//        return new entity(repository, parent, _target);
    }

};
*/

} // namespace core
} // namespace ezechiel

#endif // COREFACTORY_H
