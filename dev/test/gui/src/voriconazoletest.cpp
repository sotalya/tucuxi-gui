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

///
/// \brief TEST of Voriconazole percentiles
/// This test was meant to be ran with only voriconazole in the drug models folder.
/// It helped discover and debug issues with percentiles calculations for voriconazole.
TEST(VoriconazoleTest, DISABLED_Test1)
{

    int waitTime1       = 1;                                                  // wait time in [s/2]
    int waitTimeLong    = 1;//25;

    std::cout << "Test name : VoriconazoleTest" << std::endl;

    srv->waitPeriod(waitTimeLong);

    //*****Launch view*****//

    //_____Start a new patient____________________________________________________________________________________________________________________

    srv->startNewPatient();

    srv->waitPeriod(waitTimeLong);


    //*****Main window*****//

    //_____Select drug____________________________________________________________________________________________________________________________

    // Assume a single model: Voriconazole
    QString drugName = "Voriconazole";
    int modelIndex  = 0;
    srv->getCurrentTabIndex();
    srv->selectDrugInList(drugName, modelIndex);
    srv->getCurrentTabIndex();
    srv->waitPeriod(waitTimeLong);

    //_____Adding dosage, covariates, measure & target (n times)___________________________________________________________________________________

    DosageData dosageData1;
    CovariatesData covariatesData1;
    MeasureData measureData1;
    TargetData targetData1;

    int n = 1;

    //_____Add dosage_______________________________________________

    dosageData1.dose      = 20000;
    dosageData1.infusionTimeInMinutes    = 60;
    dosageData1.intervalInHours    = 12;
    dosageData1.steadyState = true;                                // if steadyState = false -> start date & end date needed
    // Use default date and time
    // dosageData1.dateTimeDos1.setDate(QDate(2022, 12, 16));            // start date(n)
    // dosageData1.dateTimeDos1.setTime(QTime(8, 0));                  // start time(n)

    srv->waitForSync();
    srv->addDosage(dosageData1);

    srv->waitPeriod(waitTime1);


    srv->mouseClickIfPathOk("mainWindow/flowView/covariateButton");
    srv->waitPeriod(waitTimeLong);
    srv->waitPeriod(waitTimeLong);
    srv->waitPeriod(waitTimeLong);
    srv->waitPeriod(waitTimeLong);



    srv->waitForSync();
    srv->mouseClickIfPathOk("mainWindow/flowView/dosageButton");
    srv->waitForSync();

    dosageData1.dose      = 20000;
    dosageData1.infusionTimeInMinutes    = 60;
    dosageData1.intervalInHours    = 12;
    dosageData1.steadyState = false;                                // if steadyState = false -> start date & end date needed
    // Use default date and time
    dosageData1.dateTimeDos2 = dosageData1.dateTimeDos1.addDays(3);

    srv->waitForSync();
    srv->editDosage(dosageData1, 0);

    srv->waitPeriod(waitTime1);


    srv->mouseClickIfPathOk("mainWindow/flowView/covariateButton");
    srv->waitPeriod(waitTimeLong);
    srv->waitPeriod(waitTimeLong);
    srv->waitPeriod(waitTimeLong);
    srv->waitPeriod(waitTimeLong);


    srv->waitForSync();
    srv->mouseClickIfPathOk("mainWindow/flowView/dosageButton");


    dosageData1.dose      = 20000;
    dosageData1.infusionTimeInMinutes    = 60;
    dosageData1.intervalInHours    = 12;
    dosageData1.steadyState = true;                                // if steadyState = false -> start date & end date needed


    srv->waitForSync();
    srv->editDosage(dosageData1, 0);

    srv->waitPeriod(waitTime1);


    srv->mouseClickIfPathOk("mainWindow/flowView/covariateButton");
    srv->waitPeriod(waitTimeLong);
    srv->waitPeriod(waitTimeLong);
    srv->waitPeriod(waitTimeLong);
    srv->waitPeriod(waitTimeLong);

    srv->waitForSync();


    //_____Editing dosage, covariates, measure & target (one each) with new values___________________________________________________________________

    std::cout << "It's ***EDITION*** time lads !" << std::endl;
    srv->waitPeriod(waitTimeLong);

    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    //    srv->synchronize();

    std::cout << "End of test VoriconazoleTest..." << std::endl;

}
