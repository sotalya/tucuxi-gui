//@@lisence@@

// File title       :   savedtestcomparison.cpp
// Test title       :   savedTestComparison
// Author           :   Julien Rosset
// Creation date    :   28.04.2022
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

#include <QDebug>
#include <QMessageBox>

#include "admin/src/interpretationxmlexport.h"
#include "admin/src/interpretationtorequestxmlexport.h"
#include "admin/src/interpretationxmlimport.h"


extern SpixGTest* srv;


TEST(SavedTestComparison, Test1)
{

    // initier test (créer patient, selectionner substance)
    // save fichier
    // load fichier

    int waitTime1       = 4;
    int waitTimeLong    = 10;

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);

    int drugIndex   = 13;           // drugIndex : 13 = Imatinib
    int modelIndex  = 0;            // modelIndex : 0 = only model available
    srv->selectDrugInList(drugIndex, modelIndex);
    srv->waitPeriod(waitTime1);


    DosageData dosageData1;
    dosageData1.dosage = 6666;
    srv->addDosage(dosageData1);

    srv->waitForSync();

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // SAVING interpretation in Xml file

    QString saveName =  "save_1P";       //"/Documents/Perso_stock_doc/Tests/save_1.tui";

    srv->saveIntepretation(saveName);

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // Get to Startup window
/*
    drugIndex = 20;                     // drugIndex : 20 = Vancomycin
    modelIndex = 3;
    srv->selectDrugInList(drugIndex, modelIndex);
    srv->waitPeriod(waitTime1);
*/
    srv->m_mainWindowController->goToHome();
    srv->waitPeriod(waitTimeLong);

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // LOADING interpretation from Xml file

    //    srv->m_mainWindowController->loadInterpretationFile(saveName);
    srv->waitPeriod(waitTimeLong);

    QString loadName = saveName; // "save_1.tui";

    srv->loadInterpretation(loadName);

//    srv->loadIntepretation(loadName);
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));

    srv->waitPeriod(waitTimeLong);

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    QFile fileToRead("save_1p.tui");
    if (!fileToRead.open(QFile::ReadOnly))
        std::cout << "Error : could not open file" << std::endl;

    QXmlStreamReader xmlData(&fileToRead);





    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitPeriod(waitTimeLong);
    std::cout << "End of program ..." << std::endl;
}
