/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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
