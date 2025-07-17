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


// File title       :   defaultdrugconfig.h
// Author           :   Andrea Enrile
// Creation date    :   28.11.2023
// Purpose          :   Set the default dosage, measure, etc. values for each drugs
// ...              :   ...


#ifndef DEFAULTDRUGCONFIG_H
#define DEFAULTDRUGCONFIG_H

#include "core/dal/drug/target.h"

#include <QTest>
#include <QDateTime>

enum Gender{
    female  = 0,
    male    = 1
};

class TargetType : public Tucuxi::Gui::Core::TargetMethod {

};

struct PatientData {
    QString firstName       = "Joan";
    QString lastName        = "Of Arc";
    QDate birthDate         = QDate(1960, 11, 10);
    int gender              = female;                    // gender : 1 = male, 0 = female
    QString identifier      = "Patient_0";
    QString stayNumber      = "Private Drive 4";
    QString address         = "Rue du Flon 1";
    QString city            = "Lausanne";
    QString state           = "VD";
    QString postcode        = "1003";
    QString country         = "Switzerland";

    QString titlePhy        = "Dr.";
    QString firstNamePhy    = "Doo";
    QString lastNamePhy     = "Little";
    QString affiliationPhy  = "Aff";
    QString addressPhy      = "Rte de Cheseaux 1";
    QString cityPhy         = "Yverdon-les-Bains";
    QString statePhy        = "VD";
    QString postcodePhy     = "1400";
    QString countryPhy      = "Switzerland";
};

struct DosageData {
    double dose;
    double intervalInHours;
    double infusionTimeInMinutes;
    bool steadyState       = false;
    QDateTime dateTimeDos1 = QDateTime::currentDateTime().addMonths(-6);
    QDateTime dateTimeDos2 = QDateTime::currentDateTime().addMonths(6);
};


struct CovariatesData {
    QString sex;
    double weight;
    double height;
    double scc;
    QDateTime dateTimeCovar = QDateTime::currentDateTime();
    double clearanceOfCreatinine;
    double bodyTemperature;
    double dayNightDosing;
    double asian;
    double nonValvularAtrialFibriliation;
    double acuteCoronarySyndrome;
    double strongModerateCytochromeInhibitors;
    double dose;
    double glomerularFiltrationRate;
    double atazanavirWithWithoutBooster;
    double darunavir;
    double rifampicin;
    double currentSmoking;
    double gestionalAge;
    double heamatologicalMalignacy;
};

struct CovarTypeIndex {
    int sex                                = 0;
    int weight                             = 1;
    int height                             = 2;
    int scc                                = 3;
    int dateTimeCovar                      = 4;
    int clearanceOfCreatinine              = 5;
    int bodyTemperature                    = 6;
    int dayNightDosing                     = 7;
    int asian                              = 8;
    int nonValvularAtrialFibriliation      = 9;
    int acuteCoronarySyndrome              = 10;
    int strongModerateCytochromeInhibitors = 11;
    int dose                               = 12;
    int glomerularFiltrationRate           = 13;
    int atazanavirWithWithoutBooster       = 14;
    int darunavir                          = 15;
    int rifampicin                         = 16;
    int currentSmoking                     = 17;
    int gestionalAge                       = 18;
    int heamatologicalMalignacy            = 19;
};

struct MeasureData {
    QString name;
    double measure;
    QDateTime dateTimeMeas = QDateTime::currentDateTime();
};

struct TargetData {
    int targetType = TargetType::ResidualTarget;
    double cMinInput;
    double cBestInput;
    double cMaxInput;
    double tMinInput;
    double tBestInput;
    double tMaxInput;
    double micInput;
};

struct AdjustmentsData {
    QDateTime dateTimeAdj = QDateTime::currentDateTime();
    bool loadingDose      = false;
    bool restPeriod       = false;
    int suggestAdjIndex   = 0;
    int dose;
    int interval;
};

struct ValidationData {
    QDateTime dateTimeVal   = QDateTime::currentDateTime();
    QString expectedness    = "Full conversion";
    QString suitability     = "For Kaleds only";
    QString prediction      = "Becoming a Dalek";
    QString remonitoring    = "Only if they still have emotions";
    QString warning         = "If you are not a Dalek, run";
};

struct AnalystData {
    QString analystTitle       = "Dre";
    QString analystFirstName   = "Jane";
    QString analystLastName    = "Doe";
    QString analystRole        = "Analyst";
    QString analystPhoneNumber = "123456789";
    QString analystAffiliation = "Analyst Center";
    QString analystAddress     = "Rue du Bugnon 46";
    QString analystCity        = "Lausanne";
    QString analystPostcode    = "1011";
    QString analystState       = "VD";
    QString analystCountry     = "Switzerland";
};

class DrugDefault {
public:
    PatientData patientData;
    DosageData dosageData;
    CovariatesData covariateData;
    CovarTypeIndex covarTypeIndex;
    MeasureData measureData;
    TargetData targetData;
    AdjustmentsData adjustmentData;
    ValidationData validationData;
    AnalystData analystData;
};

class DrugApixaban : DrugDefault {
public:
    QString drugName = "Apixaban";
    DrugApixaban(){
        dosageData.dose   = 2.5;  // mg
        dosageData.intervalInHours = 12.0; // h

        covariateData.dayNightDosing                     = 0.0;
        covariateData.weight                             = 65.8; // kg
        covariateData.asian                              = 0.0;
        covariateData.nonValvularAtrialFibriliation      = 0.0;
        covariateData.acuteCoronarySyndrome              = 0.0;
        covariateData.strongModerateCytochromeInhibitors = 0.0;
        covariateData.dose                               = dosageData.dose;
        covariateData.glomerularFiltrationRate           = 80.0; // ml/min

        covarTypeIndex.sex                                = 0;
        covarTypeIndex.dayNightDosing                     = 2;
        covarTypeIndex.weight                             = 3;
        covarTypeIndex.asian                              = 4;
        covarTypeIndex.nonValvularAtrialFibriliation      = 5;
        covarTypeIndex.acuteCoronarySyndrome              = 6;
        covarTypeIndex.strongModerateCytochromeInhibitors = 7;
        covarTypeIndex.dose                               = 8;
        covarTypeIndex.glomerularFiltrationRate           = 9;

        measureData.measure = 45.0; // ug/l

        targetData.cMinInput  = 50.0;  // ug/l
        targetData.cBestInput = 100.0; // ug/l
        targetData.cMaxInput  = 200.0; // ug/l
    }
};

// TODO -> TO COMPLETE terget and covariate
class DrugBictegravir : DrugDefault {
public:
    QString drugName = "Bictegravir";
    DrugBictegravir(){
        dosageData.dose   = 50.0; // mg
        dosageData.intervalInHours = 24.0; // h

        covariateData;
        covarTypeIndex;

        measureData.measure = 3100.0; // ug/l

        targetData;
    }
};

class DrugBusulfan : DrugDefault {
public:
    QString drugName = "Busulfan";
    DrugBusulfan(){
        patientData.birthDate = QDate::currentDate().addYears(-10);

        dosageData.dose   = 10.0;  // mg
        dosageData.intervalInHours = 6.0;   // h
        dosageData.infusionTimeInMinutes = 120.0; // min

        covariateData.weight = 9.0; // kg

        covarTypeIndex.weight = 1;

        measureData.measure = 0.65; // mg/l

        targetData.targetType = TargetType::PeakTarget;
        targetData.cMinInput  = 1.0;   // mg/l
        targetData.cBestInput = 1.2;   // mg/l
        targetData.cMaxInput  = 1.4;   // mg/l
        targetData.tMinInput  = 54.0;  // h
        targetData.tBestInput = 150.0; // h
        targetData.tMaxInput  = 180.0; // h
    }
};

// TODO -> TO COMPLETE terget and covariate
class DrugCaffeine : DrugDefault {
public:
    QString drugName = "Caffeine";
    DrugCaffeine(){
        dosageData.dose   = 400.0; // mg
        dosageData.intervalInHours = 24.0;  // h

        covariateData;
        covarTypeIndex;

        measureData.measure = 3000.0; // ug/l

        targetData;
    }
};

class DrugCefepime : DrugDefault {
public:
    QString drugName = "Cefepime";
    DrugCefepime(){
        dosageData.dose   = 2000.0; // mg
        dosageData.intervalInHours = 8;      // h
        dosageData.infusionTimeInMinutes = 30;     // min

        covariateData.weight = 65.0; // kg
        covariateData.scc    = 80.0; // umol/l

        covarTypeIndex.weight = 0;
        covarTypeIndex.scc    = 3;

        measureData.measure = 90.0; // mg/l

        targetData.targetType = TargetType::ResidualTarget;
        targetData.cMinInput  = 4.0;  // mg/l
        targetData.cBestInput = 8.0;  // mg/l
        targetData.cMaxInput  = 16.0; // mg/l
    }
};

class DrugDaptomycin : DrugDefault {
public:
    QString drugName = "Daptomycin";
    DrugDaptomycin(){
        dosageData.dose = 500.0;  // mg
        dosageData.intervalInHours = 24.0; // h
        dosageData.infusionTimeInMinutes = 30.0; // min

        covariateData.weight                = 75.0; // kg
        covariateData.clearanceOfCreatinine = 91.2; // ml/min
        covariateData.bodyTemperature       = 37.2; // ° celsius

        covarTypeIndex.weight                = 1;
        covarTypeIndex.clearanceOfCreatinine = 2;
        covarTypeIndex.bodyTemperature       = 3;

        measureData.measure = 45.0; // mg/l

        targetData.targetType = TargetType::AUC24DividedByMicTarget;
        targetData.tMinInput  = 400.0;  // h
        targetData.tBestInput = 666.0;  // h
        targetData.tMaxInput  = 1200.0; // h
        targetData.micInput   = 1.0;    // mg/l
    }
};

// TODO -> TO COMPLETE target
class DrugDarunavir : DrugDefault {
public:
    QString drugName = "Darunavir";
    DrugDarunavir(){
        dosageData.dose   = 400.0; // mg
        dosageData.intervalInHours = 24.0;   // h

        covariateData.weight = 70.0;  //kg
        covariateData.height = 152.0; // cm

        covarTypeIndex.weight = 0;
        covarTypeIndex.height = 2;

        measureData.measure = 1500.0; // ug/l

        targetData.targetType = TargetType::ResidualTarget;
        targetData.cMinInput  = 0.0;
        targetData.cBestInput = 0.0;
        targetData.cMaxInput  = 0.0;
        targetData.tMinInput  = 0.0;
        targetData.tBestInput = 0.0;
        targetData.tMaxInput  = 0.0;
        targetData.micInput   = 0.0;
    }
};

class Drugx : DrugDefault {
public:
    QString drugName = "xxxxxx";
    Drugx(){
        patientData;
        dosageData;

        covariateData.sex                                = "sex";
        covariateData.weight                             = 0.0;
        covariateData.height                             = 0.0;
        covariateData.scc                                = 0.0;
        covariateData.dateTimeCovar                      = QDateTime::currentDateTime();
        covariateData.clearanceOfCreatinine              = 0.0;
        covariateData.bodyTemperature                    = 0.0;
        covariateData.dayNightDosing                     = 0.0;
        covariateData.asian                              = 0.0;
        covariateData.nonValvularAtrialFibriliation      = 0.0;
        covariateData.acuteCoronarySyndrome              = 0.0;
        covariateData.strongModerateCytochromeInhibitors = 0.0;
        covariateData.dose                               = 0.0;
        covariateData.glomerularFiltrationRate           = 0.0;
        covariateData.atazanavirWithWithoutBooster       = 0.0;
        covariateData.darunavir                          = 0.0;
        covariateData.rifampicin                         = 0.0;
        covariateData.currentSmoking                     = 0.0;
        covariateData.gestionalAge                       = 0.0;
        covariateData.heamatologicalMalignacy            = 0.0;

        covarTypeIndex.sex                                = 0;
        covarTypeIndex.weight                             = 1;
        covarTypeIndex.height                             = 2;
        covarTypeIndex.scc                                = 3;
        covarTypeIndex.dateTimeCovar                      = 4;
        covarTypeIndex.clearanceOfCreatinine              = 5;
        covarTypeIndex.bodyTemperature                    = 6;
        covarTypeIndex.dayNightDosing                     = 7;
        covarTypeIndex.asian                              = 8;
        covarTypeIndex.nonValvularAtrialFibriliation      = 9;
        covarTypeIndex.acuteCoronarySyndrome              = 10;
        covarTypeIndex.strongModerateCytochromeInhibitors = 11;
        covarTypeIndex.dose                               = 12;
        covarTypeIndex.glomerularFiltrationRate           = 13;
        covarTypeIndex.atazanavirWithWithoutBooster       = 14;
        covarTypeIndex.darunavir                          = 15;
        covarTypeIndex.rifampicin                         = 16;
        covarTypeIndex.currentSmoking                     = 17;
        covarTypeIndex.gestionalAge                       = 18;
        covarTypeIndex.heamatologicalMalignacy            = 19;

        measureData;

        targetData.targetType = TargetType::ResidualTarget;
        targetData.cMinInput  = 0.0;
        targetData.cBestInput = 0.0;
        targetData.cMaxInput  = 0.0;
        targetData.tMinInput  = 0.0;
        targetData.tBestInput = 0.0;
        targetData.tMaxInput  = 0.0;
        targetData.micInput   = 0.0;

        adjustmentData;
        validationData;
        analystData;
    }
};
#endif // DEFAULTDRUGCONFIG_H
