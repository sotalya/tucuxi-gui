//@@license@@

#include "covariateidtranslator.h"
#include "drugidtranslator.h"


namespace Tucuxi {
namespace Gui {
namespace Rest {


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
    map.insert("BodyWeight", "bodyweight");
    map.insert("weight","bodyweight");
    map.insert("creatinine", "creatinine");
    map.insert("creat", "creatinine");
    map.insert("BW", "BW");
    map.insert("ga", "ga");
}

QString ChuvCovariateIdTranslator::restToInternalId(QString restId)
{
//    return map.value(restId,"unknown");
    return map.value(restId,restId);
}


ExternalCovariateIdTranslator::ExternalCovariateIdTranslator()
{

}

void ExternalCovariateIdTranslator::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

QString ExternalCovariateIdTranslator::restToInternalId(QString restId)
{
    QSettings settingsFile(m_fileName, QSettings::IniFormat);

    // Try to find it in the file, else return the initial restId
    return settingsFile.value(restId, restId).toString();
}

}
}
}
