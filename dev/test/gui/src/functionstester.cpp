// File title       :   functionstester.cpp
// Test title       :   FunctionsTester
// Author           :   Julien Rosset
// Creation date    :   22.02.2022
// Purpose          :   Calls and tests functions in guitest.cpp
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

extern SpixGTest* srv;


TEST(FunctionsTester, Test1)
{

    int waitTime1 = 2;
    int waitTimeLong = 4;

    srv->waitPeriod(waitTime1);             // default (when no arg.) value : 1/2 [s]

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    //    srv->startNewPatient();                 // call function which clicks on "New patient" icon
    //    srv->waitPeriod(waitTime1);


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    //    PatientData patientData1;
    //    patientData1.firstName = "Joan";
    //    patientData1.lastName = "Of Arc";
    //    patientData1.birthDate.setDate(1914, 11, 11);
    //    patientData1.gender = 0;                         // male = 1, female = 0
    //    patientData1.identifier = "Patient_0_virusT";
    //    patientData1.stayNumber = "Private drive n°4";
    //    patientData1.titlePhy = "Dr.";
    //    patientData1.firstNamePhy = "Window";
    //    patientData1.lastNamePhy = "Cleaner";


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    //    QString drugName = "Cefepime";          // drug to be chosen HERE

    //    srv->selectDrugInList(6,0);
    //srv->selectDrugInList(20, 7);         // (int drugIndex, int modelIndex (= domain & study)

    //    srv->waitPeriod(waitTime1);

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    //    DosageData dosageData1;
    //    dosageData1.dosage      = 7500;
    //    dosageData1.infusion    = 900;
    //    dosageData1.interval    = 4800;
    //    dosageData1.steadyState = false;
    //    dosageData1.dateTimeDos1.setDate(QDate(2022, 05, 07));
    //    dosageData1.dateTimeDos1.setTime(QTime(15,23));
    //    dosageData1.dateTimeDos2.setDate(QDate(2022, 06, 13));
    //    dosageData1.dateTimeDos2.setTime(QTime(07,55));


    //    srv->addDosage(dosageData1);


    //    srv->mouseClick(spix::ItemPath("mainWindow/flowView/removeDosage_2"));
    //    srv->waitPeriod(waitTime1*4);

    //    // new values which require modification
    //    dosageData1.dosage = 7501;
    //    dosageData1.infusion = 902;
    //    dosageData1.interval = 2400;
    //    dosageData1.dateTimeDos1.setDate(QDate(2022, 07, 06));
    //    dosageData1.dateTimeDos1.setTime(QTime(14,05));
    //    dosageData1.dateTimeDos2.setDate(QDate(2022, 07, 15));
    //    dosageData1.dateTimeDos2.setTime(QTime(15,06));

    //    srv->addDosage(dosageData1);

    //    srv->editDosage(dosageData1, INDEXTOPUTHERE);
    //    srv->waitPeriod(waitTime1);


    //    srv->removeFromList("Dosage", 0);                                         // Runs ok
    //    srv->mouseClick(spix::ItemPath("mainWindow/flowView/removeDosage_2"));    // Runs ok


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    //    CovariatesData covariatesData1;
    //    covariatesData1.sex     = "Unknown";
    //    covariatesData1.weight  = 1234;
    //    covariatesData1.scc     = 3720;
    //    covariatesData1.dateTimeCovar.setDate(QDate(2022, 12, 12));
    //    covariatesData1.dateTimeCovar.setTime(QTime(06,06));


    //    srv->addCovariates(covariatesData1, 0);


    //    srv->addCovariates(covariatesData1, 0);             // arg : (covariatesData, type)
    //    srv->addDosage(dosageData1);



    //    dosageData1.dosage = 6556;
    //    srv->editDosage(dosageData1, 0);

    //    qInfo() << lastEntryIndex;
    //    editCovariates(covariatesData1, lastEntryIndex);

    //    srv->editCovariates(covariatesData1, 0);

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    //    MeasureData measureData1;
    //    measureData1.name = ("Sample_123_b");
    //    measureData1.value = 4567;
    //    measureData1.dateTimeMeas.setDate(QDate(2022, 02, 05));
    //    measureData1.dateTimeMeas.setTime(QTime(02, 03));


    //    for (int i = 0; i < 1; i++){
    //        srv->addMeasure(measureData1);
    //        srv->waitPeriod(waitTime1);
    //    }

    //    dosageData1.dosage      = 1111;
    //    dosageData1.infusion    = 222;
    //    dosageData1.interval    = 7200;
    //    dosageData1.dateTimeDos1.setDate(QDate(2024, 10, 07));
    //    dosageData1.dateTimeDos1.setTime(QTime(14,06));
    //    dosageData1.dateTimeDos2.setDate(QDate(2025, 11, 03));
    //    dosageData1.dateTimeDos2.setTime(QTime(15,22));

    //    srv->synchronize();
    //    srv->editDosage(dosageData1, 2);
    //    srv->removeFromList("Dosage", 1);

    //    srv->waitPeriod(waitTime1*4);

    //    srv->editMeasure(745, dateTime2);
    //    srv->waitPeriod(waitTime1);

    //    srv->removeFromList("Measure", 2);



    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    //    TargetData targetData1;
    //    targetData1.targetType  = 8;        // index 0-12 : peak = 1, AUC = [3-9], MIC [5-12]
    //    targetData1.cMinInput   = 602;
    //    targetData1.cBestInput  = 813;
    //    targetData1.cMaxInput   = 951;
    //    targetData1.tMinInput   = 20;
    //    targetData1.tBestInput  = 30;
    //    targetData1.tMaxInput   = 40;
    //    targetData1.micInput    = 1300;

    //    for (int i = 0; i < 5; i++){
    //        srv->addTarget(targetData1);
    //        srv->waitPeriod();
    //    }
    //    srv->removeFromList("Target", 02);

    //    covariatesData1.weight = 4321;
    //    srv->editCovariates(covariatesData1, 0, 0);         // arg : (covariatesData, type, index)

    //    covariatesData1.dateTimeCovar.setDate(QDate(2022, 07, 07));
    //    covariatesData1.weight = 6666;
    //    srv->addCovariates(covariatesData1, 0);

    //    targetData1.targetType = 2;
    //    srv->editTarget(targetData1, 0);

    //    covariatesData1.weight = 5555;
    //    srv->editCovariates(covariatesData1, 0, 1);

    //    srv->synchronize();
    //    srv->mouseClick(spix::ItemPath("mainWindow/flowView/targetButton"));
    //    srv->waitPeriod(waitTime1);

    //    srv->removeFromList("target", 0);

    //    srv->addCovariates(covariatesData1, 0);
    //    srv->addCovariates(covariatesData1, 0);

    //    srv->addMeasure(measureData1);

    //    srv->removeFromList("covariate", -1);
    //    srv->waitPeriod(waitTime1*4);

    //    srv->removeFromList("measure", 0);




    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    //    AdjustmentsData adjustmentsData1;
    //    adjustmentsData1.dateTimeAdj.setDate(QDate(2028,03,05));
    //    adjustmentsData1.dateTimeAdj.setTime(QTime(11, 12));
    //    adjustmentsData1.loadingDose    = true;
    //    adjustmentsData1.restPeriod     = true;
    //    adjustmentsData1.suggestAdjNum  = 3;

    //    srv->addAdjustments(adjustmentsData1);

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    //    ValidationData validationData1;
    //    validationData1.dateTimeVal.setDate(QDate(2022, 01, 02));
    //    validationData1.dateTimeVal.setTime(QTime(03, 04));
    //    validationData1.expectedness    = "Full destruction of cancer cells";
    //    validationData1.suitability     = "Perfect fit";
    //    validationData1.prediction      = "Everything's gonna be allright";
    //    validationData1.remonitoring    = "None";
    //    validationData1.warning         = "Don't push it...";


    //    validationData1.expectedness    = "Hold my TDM";
    //    validationData1.suitability     = "Live news from USSR";
    //    validationData1.prediction      = "Not on my watch";
    //    validationData1.remonitoring    = "Good morning Vietnam";
    //    validationData1.warning         = "One small step for man...";

    //    srv->fillInValidationData(validationData1);

    //    int globalSpecificIndex = 2;                // globalSpecific : 0 = global, 1 = specific, 2 = both
    //    QString commentType = "prediction";

    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    srv->saveValidationComment(commentType, globalSpecificIndex);


    //    int commentIndex = 2;
    //    globalSpecificIndex = 1;
    //    commentType = "prediction";

    //    srv->deleteValidationComment(commentType, globalSpecificIndex, commentIndex);

    //    srv->validateInterpretation();



    //    srv->mouseClick(spix::ItemPath("mainWindow/flowView/predictionPaletteButton"));
    //    srv->waitPeriod(waitTime1*2);

    //    srv->mouseClick(spix::ItemPath("predictionPalette/globalDeleteButton_0"));
    //    srv->waitPeriod(waitTime1*4);

    //    srv->mouseClick(spix::ItemPath("predictionPalette/globalDeleteButton_2"));
    //    srv->waitPeriod(waitTime1*4);

    //    srv->mouseClick(spix::ItemPath("predictionPalette/globalDeleteButton_0"));
    //    srv->waitPeriod(waitTime1*4);

    //    srv->mouseClick(spix::ItemPath("predictionPalette/specificDeleteButton"));
    //    srv->waitPeriod(waitTime1*2);


    //    commentType = "expectedness";
    //    globalSpecificIndex = 0;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    commentType = "suitability";
    //    globalSpecificIndex = 1;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    commentType = "prediction";
    //    globalSpecificIndex = 2;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    commentType = "remonitoring";
    //    globalSpecificIndex = 0;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    commentType = "warning";
    //    globalSpecificIndex = 1;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);


    //    validationData1.expectedness    = "1";
    //    validationData1.suitability     = "2";
    //    validationData1.prediction      = "3";
    //    validationData1.remonitoring    = "4";
    //    validationData1.warning         = "5";

    //    srv->fillInValidationData(validationData1);

    //    commentType = "expectedness";
    //    globalSpecificIndex = 0;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    commentType = "suitability";
    //    globalSpecificIndex = 1;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    commentType = "prediction";
    //    globalSpecificIndex = 2;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    commentType = "remonitoring";
    //    globalSpecificIndex = 0;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);
    //    commentType = "warning";
    //    globalSpecificIndex = 1;
    //    srv->saveValidationComment(commentType, globalSpecificIndex);


    //    srv->validateInterpretation();


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // Code below : ValidationTest, 24.03.2022
    /*
    srv->startNewPatient();

    srv->fillInPatientData(patientData1);


    int drugIndex = 6;                                                  // drugIndex : 6 = Cefepime
    int modelIndex = 0;                                                 // domain & study
    srv->selectDrugInList(drugIndex, modelIndex);

    srv->addDosage(dosageData1);

    int covariateType = 0;                                              // covariateType : 0 = weight
    srv->addCovariates(covariatesData1, covariateType);

    srv->addMeasure(measureData1);

    srv->addTarget(targetData1);

    srv->addAdjustments(adjustmentsData1);

    int globalSpecific = 2;                                             // globalSpecific : 0 = global, 1 = specific, 2 = both
    int commentIndex = 0;                                               // commentIndex : 0 = will delete any saved comments
    srv->deleteValidationComment("prediction", globalSpecific, commentIndex);

    srv->waitPeriod(waitTime1*4);

    for (int a = 0; a <= 7; a++) {
        validationData1.prediction = QString::number(a);                // comment text = loop index (for clarity if deletion)
        srv->fillInValidationData(validationData1);                     // fills in commentts in expectedness, suitabilty, prediction, remonitoring & warning text fields
        srv->saveValidationComment("prediction", globalSpecific);       // saves prediction comments in both global and specific registries
    }

    srv->deleteValidationComment("prediction", 0, 1);                   // deletes prediction comment, global, index 1
    srv->deleteValidationComment("prediction", 1, 2);                   // deletes prediction comment, specific, index 2
    srv->deleteValidationComment("prediction", 2, 1);                   // deletes prediction comment, both global & specific, index 1
    srv->deleteValidationComment("prediction", 2, 0);                   // deletes prediction comment, both, any saved comment

    srv->validateInterpretation();

    srv->printReport("Report_1234_b");
*/


    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-


    PatientData patientData1;
    DosageData dosageData1;
    CovariatesData covariatesData1;
    MeasureData measureData1;
    QDateTime dateTimeMeas1;
    TargetData targetData1;


    srv->startNewPatient();
    srv->waitPeriod(waitTime1);

    patientData1.firstName      = "Joan";
    patientData1.lastName       = "Of Arc";
    patientData1.birthDate.setDate(1984, 11, 11);
    patientData1.gender         = 0;                                      // male = 1, female = 0
    patientData1.identifier     = "Patient_0_virusT";
    patientData1.stayNumber     = "Private drive n°4";
    patientData1.titlePhy       = "Dr.";
    patientData1.firstNamePhy   = "Window";
    patientData1.lastNamePhy    = "Cleaner";

    srv->fillInPatientData(patientData1);

    srv->selectDrugInList(6, 0);
    srv->waitPeriod(waitTime1);

    int n = 1;
/*
    for (n = 1; n <= 1; n++)                                            // index of loop is used to modify variables to avoid having to do it manually for each iteratnon
    {
        dosageData1.dosage      = n*1000 + n*200 + n*30 + n*4;          // dosage(n) = {1234, 2345, 3456, 4567, ...}
        dosageData1.infusion    = n*1000;                               // infusion(i) = {100, 200, 300, 400, ...}
        dosageData1.interval    = n*1200;                               // interval(i) = {1200, 2400, 3600, 4800, ...}
        dosageData1.steadyState = false;                                // if steadyState = false -> start date & end date needed
        dosageData1.dateTimeDos1.setDate(QDate(2022, n, n));            // start date(i)
        dosageData1.dateTimeDos1.setTime(QTime(n, n));                  // start time(i)
        dosageData1.dateTimeDos2.setDate(QDate(2022, n+1, n+1));        // end date(i)
        dosageData1.dateTimeDos2.setTime(QTime(n+1, n+1));              // end time(i)


        int covarTypeIndex = 0;                                         // covariable type index 0 = weight
        covariatesData1.weight = n*4000 + n*200 + n*30 + n*4;           // same as Dosage (see above)
        covariatesData1.dateTimeCovar.setDate(QDate(2022, n, n));
        covariatesData1.dateTimeCovar.setTime(QTime(n, n));


        measureData1.name   = "Sample_1234_b";
        measureData1.value  = n*100 + n*100 + n*10 + n*1;
        measureData1.dateTimeMeas.setDate(QDate(2022, 05, 04));
        measureData1.dateTimeMeas.setTime(QTime(n, n));


        targetData1.targetType  = 8;
        targetData1.cMinInput   = n*100 + n*10 + n;
        targetData1.cBestInput  = (n+1)*100 + (n+1)*10 + (n+1);
        targetData1.cMaxInput   = (n+2)*100 + (n+2)*10 + (n+2);
        targetData1.tMinInput   = n*10;
        targetData1.tBestInput  = (n+1)*10;
        targetData1.tMaxInput   = (n+2)*10;
        targetData1.micInput    = (n*100) + 1000;


        //_____Add dosage_______________________________________________

//        srv->waitForSync();
        srv->addDosage(dosageData1);

        srv->waitPeriod(waitTime1);
        srv->synchronize();

        //_____Add covariates___________________________________________

//        srv->waitForSync();
        srv->addCovariates(covariatesData1, covarTypeIndex);

        srv->waitPeriod(waitTime1);
        srv->synchronize();

        //_____Add measure______________________________________________

//        srv->waitForSync();
        srv->addMeasure(measureData1);

        srv->waitPeriod(waitTime1);
        srv->synchronize();

        //_____Add target_______________________________________________

//        srv->waitForSync();
        srv->addTarget(targetData1);

        srv->waitPeriod(waitTimeLong);
        srv->synchronize();

   }
*/

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitForSync();
    srv->waitPeriod(10);
    srv->synchronize();

    std::cout << "End of test..." << std::endl;
}
