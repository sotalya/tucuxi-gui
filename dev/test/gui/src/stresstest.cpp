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


extern SpixGTest* srv;

TEST(StressTest, Test1)
{
    for (int testIndex = 0; testIndex < 10; testIndex ++) {

        srv->startNewPatient();

        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Cefepime"));
        srv->waitPeriod();

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

        srv->waitPeriod(2);
    }
    srv->synchronize();

    CHECKSPIXERRORS;
}


TEST(StressTest, TestPercentiles)
{
    for (int testIndex = 0; testIndex < 10; testIndex ++) {

        srv->startNewPatient();

        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Cefepime"));
        srv->waitPeriod();

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

        srv->waitPeriod(20);

        srv->mouseClick(spix::ItemPath("mainWindow/applicationBarView/newAction"));
        srv->waitPeriod(2);
    }
    srv->synchronize();

    CHECKSPIXERRORS;
}
