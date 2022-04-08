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
#include <Spix/Events/KeyCodes.h>

#include "guitest.h"

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/drug/target.h"

#include <QDebug>

extern SpixGTest* srv;


TEST(GuidedStressTest, DrugTab)
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


    for (drugIndex = 0; drugIndex <= 20; drugIndex++)
    {
//        srv->selectDrugInList(n, 0);

        std::cout << "Drug index : " << drugIndex << std::endl;
        srv->waitPeriod(waitTime1);

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentActiveSubstance",
                                  Q_ARG(QVariant, QVariant::fromValue(drugIndex)));
        // model = DOMAIN & STUDY
        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(modelIndex)));
    }

    drugIndex = 20;

    for (modelIndex = 0; modelIndex <= 7; modelIndex++)
    {
//        srv->selectDrugInList(20, n);

        std::cout << "Model index : " << drugIndex << std::endl;
        srv->waitPeriod(waitTime1);

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentActiveSubstance",
                                  Q_ARG(QVariant, QVariant::fromValue(drugIndex)));
        // model = DOMAIN & STUDY
        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(modelIndex)));
    }

    srv->waitPeriod(waitTimeLong);


    //_____DrugModel selection : random_____


    modelIndex = 0;

    for (int loopIndex = 1; loopIndex <= 50; loopIndex++)
    {
        srv->waitPeriod(waitTime1);

        drugIndex = rand() % 20;                                // generates random index between 1 and 20
        std::cout << "DrugIndex : " << drugIndex << std::endl;

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentActiveSubstance",
                                  Q_ARG(QVariant, QVariant::fromValue(drugIndex)));
        // model = DOMAIN & STUDY
        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(modelIndex)));
    }

    drugIndex = 20;                                                 // drugIndex : 20 = Vancomycin

    for (int loopIndex = 1; loopIndex <= 30; loopIndex++)
    {
        srv->waitPeriod(waitTime1);

        modelIndex = rand() % 7 + 0;                                // generates random index between 1 and 7
        std::cout << "ModelIndex : " << modelIndex << std::endl;

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentActiveSubstance",
                                  Q_ARG(QVariant, QVariant::fromValue(drugIndex)));
        // model = DOMAIN & STUDY
        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(modelIndex)));

    }

    srv->waitForSync();
    srv->waitPeriod(10);
    std::cout << "End of test ..." << std::endl;
}


TEST(GuidedStressTest, DosageTab)
{
    int waitTime1 = 1;
    int waitTimeLong = 5;

    srv->startNewPatient();
    srv->waitPeriod(waitTime1);
    srv->selectDrugInList(13, 0);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));

    //_____Dosage tab____________________________________________________________________________________________________________________

    int dosageIndex = 0;
    int pathIndex = 0;
    std::string clickPath;

    static std::vector<std::string> paths = {

        "mainWindow/flowView/dosageButton",
        "mainWindow/flowView/addDosage",
        "mainWindow/flowView/editDosage_",                                 // '_' at the end of string to add Dosage index later in test
        "mainWindow/flowView/removeDosage_",
        "dosageDialog/okDosage",
        "dosageDialog/applyDosage",
        "dosageDialog/cancelDosage",
    };

    for (int loopIndex = 1; loopIndex <= 50; loopIndex++)
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

            if ((clickPath.find("add") != std::string::npos) or (clickPath.find("edit") != std::string::npos))
            {
                srv->waitPeriod(waitTime1);
                srv->findObjectAndSetValue("doseSpinBox", 4000+loopIndex);
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


TEST(GuidedStressTest, CovariateTab)
{
    int waitTime1 = 1;
    int waitTimeLong = 5;

    srv->startNewPatient();
    srv->waitPeriod();
    srv->selectDrugInList(13, 0);
    srv->waitPeriod();
    DosageData dosageData1;
    srv->addDosage(dosageData1);
    srv->waitPeriod();
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

    for (int loopIndex = 1; loopIndex <= 50; loopIndex++)
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

            if ((clickPath.find("add") != std::string::npos) or (clickPath.find("edit") != std::string::npos))
            {
                srv->waitPeriod(waitTime1);
                srv->findObjectAndSetValue("valueDoubleControl", 4000+loopIndex);
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


TEST(GuidedStressTest,  MeasureTab)
{
    int waitTime1 = 1;
    int waitTimeLong = 5;

    srv->startNewPatient();
    srv->waitPeriod();
    srv->selectDrugInList(13, 0);
    srv->waitPeriod();
    DosageData dosageData1;
    srv->addDosage(dosageData1);
    srv->waitPeriod();
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
        "measureDialog/applyMeasure",
        "measureDialog/cancelMeasure",
    };

    for (int loopIndex = 1; loopIndex <= 250; loopIndex++)
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

            if ((clickPath.find("add") != std::string::npos) or (clickPath.find("edit") != std::string::npos))
            {
                srv->waitPeriod();
                srv->findObjectAndSetValue("measureValueInput", 4000+loopIndex);
                srv->waitPeriod();
                srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));
            }
        }
        else std::cout << "Path not found : " << clickPath << std::endl;
    }

    //    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    std::cout << "End of test ..." << std::endl;

}


TEST(GuidedStressTest, TargetTab)
{
    int waitTime1       = 1;
    int waitTimeLong    = 5;

    srv->startNewPatient();
    srv->waitPeriod();
    srv->selectDrugInList(13, 0);
    srv->waitPeriod();
    DosageData dosageData1;
    srv->addDosage(dosageData1);
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/targetButton"));

    //_____Target Tab_______________________________________________________________________________________________________________________

    int targetIndex = 0;
    int pathIndex   = 0;
    std::string clickPath;

    static std::vector<std::string> paths = {

        "mainWindow/flowView/targetButton",
        "mainWindow/flowView/addTarget",
        "mainWindow/flowView/editTarget",
        "mainWindow/flowView/removeTarget_",
        "measureDialog/okTarget",
        "measureDialog/applyTarget",
        "measureDialog/cancelTarget",
    };

    for (int loopIndex = 1; loopIndex <= 50; loopIndex++)
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

            if ((clickPath.find("add") != std::string::npos) or (clickPath.find("edit") != std::string::npos))
            {
                srv->waitPeriod(waitTime1);
                srv->findObjectAndSetValue("cMaxInput", 4000+loopIndex);
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
    srv->waitPeriod();
    srv->selectDrugInList(6, 0);
    srv->waitPeriod();
    DosageData dosageData1;
    srv->addDosage(dosageData1);
    srv->waitPeriod();
//    TargetData targetData1;
//    srv->addTarget(targetData1);
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));

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
        "adjustmentDialog/okAdjustment",
        "adjustmentDialog/applyAdjustment",
        "adjustmentDialog/cancelAdjustment",
    };

    for (int loopIndex = 1; loopIndex <= 50; loopIndex++)
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

