//@@license@@

#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/Identifiers.h>

#include "admin/src/dal/partialrequest.h"
#include "core/dal/drug/drug.h"
#include "guitest.h"
#include "guiutils/src/appglobals.h"
#include "guiutils/src/requestscontroller.h"
#include "flatrequests/flatrequestfileclient.h"

#include <QDebug>

extern SpixGTest* srv;

using namespace Tucuxi::Gui::Admin;

TEST(FlatRequestsTest, Test1)
{
    int waitTime1       = 2;
    int waitTimeLong    = 6;

    int requestIndex = 1;

    Tucuxi::Gui::GuiUtils::AppGlobals *appGlobals = Tucuxi::Gui::GuiUtils::AppGlobals::getInstance();
    QString iccaFile = "test1_cefepime.xml"; //"test2_cefepime.xml";

    std::cout << "Test name : FlatRequestsTest.Test1" << std::endl;

    srv->waitPeriod(waitTimeLong);

    Tucuxi::Gui::FlatRequest::FlatRequestFileClient *client = new Tucuxi::Gui::FlatRequest::FlatRequestFileClient(srv->m_mainWindowController);
    client->setListFile(iccaFile);
    srv->m_mainWindowController->getRequestController()->setClient(client);

    srv->mouseClickIfPathOk("mainWindow/launchView/pendingRequests/mouseArea");
    srv->waitPeriod(waitTimeLong*10);

    int nbRequests = srv->m_mainWindowController->getRequestController()->nbRequests();

    auto request = srv->m_mainWindowController->getRequestController()->getPartialRequest(requestIndex);

    std::cout << "Starting request " << request->requestId().toStdString() << " . Patient " << request->patient()->externalId().toStdString()
              << " . Drug " << request->drug()->getSubstanceId().toStdString() << std::endl;

    srv->m_mainWindowController->getRequestController()->queryRequestByIdSignal(requestIndex);
    srv->waitPeriod(waitTimeLong*3);

    //--------------------------------------------------------------------------------------------------------------------

    // Patient tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/patientButton");   // click on patient tab button to validate tab
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    ASSERT_EQ(srv->getCurrentTabIndex(), patientTab);

    //--------------------------------------------------------------------------------------------------------------------

    // Drug tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/drugButton");  // click on drug tab button to validate tab
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    ASSERT_EQ(srv->getCurrentTabIndex(), drugsTab);

    //--------------------------------------------------------------------------------------------------------------------

    // Dosage tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/dosageButton");
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    ASSERT_EQ(srv->getCurrentTabIndex(), dosagesTab);

    int uncastedIndex = 0;      // Acknowledge all existing uncasted items

    while (srv->existsAndVisible(spix::ItemPath("mainWindow/flowView/UncastedItem_" + std::to_string(uncastedIndex))))
    {
        srv->mouseClickIfPathOk("mainWindow/flowView/UncastedItem_" + std::to_string(uncastedIndex));
        uncastedIndex++;
        srv->waitPeriod(waitTimeLong);
    }

    srv->mouseClickIfPathOk("mainWindow/flowView/dosageButton");
    srv->waitPeriod(waitTime1);
    srv->synchronize();

    //--------------------------------------------------------------------------------------------------------------------

    // Covariate tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/covariateButton");
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    ASSERT_EQ(srv->getCurrentTabIndex(), covariatesTab);

    srv->mouseClickIfPathOk("mainWindow/flowView/covariateButton");     // clicking once more to validate tab (no change made)
    srv->waitPeriod(waitTime1);
    srv->synchronize();

    //--------------------------------------------------------------------------------------------------------------------

    // Measure tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/measureButton");
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    ASSERT_EQ(srv->getCurrentTabIndex(), measuresTab);

    srv->mouseClickIfPathOk("mainWindow/flowView/measureButton");
    srv->waitPeriod(waitTime1);
    srv->synchronize();


    //--------------------------------------------------------------------------------------------------------------------

    // Target tab :

    srv->mouseClickIfPathOk("mainWindow/flowView/targetButton");
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    ASSERT_EQ(srv->getCurrentTabIndex(), targetsTab);


    srv->mouseClickIfPathOk("mainWindow/flowView/targetButton");
    srv->waitPeriod(waitTime1);
    srv->synchronize();

    //--------------------------------------------------------------------------------------------------------------------

    // Adjustment tab :

    AdjustmentsData adjustmentsData1;

    srv->mouseClickIfPathOk("mainWindow/flowView/adjustmentButton");
    srv->waitPeriod(waitTime1);
    srv->synchronize();

    ASSERT_EQ(srv->getCurrentTabIndex(), adjustementsTab);

    srv->addAdjustments(adjustmentsData1);
    srv->waitPeriod(waitTime1);
    srv->synchronize();

    //--------------------------------------------------------------------------------------------------------------------

    // Validation tab :

    srv->waitForSync();

    srv->validateInterpretation();
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    ASSERT_EQ(srv->getCurrentTabIndex(), validationTab);

    //--------------------------------------------------------------------------------------------------------------------

    //Print report :

    srv->mouseClickIfPathOk("mainWindow/flowView/reportButton");
    //    srv->printReport("Report_1234_b");

    //--------------------------------------------------------------------------------------------------------------------

}
