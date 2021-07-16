
#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlApplicationEngine>

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
/*
QObject *getObjectByName(QObject *root, std::string name)
{
    for(auto child : root->children()) {
        if (child->objectName().toStdString() == name) {
            return child;
        }
    }

    for(auto child : root->children()) {
        auto result = getObjectByName(child, name);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}
*/
TEST(RandomTest, Test1)
{
    static std::vector<std::string> paths = {
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugList_Cefepime",
        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "dosageDialog/okDosage",
        "mainWindow/flowView/adjustmentButton",
        "mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate",
        "mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate",
        "mainWindow/flowView/adjustmentsView/atMinutesSettingInput",
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugList_Cefepime",
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugList_Cefepime",
        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "dosageDialog/okDosage",
        "mainWindow/flowView/adjustmentButton",
        "mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate",
        "mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate",
        "mainWindow/flowView/adjustmentsView/atMinutesSettingInput",
        "mainWindow/applicationBarView/newAction",
        "mainWindow/launchView/newPatient/mouseArea",
        "mainWindow/launchView/newPatient/mouseArea",
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugListItem_3",
        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "dosageDialog/okDosage",
        "mainWindow/flowView/adjustmentButton",
        "mainWindow/flowView/selectAdjustment_0",
        "mainWindow/flowView/validationButton",
        "mainWindow/flowView/validateInterpretation",
        //"mainWindow/flowView/reportButton",
        //"mainWindow/flowView/printButton",
        //"mainWindow/applicationBarView/shutDownAction",
        "mainWindow/launchView/newPatient/mouseArea",
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugListItem_3",
        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "dosageDialog/okDosage",
        "mainWindow/flowView/adjustmentButton",
        "mainWindow/flowView/selectAdjustment_0",
        "mainWindow/flowView/validationButton",
        "mainWindow/flowView/validateInterpretation",
        //    "mainWindow/flowView/reportButton",
        //    "mainWindow/flowView/printButton",
        //    "mainWindow/applicationBarView/shutDownAction",
        "mainWindow/launchView/newPatient/mouseArea",
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugListItem_3",
        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "dosageDialog/okDosage",
        "mainWindow/flowView/adjustmentButton",
        "mainWindow/flowView/selectAdjustment_0",
        "mainWindow/flowView/validationButton",
        "mainWindow/flowView/validateInterpretation",
        "mainWindow/flowView/reportButton",
        "mainWindow/flowView/printButton",
        "mainWindow/applicationBarView/shutDownAction",
        "mainWindow/launchView/newPatient/mouseArea",
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugList_Cefepime",
        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "dosageDialog/okDosage",
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugList_Cefepime",
        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "dosageDialog/okDosage",
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugList_Cefepime",
        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "dosageDialog/okDosage",
        "mainWindow/applicationBarView/newAction",
        "mainWindow/flowView/drugButton",
        "mainWindow/flowView/drugList_Cefepime",
        "mainWindow/flowView/targetButton",
        "mainWindow/flowView/removeTarget_0",
        "mainWindow/flowView/addTarget",
        "targetDialog/cancelTarget",
        "mainWindow/flowView/addTarget",
        "targetDialog/okTarget",
        "mainWindow/flowView/addTarget",
        "targetDialog/typeInput",
        "targetDialog/okTarget",
        "mainWindow/flowView/removeTarget_0",
        "mainWindow/flowView/addTarget",
        "targetDialog/typeInput",
        "targetDialog/okTarget"
    };

    auto rootContext = srv->m_mainWindowController->getRootContext();
    auto engine = srv->m_mainWindowController->engine;
    QObject *root = engine->rootObjects().at(0);
    std::cout << "Root : " << root->objectName().toStdString() << std::endl;

    bool ok;
    int currentIndex = srv->m_mainWindowController->getInterpretationController()->flowView->property("currentIndex").toInt(&ok);

    return;

    for (int clickIndex = 0; clickIndex < 100; clickIndex++) {

        std::string item;
        bool notFound = true;
        while (notFound) {
            int index = std::rand() % paths.size();
            item = paths[index];

            // existsAndVisible does not work well here
            if (srv->existsAndVisible(item)) {
                notFound = false;
                std::cout << "Item : " << item << std::endl;
                srv->mouseClick(item);
                srv->waitPeriod();
            }
        }
    }

    CHECKSPIXERRORS;
}

