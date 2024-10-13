/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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
