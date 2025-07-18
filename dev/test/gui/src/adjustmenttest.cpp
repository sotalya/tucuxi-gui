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

/*
TEST(AdjustmentTest, TestFail)
{
    srv->startNewPatient();

    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/drugList_Cefepime"));
    srv->waitPeriod();

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(5);

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod();

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(5);

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod();

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(1);
    srv->synchronize();

    srv->startNewPatient();

    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/drugList_Cefepime"));
    srv->waitPeriod();



    CHECKSPIXERRORS;
}

*/


TEST(AdjustmentTest, Test1)
{
    srv->startNewPatient();

    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/drugList_Cefepime"));
    srv->waitPeriod();

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod();
    srv->mouseClickCheck(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(5);

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod();

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(5);

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod();

    srv->mouseClickCheck(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(10);
    srv->synchronize();

    CHECKSPIXERRORS;
}
