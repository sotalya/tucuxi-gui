//@@license@@

#ifndef ROUTETRANSLATOR_H
#define ROUTETRANSLATOR_H



#include <QString>
#include <QMap>

#include "core/dal/drug/adme.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

class RouteTranslator
{
public:
    RouteTranslator();
    virtual ~RouteTranslator() = default;

    virtual Tucuxi::Gui::Core::Admin::Route restToInternalRoute(QString restRoute) = 0;
};

class DummyRouteTranslator : public RouteTranslator
{
public:
    DummyRouteTranslator();

    virtual Tucuxi::Gui::Core::Admin::Route restToInternalRoute(QString restRoute) ;
};

class ChuvRouteTranslator : public RouteTranslator
{
public:
    ChuvRouteTranslator();

    virtual Tucuxi::Gui::Core::Admin::Route restToInternalRoute(QString restRoute) ;

private:
    QMap<QString,Tucuxi::Gui::Core::Admin::Route> map;
};

}
}
}

#endif // ROUTETRANSLATOR_H
