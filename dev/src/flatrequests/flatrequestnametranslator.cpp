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
    map.insert("SEX", "Sexe");
    map.insert("BIRTH_DATE", "DDN");
    map.insert("BODY_WEIGHT", "poids");
    map.insert("CREATININE_DOSAGE", "Dosage creat");
    map.insert("VANCOMICINE_DOSAGE","Dosage vanco");
    map.insert("CEFEPIME_DOSAGE", "Dosage cefepime");
    map.insert("RESIDUAL_CEFEPIME_DOSAGE", "Dosage Residuel cefepime");
    map.insert("FLOW_RATE", "debit");
    map.insert("DOSAGE", "dose");
}

FlatrequestNameTranslator* FlatrequestNameTranslator::getInstance()
{
    if(instance == nullptr) {
        instance = new FlatrequestNameTranslator();
        return instance;
    }
    else {
        return instance;
    }
}

QString FlatrequestNameTranslator::nameToInternalId(QString name)
{
    return map.value(name);
}
