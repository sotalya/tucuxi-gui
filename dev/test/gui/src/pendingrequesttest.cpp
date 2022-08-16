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
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"

#include <QDebug>                       // allows to print text in console : qInfo() << "text", or qDebug()"

extern SpixGTest* srv;


//________________________________________________________________________________________________________________________________________________


TEST(PendingRequestTest, Test1)
{

    int waitTime1 = 1;
    int waitTimeLong = 5;

    std::cout << "Test name : PendingRequestTest" << std::endl;

    srv->waitPeriod(waitTimeLong);

    bool isOnLaunchScreen = srv->existsAndVisible(spix::ItemPath("mainWindow/launchView"));

    if (!isOnLaunchScreen) {

        srv->mouseClickIfPathOk("mainWindow/applicationBarView/newAction");
        srv->waitPeriod(waitTime1);
    }
    srv->mouseClickIfPathOk("mainWindow/launchView/pendingRequests/mouseArea");
    srv->waitPeriod(waitTimeLong);

    srv->mouseClickIfPathOk("mainWindow/requestView");
    srv->waitPeriod(waitTimeLong);

    srv->mouseClickIfPathOk("mainWindow/flowView/patientButton");
    srv->waitPeriod(waitTimeLong);

    srv->selectDrugInList(0, 0);
    srv->waitPeriod(waitTimeLong);

    srv->mouseClickIfPathOk("mainWindow/flowView/dosageButton");
    srv->waitPeriod(waitTimeLong);







    srv->waitPeriod(500);

    std::cout << "End of test ..." << std::endl;

}
