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

#include <QMessageBox>


class SpixGTest;
SpixGTest* srv;


int waitTime1 = 1;

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

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

void SpixGTest::findObjectAndSetValue(QString objectName, int propertyInput)
{
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

    // invoke method to expand Patient and Physician tabs
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->patientsView, "extPatientData",
                              Q_ARG(QVariant, QVariant::fromValue(yearPat)),
                              Q_ARG(QVariant, QVariant::fromValue(monthPat)),
                              Q_ARG(QVariant, QVariant::fromValue(dayPat)));

    srv->waitPeriod(waitTime1);

    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("patientFirstName");
    item->setProperty("text", patientData1.firstName);

    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("patientLastName");
    item->setProperty("text", patientData1.lastName);

    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("patientIdentifier");
    item->setProperty("text", patientData1.identifier);

    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("patientStayNumber");
    item->setProperty("text", patientData1.stayNumber);


    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("titlePhy");
    item->setProperty("text", patientData1.titlePhy);

    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("firstNamePhy");
    item->setProperty("text", patientData1.firstNamePhy);

    item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("lastNamePhy");
    item->setProperty("text", patientData1.lastNamePhy);

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
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    findObjectAndSetValue("doseSpinBox", dosageData1.dosage);

    // fills in Interval value, in [h / 100]
    findObjectAndSetValue("intervalSpinBox", dosageData1.interval);

    //srv->synchronize();
    auto routeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("routeText");
    auto routeText = routeItem->property("text").toString();
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

    //srv->synchronize();
    srv->mouseClick(spix::ItemPath("dosageDialog/LastDoseOrFromDateInput/wholeDate"));
    srv->waitPeriod(waitTime1);

    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("dosageDialog/LastDoseOrFromDateInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("dosageDialog/LastDoseOrFromDateInput/wholeDate"), dateDos.toStdString());
    srv->waitPeriod(waitTime1);
    //    qInfo() << "TIME :" << timeDos;

    auto timeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("LastDoseOrFromTimeInput");
    timeItem->setProperty("text", timeDos);

    srv->waitPeriod(waitTime1);

    // Runs ok

    if (steadyStateValue == false)
    {
//        qInfo() << "At steady state? NO";

        //srv->synchronize();
        // double click on second date entry (for loop necessary) needs second click to get out of first date entry box

        srv->mouseClick(spix::ItemPath("dosageDialog/stoppedDateInput/wholeDate"));
        srv->mouseClick(spix::ItemPath("dosageDialog/stoppedDateInput/wholeDate"));
        srv->waitPeriod(waitTime1);

        for(int i = 0; i < 10; i++) {
            srv->enterKey(spix::ItemPath("dosageDialog/stoppedDateInput/wholeDate"), spix::KeyCodes::Backspace, 0);
            //srv->waitPeriod(waitTime1);
        }
        dateDos = dosageData1.dateTimeDos2.toString("dd.MM.yyyy");
        timeDos = dosageData1.dateTimeDos2.toString("HH:mm:ss");

        srv->inputText(spix::ItemPath("dosageDialog/stoppedDateInput/wholeDate"), dateDos.toStdString());
        srv->waitPeriod(waitTime1);

        auto timeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("stoppedTimeInput");
        timeItem->setProperty("text", timeDos);

        srv->waitPeriod(waitTime1);

        // Runs ok
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
    srv->waitPeriod(waitTime1);

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

    if (covariateType == 0)                             // if covariateType == Total Body Weight
    {
        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("valueDoubleControl");
        //    qInfo() << item;
        item->setProperty("value", covariatesData1.weight);

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
    }

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
    auto idItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleIdField");
    idItem->setProperty("text", measureData1.name);
    srv->waitPeriod(waitTime1);

    auto valueItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueInput");
    valueItem->setProperty("value", measureData1.value * 100.0);
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

    //    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->adjustmentsView, "extLoadingDose",
    //                              Q_ARG(QVariant, QVariant::fromValue(true)));

    //    item->setProperty("checked", true);
    //    srv->waitPeriod(waitTime1);

    //    auto restPeriodItem = srv->m_mainWindowController->getInterpretationController()->findChild<QObject*>("withRestPeriod");
    //    qInfo() << restPeriodItem;
    //    restPeriodItem->setProperty("checked", true);

    //    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/withLoadingDoseMousearea"));
    //    srv->waitPeriod(waitTime1*4);
    //    srv->synchronize();

    // can't pass arg type QDate to QML function, string type also allows to segregate years, months, ...
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->adjustmentsView, "extAdjDate",
                              Q_ARG(QVariant, QVariant::fromValue(yearAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(monthAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(dayAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(hourAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(minuteAdj)));

    srv->waitPeriod(waitTime1);
    srv->synchronize();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addAdjustment"));
//    srv->waitPeriod(waitTime1);
//    srv->synchronize();

//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/editAdjustments_0"));
//    srv->waitPeriod(waitTime1*10);
//    srv->synchronize();

//    srv->waitForSync();

//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/selectAdjustment_3"));    // Runs ok
    srv->waitPeriod(waitTime1*10);
    srv->synchronize();

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
