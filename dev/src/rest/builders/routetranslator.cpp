//@@license@@

#include "routetranslator.h"




RouteTranslator::RouteTranslator()
{
}


DummyRouteTranslator::DummyRouteTranslator()
{
}

Tucuxi::GuiCore::Admin::Route DummyRouteTranslator::restToInternalRoute(QString restRoute)
{
    return Tucuxi::GuiCore::Admin::UNVALID;
}

ChuvRouteTranslator::ChuvRouteTranslator()
{
    map.insert("", Tucuxi::GuiCore::Admin::DEFAULT);
    // Internal values
    map.insert("BOLUS", Tucuxi::GuiCore::Admin::BOLUS);
    map.insert("INFUSION", Tucuxi::GuiCore::Admin::INFUSION);
    map.insert("EXTRA", Tucuxi::GuiCore::Admin::EXTRA);

    // External values
    map.insert("i.v", Tucuxi::GuiCore::Admin::INFUSION);
    map.insert("i.v.", Tucuxi::GuiCore::Admin::INFUSION);
    map.insert("injection iv", Tucuxi::GuiCore::Admin::INFUSION);
    map.insert("perfusion i.v. continue", Tucuxi::GuiCore::Admin::INFUSION);
    map.insert("perfusion iv continue", Tucuxi::GuiCore::Admin::INFUSION);
    map.insert("orale", Tucuxi::GuiCore::Admin::EXTRA);
    map.insert("per os", Tucuxi::GuiCore::Admin::EXTRA);
}

Tucuxi::GuiCore::Admin::Route ChuvRouteTranslator::restToInternalRoute(QString restRoute)
{
    return map.value(restRoute,Tucuxi::GuiCore::Admin::UNVALID);
}
