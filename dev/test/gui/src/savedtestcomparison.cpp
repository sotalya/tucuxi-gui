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

//Include MCI
#include "admin/src/dal/interpretationanalysis.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/corepatient.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/person.h"
#include "admin/src/dal/interpretationrequest.h"

#include <QDebug>
#include <QMessageBox>

#include "admin/src/interpretationxmlexport.h"
#include "admin/src/interpretationtorequestxmlexport.h"
#include "admin/src/interpretationxmlimport.h"

#include "core/corefactory.h"
#include "core/core.h"

extern SpixGTest* srv;


TEST(SavedTestComparison, Test1)
{
    //Creating two maps
    QMap<QString, QString> mapSave;
    QMap<QString, QString> mapLoad;

    // Declaration of structures
    PatientData patientData;
    DosageData dosageData;
    CovariatesData covariatesData;
    MeasureData measureData;
    TargetData targetData;
    AdjustmentsData adjustmentsData;
    ValidationData validationData;

    // initier test (créer patient, selectionner substance)
    // save fichier
    // load fichier

    int waitTime1       = 4;
    int waitTimeLong    = 10;

    srv->startNewPatient();
    srv->fillInPatientData(patientData);
    srv->waitPeriod(waitTime1);

    int drugIndex   = 4;           // drugIndex : 13 = Imatinib
    int modelIndex  = 0;            // modelIndex : 0 = only model available
    srv->selectDrugInList(drugIndex, modelIndex);

//    srv->addDosage(dosageData);

    srv->waitPeriod(waitTime1);

    srv->waitForSync();

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // SAVING interpretation in Xml file

    QString saveName =  "save_1c";       //"/Documents/Perso_stock_doc/Tests/save_1.tui";

    Interpretation *interpretationSave = srv->m_mainWindowController->getInterpretationController()->getInterpretation();

    mapSave = srv->fillMapWithInterpreation(interpretationSave);
    QMapIterator<QString, QString> iSave(mapSave);
    while (iSave.hasNext()) {
        iSave.next();
        qInfo() << "key =" << iSave.key() << "value =" << iSave.value();
    }

    srv->saveIntepretation(saveName);

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // Get to Startup window

    drugIndex = 20;                     // drugIndex : 20 = Vancomycin
    modelIndex = 3;
    srv->selectDrugInList(drugIndex, modelIndex);
    srv->waitPeriod(waitTime1);

//    srv->addDosage(dosageData);

    srv->m_mainWindowController->goToHome();
    srv->waitPeriod(waitTimeLong);

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // LOADING interpretation from Xml file
    QString loadName = saveName; // "save_1.tui";

    srv->loadInterpretation(loadName);

    Interpretation *interpretationLoad = srv->m_mainWindowController->getInterpretationController()->getInterpretation();

    mapLoad = srv->fillMapWithInterpreation(interpretationLoad);
    QMapIterator<QString, QString> iLoad(mapLoad);
    while (iLoad.hasNext()) {
        iLoad.next();
        qInfo() << "key =" << iLoad.key() << "value =" << iLoad.value();
    }

    if(mapSave == mapLoad)
        qInfo("Les interprétations sont égales");
    else if(mapSave != mapLoad)
        qInfo("Les interprétations NE sont PAS égales");

    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitPeriod(waitTimeLong*10);
    std::cout << "End of program ..." << std::endl;
}
