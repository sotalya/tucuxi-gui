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
#include <Spix/Events/Identifiers.h>

#include "guitest.h"

#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"

#include <QDebug>
#include <QMessageBox>

#include "admin/src/interpretationxmlexport.h"
#include "admin/src/interpretationtorequestxmlexport.h"
#include "admin/src/interpretationxmlimport.h"

#include "core/corefactory.h"
#include "core/core.h"

using namespace Tucuxi::Gui::Admin;

extern SpixGTest* srv;

TEST(SavedTestComparison, Test1)
{
    // Creating two maps
    QMap<QString, QString> mapSave;
    QMap<QString, QString> mapLoad;

    // Declaration of structures
    PatientData patientData;
    AnalystData analystData;
    DosageData dosageData;
    CovariatesData covariatesData;
    MeasureData measureData;
    TargetData targetData;
    AdjustmentsData adjustmentsData;
    ValidationData validationData;

    // initier test (créer patient, selectionner substance)
    // save fichier
    // load fichier
    srand(time(0));
    int nbScenario = 8;
    int scenario = (rand() % nbScenario) + 1;
    qInfo() << "Scenario :" << scenario;

    int waitTime1       = 4;
    int waitTimeLong    = 10;

    srv->startNewPatient();

    if (scenario == 1)
    {
        srv->fillInPatientData(patientData);
        srv->fillInAnalystData(analystData);
        srv->waitPeriod(waitTime1);

        int drugIndex   = 4;            // drugIndex  : 4 = Busulfan
        int modelIndex  = 0;            // modelIndex : 0 = only model available
        srv->selectDrugInList(drugIndex, modelIndex);

        srv->addDosage(dosageData);

        int covariateType = 1; // Weight
        srv->addCovariatesByDrug(covariatesData, covariateType, drugIndex);

        srv->addMeasure(measureData);

        srv->addTarget(targetData);

        srv->addAdjustments(adjustmentsData);

        srv->fillInValidationData(validationData);
    }

    else if (scenario == 2)
    {
        srv->fillInPatientData(patientData);
        srv->fillInAnalystData(analystData);
        srv->waitPeriod(waitTime1);

        int drugIndex   = 6;            // drugIndex  : 6  = Cefepime
        int modelIndex  = 0;            // modelIndex : 0  = only model available
        srv->selectDrugInList(drugIndex, modelIndex);

        dosageData.dosage = 225000;
        srv->addDosage(dosageData);

        covariatesData.scc = 7660;
        for(int covariateType = 0; covariateType <= 3; covariateType++)
        {
            if (!(covariateType == 1 || covariateType == 2))
                srv->addCovariatesByDrug(covariatesData, covariateType, drugIndex);
        }

        measureData.value = 290;
        srv->addMeasure(measureData);

        targetData.cMinInput  = 30000;
        targetData.cBestInput = 45000;
        targetData.cMaxInput  = 60000;
        srv->removeFromList("target", 0);
        srv->waitPeriod(waitTimeLong);

        srv->addTarget(targetData);

        srv->addAdjustments(adjustmentsData);

        srv->fillInValidationData(validationData);

        srv->waitForSync();

        srv->validateInterpretation();

        srv->printReport("Report_1234_b");
    }

    else if (scenario == 3)
    {
        int drugIndex   = 20;               // drugIndex  : 20 = Vancomycin
        int modelIndex  = (rand() % 8);     // modelIndex : model 0:7 --> random num between 0 to 7
        srv->selectDrugInList(drugIndex, modelIndex);

        srv->addDosage(dosageData);

        srv->addAdjustments(adjustmentsData);
    }

    else if (scenario == 4)
    {
        patientData.firstName   = "";
        patientData.lastName    = "";
        patientData.identifier  = "";
        patientData.stayNumber  = "";
        patientData.address     = "";
        patientData.city        = "";
        patientData.state       = "";
        patientData.postcode    = "";
        patientData.country     = "";

        patientData.titlePhy        = "";
        patientData.firstNamePhy    = "";
        patientData.lastNamePhy     = "";
        patientData.affiliationPhy  = "";
        patientData.addressPhy      = "";
        patientData.cityPhy         = "";
        patientData.statePhy        = "";
        patientData.postcodePhy     = "";
        patientData.countryPhy      = "";

        analystData.analystTitle       = "";
        analystData.analystFirstName   = "";
        analystData.analystLastName    = "";
        analystData.analystRole        = "";
        analystData.analystPhoneNumber = "";
        analystData.analystAffiliation = "";
        analystData.analystAddress     = "";
        analystData.analystCity        = "";
        analystData.analystPostcode    = "";
        analystData.analystState       = "";
        analystData.analystCountry     = "";

        srv->fillInPatientData(patientData);
        srv->fillInAnalystData(analystData);
        srv->waitPeriod(waitTime1);


        dosageData.dosage       = 1;
        dosageData.interval     = 100;
        dosageData.steadyState  = 1;
        dosageData.dateTimeDos1 = QDateTime::currentDateTime();
        dosageData.dateTimeDos2 = QDateTime::currentDateTime();

        adjustmentsData.dateTimeAdj     = QDateTime::currentDateTime();
        adjustmentsData.loadingDose     = 0;
        adjustmentsData.restPeriod      = 0;
        adjustmentsData.suggestAdjNum   = 0;
        adjustmentsData.dose            = 0;
        adjustmentsData.interval        = 0;

        int drugIndex   = 0;
        int modelIndex  = 0;
        srv->selectDrugInList(drugIndex, modelIndex);

        srv->addDosage(dosageData);

        srv->addAdjustments(adjustmentsData);
    }

    else if (scenario == 5)
    {
        int drugIndex   = 2;     // drugIndex  : 2 = Apixaban
        int modelIndex  = 0;     // modelIndex : 0
        srv->selectDrugInList(drugIndex, modelIndex);

        srv->addDosage(dosageData);

        for(int covariateType = 2; covariateType <= 9; covariateType++)
        {
            srv->addCovariatesByDrug(covariatesData, covariateType, drugIndex);
        }

        srv->addAdjustments(adjustmentsData);
    }

    else if (scenario == 6)
    {
        int drugIndex   = 9;     // drugIndex  : 9 = Dolutegravir
        int modelIndex  = 0;     // modelIndex : 0
        srv->selectDrugInList(drugIndex, modelIndex);

        srv->addDosage(dosageData);

        for(int covariateType = 2; covariateType <= 5; covariateType++)
        {
            srv->addCovariatesByDrug(covariatesData, covariateType, drugIndex);
        }

        srv->addAdjustments(adjustmentsData);
    }

    else if (scenario == 7)
    {
        int drugIndex   = 20;     // drugIndex  : 20 = Vancomycin
        int modelIndex  = 0;      // modelIndex : 0
        srv->selectDrugInList(drugIndex, modelIndex);

        srv->addDosage(dosageData);

        covariatesData.scc = 7660;
        for(int covariateType = 0; covariateType <= 6; covariateType++)
        {
            if (!(covariateType == 1 || covariateType == 2 || covariateType == 5))
                srv->addCovariatesByDrug(covariatesData, covariateType, drugIndex);
        }

        srv->addAdjustments(adjustmentsData);
    }

    else if (scenario == 8)
    {
        int drugIndex   = 2;      // drugIndex  : 2 = Apixaban
        int modelIndex  = 0;      // modelIndex : 0
        srv->selectDrugInList(drugIndex, modelIndex);

        for (int n = 1; n <= 3; n++)        // index of loop is used to modify variables to avoid having to do it manually for each iteration
        {

            //_____Add dosage_______________________________________________
            dosageData.dosage      = n*1000 + n*200 + n*30 + n*1;          // dosage(n) = {1234, 2345, 3456, 4567, ...}
            dosageData.infusion    = n*1000;                               // infusion(n) = {100, 200, 300, 400, ...}
            dosageData.interval    = n*1200;                               // interval(n) = {1200, 2400, 3600, 4800, ...}
            dosageData.steadyState = false;                                // if steadyState = false -> start date & end date needed
            dosageData.dateTimeDos1.setDate(QDate(2022, 05, 18+n));            // start date(n)
            dosageData.dateTimeDos1.setTime(QTime(n, n));                  // start time(n)
            dosageData.dateTimeDos2.setDate(QDate(2022, 05, 19+n));        // end date(n)
            dosageData.dateTimeDos2.setTime(QTime(n+1, n+1));              // end time(n)

            srv->waitForSync();
            srv->addDosage(dosageData);
            srv->waitPeriod(waitTime1);
        }

        covariatesData.dateTimeCovar.setDate(QDate(2022, 05, 24));

        for(int covariateType = 2; covariateType <= 9; covariateType++)
        {
            srv->addCovariatesByDrug(covariatesData, covariateType, drugIndex);
        }

        for (int n = 1; n <= 3; n++)        // index of loop is used to modify variables to avoid having to do it manually for each iteration
        {
            //_____Add measure______________________________________________

            measureData.name   = "Sample_" + QString::number(n);
            measureData.value  = 500 + n*100 + n*100 + n*10 + n*1;
            measureData.dateTimeMeas.setDate(QDate(2022, 05, 24+n));
            measureData.dateTimeMeas.setTime(QTime(n, n));

            srv->waitForSync();
            srv->addMeasure(measureData);
            srv->waitPeriod(waitTime1);
        }

        for (int n = 1; n <= 3; n++)        // index of loop is used to modify variables to avoid having to do it manually for each iteration
        {
            //_____Add target_______________________________________________

            targetData.targetType  = rand() % 14;
            targetData.cMinInput   = n*100 + n*10 + n;
            targetData.cBestInput  = (n+1)*100 + (n+1)*10 + (n+1);
            targetData.cMaxInput   = (n+2)*100 + (n+2)*10 + (n+2);
            targetData.tMinInput   = n*10;
            targetData.tBestInput  = (n+1)*10;
            targetData.tMaxInput   = (n+2)*10;
            targetData.micInput    = (n*100) + 1000;

            srv->waitForSync();
            srv->addTarget(targetData);
            srv->waitPeriod(waitTime1);
        }

        srv->addAdjustments(adjustmentsData);
    }



    srv->waitPeriod(waitTime1);
    srv->waitForSync();

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // SAVING interpretation in Xml file

    QString saveName = "scenario" + QString::number(scenario);       //"/Documents/tucuxi-gui/dev/build-tucuxi-Desktop-DebugGui/dist/

    Interpretation *interpretationSave = srv->m_mainWindowController->getInterpretationController()->getInterpretation();

    mapSave = srv->fillMapWithInterpretation(interpretationSave);
//    QMapIterator<QString, QString> iSave(mapSave);
//    while (iSave.hasNext()) {
//        iSave.next();
//        qInfo() << "key =" << iSave.key() << "value =" << iSave.value();
//    }

    srv->saveIntepretation(saveName);
    qInfo() << "Saved";
    srv->waitPeriod(waitTime1);

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // Get to Startup window

    int drugIndex = 20;                     // drugIndex : 20 = Vancomycin
    int modelIndex = 3;
    srv->selectDrugInList(drugIndex, modelIndex);
    srv->waitPeriod(waitTime1);

//    srv->addDosage(dosageData);

    srv->m_mainWindowController->goToHome();
    srv->waitPeriod(waitTimeLong);

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // LOADING interpretation from Xml file
    QString loadName = saveName; // "save_1.tui";

    srv->loadInterpretation(loadName);
    qInfo() << "loaded";

    Interpretation *interpretationLoad = srv->m_mainWindowController->getInterpretationController()->getInterpretation();

    mapLoad = srv->fillMapWithInterpretation(interpretationLoad);
//    QMapIterator<QString, QString> iLoad(mapLoad);
//    while (iLoad.hasNext()) {
//        iLoad.next();
//        qInfo() << "key =" << iLoad.key() << "value =" << iLoad.value();
//    }

    if (mapSave == mapLoad)
        qInfo("Loaded and saved interpretation : Match OK");
    else if (mapSave != mapLoad){
        qInfo("Loaded and saved interpretation : Match NOK !");
        QMapIterator<QString, QString> iSave(mapSave);
        QMapIterator<QString, QString> iLoad(mapLoad);
        while(iSave.hasNext() || iLoad.hasNext()) {
            iSave.next();
            iLoad.next();
            if (iSave.value() != iLoad.value())
                qInfo() << "None matching key : " << iSave.key() << "/ mapSave value :" << iSave.value()<< "/ mapLoad value :" << iLoad.value();
        }
    }

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitPeriod(waitTimeLong);
    std::cout << "End of program ..." << std::endl;
}
