//@@license@@

// File title       :   addeditremovetest.cpp
// Test title       :   AddEditRemoveTest
// Author           :   Julien Rosset
// Creation date    :   07.03.2022
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

#include <QDebug>                       // allows to print text in console : qInfo() << "text", or qDebug()"

extern SpixGTest* srv;


//________________________________________________________________________________________________________________________________________________

TEST(AddEditRemoveTest, Test1)
{

    int waitTime1       = 1;                                                  // wait time in [s/2]
    int waitTimeLong    = 4;

    std::cout << "Test name : AddEditRemoveTest" << std::endl;

    srv->waitPeriod(waitTimeLong);

    //*****Launch view*****//

    //_____Start a new patient____________________________________________________________________________________________________________________

    srv->startNewPatient();

    srv->waitPeriod(waitTimeLong);


    //*****Main window*****//

    //_____Select drug____________________________________________________________________________________________________________________________

    int modelIndex  = 0;                                                // model index: domain & study
    srv->getCurrentTabIndex();
    srv->selectDrugInList("Imatinib", modelIndex);
    srv->getCurrentTabIndex();
    srv->waitPeriod(waitTimeLong);

    //_____Adding dosage, covariates, measure & target (n times)___________________________________________________________________________________

    DosageData dosageData1;
    CovariatesData covariatesData1;
    MeasureData measureData1;
    TargetData targetData1;

    int n = 1;

    for (n = 1; n <= 2; n++)        // index of loop is used to modify variables to avoid having to do it manually for each iteratnon
    {

        //_____Add dosage_______________________________________________

        dosageData1.dosage      = n*200 + n*10;                         // dosage(n) = {210, 420, 630, 840, ...}
        dosageData1.infusion    = n*100;                                // infusion(n) = {100, 200, 300, 400, ...}
        dosageData1.interval    = n*12;                                 // interval(n) = {12, 24, 36, 48, ...}
        dosageData1.steadyState = false;                                // if steadyState = false -> start date & end date needed
        dosageData1.dateTimeDos1.setDate(QDate(2022, n, n));            // start date(n)
        dosageData1.dateTimeDos1.setTime(QTime(n, n));                  // start time(n)
        dosageData1.dateTimeDos2.setDate(QDate(2022, n+1, n+1));        // end date(n)
        dosageData1.dateTimeDos2.setTime(QTime(n+1, n+1));              // end time(n)

        srv->addDosage(dosageData1);
        srv->waitPeriod(waitTime1);

        //_____Add covariates___________________________________________

        int covarTypeIndex = 0;                                         // covariable type index 0 = weight

        covariatesData1.weight = 63 + n*5;
        covariatesData1.dateTimeCovar.setDate(QDate(2022, n, n));
        covariatesData1.dateTimeCovar.setTime(QTime(n, n));

        srv->addCovariates(covariatesData1, covarTypeIndex);
        srv->waitPeriod(waitTime1);

        //_____Add measure______________________________________________

        measureData1.name   = "Sample_1234_b" + QString::number(n);
        measureData1.value  = 200 + n*100 + n*15;
        measureData1.dateTimeMeas.setDate(QDate(2022, n+1, n+2));
        measureData1.dateTimeMeas.setTime(QTime(n+1, n+2));

        srv->addMeasure(measureData1);
        srv->waitPeriod(waitTime1);

        //_____Add target_______________________________________________

        targetData1.targetType  = 0;
        targetData1.cMinInput   = 500 + n*50 + n;
        targetData1.cBestInput  = (n+1)*500 + (n+1)*50 + (n+1);
        targetData1.cMaxInput   = (n+2)*500 + (n+2)*50 + (n+2);
        targetData1.tMinInput   = n*10;
        targetData1.tBestInput  = (n+1)*10;
        targetData1.tMaxInput   = (n+2)*10;
        targetData1.micInput    = (n*100) + 1000;

        srv->addTarget(targetData1);

        srv->waitPeriod(waitTimeLong);

    }


    //_____Editing dosage, covariates, measure & target (one each) with new values___________________________________________________________________

    std::cout << "It's ***EDITION*** time lads !" << std::endl;
    srv->waitPeriod(waitTimeLong);

    //_____Edit dosage__________________________________________________


        dosageData1.dosage      = 432;
        dosageData1.infusion    = 18;
        dosageData1.interval    = 24;
        dosageData1.steadyState = true;
        dosageData1.dateTimeDos1.setDate(QDate(2022, 6, 6));
        dosageData1.dateTimeDos1.setTime(QTime(5, 30));

        int editDosageIndex     = 1;

        srv->editDosage(dosageData1, editDosageIndex);
        srv->waitPeriod(waitTime1);

        dosageData1.dosage = 543;
        dosageData1.steadyState = false;
        dosageData1.dateTimeDos1.setDate(QDate(2023, 7, 8));
        dosageData1.dateTimeDos1.setTime(QTime(7, 8));
        dosageData1.dateTimeDos2.setDate(QDate(2023, 7, 10));
        dosageData1.dateTimeDos2.setTime(QTime(5, 8));

        editDosageIndex = 1;

        srv->editDosage(dosageData1, editDosageIndex);
        srv->waitPeriod(waitTime1);

    //_____Edit covariate_______________________________________________

        covariatesData1.weight = 52;
        covariatesData1.dateTimeCovar.setDate(QDate(2022, 2, 3));
        covariatesData1.dateTimeCovar.setTime(QTime(2, 3));

        int covarTypeIndex = 0;                                             // covariate type index 0 = weight
        int editCovariateIndex = 0;

        srv->editCovariates(covariatesData1, covarTypeIndex, editCovariateIndex);
        srv->waitPeriod(waitTime1);


    //_____Edit measure_________________________________________________

        measureData1.name = "Sample_2345_Edit";
        measureData1.value = 454;
        measureData1.dateTimeMeas.setDate(QDate(2022, 4, 5));
        measureData1.dateTimeMeas.setTime(QTime(2, 3));

        int editMeasureIndex = 0;

        srv->editMeasure(measureData1, editMeasureIndex);
        srv->waitPeriod(waitTime1);


    //_____Edit target__________________________________________________

        targetData1.targetType  = 8;
        targetData1.cMinInput   = 1000;
        targetData1.cBestInput  = 1500;
        targetData1.cMaxInput   = 2000;
        targetData1.tMinInput   = 14;
        targetData1.tBestInput  = 36;
        targetData1.tMaxInput   = 72;
        targetData1.micInput    = 1234;

        int editTargetIndex = 0;

        srv->editTarget(targetData1, editTargetIndex);
        srv->waitPeriod(waitTimeLong);


    //_____Removing dosage, covariate, measure & target___________________________________________________________________

        std::cout << "It's ***DELETION*** time lads !" << std::endl;
        srv->waitPeriod(waitTime1);

        srv->removeFromList("target",       1);                             // if removeIndex > 0, only chosen input will be removed
        srv->waitPeriod(waitTimeLong);
        srv->removeFromList("measure",      1);                             // if removeIndex <= 0, all above inputs will also be removed
        srv->waitPeriod(waitTimeLong);
        srv->removeFromList("covariate",    1);
        srv->waitPeriod(waitTimeLong);
        srv->removeFromList("dosage",       1);
        srv->waitPeriod(waitTimeLong);

    //_____Adding once more dosage, covariate, measure & target___________________________________________________________





    //_______________________________________________________

    srv->waitPeriod(waitTimeLong);
    //    srv->synchronize();

    std::cout << "End of test AddEditRemoveTest..." << std::endl;

}
