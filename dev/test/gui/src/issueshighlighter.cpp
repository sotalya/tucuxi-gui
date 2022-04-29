//@@license@@

// File title       :   issuesHighlighter.cpp
// Test title       :   Issues Highlighter
// Author           :   Julien Rosset
// Creation date    :   04.04.2022
// Purpose          :   Contains tests which produces bugs or crashes
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


TEST(IssuesHighlighter, Test1)
{

    int waitTime1 = 2;
    int waitTimeLong = 3;

    srv->waitPeriod(waitTime1);

    //*****Launch view*****//

    //_____Start a new patient____________________________________________________________________________________________________________________

    srv->startNewPatient();

    srv->waitPeriod(waitTimeLong);


    //*****Main window*****//

    //_____Select drug____________________________________________________________________________________________________________________________

    int drugIndex   = 6;                            // drug index : 6 = Cefepime, 13 = Imatinib, ...
    int modelIndex  = 0;                            // model index: domain & study

    srv->selectDrugInList(drugIndex, modelIndex);
    srv->waitPeriod(waitTimeLong);

    //_____Adding dosage, covariates, measure & target (n times)___________________________________________________________________________________

    DosageData dosageData1;
    MeasureData measureData1;

    int n = 1;

    //_____Add dosage_______________________________________________

    dosageData1.dosage      = n*1000 + n*200 + n*30 + n*1;          // dosage(n) = {1234, 2345, 3456, 4567, ...}
    dosageData1.infusion    = n*1000;                               // infusion(i) = {100, 200, 300, 400, ...}
    dosageData1.interval    = n*1200;                               // interval(i) = {1200, 2400, 3600, 4800, ...}
    dosageData1.steadyState = false;                                // if steadyState = false -> start date & end date needed
    dosageData1.dateTimeDos1.setDate(QDate(2022, n, n));            // start date(i)
    dosageData1.dateTimeDos1.setTime(QTime(n, n));                  // start time(i)
    dosageData1.dateTimeDos2.setDate(QDate(2022, n+1, n+1));        // end date(i)
    dosageData1.dateTimeDos2.setTime(QTime(n+1, n+1));              // end time(i)

    srv->waitForSync();
    srv->addDosage(dosageData1);

    srv->waitPeriod(waitTime1);


    for (n = 1; n <= 10; n++)        // index of loop is used to modify variables to avoid having to do it manually for each iteratnon
    {

        //_____Add measure______________________________________________

        measureData1.name   = "Sample_1234_b" + QString::number(n);
        measureData1.value  = 5000 + n*100 + n*100 + n*10 + n*1;
        measureData1.dateTimeMeas.setDate(QDate(2022, n, n));
        measureData1.dateTimeMeas.setTime(QTime(n, n));

        srv->waitForSync();
        srv->addMeasure(measureData1);

        srv->waitPeriod(waitTime1);
    }

    srv->waitForSync();
    srv->waitPeriod(waitTime1);

    std::cout << "End of test..." << std::endl;
}
