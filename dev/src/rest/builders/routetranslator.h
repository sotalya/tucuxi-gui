//@@license@@

#ifndef ROUTETRANSLATOR_H
#define ROUTETRANSLATOR_H



#include <QString>
#include <QMap>

#include "core/dal/drug/adme.h"

class RouteTranslator
{
public:
    RouteTranslator();
    virtual ~RouteTranslator() = default;

    virtual Tucuxi::GuiCore::Admin::Route restToInternalRoute(QString restRoute) = 0;
};

class DummyRouteTranslator : public RouteTranslator
{
public:
    DummyRouteTranslator();

    virtual Tucuxi::GuiCore::Admin::Route restToInternalRoute(QString restRoute) ;
};

class ChuvRouteTranslator : public RouteTranslator
{
public:
    ChuvRouteTranslator();

    virtual Tucuxi::GuiCore::Admin::Route restToInternalRoute(QString restRoute) ;

private:
    QMap<QString,Tucuxi::GuiCore::Admin::Route> map;
};

#endif // ROUTETRANSLATOR_H
