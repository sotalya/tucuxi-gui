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
#include <Spix/Events/Identifiers.h>

#include "guitest.h"

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/chartdatacontroller.h"
#include <QQmlApplicationEngine>

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

    srv->selectDrugInList("Cefepime", 0);
    srv->waitPeriod(waitTime1);
    srv->waitForSync();

    DosageData dosageData1;
    dosageData1.dose = 2500;
    dosageData1.steadyState = false;
    dosageData1.dateTimeDos1.setDate(QDate(2022, 3, 24));
    dosageData1.dateTimeDos1.setTime(QTime(8, 0));
    dosageData1.dateTimeDos2.setDate(QDate(2022, 4, 15));
    dosageData1.dateTimeDos2.setTime(QTime(9, 0));
    dosageData1.intervalInHours = 24;
    dosageData1.infusionTimeInMinutes = 30;
    srv->addDosage(dosageData1);
    srv->waitForSync();

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    int pIndex = 1;             // point index
    double fValueMin = 0;       // c value [g/L]
    QDateTime fDateMin;
    int daysToGo = dosageData1.dateTimeDos1.daysTo(dosageData1.dateTimeDos2);   // number of days between two chosen dates

    std::cout << "Days to go : " << daysToGo << std::endl;

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    if (dosageData1.steadyState)    // steady state
    {
        // set a constant number of days to show on screen at steady state (6 days is enough)
        daysToGo = 6;
    }
    else                            // non steady state
    {
        (daysToGo > 10) ? (daysToGo = 10) : (daysToGo);     // allow up to 10 days to show on screen
        // work fine in non-steady state. Had to create extSetView & signal-slot to set view range at steady state
        //        srv->m_mainWindowController->getInterpretationController()->InterpretationController::setViewRange(dosageData1.dateTimeDos1, dosageData1.dateTimeDos2);
    }

    srv->extSetView(dosageData1.dateTimeDos1, dosageData1.dateTimeDos1.addDays(daysToGo));
    srv->waitForSync();

    // Fancy points are only visible points on screen !
    Tucuxi::Gui::Core::FancyPoints* fPoints = srv->m_mainWindowController->getInterpretationController()->chartData->getPopPred()->getPredictive()->getPredictionData()->getPoints();

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    for (int minimumIndex = 1; minimumIndex <= daysToGo; minimumIndex++)
    {
        while (pIndex < 100000 && !(fPoints->at(pIndex)->getValue() <= fPoints->at(pIndex-1)->getValue() && fPoints->at(pIndex)->getValue() < fPoints->at(pIndex+1)->getValue()))
        {   // uncomment to print full point data
//                        std::cout << "Point index : " << pIndex << "\t value : " << fPoints->at(pIndex)->getValue();
//                        fDateMin = QDateTime::fromMSecsSinceEpoch((fPoints->at(pIndex)->getTime()*1000));
//                        std::cout << "\t||\t date : " << fDateMin.toString("yyyy/MM/dd hh:mm.ss").toStdString() << std::endl;
            pIndex++;
        }
        fValueMin = fPoints->at(pIndex)->getValue();
        fDateMin = QDateTime::fromMSecsSinceEpoch((fPoints->at(pIndex)->getTime()*1000));

        std::cout << "\nMinimum nbr " << minimumIndex;
        std::cout << " :\t value : " << fValueMin;
        std::cout << "\t||\t date : " << fDateMin.toString("dd/MM/yyyy hh:mm.ss").toStdString() << "\n" << std::endl;

        pIndex++;       // skip a point because the last 2 points before new take have the same value
    }

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    std::cout << "End of test..." << std::endl;

}
