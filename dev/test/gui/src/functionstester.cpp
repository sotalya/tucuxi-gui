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

extern SpixGTest* srv;


TEST(FunctionsTester, Test1)
{

    int waitTime1 = 2;

    srv->waitPeriod(waitTime1);             // default (when no arg.) value : 1/2 [s]

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->startNewPatient();                 // call function which clicks on "New patient" icon
//    srv->waitPeriod(waitTime1);


//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    QString drugName = "Cefepime";          // drug to be chosen HERE

    srv->selectDrugInList(6,0);
    //srv->selectDrugInList(20, 7);         // (int drugIndex, int modelIndex (= domain & study)

    srv->waitPeriod(waitTime1);

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    DosageData dosageData1;
    dosageData1.dosage = 7500;
    dosageData1.infusion = 900;
    dosageData1.interval = 4800;
    dosageData1.steadyState = false;
    dosageData1.dateTimeDos1.setDate(QDate(2022, 05, 07));
    dosageData1.dateTimeDos1.setTime(QTime(15,23));
    dosageData1.dateTimeDos2.setDate(QDate(2022, 06, 13));
    dosageData1.dateTimeDos2.setTime(QTime(07,55));


//    srv->addDosage(dosageData1);

//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/removeDosage_2"));
//    srv->waitPeriod(waitTime1*4);

//    // new values which require modification
//    dosageData1.dosage = 7501;
//    dosageData1.infusion = 902;
//    dosageData1.interval = 2400;
//    dosageData1.dateTimeDos1.setDate(QDate(2022, 07, 06));
//    dosageData1.dateTimeDos1.setTime(QTime(14,05));
//    dosageData1.dateTimeDos2.setDate(QDate(2022, 07, 15));
//    dosageData1.dateTimeDos2.setTime(QTime(15,06));

//    srv->addDosage(dosageData1);

//    srv->editDosage(dosageData1);
//    srv->waitPeriod(waitTime1);


//    srv->removeFromList("Dosage", 0);                                         // Runs ok
//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/removeDosage_2"));    // Runs ok


//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    CovariatesData covariatesData1;
    covariatesData1.covarName = "weigth";
    covariatesData1.sex = "Unknown";
    covariatesData1.scc = 3720;
    covariatesData1.dateTimeCovar.setDate(QDate(2022, 02, 03));
    covariatesData1.dateTimeCovar.setTime(QTime(06,03));


    srv->addCovariates(covariatesData1);

    srv->waitPeriod(waitTime1);
//    srv->editCovariates(covariatesData1, 0);

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    QDateTime dateTime1;
    dateTime1.setDate(QDate(2022,02,05));
    dateTime1.setTime(QTime(02, 03, 0));

    QDateTime dateTime2;
    dateTime2.setDate(QDate(2021,07,01));
    dateTime2.setTime(QTime(05, 06, 0));

//    for (int i = 0; i < 1; i++){
//        srv->addMeasure(666+i, dateTime1);
//        srv->waitPeriod(waitTime1);
//    }

    dosageData1.dosage = 1111;
    dosageData1.infusion = 222;
    dosageData1.interval = 7200;
    dosageData1.dateTimeDos1.setDate(QDate(2024, 10, 07));
    dosageData1.dateTimeDos1.setTime(QTime(14,06));
    dosageData1.dateTimeDos2.setDate(QDate(2025, 11, 03));
    dosageData1.dateTimeDos2.setTime(QTime(15,22));

//    srv->synchronize();
//    srv->editDosage(dosageData1, 2);
//    srv->removeFromList("Dosage", 1);

//    srv->waitPeriod(waitTime1*4);

//    srv->editMeasure(745, dateTime2);
//    srv->waitPeriod(waitTime1);

//    srv->removeFromList("Measure", 2);



//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    TargetData targetData1;
    targetData1.targetType  = 8;        // index 0-12 : peak = 1, AUC = [3-9], MIC [5-12]
    targetData1.cMinInput   = 602;
    targetData1.cBestInput  = 813;
    targetData1.cMaxInput   = 951;
    targetData1.tMinInput   = 20;
    targetData1.tBestInput  = 30;
    targetData1.tMaxInput   = 40;
    targetData1.micInput    = 1300;

//    for (int i = 0; i < 5; i++){
//        srv->addTarget(targetData1);
//        srv->waitPeriod();
//    }
//    srv->removeFromList("Target", 02);


//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    AdjustmentsData adjustmentsData1;
    adjustmentsData1.dateTimeAdj.setDate(QDate(2022,04,06));
    adjustmentsData1.dateTimeAdj.setTime(QTime(13, 52,0));
    adjustmentsData1.loadingDose = true;
    adjustmentsData1.restPeriod = true;
    adjustmentsData1.suggestAdjNum = 3;

//    srv->setAdjustments(adjustmentsData1);

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-




//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-


    srv->waitPeriod(waitTime1*10);
    qInfo() << "End of test...";
}
