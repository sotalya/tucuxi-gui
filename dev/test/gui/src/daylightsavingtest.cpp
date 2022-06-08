//@@license@@

// File title       :   daylightsavingtest.cpp
// Test title       :   DaylightSavingTest
// Author           :   Julien Rosset
// Creation date    :   03.06.2022
// Purpose          :   Sets doses before daylight saving time changes to check their inpact on take times.
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
#include <QQmlApplicationEngine>

#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"


#include "core/dal/chartdata.h"


#include <QDebug>
#include <QMessageBox>

extern SpixGTest* srv;


TEST(DaylightSavingTest, Test1)
{
    int waitTime1       = 1;
    int waitTimeLong    = 5;

    srv->waitPeriod(waitTime1);

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);

    srv->selectDrugInList(13, 0);
    srv->waitPeriod(waitTime1);

    DosageData dosageData1;
    dosageData1.steadyState = true;
    dosageData1.dateTimeDos1.setDate(QDate(2022, 03, 25));
    dosageData1.dateTimeDos1.setTime(QTime(8, 00));
    dosageData1.dateTimeDos2.setDate(QDate(2022, 03, 30));
    dosageData1.dateTimeDos2.setTime(QTime(9, 00));
    dosageData1.interval = 24;
    srv->addDosage(dosageData1);
    srv->waitForSync();

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-


    // Fancy points are only the points visible
    ezechiel::core::FancyPoints* fPoints = srv->m_mainWindowController->getInterpretationController()->chartData->getPopPred()->getPredictive()->getPredictionData()->getPoints();

    int pIndex = 1;
    double fValueMin = 0;
    QDateTime fDateMin;
    int daysToGo = dosageData1.dateTimeDos1.daysTo(dosageData1.dateTimeDos2);

    (daysToGo > 15) ? (daysToGo = 15) : (daysToGo);
    std::cout << "Days to go : " << daysToGo << std::endl;

    if (!dosageData1.steadyState)
    {
        for (int minimumIndex = 1; minimumIndex <= daysToGo; minimumIndex++)
        {
            while (pIndex < 100000 && !(fPoints->at(pIndex)->getValue() <= fPoints->at(pIndex-1)->getValue() and fPoints->at(pIndex)->getValue() < fPoints->at(pIndex+1)->getValue()))
            {
                //            std::cout << "Point index : " << pIndex << "\t value : " << fPoints->at(pIndex)->getValue();
                //            fDateMin = QDateTime::fromMSecsSinceEpoch((fPoints->at(pIndex)->getTime()*1000));
                //            std::cout << "\t||\t date : " << fDateMin.toString("yyyy/MM/dd hh:mm.ss").toStdString() << std::endl;
                pIndex++;
            }
            fValueMin = fPoints->at(pIndex)->getValue();
            fDateMin = QDateTime::fromMSecsSinceEpoch((fPoints->at(pIndex)->getTime()*1000));

            std::cout << "\nMinimum n°" << minimumIndex;
            std::cout << " :\t value : " << fValueMin;
            std::cout << "\t||\t date : " << fDateMin.toString("yyyy/MM/dd hh:mm.ss").toStdString() << "\n" << std::endl;

            pIndex++;       // skip a point because last 2 points before new take have the same value
        }


//        srv->waitPeriod(4);
            // work fine in non-steady state
//        srv->extSetView();
//        srv->m_mainWindowController->getInterpretationController()->InterpretationController::setViewRange(dosageData1.dateTimeDos1, dosageData1.dateTimeDos2);

    }
    else    // steadyState
    {
        // apparently, the point of index = 0 is 1st of June 2022

        //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

//        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->chartView, "extSetView",
//                                  Q_ARG(QVariant, QVariant::fromValue(666)));

        srv->extSetView();
        srv->waitPeriod(4);
//                srv->m_mainWindowController->getInterpretationController()->InterpretationController::extSetViewRange(dosageData1.dateTimeDos1, dosageData1.dateTimeDos2);
//        srv->m_mainWindowController->getInterpretationController()->InterpretationController::setViewRange(dosageData1.dateTimeDos1, dosageData1.dateTimeDos1.addDays(4));

        //                srv->m_mainWindowController->getInterpretationController()->goToPreviousEvent();


        //        QDateTime fDate1 = QDateTime::fromMSecsSinceEpoch(range);
        //        std::cout << fDate1.toString("yyyy/MM/dd hh:mm.ss").toStdString() << "\n" << std::endl;


        //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

        //        while (fDateMin.daysTo(dosageData1.dateTimeDos1) > 0)
        //        {
        //        while(pIndex < 10000)
        //        {
        //            fDateMin = QDateTime::fromMSecsSinceEpoch((fPoints->at(pIndex)->getTime()*1000));
        //            std::cout << fDateMin.toString("dd.MM.yyyy").toStdString() << std::endl;
        //            pIndex++;
        //        }

    }

    srv->waitPeriod(40);


    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    std::cout << "End of test..." << std::endl;

}
