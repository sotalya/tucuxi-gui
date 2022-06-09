//@@license@@

#ifndef ADMINFACTORY_H
#define ADMINFACTORY_H

#include <QObject>
#include "core/interfaces/abstractrepository.h"

class AdminFactory
{
public:
    AdminFactory();

    template<typename entity>
    static entity* createEntity(ezechiel::GuiCore::AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new entity(repository, parent);
    }

    template<typename entity>
    static entity* cloneEntity(ezechiel::GuiCore::AbstractRepository *repository, QObject* parent, entity* _target)
    {
        return new entity(repository, parent, _target);
    }

};

#endif // COREFACTORY_H
