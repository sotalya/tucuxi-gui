//@@license@@

#ifndef FACTORY_H
#define FACTORY_H




#include "core/interfaces/abstractrepository.h"

template<typename entity>
class Creator
{
public:
    virtual entity *createEntity(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent = nullptr)
    {
        return new entity(repository, parent);
    }

};

template <class T>
class Factory
{
public:
    Factory()
    {
        _creator = new Creator<T>();
    }

    void registerCreator(Creator<T>* creator)
    {
        if (_creator)
            delete _creator;
        _creator = creator;
    }

    T* createEntity(Tucuxi::GuiCore::AbstractRepository *repository, QObject *parent = nullptr)
    {
        return _creator->createEntity(repository, parent);
    }

private:
    Creator<T>* _creator;
};

#endif // FACTORY_H
