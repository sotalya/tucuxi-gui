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

    QString drugName    = "Cefepime";                            // drug index : 6 = Cefepime, 13 = Imatinib, ...
    int modelIndex  = 0;                            // model index: domain & study

    srv->selectDrugInList(drugName, modelIndex);
    srv->waitForSync();

    //_____Adding dosage, covariates, measure & target (n times)___________________________________________________________________________________

    DosageData dosageData1;
    MeasureData measureData1;

    int m = 1;

    //_____Add dosage_______________________________________________

    dosageData1.dosage      = 2000 + (m-1)*1000 + m*100 + m*30 + m*1;   // dosage(n) = {2234, 3345, 4456, 5567, ...}
    dosageData1.infusion    = m*100;                                    // infusion(i) = {100, 200, 300, 400, ...}
    dosageData1.interval    = m*12;                                     // interval(i) = {12, 24, 36, 48, ...}
    dosageData1.steadyState = false;                                    // if steadyState = false -> start date & end date needed
    dosageData1.dateTimeDos1.setDate(QDate(2022, m, m));                // start date(i)
    dosageData1.dateTimeDos1.setTime(QTime(m, m));                      // start time(i)
    dosageData1.dateTimeDos2.setDate(QDate(2022, m+1, m+1));            // end date(i)
    dosageData1.dateTimeDos2.setTime(QTime(m, m));                      // end time(i)

    srv->addDosage(dosageData1);
    srv->waitForSync();


    for (int n = 1; n <= 10; n++)        // index of loop is used to modify variables to avoid having to do it manually for each iteratnon
    {

        //_____Add measure______________________________________________

        measureData1.name   = "Sample_1234_b" + QString::number(n);
        measureData1.measure  = 100 + n*10 + n*1;
        measureData1.dateTimeMeas.setDate(QDate(2022, m, m+n));
        measureData1.dateTimeMeas.setTime(QTime(m, m));

        srv->addMeasure(measureData1);
        srv->waitPeriod(waitTime1);
    }

    srv->waitForSync();
    srv->waitPeriod(waitTime1);

    std::cout << "End of test..." << std::endl;
}
