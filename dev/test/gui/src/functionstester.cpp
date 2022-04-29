//@@license@@

// File title       :   functionstester.cpp
// Test title       :   FunctionsTester
// Author           :   Julien Rosset
// Creation date    :   22.02.2022
// Purpose          :   Calls and tests functions in guitest.cpp
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


TEST(FunctionsTester, Test1)
{

    int waitTime1 = 1;
    int waitTimeLong = 4;

    srv->waitPeriod(waitTime1);                 // default (when no arg.) value : 1/2 [s]

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->startNewPatient();                     // call function which clicks on "New patient" icon
    srv->waitPeriod(waitTime1);

    PatientData patientData1;
    srv->fillInPatientData(patientData1);
    srv->waitPeriod(waitTime1*20);

    srv->selectDrugInList(6, 0);
    srv->waitPeriod(waitTime1);

    CovariatesData covariatesData1;
    int covariateType   = 3;                      // covariateType : 3 = Serum creatinin concentration
    int sccInitialValue = 8010;

    for (int n = 1; n <= 10; n++)
    {
        covariatesData1.scc = sccInitialValue + n*1;
        covariatesData1.dateTimeCovar.setDate(QDate::currentDate().addDays(-n));
        srv->addCovariates(covariatesData1, covariateType);
        srv->waitPeriod(waitTime1*4);
        srv->synchronize();
    }

        srv->waitPeriod(waitTimeLong*2);
    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

//    DosageData dosageData1;


//    srv->addDosage(dosageData1);
//    srv->waitPeriod(waitTime1);

//    MeasureData measureData1;
//    srv->addMeasure(measureData1);
//    srv->waitPeriod(waitTime1);

//    TargetData targetData1;
//    srv->addTarget(targetData1);

//    AdjustmentsData adjustmentsData1;

//    srv->addAdjustments(adjustmentsData1);          // manually add new adjustment
//    srv->selectAdjustments(1);                      // select suggested adjustment from list
//    srv->waitPeriod(waitTime1);
//    srv->editAdjustments(adjustmentsData1, 0);
//    srv->waitPeriod(waitTimeLong*4);

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    std::cout << "End of test..." << std::endl;
}
