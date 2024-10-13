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


// File title       :   validationtest.cpp
// Test title       :   ValidationTest
// Author           :   Julien Rosset
// Creation date    :   24.03.2022
// Purpose          :   Run test through all tabs all the way to validation
// ...              :   ...


#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/Identifiers.h>

#include "guitest.h"

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"

#include <QDebug>

extern SpixGTest* srv;


TEST(ValidationTest, Test1)
{

    int waitTime1 = 1;

    srv->waitPeriod(waitTime1);                             // default (when no arg.) value = 1/2 [s]


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

    PatientData patientData1;
    patientData1.firstName      = "Joan";
    patientData1.lastName       = "Of Arc";
    patientData1.birthDate.setDate(1984, 11, 11);
    patientData1.gender         = 0;                        // male = 1, female = 0
    patientData1.identifier     = "Patient_0_virusT";
    patientData1.stayNumber     = "Private drive n°4";


    DosageData dosageData1;
    dosageData1.dosage      = 7500;
    dosageData1.infusion    = 900;
    dosageData1.interval    = 4800;
    dosageData1.steadyState = false;
    dosageData1.dateTimeDos1.setDate(QDate(2022, 05, 07));
    dosageData1.dateTimeDos1.setTime(QTime(15,23));
    dosageData1.dateTimeDos2.setDate(QDate(2022, 06, 13));
    dosageData1.dateTimeDos2.setTime(QTime(07,55));

    CovariatesData covariatesData1;
    covariatesData1.sex     = "Unknown";
    covariatesData1.weight  = 4321;
    covariatesData1.scc     = 3720;
    covariatesData1.dateTimeCovar.setDate(QDate(2022, 12, 12));
    covariatesData1.dateTimeCovar.setTime(QTime(06,06));

    MeasureData measureData1;
    measureData1.name   = ("Sample_123_b");
    measureData1.measure  = 4567;
    measureData1.dateTimeMeas.setDate(QDate(2022, 02, 05));
    measureData1.dateTimeMeas.setTime(QTime(02, 03));

    TargetData targetData1;
    targetData1.targetType  = 8;                        // index 0-12 : peak = 1, AUC = [3-9], MIC [5-12]
    targetData1.cMinInput   = 602;
    targetData1.cBestInput  = 813;
    targetData1.cMaxInput   = 951;
    targetData1.tMinInput   = 20;
    targetData1.tBestInput  = 30;
    targetData1.tMaxInput   = 40;
    targetData1.micInput    = 1300;

    AdjustmentsData adjustmentsData1;
    adjustmentsData1.dateTimeAdj.setDate(QDate(2028,03,05));
    adjustmentsData1.dateTimeAdj.setTime(QTime(11, 12));
    adjustmentsData1.loadingDose    = true;
    adjustmentsData1.restPeriod     = true;
    adjustmentsData1.suggestAdjIndex  = 3;

    ValidationData validationData1;
    validationData1.dateTimeVal.setDate(QDate(2022, 01, 02));
    validationData1.dateTimeVal.setTime(QTime(03, 04));

    validationData1.expectedness    = "Hold my TDM";
    validationData1.suitability     = "Live news from USSR";
    validationData1.prediction      = "Not on my watch";
    validationData1.remonitoring    = "Good morning Vietnam";
    validationData1.warning         = "One small step for man...";


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-


    srv->startNewPatient();

    srv->fillInPatientData(patientData1);

    QString drugName = "Cefepime";  // index 15 previously (not sure what drug it is)
    int modelIndex = 0;                                                 // domain & study
    srv->selectDrugInList(drugName, modelIndex);

    srv->addDosage(dosageData1);

    int covariateType = 0;                                              // covariateType : 0 = weight
//    srv->addCovariates(covariatesData1, covariateType);

    srv->addMeasure(measureData1);

    srv->addTarget(targetData1);

    srv->addAdjustments(adjustmentsData1);

    int globalSpecific = 2;                                             // globalSpecific : 0 = global, 1 = specific, 2 = both
    int commentIndex = 0;                                               // commentIndex : 0 = will delete any saved comments
    srv->deleteValidationComment("prediction", globalSpecific, commentIndex);

    srv->waitPeriod(waitTime1*4);

    for (int a = 0; a <= 7; a++) {
        validationData1.prediction = QString::number(a);                // comment text = loop index (for clarity if deletion)
        srv->fillInValidationData(validationData1);                     // fills in commentts in expectedness, suitabilty, prediction, remonitoring & warning text fields
        srv->saveValidationComment("prediction", globalSpecific);       // saves prediction comments in both global and specific registries
    }

    srv->deleteValidationComment("prediction", 0, 1);                   // deletes prediction comment, global, index 1
    srv->deleteValidationComment("prediction", 1, 2);                   // deletes prediction comment, specific, index 2
    srv->deleteValidationComment("prediction", 2, 1);                   // deletes prediction comment, both global & specific, index 1
    srv->waitPeriod(waitTime1*5);
    srv->deleteValidationComment("prediction", 2, 0);                   // deletes prediction comment, both, any saved comment

    srv->validateInterpretation();

    srv->printReport("Report_1234_b");


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

    srv->waitForSync();
    srv->waitPeriod(waitTime1*10);
    srv->synchronize();

    std::cout << "End of test..." << std::endl;
}
