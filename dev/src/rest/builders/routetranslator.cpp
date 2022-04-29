//@@license@@

#include "routetranslator.h"




RouteTranslator::RouteTranslator()
{
}


DummyRouteTranslator::DummyRouteTranslator()
{
}

ezechiel::core::Admin::Route DummyRouteTranslator::restToInternalRoute(QString restRoute)
{
    return ezechiel::core::Admin::UNVALID;
}

ChuvRouteTranslator::ChuvRouteTranslator()
{
    map.insert("", ezechiel::core::Admin::DEFAULT);
    // Internal values
    map.insert("BOLUS", ezechiel::core::Admin::BOLUS);
    map.insert("INFUSION", ezechiel::core::Admin::INFUSION);
    map.insert("EXTRA", ezechiel::core::Admin::EXTRA);

    // External values
    map.insert("i.v", ezechiel::core::Admin::INFUSION);
    map.insert("i.v.", ezechiel::core::Admin::INFUSION);
    map.insert("injection iv", ezechiel::core::Admin::INFUSION);
    map.insert("perfusion i.v. continue", ezechiel::core::Admin::INFUSION);
    map.insert("perfusion iv continue", ezechiel::core::Admin::INFUSION);
    map.insert("orale", ezechiel::core::Admin::EXTRA);
    map.insert("per os", ezechiel::core::Admin::EXTRA);
}

ezechiel::core::Admin::Route ChuvRouteTranslator::restToInternalRoute(QString restRoute)
{
    return map.value(restRoute,ezechiel::core::Admin::UNVALID);
}
