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

    int drugIndex   = 6;           // drugIndex : 13 = Imatinib
    int modelIndex  = 0;            // modelIndex : 0 = only model available
    srv->selectDrugInList(drugIndex, modelIndex);
    srv->waitPeriod(waitTime1);

    // ...

    srv->waitForSync();

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-


//    QString dirPath;
//    dirPath = SETTINGS.get(ezechiel::coe::Module::GUI,"interpretationDirPath", "").toString();

    QString fileName =  "save_1";//"/Documents/Perso_stock_doc/Tests/save_1.tui";

//    if (fileName.isEmpty())
//        return;

    if (!fileName.endsWith(".tui"))     // add file extension .tui if necessary
        fileName += ".tui";

    InterpretationXmlExport exporter;

    auto interpretation1 = srv->m_mainWindowController->getInterpretationController()->getInterpretation();
    QString xml = exporter.toXml(interpretation1);
    QFile interpretationFile(fileName);

    if (!interpretationFile.open(QFile::WriteOnly)) {
        exit(0);
    }

    QTextStream out(&interpretationFile);
    out.setCodec("UTF-8");
    out << xml;
    interpretationFile.close();

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-


    srv->waitPeriod(waitTimeLong);
    std::cout << "End of program ..." << std::endl;
}
