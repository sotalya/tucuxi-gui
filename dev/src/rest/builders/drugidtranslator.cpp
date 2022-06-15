//@@license@@


#include <QSettings>

#include "drugidtranslator.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


DrugIdTranslator::DrugIdTranslator()
{

}

DrugIdTranslator::~DrugIdTranslator()
{

}

DummyDrugIdTranslator::DummyDrugIdTranslator()
{

}

QString DummyDrugIdTranslator::restToInternalId(QString restId)
{
    return restId;
}



ChuvDrugIdTranslator::ChuvDrugIdTranslator()
{
//    map.insert("AMIKP","ch.heig-vd.ezechiel.amikacin");
    map.insert("AMIKP","ch.heig-vd.ezechiel.amikacin.adult_1CP");
    map.insert("GNTP","ch.heig-vd.ezechiel.gentamicin_neonates");
    map.insert("DIGO","ch.heig-vd.ezechiel.digoxin");
    map.insert("TBR","ch.heig-vd.ezechiel.tobramycin");

//    map.insert("VANCP","ch.heig-vd.ezechiel.vancomycin.bolus");
    map.insert("VANCR","ch.heig-vd.ezechiel.vancomycin.adult.1CP");
    map.insert("VANC","ch.heig-vd.ezechiel.vancomycin.adult.1CP");

    map.insert("VANCP", "ch.heig-vd.ezechiel.vancomycin.adult.1CP");

    map.insert("Imatinib","ch.heig-vd.ezechiel.imatinib");

    // Not really CHUV, for testing purpose now
    map.insert("Atazanavir","ch.heig-vd.ezechiel.atazanavir");
}

QString ChuvDrugIdTranslator::restToInternalId(QString restId)
{
    return map.value(restId,"ch.heig-vd.ezechiel.unknown");
}


ExternalDrugIdTranslator::ExternalDrugIdTranslator()
{

}

void ExternalDrugIdTranslator::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

QString ExternalDrugIdTranslator::restToInternalId(QString restId)
{
    QSettings settingsFile(m_fileName, QSettings::IniFormat);

    // First try to have a look if the restId is a known Id
    QStringList allKeys = settingsFile.allKeys();
    foreach(QString key, allKeys) {
        if (settingsFile.value(key).toString().compare(restId) == 0)
            return restId;
    }

    // Else try to find it in the file
    return settingsFile.value(restId, "ch.heig-vd.ezechiel.unknown").toString();
}

}
}
}

