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
    for (int i = 0; i < targetIndex; i++) {
        srv->enterKey("targetDialog/typeInput", spix::KeyCodes::Down, 0); // Down
    }
    srv->waitPeriod(1);
    srv->enterKey("targetDialog/typeInput", spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(1);
}

TEST(TargetTest, Test1)
{
    int waitTime1 = 1;
    int waitTimeLong = 10;

    auto addTarget      = spix::ItemPath("mainWindow/flowView/addTarget");
    auto okTarget       = spix::ItemPath("targetDialog/okTarget");
    auto cancelTarget   = spix::ItemPath("targetDialog/cancelTarget");
    auto removeTarget_0 = spix::ItemPath("mainWindow/flowView/removeTarget_0");

    srv->startNewPatient();

    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod(waitTime1);
    srv->selectDrugInList("Cefepime", 0);
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/targetButton"));
    srv->waitPeriod(waitTime1);

    // First remove the existing targets
    while (srv->existsAndVisible(removeTarget_0)) {
        srv->mouseClick(removeTarget_0);
        srv->waitPeriod(waitTime1);
    }

    srv->mouseClick(addTarget);
    srv->waitPeriod(waitTime1);
    srv->mouseClick(cancelTarget);
    srv->waitPeriod(waitTime1);

    srv->synchronize();
    auto mainWindow = srv->m_mainWindowController;
    auto interpretationController = mainWindow->getInterpretationController();
    auto targets = interpretationController->getInterpretation()
            ->getDrugResponseAnalysis()->getTreatment()->getTargets();

    EXPECT_EQ(targets->size(), 0);

    srv->mouseClick(addTarget);
    srv->waitPeriod(waitTime1);
    srv->mouseClick(okTarget);
    srv->waitPeriod(waitTime1);

    srv->synchronize();
    targets = srv->m_mainWindowController->getInterpretationController()->getInterpretation()
            ->getDrugResponseAnalysis()->getTreatment()->getTargets();

    EXPECT_EQ(targets->size(), 1);

    srv->mouseClick(addTarget);
    srv->waitPeriod(waitTime1);

    srv->mouseClick("targetDialog/typeInput");
    srv->waitPeriod(waitTime1);
    srv->synchronize();

    selectTargetType(2);

    srv->mouseClick(okTarget);
    srv->waitPeriod(waitTime1);
    srv->synchronize();

    targets = srv->m_mainWindowController->getInterpretationController()->getInterpretation()
            ->getDrugResponseAnalysis()->getTreatment()->getTargets();

    EXPECT_EQ(targets->size(), 2);

    auto target2 = targets->at(targets->size()-1);

    EXPECT_EQ(target2->getType()->getTargetType(), Tucuxi::Gui::Core::TargetMethod::MeanTarget);

    EXPECT_EQ(target2->getType()->getValue(), 2);

    // First remove the existing targets
    while (srv->existsAndVisible(removeTarget_0)) {
        srv->mouseClick(removeTarget_0);
        srv->waitPeriod(waitTime1);
    }

    for (int targetIndex = 0; targetIndex < 12; targetIndex ++) {
        srv->mouseClick(addTarget);
        srv->waitPeriod(waitTime1);

        srv->mouseClick("targetDialog/typeInput");
        srv->waitPeriod(waitTime1);
        srv->synchronize();

        selectTargetType(targetIndex);

        srv->mouseClick(okTarget);
        srv->waitPeriod(waitTime1);
        srv->synchronize();

        targets = srv->m_mainWindowController->getInterpretationController()->getInterpretation()
                ->getDrugResponseAnalysis()->getTreatment()->getTargets();

        EXPECT_EQ(targets->size(), targetIndex + 1);

        auto target = targets->at(targetIndex);
        EXPECT_EQ(target->getType()->getValue(), targetIndex);

        switch (targetIndex) {
        case 0: EXPECT_EQ(target->getCmax()->getUnitstring(), QString("mg/l")); break;
        case 1: case 2: EXPECT_EQ(target->getCmax()->getUnitstring(), QString("ug/l")); break;
        case 3: case 4: case 5: case 8: case 9: EXPECT_EQ(target->getCmax()->getUnitstring(), QString("ug*h/l")); break;
        case 6: case 7: case 11: EXPECT_EQ(target->getCmax()->getUnitstring(), QString("h")); break;
        case 10: EXPECT_EQ(target->getCmax()->getUnitstring(), QString("")); break;
        }
    }

    srv->waitPeriod(5);
    srv->synchronize();

    CHECKSPIXERRORS;
}


