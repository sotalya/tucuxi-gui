//@@license@@

#include "flatrequestparameters.h"


FlatRequestParameters* FlatRequestParameters::instance = nullptr;

FlatRequestParameters::FlatRequestParameters():m_frenchTags(false) {
}

FlatRequestParameters* FlatRequestParameters::getInstance()
{
    if(instance == nullptr) {
        instance = new FlatRequestParameters();
        return instance;
    }
    else {
        return instance;
    }
}

void FlatRequestParameters::setFrenchTag(bool enable) {
    m_frenchTags = enable;
}

bool FlatRequestParameters::getIsFrenchTag() {
    return m_frenchTags;
}

QString FlatRequestParameters::reportNameXml() {
    return m_frenchTags ? "Report" : "report";
}

QString FlatRequestParameters::fullDataNameXml() {
    return m_frenchTags ? "Name" : "name";
}

QString FlatRequestParameters::detailsListNameXml() {
    return m_frenchTags ? "Détails_Collection" : "details_list";
}

QString FlatRequestParameters::siteNameXmls() {
    return "site";
}

QString FlatRequestParameters::drugNameXml() {
    return m_frenchTags ? "nom_medicament" : "drug_name";
}

QString FlatRequestParameters::timeNameXml() {
    return m_frenchTags ? "horaire" : "time";
}

QString FlatRequestParameters::unitNameXml() {
    return m_frenchTags ? "unite" : "unit";
}

QString FlatRequestParameters::valueNameXml() {
    return m_frenchTags ? "valeur" : "value";
}

QString FlatRequestParameters::dataNameXml() {
    return m_frenchTags ? "donnees" : "data";
}

QString FlatRequestParameters::encounteridNameXml() {
    return "encounterid";
}

QString FlatRequestParameters::detailsNameXml() {
    return m_frenchTags ? "Détails" : "details";
}
