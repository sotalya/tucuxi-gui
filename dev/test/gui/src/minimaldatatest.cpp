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


// File title       :   minimaldatatest.cpp
// Test title       :   minimalDataTest
// Author           :   Julien Rosset
// Creation date    :   27.04.2022
// Purpose          :
// ...              :   ...

#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/Identifiers.h>

#include "guitest.h"

#include <QDebug>
#include <QMessageBox>

extern SpixGTest* srv;


TEST(MinimalDataTest, Test1)
{
    PatientData patientData1;
    DosageData dosageData1;
    CovariatesData covariatesData1;
    MeasureData measureData1;
    TargetData targetData1;
    AdjustmentsData adjustmentsData1;
    ValidationData validationData1;

    int waitTime1 = 1;
    int waitTimeLong = 2;

    srv->waitPeriod(waitTime1);

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);

    srv->fillInPatientData(patientData1);
    srv->waitPeriod(waitTime1);

    srv->selectDrugInList("Cefepime", 0);
    srv->waitPeriod(waitTime1);

    dosageData1.dose = 2345;

    srv->addDosage(dosageData1);
    srv->waitPeriod(waitTime1);

    srv->addCovariates(covariatesData1, 0);
    srv->waitPeriod(waitTime1);

    srv->addCovariates(covariatesData1, 3);
    srv->waitPeriod(waitTime1);

    srv->addMeasure(measureData1);
    srv->waitPeriod(waitTime1);

    srv->addTarget(targetData1);
    srv->waitPeriod(waitTime1);

    srv->addAdjustments(adjustmentsData1);
    srv->waitPeriod(waitTime1);

    srv->fillInValidationData(validationData1);
    srv->waitForSync();

    srv->validateInterpretation();
    srv->waitPeriod(waitTime1);


    srv->waitPeriod(waitTimeLong);

    std::cout << "End of test..." << std::endl;



}
