//@@license@@

#include "routetranslator.h"




RouteTranslator::RouteTranslator()
{
}


DummyRouteTranslator::DummyRouteTranslator()
{
}

ezechiel::GuiCore::Admin::Route DummyRouteTranslator::restToInternalRoute(QString restRoute)
{
    return ezechiel::GuiCore::Admin::UNVALID;
}

ChuvRouteTranslator::ChuvRouteTranslator()
{
    map.insert("", ezechiel::GuiCore::Admin::DEFAULT);
    // Internal values
    map.insert("BOLUS", ezechiel::GuiCore::Admin::BOLUS);
    map.insert("INFUSION", ezechiel::GuiCore::Admin::INFUSION);
    map.insert("EXTRA", ezechiel::GuiCore::Admin::EXTRA);

    // External values
    map.insert("i.v", ezechiel::GuiCore::Admin::INFUSION);
    map.insert("i.v.", ezechiel::GuiCore::Admin::INFUSION);
    map.insert("injection iv", ezechiel::GuiCore::Admin::INFUSION);
    map.insert("perfusion i.v. continue", ezechiel::GuiCore::Admin::INFUSION);
    map.insert("perfusion iv continue", ezechiel::GuiCore::Admin::INFUSION);
    map.insert("orale", ezechiel::GuiCore::Admin::EXTRA);
    map.insert("per os", ezechiel::GuiCore::Admin::EXTRA);
}

ezechiel::GuiCore::Admin::Route ChuvRouteTranslator::restToInternalRoute(QString restRoute)
{
    return map.value(restRoute,ezechiel::GuiCore::Admin::UNVALID);
}
