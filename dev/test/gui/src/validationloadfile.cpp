//@@lisence@@

// File title       :   validationLoadFile.cpp
// Test title       :   validationLoadFile
// Author           :   Mattia Crasi
// Creation date    :   10.05.2022
// Purpose          :
// ...              :   ...

#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/KeyCodes.h>

#include "core/dal/drug/drug.h"
#include "guitest.h"

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"

#include <QDebug>
#include <QMessageBox>

#include "admin/src/interpretationxmlexport.h"
#include "admin/src/interpretationtorequestxmlexport.h"
#include "admin/src/interpretationxmlimport.h"

#include "core/corefactory.h"
#include "core/core.h"

extern SpixGTest* srv;

TEST(ValidationLoadFile, Test1)
{
    int waitTime1       = 4;
    int waitTimeLong    = 120;

    // LOADING interpretation from Xml file

    QString loadName = "scenario2"; // "save_1.tui";

    srv->loadInterpretation(loadName);

    InterpretationXmlExport *interpretationXmlExport;
    Interpretation *interpretationLoad = srv->m_mainWindowController->getInterpretationController()->getInterpretation();

    QString status = interpretationXmlExport->toStringValidation(interpretationLoad->getValidationStatus()->getValidationStatus(StepType::Validation));

//    if (status == "validated")
//    {
//        srv->synchronize();
//        srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
//        srv->waitPeriod(waitTime1);
//        srv->synchronize();
//        srv->waitForSync();
//        srv->validateInterpretation();
//    }

//--------------------------------------------------
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitPeriod(waitTimeLong*100);
    std::cout << "End of program ..." << std::endl;
}
