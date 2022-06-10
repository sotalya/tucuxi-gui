//@@license@@

#include <QQuickWindow>

#include <QTest>
#include <QQmlApplicationEngine>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/KeyCodes.h>

#include "guitest.h"
#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "guiutils/src/controllers/adjustmenttabcontroller.h"
#include "admin/src/dal/interpretation.h"

#include "admin/src/dal/interpretationanalysis.h"
#include "core/dal/drugresponseanalysis.h"
#include "core/dal/drugtreatment.h"
#include "core/dal/corepatient.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/practician.h"
#include "admin/src/dal/person.h"
#include "admin/src/dal/interpretationrequest.h"
#include "core/dal/dosage.h"
#include "core/dal/drug/drug.h"
#include "core/dal/covariate.h"
#include "core/dal/coremeasure.h"
#include "admin/src/dal/measure.h"
#include "core/dal/drug/target.h"
#include "admin/src/dal/phone.h"
#include "admin/src/interpretationxmlexport.h"


#include <QMessageBox>


class SpixGTest;
SpixGTest* srv;


int waitTime1 = 1;
int waitTimeLong = 10;

SpixGTest::SpixGTest(MainWindowController *mainWindowController, QQuickWindow *window, int argc, char* argv[])
{
    m_mainWindowController = mainWindowController;
    m_window = window;
    m_argc = argc;
    m_argv = argv;
}

int SpixGTest::testResult() { return m_result.load(); }

void SpixGTest::executeTest()
{
    srv = this;
    ::testing::InitGoogleTest(&m_argc, m_argv);
    auto testResult = RUN_ALL_TESTS();
    m_result.store(testResult);

    srv->quit();
}

bool SpixGTest::synchronize()
{
    return existsAndVisible(spix::ItemPath("paththatdonotexist"));
}

void SpixGTest::waitForSync()
{
    QVariant waitStatus = false;
    bool isRunning = false;

    int tabIndex = getCurrentTabIndex();

    if (tabIndex != 0 and tabIndex != 1)
    {
        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("chartOverlayView");

        srv->waitPeriod();
        srv->synchronize();

        do {
            srv->waitPeriod();
            srv->synchronize();

            waitStatus = item->property("waitStatus");
            isRunning = waitStatus.toBool();
            std::cout << "Sync : Is still running ..." << std::endl;

        } while (isRunning == true);

    }
    //else std::cout << "Tab index 0 or 1";
    std::cout << "Sync : OK" << std::endl;
}

int SpixGTest::getCurrentTabIndex()
{
    int currentTabIndex = 666;

//    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->flowView, "extGetTabIndex",
//                              Q_RETURN_ARG(QVariant, currentTabIndex));

    currentTabIndex = srv->m_mainWindowController->getInterpretationController()->flowView->property("currentIndex").toInt();
    std::cout << "Current tab index : " << currentTabIndex << std::endl;

    srv->waitPeriod(waitTime1);

    return currentTabIndex;
}

QObject *SpixGTest::getObjectByName(QObject *root, std::string name)
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

void SpixGTest::startNewPatient()
{
    srv->synchronize();
    bool isOnLaunchScreen = existsAndVisible(spix::ItemPath("mainWindow/launchView"));

    if (!isOnLaunchScreen) {
        srv->mouseClick(spix::ItemPath("mainWindow/applicationBarView/newAction"));
        srv->waitPeriod(waitTime1);
    }
    mouseClick(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));
    srv->waitPeriod(waitTime1);
}

void SpixGTest::saveIntepretation(QString saveName)
{
    if (!saveName.endsWith(".tui"))
        saveName += ".tui";
    srv->m_mainWindowController->getInterpretationController()->saveInterpretation(saveName);
    srv->waitPeriod(waitTime1);
}

void SpixGTest::loadInterpretation(QString loadName)
{
    if (!loadName.endsWith(".tui"))
        loadName += ".tui";

    fromGuiTest obj;
    if (QObject::connect(&obj, SIGNAL(loadInterpretation(QString)), srv->m_mainWindowController, SLOT(loadInterpretationFile(QString))) != 0) {
        std::cout << "Interpretation load : Ok" << std::endl;
    }
    obj.toMainWindowController(loadName);
    srv->waitPeriod(waitTime1);
}

void SpixGTest::extSetView(QDateTime startView, QDateTime endView)
{
    fromGuiTest obj;
    if (QObject::connect(&obj, SIGNAL(extSignalSetView(QDateTime, QDateTime)), srv->m_mainWindowController, SLOT(extSlotSetView(QDateTime, QDateTime)/*emptySlot()*/)) != 0) {
//        std::cout << "setViewRange : Ok" << std::endl;
    }else std::cout << "setViewRange : Nok" << std::endl;
    obj.toIntController(startView, endView);

}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

void SpixGTest::findObjectAndSetValue(QString objectName, int propertyInput)
{
    // retrieves object by name and puts it into an item to set or get its properties (value)
    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>(objectName);
    if (item != (0x0))
        item->setProperty("value", propertyInput);
    else std::cout << "Item not found !" << std::endl;

    srv->waitPeriod(waitTime1);
}

void SpixGTest::removeFromList(std::string removeFrom, int removeIndex = 0)      // default index = 0, will remove any existing inputs
{
    // if removeIndex > 0, only chosen input will be removed
    // if removeIndex <= 0, all above inputs will also be removed
    // applies to {Dosage, Target, Measure}

    std::string buttonPath = "mainWindow/flowView/" + removeFrom + "Button";
    std::string removePath = "mainWindow/flowView/remove";

    srv->synchronize();
    srv->mouseClick(spix::ItemPath(buttonPath));
    srv->waitPeriod(waitTime1);

    removeFrom[0] = toupper(removeFrom[0]);             // caps 1st letter

    if (removeIndex > 0)
{
        removePath = removePath + removeFrom + "_" + std::to_string(removeIndex);

        if (srv->existsAndVisible(spix::ItemPath(removePath)))
            srv->mouseClick(spix::ItemPath(removePath));
    }
    else
    {
        removePath = removePath + removeFrom + "_" + std::to_string(removeIndex*(-1));

        while (srv->existsAndVisible(spix::ItemPath(removePath))) {
            srv->mouseClick(spix::ItemPath(removePath));
            srv->waitPeriod(waitTime1*2);
        }
    }
    // Runs ok
}

void SpixGTest::selectDrugInList(int drugIndex, int modelIndex)
{
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod(waitTime1);

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentActiveSubstance",
                              Q_ARG(QVariant, QVariant::fromValue(drugIndex)));
//    srv->waitPeriod();

    // model = DOMAIN & STUDY
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentDrugModel",
                              Q_ARG(QVariant, QVariant::fromValue(modelIndex)));
    srv->waitPeriod(waitTime1);
    // Runs ok
}


void SpixGTest::fillInPatientData(PatientData patientData1)
{

    QString yearPat    = patientData1.birthDate.toString("yyyy");
    QString monthPat   = QString::number(patientData1.birthDate.toString("MM").toInt() - 1);
    QString dayPat     = patientData1.birthDate.toString("dd");

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/patientButton"));
    srv->waitPeriod(waitTime1*2);

    if (patientData1.gender == 0)
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/femaleRadioInput"));
    else
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/maleRadioInput"));

    srv->synchronize();

    // invoke method to expand Patient and Physician tabs and then fill in data
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->patientsView, "extPatientData",
                              Q_ARG(QVariant, QVariant::fromValue(yearPat)),
                              Q_ARG(QVariant, QVariant::fromValue(monthPat)),
                              Q_ARG(QVariant, QVariant::fromValue(dayPat)),

                              Q_ARG(QVariant, QVariant::fromValue(patientData1.firstName)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.lastName)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.identifier)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.stayNumber)));

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->patientsView, "extPatientContactData",
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.address)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.city)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.state)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.postcode)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.country)));

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->patientsView, "extPhysicianData",
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.titlePhy)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.firstNamePhy)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.lastNamePhy)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.affiliationPhy)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.addressPhy)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.cityPhy)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.statePhy)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.postcodePhy)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.countryPhy)));

    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/patientButton"));
    srv->synchronize();
}

void SpixGTest::addDosage(DosageData dosageData1)
{
    // clicks on Dosage tab
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod(waitTime1);

    // adds new Dose (will open Dosage dialog window)
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod(waitTime1);

    setSteadyStateDosage(dosageData1.steadyState);
    fillInDosageData(dosageData1);

    // Runs ok
}

void SpixGTest::editDosage(DosageData dosageData1, int editIndex)
{
    // edits existing Dose (will open Dosage dialog window)
    std::string editPath = "mainWindow/flowView/editDosage_" + std::to_string(editIndex);

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod(waitTime1);

    if (srv->existsAndVisible(spix::ItemPath(editPath)))
    {
        srv->synchronize();
//        srv->mouseClick(spix::ItemPath("mainWindow/flowView/editDosage_0"));
        srv->mouseClick(spix::ItemPath(editPath));
        srv->waitPeriod(waitTime1);

        setSteadyStateDosage(dosageData1.steadyState);
        fillInDosageData(dosageData1);
    }

    // Runs ok
}

void SpixGTest::setSteadyStateDosage(bool value)
{
    srv->synchronize();
    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("atSteadyState");
    item->setProperty("checked", value);
    srv->waitPeriod(waitTime1);
}

QVariant SpixGTest::getSteadyStateDosage()
{
    // QVariant type can be tested as a bool type (in if loop for example)
    srv->synchronize();
    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("atSteadyState");
    auto steadyStateDosage = item->property("checked");
//    qInfo() << "SteadyStateDosage value : " << steadyStateDosage;
    srv->waitPeriod(waitTime1);
    return steadyStateDosage;
}

void SpixGTest::fillInDosageData(DosageData dosageData1)
{
    // fills in Dose, Interval, reads Route, fills in Infusion, ...

    QString dateDos = dosageData1.dateTimeDos1.date().toString("dd.MM.yyyy");
    QString timeDos = dosageData1.dateTimeDos1.time().toString("HH:mm:ss");

    // fills in Dose value, in [µg]
    srv->waitPeriod(waitTime1);

    findObjectAndSetValue("doseSpinBox", dosageData1.dosage);

    // fills in Interval value, in [h / 100]
    findObjectAndSetValue("intervalSpinBox", dosageData1.interval);

    //srv->synchronize();
    auto routeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("routeText");
    QString routeText = routeItem->property("text").toString();
//    qInfo() << "Route : " << routeText;

    if (routeText == "Intravenous drip")
    {
        // Infusion parameter only to be filled if Route = Intravenous drip != Oral
//        qInfo() << "Infusion value to be filled in";
        findObjectAndSetValue("infusionSpinBox", dosageData1.infusion);

    }
    else if (routeText == "Oral")
    {
        // do nothing ..?
//        qInfo() << "No infusion to be done";
    }

    QVariant steadyStateValue = getSteadyStateDosage();

    auto dateItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("LastDoseOrFromDateInput");
    dateItem->setProperty("date", dosageData1.dateTimeDos1.date());

    auto timeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("LastDoseOrFromTimeInput");
    timeItem->setProperty("date", dosageData1.dateTimeDos1.time());

    srv->waitPeriod(waitTime1);

    if (steadyStateValue == false)
    {
//        qInfo() << "At steady state? NO";

        auto dateItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("stoppedDateInput");
        dateItem->setProperty("date", dosageData1.dateTimeDos2.date());

        auto timeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("stoppedTimeInput");
        timeItem->setProperty("date", dosageData1.dateTimeDos2.time());

        srv->waitPeriod(waitTime1);
    }

//    else qInfo() << "At steady state? YES";

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("dosageDialog/applyDosage"));

    // closes Dosage dialog window when editing is done
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));
    srv->waitPeriod(waitTime1);
    srv->synchronize();
    // Runs ok
}


void SpixGTest::addCovariates(CovariatesData covariatesData1, int covariateType)
{
    srv->synchronize();
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/covariateButton"));

    srv->synchronize();

    // selects covariate (index) to edit {Sex, Weight, ...}
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->covariatesView, "setCovariateType",
                              Q_ARG(QVariant, QVariant::fromValue(covariateType)));

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addCovariate"));      // will open Covariate dialog
    srv->waitPeriod(waitTime1*4);

    fillInCovariatesData(covariatesData1, covariateType);

    // Runs ok
}

void SpixGTest::editCovariates(CovariatesData covariatesData1, int covariateType, int editIndex)
{
    std::string editPath = "mainWindow/flowView/editCovariate_" + std::to_string(editIndex);

    srv->synchronize();
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/covariateButton"));

    // selects covariate (index) to edit {Sex, Weight, ...}
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->covariatesView, "setCovariateType",
                              Q_ARG(QVariant, QVariant::fromValue(covariateType)));

    srv->synchronize();

    if (srv->existsAndVisible(spix::ItemPath(editPath)))
    {
        srv->mouseClick(spix::ItemPath(editPath));
        srv->waitPeriod(waitTime1);
        fillInCovariatesData(covariatesData1, covariateType);
    }

    // Runs ok
}

void SpixGTest::fillInCovariatesData(CovariatesData covariatesData1, int covariateType)
{
    srv->synchronize();

//    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("valueDoubleControl");  // old spinbox
    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("covarValueEntry");        // new textEntry
    //    qInfo() << item;


    if (covariateType == 0)                             // if covariateType == Total Body Weight
    {
        item->setProperty("text", covariatesData1.weight);
    }
    else if (covariateType == 3)
    {
        item->setProperty("text", covariatesData1.scc + QTime::currentTime().second());
    }



    srv->synchronize();
    srv->waitPeriod(waitTime1);

    auto dateItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("dateInputCovar");
    dateItem->setProperty("date", covariatesData1.dateTimeCovar.date());
    auto timeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("timeInputCovar");
    timeItem->setProperty("date", covariatesData1.dateTimeCovar.time());
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("covariateDialog/okCovariate"));

    srv->waitPeriod(waitTime1);


    // Runs ok
}

void SpixGTest::addCovariatesByDrug(CovariatesData covariatesData1, int covariateType, int drugIndex)
{
    srv->synchronize();
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/covariateButton"));

    srv->synchronize();

    // selects covariate (index) to edit {Sex, Weight, ...}
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->covariatesView, "setCovariateType",
                              Q_ARG(QVariant, QVariant::fromValue(covariateType)));

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addCovariate"));
    srv->waitPeriod(waitTime1*4);

    fillInCovariatesDataByDrug(covariatesData1, covariateType, drugIndex);
}

void SpixGTest::fillInCovariatesDataByDrug(CovariatesData covariatesData1, int covariateType, int drugIndex)
{
    srv->synchronize();

    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("valueDoubleControl");

    if(drugIndex == 2) // Apixaban
    {
        if (covariateType == 2)
            item->setProperty("value", covariatesData1.dayNightDosing);
        else if (covariateType == 3)
            item->setProperty("value", covariatesData1.weight);
        else if (covariateType == 4)
            item->setProperty("value", covariatesData1.asian);
        else if (covariateType == 5)
            item->setProperty("value", covariatesData1.nonValvularAtrialFibriliation);
        else if (covariateType == 6)
            item->setProperty("value", covariatesData1.acuteCoronarySyndrome);
        else if (covariateType == 7)
            item->setProperty("value", covariatesData1.strongModerateCytochromeInhibitors);
        else if (covariateType == 8)
            item->setProperty("value", covariatesData1.dose);
        else if (covariateType == 9)
            item->setProperty("value", covariatesData1.glomerularFiltrationRate);
    }

    else if(drugIndex == 4) // Busulfan
    {
        if (covariateType == 1)
            item->setProperty("value", covariatesData1.weight);
    }

    else if (drugIndex == 6) // Cefepime
    {
        if (covariateType == 0)
            item->setProperty("value", covariatesData1.weight);
        else if (covariateType == 3)
            item->setProperty("value", covariatesData1.scc);
    }

    else if(drugIndex == 9) // Dolutegravir
    {
        if (covariateType == 2)
            item->setProperty("value", covariatesData1.atazanavirWithWithoutBooster);
        else if (covariateType == 3)
            item->setProperty("value", covariatesData1.darunavir);
        else if (covariateType == 4)
            item->setProperty("value", covariatesData1.rifampicin);
        else if (covariateType == 5)
            item->setProperty("value", covariatesData1.currentSmoking);
    }

    else if (drugIndex == 13) // Imanitib
    {
        if (covariateType == 0)
            item->setProperty("value", covariatesData1.weight);
    }

    else if (drugIndex == 20) // Vancomycin
    {
        if (covariateType == 0)
            item->setProperty("value", covariatesData1.weight);
        else if (covariateType == 3)
            item->setProperty("value", covariatesData1.scc);
        else if (covariateType == 4)
            item->setProperty("value", covariatesData1.gestionalAge);
        else if (covariateType == 6)
            item->setProperty("value", covariatesData1.heamatologicalMalignacy);
    }


    srv->synchronize();
    srv->waitPeriod(waitTime1);

    auto dateItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("dateInputCovar");
    dateItem->setProperty("date", covariatesData1.dateTimeCovar.date());
    auto timeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("timeInputCovar");
    timeItem->setProperty("date", covariatesData1.dateTimeCovar.time());
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("covariateDialog/okCovariate"));

    srv->waitPeriod(waitTime1);


    // Runs ok
}


void SpixGTest::addMeasure(MeasureData measureData1)
{    
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/measureButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/measuresView/addMeasure"));
    srv->waitPeriod(waitTime1);

    fillInMeasureData(measureData1);
}

void SpixGTest::editMeasure(MeasureData measureData1, int editIndex)
{
    std::string editPath = "mainWindow/flowView/editMeasure_" + std::to_string(editIndex);

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/measureButton"));
    srv->waitPeriod(waitTime1);

    if (srv->existsAndVisible(spix::ItemPath(editPath)))
    {
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(editPath));
        srv->waitPeriod(waitTime1);
        fillInMeasureData(measureData1);
    }

}

void SpixGTest::fillInMeasureData(MeasureData measureData1)
{
    srv->synchronize();
//    auto idItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleIdField");
//    idItem->setProperty("text", measureData1.name);
    srv->mouseClick(spix::ItemPath("measureDialog/sampleIdField"));
    srv->inputText(spix::ItemPath("measureDialog/sampleIdField"), measureData1.name.toStdString());

    srv->waitPeriod(waitTime1);

    auto valueItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueEntry");
    valueItem->setProperty("text", measureData1.value * 100.0);
    srv->waitPeriod(waitTime1);

    auto dateItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleDateInput");
    dateItem->setProperty("date", measureData1.dateTimeMeas.date());
    auto timeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleTimeInput");
    timeItem->setProperty("time", measureData1.dateTimeMeas.time());

    srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));

    srv->waitPeriod(waitTime1);

}


void SpixGTest::addTarget(TargetData targetData1)
{
    srv->synchronize();
    if (existsAndVisible(spix::ItemPath("mainWindow/flowView/addTarget")) == false)
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/targetButton"));

    srv->waitPeriod(waitTime1);

    // adds new Dose (will open Dosage dialog window)
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addTarget"));
    srv->waitPeriod(waitTime1);

    fillInTargetData(targetData1);

}

void SpixGTest::editTarget(TargetData targetData1, int editIndex)
{
    std::string editPath = "mainWindow/flowView/editTarget_" + std::to_string(editIndex);

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/targetButton"));
    srv->waitPeriod(waitTime1);

    // edits Dose (will open Dosage dialog window)
    if(srv->existsAndVisible(spix::ItemPath(editPath)))
    {
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(editPath));
        srv->waitPeriod(waitTime1);
        fillInTargetData(targetData1);
    }
}

void SpixGTest::fillInTargetData(TargetData targetData1)
{
    srv->synchronize();
    srv->waitPeriod(waitTime1);

//    srv->mouseClick(spix::ItemPath("targetDialog/typeInput"));        // to click on combobox, not used anymore
//    srv->waitPeriod(waitTime1);

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->targetDialog, "setActiveType",
                              Q_ARG(QVariant, QVariant::fromValue(targetData1.targetType)));
    srv->synchronize();

    // cMax & tMax values are filled in first to avoir red font color warning due to value inconsistency : cMin > cMax (which = 0 before edition)

    findObjectAndSetValue("cMaxInput",  targetData1.cMaxInput);
    findObjectAndSetValue("cBestInput", targetData1.cBestInput);
    findObjectAndSetValue("cMinInput",  targetData1.cMinInput);

    if (targetData1.targetType == 1)        // target type 1 = Peak
    {
        findObjectAndSetValue("tMaxInput",  targetData1.tMaxInput);
        findObjectAndSetValue("tBestInput", targetData1.tBestInput);
        findObjectAndSetValue("tMinInput",  targetData1.tMinInput);
    }

    if (targetData1.targetType > 5)         // target types over 5 include MIC
    {
         findObjectAndSetValue("micInput",   targetData1.micInput);
    }

    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("targetDialog/okTarget"));

    srv->waitPeriod(waitTime1);

//    qInfo() << "Target type : " << targetData1.targetType;
}


void SpixGTest::addAdjustments(AdjustmentsData adjustmentsData1)
{
    QString yearAdj = adjustmentsData1.dateTimeAdj.date().toString("yyyy");
    QString monthAdj = adjustmentsData1.dateTimeAdj.date().toString("MM");
    QString dayAdj = adjustmentsData1.dateTimeAdj.date().toString("dd");

    QString hourAdj = adjustmentsData1.dateTimeAdj.time().toString("HH");
    QString minuteAdj = adjustmentsData1.dateTimeAdj.time().toString("mm");

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod(waitTime1);

    // can't pass arg type QDate to QML function, string type also allows to segregate years, months, ...
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->adjustmentsView, "extAdjDate",
                              Q_ARG(QVariant, QVariant::fromValue(yearAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(monthAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(dayAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(hourAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(minuteAdj)));

    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addAdjustment"));
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/editAdjustments_0"));
    srv->waitPeriod(waitTime1);

//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/selectAdjustment_3"));    // Runs ok
    srv->waitPeriod(waitTime1*10);
    srv->synchronize();

}

void SpixGTest::selectAdjustments(int selectIndex)
{
    std::string selectPath = "mainWindow/flowView/selectAdjustment_" + std::to_string(selectIndex);

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod(waitTime1);

    if (srv->existsAndVisible(spix::ItemPath(selectPath)))
    {
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(selectPath));
        srv->waitPeriod(waitTime1);
    }
}

void SpixGTest::editAdjustments(AdjustmentsData adjustmentsData1, int editIndex)
{
    std::string editPath = "mainWindow/flowView/editAdjustment_" + std::to_string(editIndex);

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod(waitTime1);

    if (srv->existsAndVisible(spix::ItemPath(editPath)))
    {
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(editPath));
        srv->waitPeriod(waitTime1);

//        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->adjustmentsView, "extInputData",

        srv->synchronize();
        srv->findObjectAndSetValue("doseSpinBoxAdj", adjustmentsData1.dose);
        srv->findObjectAndSetValue("intervalSpinBoxAdj", adjustmentsData1.interval);

        srv->waitPeriod(waitTimeLong);

        srv->mouseClick(spix::ItemPath("adjustmentDialog/applyAdj"));
        srv->mouseClick(spix::ItemPath("adjustmentDialog/okAdj"));
        srv->synchronize();

    }

}


void SpixGTest::fillInValidationData(ValidationData validationData1)
{
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod(waitTime1);
    srv->synchronize();

    auto validItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("nextControlDate");
    validItem->setProperty("date", validationData1.dateTimeVal.date());

    validItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("nextControlTime");
    validItem->setProperty("date", validationData1.dateTimeVal.time());

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->validationView, "extTextInputs",
                              Q_ARG(QVariant, QVariant::fromValue(validationData1.expectedness)),
                              Q_ARG(QVariant, QVariant::fromValue(validationData1.suitability)),
                              Q_ARG(QVariant, QVariant::fromValue(validationData1.prediction)),
                              Q_ARG(QVariant, QVariant::fromValue(validationData1.remonitoring)),
                              Q_ARG(QVariant, QVariant::fromValue(validationData1.warning)));
    // Runs ok

    srv->waitPeriod(waitTime1);
    srv->synchronize();
}

void SpixGTest::saveValidationComment(QString commentType, int globalSpecificIndex)
{
    // commentType = "expectedness", "suitability", "predition", "remonitoring" or "warning"
    // globalSpecificIndex : 0 = global, 1 = specific, 2 = both

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));

    srv->synchronize();

    std::string saveButtonPath = "mainWindow/flowView/" + commentType.toStdString() + "SaveButton";
    srv->mouseClick(spix::ItemPath(saveButtonPath));
    srv->waitPeriod(waitTime1);

    // Runs ok

    std::string addCommentPath = commentType.toStdString() + "Palette/";

    switch (globalSpecificIndex) {

    case 0:
        srv->mouseClick(spix::ItemPath(addCommentPath + "addGlobalListButton"));
        break;
    case 1:
        srv->mouseClick(spix::ItemPath(addCommentPath + "addSpecificListButton"));
        break;
    case 2:
        srv->mouseClick(spix::ItemPath(addCommentPath + "addGlobalListButton"));
        srv->mouseClick(spix::ItemPath(addCommentPath + "addSpecificListButton"));
        break;
    default:
        break;
    }

    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath(commentType.toStdString() + "Palette/validationCloseButton"));

    // Runs ok
}

void SpixGTest::deleteValidationComment(QString commentType, int globalSpecificIndex, int commentIndex)
{
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));

    srv->synchronize();

    std::string deleteButtonPath = "mainWindow/flowView/" + commentType.toStdString() + "PaletteButton";
    srv->mouseClick(spix::ItemPath(deleteButtonPath));
    srv->waitPeriod(waitTime1);

    std::string deleteCommentPath = commentType.toStdString() + "Palette/";

    if (commentIndex == 0) {

        if (globalSpecificIndex == (0) or globalSpecificIndex == (2)) {
            while (srv->existsAndVisible(spix::ItemPath(deleteCommentPath + "globalDeleteButton_" + std::to_string(0)))) {
                srv->mouseClick(spix::ItemPath(deleteCommentPath + "globalDeleteButton_" + std::to_string(0)));
                srv->waitPeriod();
            }
        }
        if (globalSpecificIndex == (1) or globalSpecificIndex == (2)) {
            while (srv->existsAndVisible(spix::ItemPath(deleteCommentPath + "specificDeleteButton_" + std::to_string(0)))) {
                srv->mouseClick(spix::ItemPath(deleteCommentPath + "specificDeleteButton_" + std::to_string(0)));
                srv->waitPeriod();
            }
        }

    }
    else  {
        switch (globalSpecificIndex) {

        case 0:
            srv->mouseClick(spix::ItemPath(deleteCommentPath + "globalDeleteButton_" + std::to_string(commentIndex)));
            break;
        case 1:
            srv->mouseClick(spix::ItemPath(deleteCommentPath + "specificDeleteButton_" + std::to_string(commentIndex)));
            break;
        case 2:
            srv->mouseClick(spix::ItemPath(deleteCommentPath + "globalDeleteButton_" + std::to_string(commentIndex)));
            srv->mouseClick(spix::ItemPath(deleteCommentPath + "specificDeleteButton_" + std::to_string(commentIndex)));
            break;
        default:
            break;
        }
        }

    srv->waitPeriod(waitTime1);
    srv->synchronize();

    srv->mouseClick(spix::ItemPath(commentType.toStdString() + "Palette/validationCloseButton"));
}

void SpixGTest::validateInterpretation()
{
    srv->synchronize();
    if (existsAndVisible(spix::ItemPath("mainWindow/flowView/validateInterpretation")) == false)
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validateInterpretation"));
    srv->waitPeriod(waitTime1);
    srv->synchronize();

//    srv->enterKey(spix::ItemPath("mainWindow"), spix::KeyCodes::Enter, 0);
//    srv->synchronize();
}


    //QTest::keyClick(qApp->activeWindow(), Qt::Key_Enter);     // keeping it for later :)

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

void SpixGTest::printReport(QString reportFileName)
{
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
    srv->waitPeriod();
    srv->synchronize();
    std::cout << "Printing report" << std::endl;

    srv->m_mainWindowController->getInterpretationController()->setReportFileName(reportFileName);
    srv->mouseClick(spix::ItemPath("mainWindow/reportView/printButton"));

    srv->waitPeriod(10);
    srv->synchronize();
}

int SpixGTest::getNbProposedAdjustments()
{
    auto engine = srv->m_mainWindowController->engine;
    QObject *root = engine->rootObjects().at(0);

    auto adjList = getObjectByName(root, "adjustmentListView");

    int nbAdjs = -1;
    QVariant v;

    if (!QMetaObject::invokeMethod(adjList, "nbProposedAdjustments", Qt::DirectConnection, Q_RETURN_ARG(QVariant, v))) {
        std::cout << "Error with invoke" << std::endl;
    }
    QString s = v.toString();
    bool ok;
    nbAdjs = v.toInt(&ok);
    return nbAdjs;
}


QMap<QString, QString> SpixGTest::fillMapWithInterpretation(Interpretation *interpretation)
{
    InterpretationXmlExport *interpretationXmlExport;

    QMap<QString, QString> map;
    auto i = 0;
    auto j = 0;
    auto k = 0;

    //----------------------------------------
    //  Interpretation informations
    //----------------------------------------
    map["interpretationStartInterpretationTime"]    = interpretation->getStartInterpretationTime().toString(Qt::ISODate);
    map["interpretationValidateInterpretationTime"] = interpretation->getValidateInterpretationTime().toString(Qt::ISODate);

    //----------------------------------------
    //  Patient information
    //----------------------------------------
    auto corePatient        = interpretation->getDrugResponseAnalysis()->getTreatment()->getPatient();
    auto patient            = dynamic_cast<Patient*>(corePatient);
    auto patientPerson      = patient->person();
    auto patientLocation    = patientPerson->location();

    map["patientPersonId"]      = QString::number(patient->person_id());
    map["patientExternalId"]    = patient->externalId();
    map["patientStayNumber"]    = patient->stayNumber();

    map["patientName"]              = patientPerson->name();
    map["patientFirstName"]         = patientPerson->firstname();
    map["patientBirthday"]          = QVariant(patientPerson->birthday()).toString();
    map["patientGender"]            = QVariant(patientPerson->gender()).toString();
//    map["physicianLocation_id"]    = patientPerson->location_id(); //voir si doit laisser MCI


    map["patientAddress"]       = patientLocation->address();
    map["patientPostcode"]      = patientLocation->postcode();
    map["patientCity"]          = patientLocation->city();
    map["patientState"]         = patientLocation->state();
    map["patientCountry"]       = patientLocation->country();
//    map["patientLocation_id"]   = patientPerson->location_id(); //voir si doit laisser MCI
    //End of Patient information

    //----------------------------------------
    //  Physician information
    //----------------------------------------
    auto physician                    = interpretation->getRequest()->getPractician();
    auto physicianPerson              = physician->person();
    auto physicianPersonLocation      = physicianPerson->location();
    auto physicianInstitute           = physician->institute();
    auto physicianInstituteLocation   = physicianInstitute->location();

//    map["physicianExternalId"]     = physician->externalId(); //voir si doit laisser MCI
    map["physicianTitle"]          = physician->title();
//    map["physicianPersonId"]       = QString::number(physician->person_id()); //voir si doit laisser MCI
//    map["physicianInstituteId"]    = QString::number(physician->institute_id()); //voir si doit laisser MCI

    map["physicianName"]           = physicianPerson->name();
    map["physicianFirstName"]      = physicianPerson->firstname();
//    map["physicianLocation_id"]    = QString::number(physicianPerson->location_id()); //voir si doit laisser MCI

    map["physicianAddress"]     = physicianInstituteLocation->address();
    map["physicianPostcode"]    = physicianPersonLocation->postcode();
    map["physicianCity"]        = physicianPersonLocation->city();
    map["physicianState"]       = physicianPersonLocation->state();
    map["physicianCountry"]     = physicianPersonLocation->country();
    map["physicianAffiliation"] = physicianInstitute->name();
    //End of physician information

    //----------------------------------------
    //  Analyst information
    //----------------------------------------
    auto analyst                    = interpretation->getAnalyst();
    auto analystPerson              = analyst->person();
    auto analystPersonLocation      = analystPerson->location();
    auto analystInstitute           = analyst->institute();
    auto analystInstituteLocation   = analystInstitute->location();
    auto analystPhoneList           = analystPerson->getPhones();


//    map["analystExternalId"]    = analyst->externalId();  //voir si doit laisser MCI
    map["analystTitle"]         = analyst->title();
    map["analystRole"]          = analyst->role();
//    map["analystPersonId"]       = QString::number(analyst->person_id()); //voir si doit laisser MCI
//    map["analystInstituteId"]    = QString::number(analyst->institute_id()); //voir si doit laisser MCI

    map["analystName"]           = analystPerson->name();
    map["analystFirstName"]      = analystPerson->firstname();
//    map["analystLocation_id"]    = QString::number(analystPerson->location_id()); //voir si doit laisser MCI

    map["analystAddress"]       = analystInstituteLocation->address();
    map["analystPostcode"]      = analystPersonLocation->postcode();
    map["analystCity"]          = analystPersonLocation->city();
    map["analystState"]         = analystPersonLocation->state();
    map["analystCountry"]       = analystPersonLocation->country();
    map["analystAffiliation"]   = analystInstitute->name();

    foreach(Phone *phone, analystPhoneList->getList()) {
        map["analystPhoneNumber"] = phone->getNumber();
    }
    //End of analyst information

    //----------------------------------------
    //  Drugs informations
    //----------------------------------------
    map["drugsActiveSubstanceId"] = interpretation->getDrugResponseAnalysis()->getTreatment()->getActiveSubstanceId();
    map["drugsStudyName"]         = interpretation->getDrugResponseAnalysis()->getDrugModel()->getStudyName();
    map["drugsDomainName"]        = interpretation->getDrugResponseAnalysis()->getDrugModel()->getDomainName();
    // End of Drugs information

    //----------------------------------------
    //  Dosages informations
    //----------------------------------------
    auto dosageFirstTake = interpretation->getRequest()->getTreatment()->getDosages()->firsttake();
    auto dosage          = interpretation->getRequest()->getTreatment()->getDosages()->dosage(dosageFirstTake);
    auto dosageRoute     = dosage->getRoute();

    map["dosageDbInterval"]               = QString::number(dosage->getDbinterval());
    map["dosageDbTInf"]                   = QString::number(dosage->getDbtinf());
    map["dosageQuantity"]                 = QString::number(dosage->getQuantity()->getDbvalue());
    map["dosageApplied"]                  = dosage->getApplied().toString(Qt::ISODate);
    map["dosageEndTime"]                  = dosage->getEndTime().toString(Qt::ISODate);
    map["dosageIsAtSteadyState"]          = QVariant(dosage->getIsAtSteadyState()).toString();
    map["dosageSteadyStateLastDoseDate"]  = dosage->getSteadyStateLastDoseDate().toString(Qt::ISODate);
    map["dosageTinf"]                     = QString::number(dosage->getTinf().toMinutes());
    map["dosageInterval"]                 = QString::number(dosage->getInterval().toMinutes());

    map["dosageAdministrationRoute"]  = dosageRoute->getAdministrationRoute();
    map["dosageAdministrationName"]   = dosageRoute->getAdministrationName();
    map["dosageFormulation"]          = dosageRoute->getFormulationString();
    map["dosageLabel"]                = dosageRoute->getLabelString();
    //End of Dosages informations

    //----------------------------------------
    //  Covariates informations
    //----------------------------------------
    auto patientVariateList = interpretation->getDrugResponseAnalysis()->getTreatment()->getCovariates();

    foreach(Tucuxi::GuiCore::PatientVariate *variate, patientVariateList->getList()) {
        map["covariateCovariateId_"     + QString::number(i)]    = variate->getCovariateId();
        map["covariateDateCovariate_"   + QString::number(i)]  = variate->getDate().toString(Qt::ISODate);
        if(map["covariateCovariateId_"  + QString::number(i)] == "birthdate")
            map["covariateValueCovariate_"  + QString::number(i)] = variate->getValueAsString();
        else
            map["covariateValueCovariate_"  + QString::number(i)] = QString::number(variate->getQuantity()->getDbvalue());
        i++;
    }
    //End of Covariates informations

//    //----------------------------------------
//    //  Measures informations
//    //----------------------------------------
    auto coreMeasureList = interpretation->getDrugResponseAnalysis()->getTreatment()->getMeasures();

    foreach(Tucuxi::GuiCore::CoreMeasure *coreMeasure, coreMeasureList->getList()) {
        Measure *measure = dynamic_cast<Measure*>(coreMeasure);
        map["measureSampleId_"     + QString::number(j)]   = measure->sampleID();
        map["measureArrivalDate_"  + QString::number(j)]   = measure->arrivalDate().toString(Qt::ISODate);
        map["measureMoment_"       + QString::number(j)]   = measure->getMoment().toString(Qt::ISODate);
        map["measureConcentration_"+ QString::number(j)]   = QString::number(measure->getConcentration()->getDbvalue());
        j++;
    }
    //End of Measure informations

    //----------------------------------------
    //  Targets informations
    //----------------------------------------
    auto targetList = interpretation->getDrugResponseAnalysis()->getTreatment()->getTargets();

    foreach(Tucuxi::GuiCore::Target *target, targetList->getList()) {
        map["targetMethod_"             + QString::number(k)]   = target->getType()->getLabel();
        map["targetConcentrationsCmin_" + QString::number(k)]   = QString::number(target->getCmin()->getDbvalue());
        map["targetConcentrationsCmax_" + QString::number(k)]   = QString::number(target->getCmax()->getDbvalue());
        map["targetConcentrationsCbest_"+ QString::number(k)]   = QString::number(target->getCbest()->getDbvalue());
        map["targetConcentrationsTmin_" + QString::number(k)]   = QString::number(target->getTmin()->getDbvalue());
        map["targetConcentrationsTmax_" + QString::number(k)]   = QString::number(target->getTmax()->getDbvalue());
        map["targetConcentrationsTbest_"+ QString::number(k)]   = QString::number(target->getTbest()->getDbvalue());
        map["targetConcentrationsMic_"  + QString::number(k)]   = QString::number(target->getMic()->getDbvalue());
        k++;
    }
    //End of Targets informations

    //----------------------------------------
    //  Adjustments informations
    //----------------------------------------
    auto adjustmentsFirstTake = interpretation->getRequest()->getTreatment()->getAdjustments()->firsttake();
    auto adjustments = interpretation->getRequest()->getTreatment()->getAdjustments()->dosage(adjustmentsFirstTake);
    auto adjustmentsRoute = adjustments->getRoute();

    map["adjustmentDateIsSet"]       = QVariant(interpretation->getAdjustmentDateIsSet()).toString();
    map["adjustmentWithLoadingDose"] = QVariant(interpretation->getAdjustmentWithLoadingDose()).toString();
    map["adjustmentWithRestPeriod"]  = QVariant(interpretation->getAdjustmentWithRestPeriod()).toString();
    map["adjustmentDate"]            = interpretation->getAdjustmentDate().toString(Qt::ISODate);

    map["adjustmentsDbInterval"]               = QString::number(adjustments->getDbinterval());
    map["adjustmentsDbTInf"]                   = QString::number(adjustments->getDbtinf());
    map["adjustmentsQuantity"]                 = QString::number(adjustments->getQuantity()->getDbvalue());
    map["adjustmentsApplied"]                  = adjustments->getApplied().toString(Qt::ISODate);
    map["adjustmentsEndTime"]                  = adjustments->getEndTime().toString(Qt::ISODate);
    map["adjustmentsTinf"]                     = QString::number(adjustments->getTinf().toMinutes());
    map["adjustmentsInterval"]                 = QString::number(adjustments->getInterval().toMinutes());

    map["adjustmentsAdministrationRoute"]  = adjustmentsRoute->getAdministrationRoute();
    map["adjustmentsAdministrationName"]   = adjustmentsRoute->getAdministrationName();
    map["adjustmentsFormulation"]          = adjustmentsRoute->getFormulationString();
    map["adjustmentsLabel"]                = adjustmentsRoute->getLabelString();

    //----------------------------------------
    //  Validation informations
    //----------------------------------------
    auto analysis = interpretation->getAnalysis();
    map["analysNextControl"]          = QVariant(analysis->getNextControl()).toString();
    map["analysDosageDescription"]    = analysis->getDosageDescription();
    map["analysExpectedness"]         = analysis->getExpectedness();
    map["analysSuitability"]          = analysis->getSuitability();
    map["analysPrediction"]           = analysis->getPrediction();
    map["analysRemonitoring"]         = analysis->getRemonitoring();
    map["analysWarning"]              = analysis->getWarning();
    //End of Validation informations

    //----------------------------------------
    //  Validation status
    //----------------------------------------
    for(auto step = StepType::first; step <= StepType::last; step ++) {
        QString status = interpretationXmlExport->toStringValidation(interpretation->getValidationStatus()->getValidationStatus(StepType::convert(step)));

        map["status " + interpretationXmlExport->stepToString(step)] = status;
    }
    //End of VValidation status

    return map;
}

void SpixGTest::fillInAnalystData(AnalystData analystData1)
{
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/applicationBarView/settingsAction"));
    srv->waitPeriod(waitTime1);

    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystTitle");
    item->setProperty("text", analystData1.analystTitle);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystFirstname");
    item->setProperty("text", analystData1.analystFirstName);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystLastname");
    item->setProperty("text", analystData1.analystLastName);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystRole");
    item->setProperty("text", analystData1.analystRole);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystPhone");
    item->setProperty("text", analystData1.analystPhoneNumber);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystAffiliation");
    item->setProperty("text", analystData1.analystAffiliation);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystAddress");
    item->setProperty("text", analystData1.analystAddress);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystCity");
    item->setProperty("text", analystData1.analystCity);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystPostcode");
    item->setProperty("text", analystData1.analystPostcode);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystState");
    item->setProperty("text", analystData1.analystState);
    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("analystCountry");
    item->setProperty("text", analystData1.analystCountry);

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("settingsDialog/applySettings"));
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("settingsDialog/okSettings"));
    srv->waitPeriod(waitTime1);
}



/*
TEST(GTestExample, AdvancedUITest)
{
    srv->wait(std::chrono::milliseconds(1000));
    bool ok;

    spix::ItemPath path("mainWindow");
    std::string root = path.rootComponent();
    const std::vector<std::string> children = path.components();
    //        const std::vector<std::string>& components() const;
    //        size_t length() const { return m_components.size(); };
    //        std::string rootComponent() const;

    ok = srv->existsAndVisible(spix::ItemPath("mainWindow"));
    ok = srv->existsAndVisible(spix::ItemPath("mainWindow/launchView"));
    ok = srv->existsAndVisible(spix::ItemPath("mainWindow/launchView/newPatient"));
    ok = srv->existsAndVisible(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));
    srv->mouseClick(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));

    spix::ItemPath path1("mainWindow/launchView/newPatient");
    std::string root1 = path.rootComponent();
    const std::vector<std::string> children1 = path.components();

    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugListItem_3"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->wait(std::chrono::milliseconds(1500));
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));
    srv->wait(std::chrono::milliseconds(1500));
    srv->takeScreenshot("mainWindow/flowView", "screen.png");

    srv->wait(std::chrono::milliseconds(1500));
    srv->takeScreenshot("mainWindow/flowView", "screen2.png");

    // We call this function to synchronize the thread before clicking
    srv->existsAndVisible(spix::ItemPath("fake"));
    QTest::keyClick(srv->m_window, Qt::Key_Right);
    srv->wait(std::chrono::milliseconds(1500));
    srv->takeScreenshot("mainWindow/flowView", "screen3.png");
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/selectAdjustment_0"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validateInterpretation"));
    srv->wait(std::chrono::milliseconds(500));

    srv->existsAndVisible(spix::ItemPath("fake"));
//        std::cout << " dialog: " << qPrintable(qApp->activeWindow()->windowTitle()) << std::endl;
    QTest::keyClick(qApp->activeWindow(), Qt::Key_Enter);


    //srv->mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
    srv->wait(std::chrono::milliseconds(20000));
    //srv->mouseClick(spix::ItemPath("mainWindow/flowView/printButton"));
    //srv->wait(std::chrono::milliseconds(2000));
    //srv->mouseClick(spix::ItemPath("mainWindow/applicationBarView/shutDownAction"));

    srv->existsAndVisible(spix::ItemPath("fake"));
    EXPECT_EQ(true, false);
}


TEST(GTestExample, BasicUITest)
{
    srv->wait(std::chrono::milliseconds(1000));
    bool ok;

    spix::ItemPath path("mainWindow");
    std::string root = path.rootComponent();
    const std::vector<std::string> children = path.components();
    //        const std::vector<std::string>& components() const;
    //        size_t length() const { return m_components.size(); };
    //        std::string rootComponent() const;

    ok = srv->existsAndVisible(spix::ItemPath("mainWindow"));
    ok = srv->existsAndVisible(spix::ItemPath("mainWindow/launchView"));
    ok = srv->existsAndVisible(spix::ItemPath("mainWindow/launchView/newPatient"));
    ok = srv->existsAndVisible(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));
    srv->mouseClick(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));

    spix::ItemPath path1("mainWindow/launchView/newPatient");
    std::string root1 = path.rootComponent();
    const std::vector<std::string> children1 = path.components();

    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugListItem_3"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->wait(std::chrono::milliseconds(1500));
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));
    srv->wait(std::chrono::milliseconds(1500));
    srv->takeScreenshot("mainWindow/flowView", "screen.png");

    srv->wait(std::chrono::milliseconds(1500));
    srv->takeScreenshot("mainWindow/flowView", "screen2.png");

    // We call this function to synchronize the thread before clicking
    srv->existsAndVisible(spix::ItemPath("fake"));
    QTest::keyClick(srv->m_window, Qt::Key_Right);
    srv->wait(std::chrono::milliseconds(1500));
    srv->takeScreenshot("mainWindow/flowView", "screen3.png");
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/selectAdjustment_0"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validateInterpretation"));
    srv->wait(std::chrono::milliseconds(500));

    srv->existsAndVisible(spix::ItemPath("fake"));
//        std::cout << " dialog: " << qPrintable(qApp->activeWindow()->windowTitle()) << std::endl;
    QTest::keyClick(qApp->activeWindow(), Qt::Key_Enter);


//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
    srv->wait(std::chrono::milliseconds(20000));
//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/printButton"));
//    srv->wait(std::chrono::milliseconds(2000));
//    srv->mouseClick(spix::ItemPath("mainWindow/applicationBarView/shutDownAction"));

    srv->existsAndVisible(spix::ItemPath("fake"));
    EXPECT_EQ(true, true);
}
*/

/*

class GuiTests : public spix::TestServer {
public:
    GuiTests(QQuickWindow *window) : m_window(window) {}

protected:

    QQuickWindow *m_window;

    void executeTest() override
    {
        wait(std::chrono::milliseconds(1000));
        bool ok;

        spix::ItemPath path("mainWindow");
        std::string root = path.rootComponent();
        const std::vector<std::string> children = path.components();
        //        const std::vector<std::string>& components() const;
        //        size_t length() const { return m_components.size(); };
        //        std::string rootComponent() const;

        ok = existsAndVisible(spix::ItemPath("mainWindow"));
        ok = existsAndVisible(spix::ItemPath("mainWindow/launchView"));
        ok = existsAndVisible(spix::ItemPath("mainWindow/launchView/newPatient"));
        ok = existsAndVisible(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));
        mouseClick(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));

        spix::ItemPath path1("mainWindow/launchView/newPatient");
        std::string root1 = path.rootComponent();
        const std::vector<std::string> children1 = path.components();

        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/drugListItem_3"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
        wait(std::chrono::milliseconds(1500));
        mouseClick(spix::ItemPath("dosageDialog/okDosage"));
        wait(std::chrono::milliseconds(1500));
        this->takeScreenshot("mainWindow/flowView", "screen.png");

        wait(std::chrono::milliseconds(1500));
        takeScreenshot("mainWindow/flowView", "screen2.png");

        // We call this function to synchronize the thread before clicking
        existsAndVisible(spix::ItemPath("fake"));
        QTest::keyClick(m_window, Qt::Key_Right);
        wait(std::chrono::milliseconds(1500));
        takeScreenshot("mainWindow/flowView", "screen3.png");
        mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/selectAdjustment_0"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/flowView/validateInterpretation"));
        wait(std::chrono::milliseconds(500));

        existsAndVisible(spix::ItemPath("fake"));
//        std::cout << " dialog: " << qPrintable(qApp->activeWindow()->windowTitle()) << std::endl;
        QTest::keyClick(qApp->activeWindow(), Qt::Key_Enter);


        mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
        wait(std::chrono::milliseconds(20000));
        mouseClick(spix::ItemPath("mainWindow/flowView/printButton"));
        wait(std::chrono::milliseconds(2000));
        mouseClick(spix::ItemPath("mainWindow/applicationBarView/shutDownAction"));

//        QTest::keyClick(qApp->activeModalWidget(), Qt::Key_A);
//        QTest::keyClick(qApp->activeModalWidget(), Qt::Key_B);
//        QTest::keyClick(qApp->activeModalWidget(), Qt::Key_C);
//        QTest::keyClick(qApp->activeModalWidget(), Qt::Key_Enter);

//        QTest::keyClick(qApp->activePopupWidget(), Qt::Key_A);
//        QTest::keyClick(qApp->activePopupWidget(), Qt::Key_B);
//        QTest::keyClick(qApp->activePopupWidget(), Qt::Key_C);
//        QTest::keyClick(qApp->activePopupWidget(), Qt::Key_Enter);

    }
};
*/
