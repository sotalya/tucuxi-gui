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

    int waitTime1 = 1;
    int waitTimeLong = 4;

    srv->waitPeriod(waitTime1);             // default (when no arg.) value : 1/2 [s]

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

        srv->startNewPatient();                 // call function which clicks on "New patient" icon
        srv->waitPeriod(waitTime1);

        srv->selectDrugInList(13, 0);
        srv->waitPeriod(waitTime1);


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

    DosageData dosageData1;

    dosageData1.interval = 1750;
    dosageData1.dateTimeDos1.setDate(QDate(2022, 02, 10));
    dosageData1.dateTimeDos1.setTime(QTime(7, 30));
    dosageData1.steadyState = false;
    dosageData1.dateTimeDos2.setDate(QDate(2022, 03, 05));
    dosageData1.dateTimeDos2.setTime(QTime(8, 32));
    srv->addDosage(dosageData1);

    srv->waitPeriod(waitTime1);

    TargetData targetData1;
    srv->addTarget(targetData1);

    AdjustmentsData adjustmentsData1;
    srv->addAdjustments(adjustmentsData1);

    srv->waitPeriod(waitTimeLong);

    //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    srv->waitForSync();
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    std::cout << "End of test..." << std::endl;
}
