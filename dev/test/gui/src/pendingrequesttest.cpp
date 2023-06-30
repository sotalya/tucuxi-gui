//@@license@@

// File title       :   pendingrequesttest.cpp
// Test title       :   PendingRequestTest
// Author           :   Julien Rosset
// Creation date    :   15.08.2022
// Purpose          :
// ...              :   ...

#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/KeyCodes.h>

#include "guitest.h"

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/requestscontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"
#include "core/dal/drug/drug.h"
#include "admin/src/dal/partialrequest.h"

#include <QDebug>                       // allows to print text in console : qInfo() << "text", or qDebug()"

extern SpixGTest* srv;


//________________________________________________________________________________________________________________________________________________


TEST(PendingRequestTest, Test1)
{

    int waitTime1       = 2;
    int waitTimeLong    = 6;

    int currentRequestIndex = -1;
    bool cont = true;

    std::cout << "Test name : PendingRequestTest" << std::endl;

    srv->waitPeriod(waitTimeLong);

    while (cont) {
        bool isOnLaunchScreen = srv->existsAndVisible(spix::ItemPath("mainWindow/launchView"));

        if (!isOnLaunchScreen) {

            srv->mouseClickIfPathOk("mainWindow/applicationBarView/newAction");
            srv->waitPeriod(waitTime1);
        }
        srv->mouseClickIfPathOk("mainWindow/launchView/pendingRequests/mouseArea");
        srv->waitPeriod(waitTimeLong);

        srv->mouseClickIfPathOk("mainWindow/requestView");      // changed onDoubleClicked to onClicked in RequestList.qml (can't double click from Gui test apparently)
        srv->waitPeriod(waitTimeLong);                          // unfortunately clicks in the middle of the screen (can't yet select specific request)


        //    for (int i = 0; i < 4; i++){                     // Trying to select specific request by pressing arrows, but can't double click after this step...
        //    srv->enterKey(spix::ItemPath("mainWindow/requestView"), spix::KeyCodes::Down, 0);
        //    srv->waitPeriod(waitTime1);
        //    }

        srv->mouseClickIfPathOk("mainWindow/requestView");
        srv->waitPeriod(waitTimeLong);

        int nbRequests = srv->m_mainWindowController->getRequestController()->nbRequests();

        if (nbRequests == 0) {
            cont = false;
            break;
        }
        currentRequestIndex++;
        if (currentRequestIndex == nbRequests - 1) {
            cont = false;
        }

        auto request = srv->m_mainWindowController->getRequestController()->getPartialRequest(currentRequestIndex);

        std::cout << "Starting request " << request->requestId().toStdString() << " . Patient " << request->patient()->externalId().toStdString()
                  << " . Drug " << request->drug()->getSubstanceId().toStdString() << std::endl;

        srv->m_mainWindowController->getRequestController()->queryRequestByIdSignal(currentRequestIndex);


        //--------------------------------------------------------------------------------------------------------------------

        srv->mouseClickIfPathOk("mainWindow/flowView/patientButton");   // click on patient tab button to validate tab
        srv->waitPeriod(waitTimeLong);

        //--------------------------------------------------------------------------------------------------------------------

        srv->selectDrugInList(0, 0);        // select 1st drugModel
        srv->waitPeriod(waitTime1);

        srv->mouseClickIfPathOk("mainWindow/flowView/drugButton");  // click on drug tab button to validate tab
        srv->waitPeriod(waitTimeLong);

        //--------------------------------------------------------------------------------------------------------------------

        // Dosage tab :

        srv->mouseClickIfPathOk("mainWindow/flowView/dosageButton");
        srv->waitPeriod(waitTimeLong);

        int uncastedIndex = 0;      // Acknowledge all existing uncasted items

        while (srv->existsAndVisible(spix::ItemPath("mainWindow/flowView/UncastedItem_" + std::to_string(uncastedIndex))))
        {
            srv->mouseClickIfPathOk("mainWindow/flowView/UncastedItem_" + std::to_string(uncastedIndex));
            uncastedIndex++;
            srv->waitPeriod(waitTimeLong);
        }

        // After those Pending-request-specific steps, can't keep going like for a "regular" new patient test
        // because not supposed to modify dosage dates, steady state and some other data for example

        /*
    DosageData dosageData1;         // Default values from structure

    srv->mouseClickIfPathOk("mainWindow/flowView/editDosage_0");
    srv->waitPeriod(waitTime1);

    srv->findObjectAndSetValue("doseSpinBox", dosageData1.dosage);
    srv->waitPeriod(waitTimeLong);

    srv->mouseClickIfPathOk("dosageDialog/okDosage");
    srv->waitPeriod(waitTime1);
*/
        srv->mouseClickIfPathOk("mainWindow/flowView/dosageButton");
        srv->waitPeriod(waitTime1);

        //--------------------------------------------------------------------------------------------------------------------

        // Covariate tab :

        srv->mouseClickIfPathOk("mainWindow/flowView/covariateButton");
        srv->waitPeriod(waitTimeLong);
        srv->mouseClickIfPathOk("mainWindow/flowView/covariateButton");     // clicking once more to validate tab (no change made)
        srv->waitPeriod(waitTimeLong);

        //--------------------------------------------------------------------------------------------------------------------

        // Measure tab :

        srv->mouseClickIfPathOk("mainWindow/flowView/measureButton");
        srv->waitPeriod(waitTimeLong);
        srv->mouseClickIfPathOk("mainWindow/flowView/measureButton");
        srv->waitPeriod(waitTimeLong);


        //--------------------------------------------------------------------------------------------------------------------

        // Target tab :

        srv->mouseClickIfPathOk("mainWindow/flowView/targetButton");
        srv->waitPeriod(waitTimeLong);
        srv->mouseClickIfPathOk("mainWindow/flowView/targetButton");
        srv->waitPeriod(waitTimeLong);

        //--------------------------------------------------------------------------------------------------------------------

        // Adjustment tab :

        AdjustmentsData adjustmentsData1;

        srv->mouseClickIfPathOk("mainWindow/flowView/adjustmentButton");
        srv->waitPeriod(waitTime1);

        srv->addAdjustments(adjustmentsData1);
        srv->waitPeriod(waitTimeLong);

        //--------------------------------------------------------------------------------------------------------------------

        // Validation tab :

        srv->validateInterpretation();
        srv->waitPeriod(waitTimeLong);

        //--------------------------------------------------------------------------------------------------------------------

        //Print report :

        srv->mouseClickIfPathOk("mainWindow/flowView/reportButton");
        //    srv->printReport("Report_1234_b");

        //--------------------------------------------------------------------------------------------------------------------
    }

    srv->waitPeriod(20);

    std::cout << "End of test ..." << std::endl;

}

