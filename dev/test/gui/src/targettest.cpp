//@@license@@

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

void selectTargetType(int targetIndex)
{
    for (int i = 0; i < 10; i++) {
        srv->enterKey("targetDialog/typeInput", spix::KeyCodes::Up, 0); // Up
    }
    for (int i = 0; i < targetIndex; i++) {
        srv->enterKey("targetDialog/typeInput", spix::KeyCodes::Down, 0); // Down
    }
    srv->enterKey("targetDialog/typeInput", spix::KeyCodes::Enter, 0); // Enter
}

TEST(TargetTest, Test1)
{

    srv->startNewPatient();

    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Cefepime"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/targetButton"));
    srv->waitPeriod();

    // First remove the existing targets
    while (srv->existsAndVisible(spix::ItemPath("mainWindow/flowView/removeTarget_0"))) {
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/removeTarget_0"));
        srv->waitPeriod();
    }

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addTarget"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("targetDialog/cancelTarget"));
    srv->waitPeriod();

    srv->synchronize();
    auto mainWindow = srv->m_mainWindowController;
    auto interpretationController = mainWindow->getInterpretationController();
    auto targets = interpretationController->getInterpretation()
            ->getDrugResponseAnalysis()->getTreatment()->getTargets();

    EXPECT_EQ(targets->size(), 0);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addTarget"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("targetDialog/okTarget"));
    srv->waitPeriod();

    srv->synchronize();
    targets = srv->m_mainWindowController->getInterpretationController()->getInterpretation()
            ->getDrugResponseAnalysis()->getTreatment()->getTargets();

    EXPECT_EQ(targets->size(), 1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addTarget"));
    srv->waitPeriod();

    srv->mouseClick("targetDialog/typeInput");
    srv->waitPeriod();
    srv->synchronize();

    selectTargetType(2);

    srv->mouseClick(spix::ItemPath("targetDialog/okTarget"));

    srv->synchronize();
    srv->existsAndVisible(spix::ItemPath("fake"));

    targets = srv->m_mainWindowController->getInterpretationController()->getInterpretation()
            ->getDrugResponseAnalysis()->getTreatment()->getTargets();

    EXPECT_EQ(targets->size(), 2);

    auto target2 = targets->at(1);
    EXPECT_EQ(target2->getType()->getTargetType(), Tucuxi::Gui::Core::TargetMethod::MeanTarget);

    EXPECT_EQ(target2->getType()->getValue(), 2);

    // First remove the existing targets
    while (srv->existsAndVisible(spix::ItemPath("mainWindow/flowView/removeTarget_0"))) {
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/removeTarget_0"));
        srv->waitPeriod();
    }

    for (int targetIndex = 0; targetIndex < 12; targetIndex ++) {
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/addTarget"));
        srv->waitPeriod();

        srv->mouseClick("targetDialog/typeInput");
        srv->waitPeriod();
        srv->synchronize();

        selectTargetType(targetIndex);

        srv->mouseClick(spix::ItemPath("targetDialog/okTarget"));

        srv->synchronize();
        srv->existsAndVisible(spix::ItemPath("fake"));

        targets = srv->m_mainWindowController->getInterpretationController()->getInterpretation()
                ->getDrugResponseAnalysis()->getTreatment()->getTargets();

        EXPECT_EQ(targets->size(), targetIndex + 1);

        auto target = targets->at(targetIndex);
        EXPECT_EQ(target->getType()->getValue(), targetIndex);

        switch (targetIndex) {
        case 0: EXPECT_EQ(target->getCmax()->getUnitstring(), "mg/l"); break;
        case 1: case 2: EXPECT_EQ(target->getCmax()->getUnitstring(), "ug/l"); break;
        case 3: case 4: case 5: case 8: case 9: EXPECT_EQ(target->getCmax()->getUnitstring(), "ug*h/l"); break;
        case 6: case 7: case 11: EXPECT_EQ(target->getCmax()->getUnitstring(), "h"); break;
        case 10: EXPECT_EQ(target->getCmax()->getUnitstring(), ""); break;
        }
    }

    srv->waitPeriod(5);
    srv->synchronize();

    CHECKSPIXERRORS;
}


