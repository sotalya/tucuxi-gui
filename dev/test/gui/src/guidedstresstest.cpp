//@@license@@

// File title       :   guidedstresstest.cpp
// Test title       :   Guided stress test
// Author           :   Julien Rosset
// Creation date    :   22.02.2022
// Purpose          :
// ...              :   ...


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

#include <QDebug>

#define NUMBER_OF_ITERATIONS (50)

extern SpixGTest* srv;


TEST(GuidedStressTest, DrugsTab)
{

    int waitTime1 = 0;
    int waitTimeLong = 5;

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));

    //_____Drug tab_______________________________________________________________________________________________________________________

    int drugIndex   = 0;
    int modelIndex  = 0;

    //_____DrugModel selection : in order_____
    // Find total number of drugs present
    QVariant returnValue;
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>("drugListView"),
                              "getItemsCount",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, returnValue));
    auto totalItems = returnValue.toInt();

    for (drugIndex = 0; drugIndex <= (totalItems-1); drugIndex++)
    {

        std::cout << "Drug index : " << drugIndex << std::endl;
        srv->waitPeriod(waitTime1);

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView,
                                  "setExtCurrentActiveSubstance",
                                  Q_ARG(QVariant, QVariant::fromValue(drugIndex)));
        // model = DOMAIN & STUDY
        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView,
                                  "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(modelIndex)));
    }

    srv->selectDrugInList("Vancomycin", 0);    // Force Vancomycin

    // Get number of model available for the current drug
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>("domainListView"),
                              "getItemsCount",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, returnValue));
    auto drugTotalModels = returnValue.toInt();
    for (modelIndex = 0; modelIndex <= (drugTotalModels-1); modelIndex++)
    {

        std::cout << "Model index : " << modelIndex << std::endl;
        srv->waitPeriod(waitTime1);

        // model = DOMAIN & STUDY
        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView,
                                  "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(modelIndex)));
    }

    srv->waitPeriod(waitTimeLong);


    //_____DrugModel selection : random_____


    modelIndex = 0;

    for (int loopIndex = 1; loopIndex <= NUMBER_OF_ITERATIONS; loopIndex++)
    {
        srv->waitPeriod(waitTime1);

        drugIndex = rand() % (totalItems-1);                              // generates random index between 1 and 20
        std::cout << "DrugIndex : " << drugIndex << std::endl;

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView,
                                  "setExtCurrentActiveSubstance",
                                  Q_ARG(QVariant, QVariant::fromValue(drugIndex)));
        // model = DOMAIN & STUDY
        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView,
                                  "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(modelIndex)));
    }

    srv->selectDrugInList("Vancomycin", 0);    // Force Vancomycin
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>("domainListView"),
                              "getItemsCount",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, returnValue));
    drugTotalModels = returnValue.toInt();
    for (int loopIndex = 1; loopIndex <= (NUMBER_OF_ITERATIONS/2); loopIndex++)
    {
        srv->waitPeriod(waitTime1);

        modelIndex = rand() % (drugTotalModels-1);                                // generates random index between 0 and drugTotalModels-1
        std::cout << "ModelIndex : " << modelIndex << std::endl;

        // model = DOMAIN & STUDY
        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView,
                                  "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(modelIndex)));

    }

    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    std::cout << "End of test ..." << std::endl;
}


TEST(GuidedStressTest, DosagesTab)
{
    int waitTime1 = 1;
    int waitTimeLong = 5;

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);
    srv->selectDrugInList("Imatinib", 0);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));

    //_____Dosage tab____________________________________________________________________________________________________________________

    int dosageIndex = 0;
    int pathIndex = 0;
    std::string clickPath;

    static std::vector<std::string> paths = {

        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "mainWindow/flowView/editDosage_",                          // '_' at the end of string to add Dosage index later in test
        "mainWindow/flowView/removeDosage_",
        "dosageDialog/okDosage",
        "dosageDialog/applyDosage",
        "dosageDialog/cancelDosage",
    };

    for (int loopIndex = 1; loopIndex <= NUMBER_OF_ITERATIONS; loopIndex++)
    {
        srv->waitPeriod(waitTime1);

        pathIndex = rand() % paths.size();                          // two distinct lines for clarity and also in case pathIndex is needed later
        clickPath = paths[pathIndex];
        dosageIndex = rand() % 10;

        if (clickPath.find('_') != std::string::npos)               // check for '_' in path string and if so add random index to string
            clickPath = clickPath + std::to_string(dosageIndex);

        if (srv->existsAndVisible(clickPath))
        {
            std::cout << "Path OK : " << clickPath << std::endl;
            srv->mouseClick(spix::ItemPath(clickPath));

            if ((clickPath.find("add") != std::string::npos) || (clickPath.find("edit") != std::string::npos))
            {
                srv->waitPeriod(waitTime1);
                srv->findObjectAndSetValue("doseSpinBox", 200+loopIndex);
                srv->waitPeriod(waitTime1);
                srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));
            }
        }
        else std::cout << "Path not found : " << clickPath << std::endl;
    }


//    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    std::cout << "End of test ..." << std::endl;
}


TEST(GuidedStressTest, CovariatesTab)
{
    int waitTime1 = 1;
    int waitTimeLong = 5;

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);
    srv->selectDrugInList("Imatinib", 0);
    srv->waitPeriod(waitTime1);
    DosageData dosageData1;
    srv->addDosage(dosageData1);
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/covariateButton"));

    //_____Covariate tab________________________________________________________________________________________________________________

    int covariateIndex = 0;
    int pathIndex = 0;
    std::string clickPath;

    static std::vector<std::string> paths = {

        "mainWindow/flowView/covariateButton",
        "mainWindow/flowView/addCovariate",
        "mainWindow/flowView/editCovariate_",                              // '_' at the end of string to add Dosage index later in test
        "mainWindow/flowView/removeCovariate_",
        "covariateDialog/okCovariate",                                 // note : no "apply" Qml button in covariate dialog
        "covariateDialog/cancelCovariate",
    };

    for (int loopIndex = 1; loopIndex <= NUMBER_OF_ITERATIONS; loopIndex++)
    {
        srv->waitPeriod(waitTime1);

        pathIndex = rand() % paths.size();                          // two distinct lines for clarity and also in case pathIndex is needed later
        clickPath = paths[pathIndex];
        covariateIndex = rand() % 5;

        if (clickPath.find('_') != std::string::npos)               // check for '_' in path string and if so add random index to string
            clickPath = clickPath + std::to_string(covariateIndex);

        if (srv->existsAndVisible(clickPath))
        {
            std::cout << "Path OK : " << clickPath << std::endl;
            srv->mouseClick(spix::ItemPath(clickPath));

            if ((clickPath.find("add") != std::string::npos) || (clickPath.find("edit") != std::string::npos))
            {
                srv->waitPeriod(waitTime1);
                srv->findEntityTextValueFieldAndSetValue("covarValueEntry", 55+loopIndex);
                srv->waitPeriod(waitTime1);
                srv->mouseClick(spix::ItemPath("covariateDialog/okCovariate"));
            }
        }
        else std::cout << "Path not found : " << clickPath << std::endl;

    }

    //    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    std::cout << "End of test ..." << std::endl;

}


TEST(GuidedStressTest, MeasuresTab)
{
    int waitTime1 = 1;
    int waitTimeLong = 5;

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);
    srv->selectDrugInList("Imatinib", 0);
    srv->waitPeriod(waitTime1);
    DosageData dosageData1;
    srv->addDosage(dosageData1);
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/measureButton"));

    //_____Measure tab___________________________________________________________________________________________________________________

    int measureIndex = 0;
    int pathIndex = 0;
    std::string clickPath;

    static std::vector<std::string> paths = {

        "mainWindow/flowView/measureButton",
        "mainWindow/flowView/addMeasure",
        "mainWindow/flowView/editMeasure_",
        "mainWindow/flowView/removeMeasure_",
        "measureDialog/okMeasure",
        "measureDialog/cancelMeasure",
    };

    for (int loopIndex = 1; loopIndex <= NUMBER_OF_ITERATIONS; loopIndex++)
    {
        srv->waitPeriod(waitTime1);

        pathIndex = rand() % paths.size();

        clickPath = paths[pathIndex];
        measureIndex = rand() % 5 + 0;

        if (clickPath.find('_') != std::string::npos)               // check for '_' in path string and if so add random index to string
            clickPath = clickPath + std::to_string(measureIndex);

        if (srv->existsAndVisible(clickPath))
        {
            std::cout << "Path OK : " << clickPath << std::endl;
            srv->mouseClick(spix::ItemPath(clickPath));

            if ((clickPath.find("add") != std::string::npos) || (clickPath.find("edit") != std::string::npos))
            {
                srv->waitPeriod(waitTime1);
                srv->findEntityTextValueFieldAndSetValue("measureValueEntry", 400+loopIndex);
                srv->waitPeriod(waitTime1);
                srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));
            }
        }
        else std::cout << "Path not found : " << clickPath << std::endl;
    }

    //    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    std::cout << "End of test ..." << std::endl;

}


TEST(GuidedStressTest, TargetsTab)
{
    int waitTime1       = 1;
    int waitTimeLong    = 5;

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);
    srv->selectDrugInList("Imatinib", 0);
    srv->waitPeriod(waitTime1);
    DosageData dosageData1;
    srv->addDosage(dosageData1);
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/targetButton"));

    //_____Target Tab_______________________________________________________________________________________________________________________

    int targetIndex = 0;
    int pathIndex   = 0;
    std::string clickPath;

    static std::vector<std::string> paths = {

        "mainWindow/flowView/targetButton",
        "mainWindow/flowView/addTarget",
        "mainWindow/flowView/editTarget_",
        "mainWindow/flowView/removeTarget_",
        "targetDialog/okTarget",
        "targetDialog/cancelTarget",
    };

    for (int loopIndex = 1; loopIndex <= NUMBER_OF_ITERATIONS; loopIndex++)
    {
        srv->waitPeriod(waitTime1);

        pathIndex = rand() % paths.size();

        clickPath = paths[pathIndex];
        targetIndex = rand() % 5;

        if (clickPath.find('_') != std::string::npos)               // check for '_' in path string and if so add random index to string
            clickPath = clickPath + std::to_string(targetIndex);

        if (srv->existsAndVisible(clickPath))
        {
            std::cout << "Path OK : " << clickPath << std::endl;
            srv->mouseClick(spix::ItemPath(clickPath));

            if ((clickPath.find("add") != std::string::npos) || (clickPath.find("edit") != std::string::npos))
            {
                srv->waitPeriod(waitTime1);
                srv->findObjectAndSetValue("cMaxInput", 1500+loopIndex);
                srv->waitPeriod(waitTime1);
                srv->mouseClick(spix::ItemPath("targetDialog/okTarget"));
            }
        }
        else std::cout << "Path not found : " << clickPath << std::endl;
    }

    //    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    std::cout << "End of test ..." << std::endl;
}


TEST(GuidedStressTest, AdjustmentsTab)
{
    int waitTime1       = 1;
    int waitTimeLong    = 5;

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);
    srv->selectDrugInList("Cefepime", 0);
    srv->waitPeriod(waitTime1);
    DosageData dosageData1;
    srv->addDosage(dosageData1);
//    TargetData targetData1;
//    srv->addTarget(targetData1);
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitForSync();


    //_____Adjustment tab__________________________________________________________________________________________________________________

    int adjIndex    = 0;
    int pathIndex   = 0;
    std::string clickPath;

    static std::vector<std::string> paths = {

        "mainWindow/flowView/adjustmentButton",
        "mainWindow/flowView/addAdjustment",
        "mainWindow/flowView/selectAdjustment_",
        "mainWindow/flowView/editAdjustment_",
        "mainWindow/flowView/removeAdjustment_",
        "adjustmentDialog/okAdj",
        "adjustmentDialog/applyAdj",
        "adjustmentDialog/cancelAdj",
    };

    for (int loopIndex = 1; loopIndex <= NUMBER_OF_ITERATIONS; loopIndex++)
    {
        srv->waitPeriod(waitTime1);

        pathIndex = rand() % paths.size();

        clickPath = paths[pathIndex];
        adjIndex = rand() % 2;

        if (clickPath.find('_') != std::string::npos)
            clickPath = clickPath + std::to_string(adjIndex);

        if (srv->existsAndVisible(clickPath))
        {
            std::cout << "Path OK : " << clickPath << std::endl;
            srv->mouseClick(spix::ItemPath(clickPath));

            if (clickPath.find("edit") != std::string::npos)
            {
//                srv->waitPeriod(waitTime1);
//                srv->findObjectAndSetValue("");
            }
        }
        else std::cout << "Path not found : " << clickPath << std::endl;
    }
    //  srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    std::cout << "End of test ..." << std::endl;

}

