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

#include "flatrequestnametranslator.h"

FlatrequestNameTranslator* FlatrequestNameTranslator::instance = nullptr;

FlatrequestNameTranslator::FlatrequestNameTranslator()
{
    map.insert("Sexe", "SEX");
    map.insert("DDN", "BIRTH_DATE");
    map.insert("poids", "BODY_WEIGHT");
    map.insert("Dosage creat", "CREATININE_MEASURE");
    map.insert("Dosage vanco", "DRUG_MEASURE");
    map.insert("Dosage cefepime", "DRUG_MEASURE");
    map.insert("Dosage Residuel cefepime", "DRUG_MEASURE");
    map.insert("Tx Vorico", "DRUG_MEASURE");
    map.insert("debit", "FLOW_RATE");
    map.insert("dose", "DOSAGE");
    map.insert("durée", "DURATION");
}

FlatrequestNameTranslator* FlatrequestNameTranslator::getInstance()
{
    if(instance == nullptr) {
        instance = new FlatrequestNameTranslator();
    }
    return instance;
}

QString FlatrequestNameTranslator::nameToInternalId(QString name)
{
    return map.value(name);
}
