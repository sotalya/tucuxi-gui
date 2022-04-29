//@@license@@

#include <QQuickWindow>

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

/*
TEST(AdjustmentTest, TestFail)
{
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

    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
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
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Cefepime"));
    srv->waitPeriod();



    CHECKSPIXERRORS;
}

*/


TEST(AdjustmentTest, Test1)
{
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

    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
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
