#include <QQuickWindow>

#include <QTest>
#include <QQmlApplicationEngine>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/KeyCodes.h>

#include "guitest.h"
#include "guiutils/src/controllers/interpretationcontroller.h"
#include "guiutils/src/controllers/targettabcontroller.h"
#include "admin/src/dal/interpretation.h"



class SpixGTest;
SpixGTest* srv;


int waitTime1 = 2;

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
    item->setProperty("value", propertyInput);
    srv->waitPeriod(waitTime1);
}

void SpixGTest::removeFromList(std::string removeFrom, int removeIndex = 0)      // default index = 0, will remove any existing inputs
{
    // if removeIndex > 0, only chosen input will be removed
    // if removeIndex <= 0, all above inputs will also be removed
    // applies to {Dosage, Target, Measure}

    qInfo() << removeIndex;

    std::string removePath = "mainWindow/flowView/remove";

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
}

void SpixGTest::selectDrugInList(int drugIndex, int modelIndex)
{
    // concatenates path_ & drugName
//    QString drugPath = QString("mainWindow/flowView/drugList_%1").arg(drugName.data());
//    qInfo() << drugPath;

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod(waitTime1);

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentActiveSubstance",
                              Q_ARG(QVariant, QVariant::fromValue(drugIndex)));
    srv->synchronize();
    srv->waitPeriod();

    // model = DOMAIN & STUDY
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentDrugModel",
                              Q_ARG(QVariant, QVariant::fromValue(modelIndex)));
//    srv->synchronize();
//    srv->waitPeriod();


//    srv->mouseClick(spix::ItemPath(drugPath.toStdString()));    //QString to std string
//    srv->waitPeriod(waitTime1);

    // Runs ok
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
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/editDosage_1"));
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
    qInfo() << "SteadyStateDosage value : " << steadyStateDosage;
    srv->waitPeriod(waitTime1);
    return steadyStateDosage;
}

void SpixGTest::fillInDosageData(DosageData dosageData1)
{
    // fills in Dose, Interval, reads Route, fills in Infusion,

    QString dateDos = dosageData1.dateTimeDos1.date().toString("dd.MM.yyyy");
    QString timeDos = dosageData1.dateTimeDos1.time().toString("HH:mm:ss");

    // fills in Dose value, in [µg]
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    findObjectAndSetValue("doseSpinBox", dosageData1.dosage);
//    auto doseItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("doseSpinBox");
//    doseItem->setProperty("value", 750.0);
//    srv->waitPeriod(waitTime1);

    // fills in Interval value, in [h / 100]

    findObjectAndSetValue("intervalSpinBox", dosageData1.interval);
//    auto intervalItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("intervalSpinBox");
//    intervalItem->setProperty("value", 800);
//    srv->waitPeriod(waitTime1);

    //srv->synchronize();
    auto routeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("routeText");
    auto routeText = routeItem->property("text").toString();
    qInfo() << "Route : " << routeText;

    if (routeText == "Intravenous drip")
    {
        // Infusion parameter only to be filled if Route = Intravenous drip != Oral
        //srv->synchronize();
        qInfo() << "Infusion value to be filled in";

        findObjectAndSetValue("infusionSpinBox", dosageData1.infusion);
//        auto infusionItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("infusionSpinBox");
//        infusionItem->setProperty("value", 1000);                 // infusion value in [min / 100]
//        srv->waitPeriod(waitTime1);

    }
    else if (routeText == "Oral")
    {
        // do nothing ..?
        qInfo() << "No infusion to be done";
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
    qInfo() << "TIME :" << timeDos;

    auto timeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("LastDoseOrFromTimeInput");
    timeItem->setProperty("text", timeDos);

    srv->waitPeriod(waitTime1);

    // Runs ok

    if (steadyStateValue == false)
    {
        qInfo() << "At steady state? NO";

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
    else qInfo() << "At steady state? YES";

    // closes Dosage dialog window when editing is done
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));
    srv->waitPeriod(waitTime1);

    // Runs ok
}


void SpixGTest::addCovariates(CovariatesData covariatesData1)
{
    srv->synchronize();
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/covariateButton"));

    srv->synchronize();

    // selects covariate (index) to edit {Sex, Weight, ...}

    for (int i = 0; i < 4; i++) {
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->covariatesView, "setCovariateType",
                              Q_ARG(QVariant, QVariant::fromValue(0)));
    srv->synchronize();
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/covariatesView/addCovariate"));      // will open Covariate dialog
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("covariateDialog/okCovariate"));                 // need to close dialog before editing it
    srv->synchronize();
    srv->waitPeriod(waitTime1);

    }

    //---------------------------------------------------------------------------------------------------------------------------------

    int lastEntryIndex = 10;

    while ((existsAndVisible(spix::ItemPath("mainWindow/covariatesView/editCovariate_" + std::to_string(lastEntryIndex)))) == false) {
//        srv->waitPeriod(waitTime1);
        lastEntryIndex--;
    }

    srv->mouseClick(spix::ItemPath("mainWindow/covariatesView/editCovariate_" + std::to_string(lastEntryIndex)));
    srv->waitPeriod(waitTime1);
    srv->synchronize();

    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("valueDoubleControl");
    qInfo() << item;
    item->setProperty("value", 1234);
    srv->synchronize();
    srv->waitPeriod(waitTime1);


    srv->mouseClick(spix::ItemPath("covariateDialog/okCovariate"));

//    qInfo() << lastEntryIndex;
//    editCovariates(covariatesData1, lastEntryIndex);

    // Runs ok !

    //     sets value of the previously chosen covariate
    //    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->covariateDialog, "setCovariateValue",
    //                              Q_ARG(QVariant, QVariant::fromValue(75)));

    srv->waitPeriod(waitTime1);
}

void SpixGTest::editCovariates(CovariatesData covariatesData1, int editIndex)
{
    std::string editPath = "mainWindow/flowView/editCovariate_" + std::to_string(editIndex);

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/CovariateButton"));
    srv->waitPeriod(waitTime1);


    if (srv->existsAndVisible(spix::ItemPath(editPath)))
    {
        srv->mouseClick(spix::ItemPath(editPath));
        srv->synchronize();
        srv->waitPeriod(waitTime1);

        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("valueDoubleControl");
        qInfo() << item;
        item->setProperty("value", 1234);
        srv->synchronize();
        srv->waitPeriod(waitTime1);

//        fillInCovariatesData(covariatesData1);

        srv->mouseClick(spix::ItemPath("covariateDialog/okCovariate"));
    }
}

void SpixGTest::fillInCovariatesData(CovariatesData covariatesData1)        // not used at this point
{
    srv->synchronize();


    srv->mouseClick(spix::ItemPath("covariatesDialog/okCovariates"));
}

void SpixGTest::addMeasure(double value, QDateTime dateTime)
{    
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/measureButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/measuresView/addMeasure"));
    srv->waitPeriod(waitTime1);

    fillInMeasureData(value, dateTime);
}

void SpixGTest::editMeasure(double value, QDateTime dateTime, int editIndex)
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
        fillInMeasureData(value, dateTime);
    }

}

void SpixGTest::fillInMeasureData(double value, QDateTime dateTime)
{
    srv->synchronize();
    auto idItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleIdField");
    idItem->setProperty("text", "Sample_1234_b");
    srv->waitPeriod(waitTime1);

    auto valueItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueInput");
    valueItem->setProperty("value", value * 100.0);
    srv->waitPeriod(waitTime1);

    auto dateItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleDateInput");
    dateItem->setProperty("date", dateTime.date());
    auto timeItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleTimeInput");
    timeItem->setProperty("time", dateTime.time());

    srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));

    srv->waitPeriod(waitTime1);


}



void SpixGTest::addTarget(TargetData targetData1)
{
    srv->synchronize();
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

    findObjectAndSetValue("cMinInput",  targetData1.cMinInput);
    findObjectAndSetValue("cBestInput", targetData1.cBestInput);
    findObjectAndSetValue("cMaxInput",  targetData1.cMaxInput);

    if (targetData1.targetType == 1)        // target type 1 = Peak
    {
        findObjectAndSetValue("tMinInput",  targetData1.tMinInput);
        findObjectAndSetValue("tBestInput", targetData1.tBestInput);
        findObjectAndSetValue("tMaxInput",  targetData1.tMaxInput);
    }

    if (targetData1.targetType > 5)         // target types over 5 include MIC
    {
         findObjectAndSetValue("micInput",   targetData1.micInput);
    }

    srv->waitPeriod(waitTime1);

     srv->mouseClick(spix::ItemPath("targetDialog/okTarget"));

    srv->waitPeriod(waitTime1);

    qInfo() << "Target type : " << targetData1.targetType;
}


void SpixGTest::setAdjustments(AdjustmentsData adjustmentsData1)
{
    QString dateAdj = adjustmentsData1.dateTimeAdj.date().toString("dd.MM.yyyy");
    QString timeAdj = adjustmentsData1.dateTimeAdj.time().toString("HH:mm:ss");

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));        // Runs ok
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), dateAdj.toStdString());
    srv->waitPeriod(waitTime1);


    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), timeAdj.toStdString());
    srv->waitPeriod(waitTime1);


//    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->adjustmentsView, "setLoadingDose",
//                              Q_ARG(QVariant, QVariant::fromValue(true)));


//    auto restPeriodItem = srv->m_mainWindowController->getInterpretationController()->findChild<QObject*>("atSteadyState");
//    qInfo() << restPeriodItem;
//    restPeriodItem->setProperty("checked", true);

//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/with0LoadingDose"));


    if (true) {

//        qInfo() << restPeriodItem;
    }
    else qInfo() << "Rest period : NO";

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addAdjustment"));         // Runs ok
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/selectAdjustment_3"));    // Runs ok
    srv->waitPeriod(waitTime1);

    srv->waitPeriod(waitTime1*10);


}



//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

void SpixGTest::printReport(QString reportFileName)
{
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
    srv->waitPeriod();

    srv->m_mainWindowController->getInterpretationController()->setReportFileName(reportFileName);
    srv->mouseClick(spix::ItemPath("mainWindow/reportView/printButton"));

    srv->waitPeriod(10);
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
