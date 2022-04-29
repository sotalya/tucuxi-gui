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
#include <Spix/Events/KeyCodes.h>

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
    int waitTimeLong = 1;

    srv->waitPeriod(waitTime1);

    srv->startNewPatient();
    srv->waitPeriod(waitTimeLong);

    srv->fillInPatientData(patientData1);
    srv->waitPeriod(waitTimeLong);

    srv->selectDrugInList(13, 0);
    srv->waitPeriod(waitTimeLong);

    srv->addDosage(dosageData1);
    srv->waitPeriod(waitTimeLong);

    srv->addCovariates(covariatesData1, 0);
    srv->waitPeriod(waitTimeLong);

    srv->addMeasure(measureData1);
    srv->waitPeriod(waitTimeLong);

    srv->addTarget(targetData1);
    srv->waitPeriod(waitTimeLong);

    srv->addAdjustments(adjustmentsData1);
    srv->waitPeriod(waitTimeLong);

    srv->fillInValidationData(validationData1);
    srv->waitPeriod(waitTimeLong);

    srv->saveValidationComment("warning", 2);
    srv->waitPeriod(waitTimeLong);

    srv->validateInterpretation();
    srv->waitPeriod(waitTimeLong);


    srv->waitPeriod(waitTimeLong);

    std::cout << "End of test..." << std::endl;



}
