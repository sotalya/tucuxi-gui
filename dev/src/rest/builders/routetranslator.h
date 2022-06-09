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

    virtual ezechiel::GuiCore::Admin::Route restToInternalRoute(QString restRoute) = 0;
};

class DummyRouteTranslator : public RouteTranslator
{
public:
    DummyRouteTranslator();

    virtual ezechiel::GuiCore::Admin::Route restToInternalRoute(QString restRoute) ;
};

class ChuvRouteTranslator : public RouteTranslator
{
public:
    ChuvRouteTranslator();

    virtual ezechiel::GuiCore::Admin::Route restToInternalRoute(QString restRoute) ;

private:
    QMap<QString,ezechiel::GuiCore::Admin::Route> map;
};

#endif // ROUTETRANSLATOR_H
