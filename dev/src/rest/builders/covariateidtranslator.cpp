#include "covariateidtranslator.h"
#include "drugidtranslator.h"

CovariateIdTranslator::CovariateIdTranslator()
{

}

DummyCovariateIdTranslator::DummyCovariateIdTranslator()
{

}

QString DummyCovariateIdTranslator::restToInternalId(QString restId)
{
    return restId;
}



ChuvCovariateIdTranslator::ChuvCovariateIdTranslator()
{
    map.insert("weight","bodyweight");
    map.insert("creatinine", "creatinine");
    map.insert("BW", "BW");
    map.insert("ga", "ga");
}

QString ChuvCovariateIdTranslator::restToInternalId(QString restId)
{
//    return map.value(restId,"unknown");
    return map.value(restId,restId);
}
