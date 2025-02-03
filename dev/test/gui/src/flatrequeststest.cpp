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


#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/Identifiers.h>

#include "admin/src/dal/partialrequest.h"
#include "core/dal/drug/drug.h"
#include "guitest.h"
#include "guiutils/src/requestscontroller.h"
#include "flatrequests/flatrequestfileclient.h"

#include <QDebug>

extern SpixGTest* srv;

using namespace Tucuxi::Gui::Admin;

TEST(FlatRequestsTest, Test1)
{
    int waitTime1       = 2;
    int waitTimeLong    = 6;

    QString flatFile = "test1_cefepime.xml"; //"test2_cefepime.xml";

    std::cout << "\033[1;36m" << "Test name : FlatRequestsTest.Test1" << "\033[0m" << std::endl;

    srv->waitPeriod(waitTimeLong);

    Tucuxi::Gui::FlatRequest::FlatRequestFileClient *client = new Tucuxi::Gui::FlatRequest::FlatRequestFileClient(srv->m_mainWindowController);
    client->setListFile(flatFile);
    srv->m_mainWindowController->getRequestController()->setClient(client);

    srv->mouseClickIfPathOk("mainWindow/launchView/pendingRequests/mouseArea");
    srv->waitPeriod(waitTimeLong);

    int nbRequests = srv->m_mainWindowController->getRequestController()->nbRequests();

    int requestIndex = 1;

    auto request = srv->m_mainWindowController->getRequestController()->getPartialRequest(requestIndex);

    std::cout << "Starting request " << request->requestId().toStdString() << " . Patient " << request->patient()->externalId().toStdString()
              << " . Drug " << request->drug()->getSubstanceId().toStdString() << std::endl;

    srv->m_mainWindowController->getRequestController()->queryRequestByIdSignal(requestIndex);
    srv->waitPeriod(waitTimeLong*3);

    //--------------------------------------------------------------------------------------------------------------------

    // Patient tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/patientButton");   // click on patient tab button to validate tab
    srv->synchronize();
    srv->waitPeriod(waitTimeLong);

    ASSERT_EQ(srv->getCurrentTabIndex(), patientTab);

    //--------------------------------------------------------------------------------------------------------------------

    // Drug tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/drugButton");
    srv->synchronize();
    srv->waitPeriod(waitTimeLong);

    ASSERT_EQ(srv->getCurrentTabIndex(), drugsTab);

    srv->mouseClickIfPathOk("mainWindow/flowView/drugButton");   // click on drug tab button again to validate tab
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    //--------------------------------------------------------------------------------------------------------------------

    // Dosage tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/dosageButton");
    srv->synchronize();
    srv->waitPeriod(waitTimeLong);

    ASSERT_EQ(srv->getCurrentTabIndex(), dosagesTab);

    int uncastedIndex = 0;      // Acknowledge all existing uncasted items

    while (srv->existsAndVisible(spix::ItemPath("mainWindow/flowView/UncastedItem_" + std::to_string(uncastedIndex))))
    {
        srv->mouseClickIfPathOk("mainWindow/flowView/UncastedItem_" + std::to_string(uncastedIndex));
        uncastedIndex++;
        srv->waitPeriod(waitTimeLong);
    }

    srv->mouseClickIfPathOk("mainWindow/flowView/dosageButton");
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    //--------------------------------------------------------------------------------------------------------------------

    // Covariate tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/covariateButton");
    srv->synchronize();
    srv->waitPeriod(waitTimeLong);

    ASSERT_EQ(srv->getCurrentTabIndex(), covariatesTab);

    srv->mouseClickIfPathOk("mainWindow/flowView/covariateButton");     // clicking once more to validate tab (no change made)
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    //--------------------------------------------------------------------------------------------------------------------

    // Measure tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/measureButton");
    srv->synchronize();
    srv->waitPeriod(waitTimeLong);

    ASSERT_EQ(srv->getCurrentTabIndex(), measuresTab);

    srv->mouseClickIfPathOk("mainWindow/flowView/measureButton");
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    //--------------------------------------------------------------------------------------------------------------------

    // Target tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/targetButton");
    srv->synchronize();
    srv->waitPeriod(waitTimeLong);

    ASSERT_EQ(srv->getCurrentTabIndex(), targetsTab);


    srv->mouseClickIfPathOk("mainWindow/flowView/targetButton");
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    //--------------------------------------------------------------------------------------------------------------------

    // Adjustment tab :

    AdjustmentsData adjustmentsData1;

    srv->mouseClickIfPathOk("mainWindow/flowView/adjustmentButton");
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    ASSERT_EQ(srv->getCurrentTabIndex(), adjustmentsTab);

    srv->addAdjustments(adjustmentsData1);
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    //--------------------------------------------------------------------------------------------------------------------

    // Validation tab :

    srv->waitForSync();

    srv->validateInterpretation();
    srv->synchronize();
    srv->waitPeriod(waitTimeLong);

    ASSERT_EQ(srv->getCurrentTabIndex(), validationTab);

    //--------------------------------------------------------------------------------------------------------------------

    //Print report :

    srv->mouseClickIfPathOk("mainWindow/flowView/reportButton");
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    srv->printReport("Report_1234_b");
    srv->synchronize();
    srv->waitPeriod(waitTimeLong);

    //--------------------------------------------------------------------------------------------------------------------

    #define PRINT_SUCCESS(x) (std::cout << "\033[1;32m" << x << "\033[0m" << std::endl)
    std::cout << "\033[1;36m" << "End of program ..." << "\033[0m" << std::endl;
    srv->waitPeriod(waitTimeLong*3);
}
