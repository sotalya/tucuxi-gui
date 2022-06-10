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
#include "guiutils/src/chartdatacontroller.h"

#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"


#include "core/dal/chartdata.h"
#include "core/dal/predictionresult.h"


#include <QDebug>
#include <QMessageBox>

extern SpixGTest* srv;


TEST(FunctionsTester, Test1)
{

    int waitTime1       = 1;
    int waitTimeLong    = 5;

    srv->waitPeriod(waitTime1);                 // default (when no arg.) value : 1/2 [s]

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->startNewPatient();                     // call function which clicks on "New patient" icon
    srv->waitPeriod(waitTime1);

    PatientData patientData1;
    srv->fillInPatientData(patientData1);
    srv->waitPeriod(waitTime1);

    srv->selectDrugInList(13, 0);
    srv->waitPeriod(waitTime1);

    DosageData dosageData1;
    dosageData1.steadyState = false;
    dosageData1.dateTimeDos1.setDate(QDate(2022, 03, 25));
    dosageData1.dateTimeDos1.setTime(QTime(8, 00));
    dosageData1.dateTimeDos2.setDate(QDate(2022, 03, 30));
    dosageData1.dateTimeDos2.setTime(QTime(8, 01));
    dosageData1.interval = 24;
    srv->addDosage(dosageData1);
    srv->waitForSync();

    ezechiel::GuiCore::FancyPoints* fPoints = srv->m_mainWindowController->getInterpretationController()->chartData->getPopPred()->getPredictive()->getPredictionData()->getPoints();
    //    ezechiel::core::FancyPoint* fPoint1 = fPoints->at(3);
    //    std::cout << "fPoint1 value : " << fPoint1->getValue() << std::endl;
    //    std::cout << "fPoint1 time : " << fPoint1->getTime() << std::endl;
    //    QDateTime tTime1 = QDateTime::fromMSecsSinceEpoch((fPoint1->getTime()*1000));
    //    qInfo() << "fPoint1 QDateTime : " << tTime1.toString();

    int index = 1;

    for (int minimumIndex = 0; minimumIndex <= 5; minimumIndex++)
    {
        while (index < 10000 && !(fPoints->at(index)->getValue() <= fPoints->at(index-1)->getValue() and fPoints->at(index)->getValue() < fPoints->at(index+1)->getValue()))
        {
            //        std::cout << "index : " << index << " || value : " << fPoints->at(index)->getValue() << std::endl;
            index++;
        }

        double fPointMin = fPoints->at(index)->getValue();
        QDateTime fDateMin = QDateTime::fromMSecsSinceEpoch((fPoints->at(index)->getTime()*1000));

        std::cout << "Minimum n°" << minimumIndex;
        std::cout << "  || value : " << fPoints->at(index)->getValue();
        std::cout << "  || date : " << fDateMin.toString("yyyy/MM/dd hh:mm:ss").toStdString() << std::endl;

        index++;
    }

//    double fPointMin = fPoints->at(index)->getValue();

    srv->waitPeriod(300);


    //    CovariatesData covariatesData1;
    //    covariatesData1.weight = 67.45;
    //    covariatesData1.scc = 8123;
    //    srv->addCovariates(covariatesData1, 0);
    //    srv->addCovariates(covariatesData1, 3);

    //    CovariatesData covariatesData1;
    //    int covariateType   = 3;                   // covariateType : 3 = Serum creatinin concentration
    //    int sccInitialValue = 8010;

    //    for (int n = 1; n <= 10; n++)
    //    {
    //        covariatesData1.scc = sccInitialValue + n*1;
    //        covariatesData1.dateTimeCovar.setDate(QDate::currentDate().addDays(-n));
    //        srv->addCovariates(covariatesData1, covariateType);
    //        srv->waitPeriod(waitTime1*4);
    //        srv->synchronize();
    //    }

    //        srv->waitPeriod(waitTimeLong);
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
