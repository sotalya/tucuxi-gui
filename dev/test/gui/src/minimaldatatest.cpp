//@@lisence@@

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

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"

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

    dosageData1.dosage = 2345;

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
