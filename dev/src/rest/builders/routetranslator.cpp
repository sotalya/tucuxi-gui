//@@license@@

#include "routetranslator.h"




RouteTranslator::RouteTranslator()
{
}


DummyRouteTranslator::DummyRouteTranslator()
{
}

Tucuxi::Gui::Core::Admin::Route DummyRouteTranslator::restToInternalRoute(QString restRoute)
{
    return Tucuxi::Gui::Core::Admin::UNVALID;
}

ChuvRouteTranslator::ChuvRouteTranslator()
{
    map.insert("", Tucuxi::Gui::Core::Admin::DEFAULT);
    // Internal values
    map.insert("BOLUS", Tucuxi::Gui::Core::Admin::BOLUS);
    map.insert("INFUSION", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("EXTRA", Tucuxi::Gui::Core::Admin::EXTRA);

    // External values
    map.insert("i.v", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("i.v.", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("injection iv", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("perfusion i.v. continue", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("perfusion iv continue", Tucuxi::Gui::Core::Admin::INFUSION);
    map.insert("orale", Tucuxi::Gui::Core::Admin::EXTRA);
    map.insert("per os", Tucuxi::Gui::Core::Admin::EXTRA);
}

Tucuxi::Gui::Core::Admin::Route ChuvRouteTranslator::restToInternalRoute(QString restRoute)
{
    return map.value(restRoute,Tucuxi::Gui::Core::Admin::UNVALID);
}
