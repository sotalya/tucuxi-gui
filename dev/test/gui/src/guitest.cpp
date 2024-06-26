//@@license@@

#include <QQuickWindow>

#include <QTest>
#include <QQmlApplicationEngine>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
#include <Spix/Events/Identifiers.h>

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

using namespace Tucuxi::Gui::Admin;

class SpixGTest;
SpixGTest* srv;


int waitTime1 = 1;
int waitTimeLong = 10;

QMap<int, QString> flowViewMap = {std::pair{patientTab,"Patient"}, {drugsTab,"Drugs"}, {dosagesTab,"Dosages"},
                                 {covariatesTab,"Covariates"}, {measuresTab,"Measures"}, {targetsTab,"Targets"},
                                 {adjustmentsTab,"Adjustments"}, {validationTab,"Validation"}, {reportsTab,"Reports"}};

SpixGTest::SpixGTest(Tucuxi::Gui::GuiUtils::MainWindowController *mainWindowController, QQuickWindow *window, int argc, char* argv[])
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

    if (tabIndex != 0 && tabIndex != 1)
    {
        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("chartOverlayView");

        srv->waitPeriod(waitTime1);
        srv->synchronize();

        QMetaObject::invokeMethod(item,
                                  "getWaitStatus",
                                  Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QVariant, waitStatus));
        isRunning = waitStatus.toBool();

        while(isRunning == true){
            std::cout << "Sync : Is still running ..." << std::endl;
            srv->waitPeriod(waitTimeLong*2);
            srv->synchronize();
            QMetaObject::invokeMethod(item,
                                      "getWaitStatus",
                                      Qt::SingleShotConnection,
                                      Q_RETURN_ARG(QVariant, waitStatus));
            isRunning = waitStatus.toBool();
        }
    }
    //else std::cout << "Tab index 0 or 1";
    std::cout << "Sync : OK" << std::endl;
}

int SpixGTest::getCurrentTabIndex()
{
    QVariant currentTabIndex = QVariant(666);

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->flowView,
                              "getCurrentIndex",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, currentTabIndex));

    std::cout << "Current tab : " << flowViewMap[currentTabIndex.toInt()].toStdString() << std::endl;

    srv->waitPeriod(waitTime1);

    return currentTabIndex.toInt();
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

    Tucuxi::Gui::GuiUtils::fromGuiTest obj;
    if (QObject::connect(&obj, SIGNAL(loadInterpretation(QString)), srv->m_mainWindowController, SLOT(loadInterpretationFile(QString))) != 0) {
        std::cout << "Interpretation load : Ok" << std::endl;
    }
    obj.toMainWindowController(loadName);
    srv->waitPeriod(waitTime1);
}

void SpixGTest::extSetView(QDateTime startView, QDateTime endView)
{
    Tucuxi::Gui::GuiUtils::fromGuiTest obj;
    if (QObject::connect(&obj, SIGNAL(extSignalSetView(QDateTime, QDateTime)), srv->m_mainWindowController, SLOT(extSlotSetView(QDateTime, QDateTime)/*emptySlot()*/)) != 0) {
//        std::cout << "setViewRange : Ok" << std::endl;
    }else std::cout << "setViewRange : Nok" << std::endl;
    obj.toIntController(startView, endView);

}

void SpixGTest::mouseClickIfPathOk(std::string clickPath)
{
    if (srv->existsAndVisible(spix::ItemPath(clickPath)))
    {
        std::cout << "Path exists : " << clickPath << std::endl;

        srv->mouseClick(spix::ItemPath(clickPath));
        srv->synchronize();
    }
    else std::cout << "Path does not exists : " << clickPath << std::endl;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

void SpixGTest::findObjectAndSetValue(QString objectName, int propertyInput)
{
    // retrieves object by name and puts it into an item to set or get its properties (value)
    srv->synchronize();
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>(objectName),
                              "setRealValue",
                              Q_ARG(QVariant, QVariant::fromValue(propertyInput)));

    srv->waitPeriod(waitTime1);
}

void SpixGTest::findDateObjectAndSetValue(QString dateObjectName, QString timeObjectName, QDateTime date){
    srv->synchronize();
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>(dateObjectName),
                              "setDate",
                              Q_ARG(QVariant, QVariant::fromValue(date)));
    srv->waitPeriod(waitTime1);
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>(timeObjectName),
                              "setDate",
                              Q_ARG(QVariant, QVariant::fromValue(date)));
    srv->waitPeriod(waitTime1);
}

void SpixGTest::findEntityTextFieldAndSetValue(QString objectName, QString textString){
    srv->synchronize();
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>(objectName),
                              "setEntityText",
                              Q_ARG(QVariant, QVariant::fromValue(textString)));
    srv->waitPeriod(waitTime1);
}

void SpixGTest::findEntityTextValueFieldAndSetValue(QString objectName, double value){
    srv->synchronize();
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>(objectName),
                              "setEntityValueText",
                              Q_ARG(QVariant, QVariant::fromValue(value)));
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

void SpixGTest::selectDrugInList(QString drugName, int modelIndex)
{

    auto drugItemName = "drugList_" + drugName;
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod(waitTime1);

    srv->synchronize();
    auto drugListView = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("drugListView");
    QVariant returnValue;
    QMetaObject::invokeMethod(drugListView,
                              "getItemsCount",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, returnValue));
    auto totalItems = returnValue.toInt();

    // Parse the drug list until the wanted one is found
    // First check outside while to speed up search
    QVariant currentItem;
    srv->mouseClick("mainWindow/flowView/" + drugItemName.toStdString());
    srv->waitPeriod(waitTime1);
    QMetaObject::invokeMethod(drugListView,
                              "getCurrentItemName",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, currentItem));
    int listViewIndex = 5;
    while(currentItem.toString() != drugItemName && !(listViewIndex > totalItems)){
        QMetaObject::invokeMethod(drugListView,
                                  "setPositionAtIndex",
                                  Q_ARG(QVariant, QVariant::fromValue(listViewIndex)));
        srv->waitPeriod(waitTime1);
        srv->mouseClick("mainWindow/flowView/" + drugItemName.toStdString());
        srv->waitPeriod(waitTime1);
        QMetaObject::invokeMethod(drugListView,
                                  "getCurrentItemName",
                                  Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QVariant, currentItem));
        listViewIndex += 5;
    }

    if (listViewIndex > totalItems && currentItem.toString() != drugItemName) GTEST_FATAL_FAILURE_("Failed to find the drug or drug specified not present inside the list");

    srv->waitPeriod(waitTime1);

    // model = DOMAIN & STUDY
    srv->synchronize();
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView,
                              "setExtCurrentDrugModel",
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
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->patientsView,
                              "extPatientData",
                              Q_ARG(QVariant, QVariant::fromValue(yearPat)),
                              Q_ARG(QVariant, QVariant::fromValue(monthPat)),
                              Q_ARG(QVariant, QVariant::fromValue(dayPat)),

                              Q_ARG(QVariant, QVariant::fromValue(patientData1.firstName)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.lastName)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.identifier)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.stayNumber)));

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->patientsView,
                              "extPatientContactData",
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.address)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.city)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.state)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.postcode)),
                              Q_ARG(QVariant, QVariant::fromValue(patientData1.country)));

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->patientsView,
                              "extPhysicianData",
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

    // adds new Dose (will open Dosage dialog window)audi a3 2016
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
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>("atSteadyState"),
                              "setSteadyState",
                              Q_ARG(QVariant, QVariant::fromValue(value)));
    srv->waitPeriod(waitTime1);
}

QVariant SpixGTest::getSteadyStateDosage()
{
    // QVariant type can be tested as a bool type (in if loop for example)
    srv->synchronize();
    QVariant steadyStateDosage;
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>("atSteadyState"),
                              "getSteadyState",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, steadyStateDosage));
//    qInfo() << "SteadyStateDosage value : " << steadyStateDosage;
    srv->waitPeriod(waitTime1);
    return steadyStateDosage.toBool();
}

void SpixGTest::fillInDosageData(DosageData dosageData1)
{
    // fills in Dose, Interval, reads Route, fills in Infusion, ...

    QString dateDos = dosageData1.dateTimeDos1.date().toString("dd.MM.yyyy");
    QString timeDos = dosageData1.dateTimeDos1.time().toString("HH:mm:ss");

    // fills in Dose value, in [µg]
    srv->waitPeriod(waitTime1);
    findObjectAndSetValue("doseSpinBox", dosageData1.dosage);

    // fills in Interval value, in [h]
    findObjectAndSetValue("intervalSpinBox", dosageData1.interval);

    //srv->synchronize();
    QVariant routeText;
    QMetaObject::invokeMethod(srv->m_mainWindowController->getRootObject()->findChild<QObject*>("routeText"),
                              "getEntityText",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, routeText));
//    qInfo() << "Route : " << routeText;

    if (routeText.toString() == "Intravenous drip")
    {
        // Infusion parameter only to be filled if Route = Intravenous drip != Oral
//        qInfo() << "Infusion value to be filled in";
        findObjectAndSetValue("infusionSpinBox", dosageData1.infusion);

    }
    else if (routeText.toString() == "Oral")
    {
        // do nothing ..?
//        qInfo() << "No infusion to be done";
    }

    QVariant steadyStateValue = getSteadyStateDosage();
    srv->waitPeriod(waitTime1);

    findDateObjectAndSetValue("LastDoseOrFromDateInput", "LastDoseOrFromTimeInput", dosageData1.dateTimeDos1);

    if (steadyStateValue == false)
    {
//        qInfo() << "At steady state? NO";
        findDateObjectAndSetValue("stoppedDateInput", "stoppedTimeInput", dosageData1.dateTimeDos2);
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
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->covariatesView,
                              "setCovariateType",
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
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->covariatesView,
                              "setCovariateType",
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

    if (covariateType == 0) // if covariateType == Total Body Weight
    {
        findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.weight);
    }
    else if (covariateType == 2)
    {
        findEntityTextValueFieldAndSetValue("covarValueEntry", (covariatesData1.scc + QTime::currentTime().second()));
    }
    srv->waitPeriod(waitTime1);

    findDateObjectAndSetValue("dateInputCovar", "timeInputCovar", covariatesData1.dateTimeCovar);

    srv->synchronize();
    srv->mouseClick(spix::ItemPath("covariateDialog/okCovariate"));
    srv->waitPeriod(waitTime1);


    // Runs ok
}

void SpixGTest::addCovariatesByDrug(CovariatesData covariatesData1, int covariateType, QString drugName)
{
    srv->synchronize();
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/covariateButton"));

    srv->synchronize();

    // selects covariate (index) to edit {Sex, Weight, ...}
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->covariatesView,
                              "setCovariateType",
                              Q_ARG(QVariant, QVariant::fromValue(covariateType)));

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addCovariate"));
    srv->waitPeriod(waitTime1*4);

    fillInCovariatesDataByDrug(covariatesData1, covariateType, drugName);
}

void SpixGTest::fillInCovariatesDataByDrug(CovariatesData covariatesData1, int covariateType, QString drugName)
{
    srv->synchronize();

    if (drugName == "Cefepime")
    {
        if (covariateType == 0)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.weight);
        else if (covariateType == 3)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.scc);
    }

    else if (drugName == "Imanitib")
    {
        if (covariateType == 0)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.weight);
    }

    else if (drugName == "Vancomycin")
    {
        if (covariateType == 0)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.weight);
        else if (covariateType == 3)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.scc);
        else if (covariateType == 4)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.gestionalAge);
        else if (covariateType == 6)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.heamatologicalMalignacy);
    }

    else if(drugName == "Apixaban")
    {
        if (covariateType == 2)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.dayNightDosing);
        else if (covariateType == 3)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.weight);
        else if (covariateType == 4)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.asian);
        else if (covariateType == 5)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.nonValvularAtrialFibriliation);
        else if (covariateType == 6)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.acuteCoronarySyndrome);
        else if (covariateType == 7)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.strongModerateCytochromeInhibitors);
        else if (covariateType == 8)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.dose);
        else if (covariateType == 9)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.glomerularFiltrationRate);
    }

    else if(drugName == "Busulfan")
    {
        if (covariateType == 1)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.weight);
    }

    else if(drugName == "Dolutegravir")
    {
        if (covariateType == 2)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.atazanavirWithWithoutBooster);
        else if (covariateType == 3)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.darunavir);
        else if (covariateType == 4)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.rifampicin);
        else if (covariateType == 5)
            findEntityTextValueFieldAndSetValue("covarValueEntry", covariatesData1.currentSmoking);
    }
    srv->waitPeriod(waitTime1);

    findDateObjectAndSetValue("dateInputCovar", "timeInputCovar", covariatesData1.dateTimeCovar);

    srv->synchronize();
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

    findEntityTextFieldAndSetValue("sampleIdField", measureData1.name);

    findEntityTextValueFieldAndSetValue("measureValueEntry", measureData1.measure);

    findDateObjectAndSetValue("sampleDateInput", "sampleTimeInput", measureData1.dateTimeMeas);

    srv->synchronize();
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

    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->targetDialog,
                              "setActiveType",
                              Q_ARG(QVariant, QVariant::fromValue(targetData1.targetType)));
    srv->waitPeriod(waitTime1);

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

    srv->synchronize();
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
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->adjustmentsView,
                              "extAdjDate",
                              Q_ARG(QVariant, QVariant::fromValue(yearAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(monthAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(dayAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(hourAdj)),
                              Q_ARG(QVariant, QVariant::fromValue(minuteAdj)));

    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addAdjustment"));
    srv->waitPeriod(waitTime1);

//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/editAdjustments_0"));
//    srv->waitPeriod(waitTime1);

//    srv->mouseClick(spix::ItemPath("mainWindow/flowView/selectAdjustment_3"));    // Runs ok
    srv->waitPeriod(waitTime1);
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

bool SpixGTest::addGlobalSentencePalette(QString sentence, QString shortcutModifier, QString shortcutKey, QString section)
{
    auto sentenceShortcutDialogItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>(section + "SentenceShortcutDialog");

    srv->synchronize();
    std::string PaletteButton = "mainWindow/flowView/" + section.toStdString() + "PaletteButton";
    srv->mouseClick(spix::ItemPath(PaletteButton));
    srv->waitPeriod(waitTime1);

    srv->synchronize();
    std::string addSentenceButton = section.toStdString() + "Palette/addGlobalListButton";
    srv->mouseClick(spix::ItemPath(addSentenceButton));
    srv->waitPeriod(waitTime1);

    // Insert sentence
    srv->synchronize();

    QMetaObject::invokeMethod(sentenceShortcutDialogItem,
                              "setText",
                              Q_ARG(QVariant, QVariant::fromValue(sentence)));
    srv->waitPeriod(waitTime1);

    // Select shortcut modifiers
    if(shortcutModifier.contains("CTRL", Qt::CaseInsensitive)){
        srv->synchronize();
        QMetaObject::invokeMethod(sentenceShortcutDialogItem,
                                  "setModifier",
                                  Q_ARG(QVariant, QVariant("CTRL")));
        srv->waitPeriod(waitTime1);
    }
    if(shortcutModifier.contains("SHIFT", Qt::CaseInsensitive)){
        srv->synchronize();
        QMetaObject::invokeMethod(sentenceShortcutDialogItem,
                                  "setModifier",
                                  Q_ARG(QVariant, QVariant("SHIFT")));
        srv->waitPeriod(waitTime1);
    }
    if(shortcutModifier.contains("ALT", Qt::CaseInsensitive)){
        srv->synchronize();
        QMetaObject::invokeMethod(sentenceShortcutDialogItem,
                                  "setModifier",
                                  Q_ARG(QVariant, QVariant("ALT")));
        srv->waitPeriod(waitTime1);
    }

    // Input shortcut key
    srv->synchronize();
    // Insert sentence
    QMetaObject::invokeMethod(sentenceShortcutDialogItem,
                              "setKey",
                              Q_ARG(QVariant, QVariant::fromValue(shortcutKey)));
    srv->waitPeriod(waitTime1);

    // Get save button status
    srv->synchronize();
    QVariant saveBtnStatus;
    QMetaObject::invokeMethod(sentenceShortcutDialogItem,
                              "getSaveButtonStatus",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, saveBtnStatus));
    srv->waitPeriod(waitTime1);

    std::string closeButton = section.toStdString() + "Palette/validationCloseButton";
    if(saveBtnStatus.toBool()){
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(section.toStdString() + "SentenceShortcutDialog/sentencesSaveButton"));
        srv->waitPeriod(waitTime1);
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(closeButton));
        srv->waitPeriod(waitTime1);
        return true;
    }
    else{
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(section.toStdString() + "SentenceShortcutDialog/sentencesCancelButton"));
        srv->waitPeriod(waitTime1);
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(closeButton));
        srv->waitPeriod(waitTime1);
        return false;
    }
}

bool SpixGTest::addSpecificSentencePalette(QString sentence, QString shortcutModifier, QString shortcutKey, QString section)
{
    srv->synchronize();
    std::string PaletteButton = "mainWindow/flowView/" + section.toStdString() + "PaletteButton";
    srv->mouseClick(spix::ItemPath(PaletteButton));
    srv->waitPeriod(waitTime1);

    srv->synchronize();
    std::string addSentenceButton = section.toStdString() + "Palette/addSpecificListButton";
    srv->mouseClick(spix::ItemPath(addSentenceButton));
    srv->waitPeriod(waitTime1);

    srv->synchronize();
    std::string sentenceInput = section.toStdString() + "Palette/sentenceShortcutDialog/sentenceInput";
        // Insert sentence
    for(auto const &_char : sentence){
        srv->enterKey(spix::ItemPath(sentenceInput), QKeySequence(_char)[0], Qt::NoModifier);
        srv->waitPeriod(waitTime1);
    }
    srv->waitPeriod(waitTime1);

    // Select shortcut modifiers
    if(shortcutModifier.contains("CTRL", Qt::CaseInsensitive)){
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(section.toStdString() + "Palette/sentenceShortcutDialog/controlCheckBox"));
        srv->waitPeriod(waitTime1);
    }
    if(shortcutModifier.contains("SHIFT", Qt::CaseInsensitive)){
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(section.toStdString() + "Palette/sentenceShortcutDialog/shiftCheckBox"));
        srv->waitPeriod(waitTime1);
    }
    if(shortcutModifier.contains("ALT", Qt::CaseInsensitive)){
        srv->synchronize();
        srv->mouseClick(spix::ItemPath(section.toStdString() + "Palette/sentenceShortcutDialog/altCheckBox"));
        srv->waitPeriod(waitTime1);
    }

    // Input shortcut key
    srv->synchronize();
    srv->enterKey(spix::ItemPath(section.toStdString() + "Palette/sentenceShortcutDialog/keyShortcut"), QKeySequence(shortcutKey)[0], Qt::NoModifier);
    srv->waitPeriod(waitTime1);

    // Get save button status
    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sentenceShortcutDialog");
    QVariant saveBtnStatus;
    QMetaObject::invokeMethod(item,
                              "getSaveButtonStatus",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, saveBtnStatus));


    if(saveBtnStatus.toBool()){
        srv->mouseClick(spix::ItemPath(section.toStdString() + "Palette/sentenceShortcutDialog/sentencesSaveButton"));
        srv->synchronize();
        std::string closeButton = section.toStdString() + "Palette/validationCloseButton";
        srv->mouseClick(spix::ItemPath(closeButton));
        srv->waitPeriod(waitTime1);
        return true;
    }
    else{
        srv->mouseClick(spix::ItemPath(section.toStdString() + "Palette/sentenceShortcutDialog/sentencesCancelButton"));
        srv->synchronize();
        std::string closeButton = section.toStdString() + "Palette/validationCloseButton";
        srv->mouseClick(spix::ItemPath(closeButton));
        srv->waitPeriod(waitTime1);
        return false;
    }
}

void SpixGTest::removeAllSentences(){
    static const std::vector<QString> sectionObjectsNames = {
        "expectednessPalette",
        "suitabilityPalette",
        "predictionPalette",
        "remonitoringPalette",
        "warningPalette"
    };

    for(auto const &_sectionObjectName : sectionObjectsNames){
        srv->synchronize();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/" + _sectionObjectName.toStdString()));
        srv->waitPeriod(waitTime1);
        // Get global list element count
        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>(_sectionObjectName);
        QVariant globalSentenceCount;
        QMetaObject::invokeMethod(item,
                                  "getGlobalSentencesCount",
                                  Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QVariant, globalSentenceCount));
        QVariant specificSentenceCount;
        QMetaObject::invokeMethod(item,
                                  "getSpecificSentencesCount",
                                  Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QVariant, specificSentenceCount));

        // Remove global sentences
        for(int i=0; i<globalSentenceCount.toInt(); i++){
            srv->synchronize();
            std::string removeSentenceButton = _sectionObjectName.toStdString() + "/globalDeleteButton_0";
            srv->mouseClick(spix::ItemPath(removeSentenceButton));
            srv->waitPeriod(waitTime1);
        }

        // Remove specific sentences
        for(int i=0; i<specificSentenceCount.toInt(); i++){
            srv->synchronize();
            std::string removeSentenceButton = _sectionObjectName.toStdString() + "/specificDeleteButton_0";
            srv->mouseClick(spix::ItemPath(removeSentenceButton));
            srv->waitPeriod(waitTime1);
        }
    }
}

void SpixGTest::fillInValidationData(ValidationData validationData1)
{
    srv->synchronize();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod(waitTime1);

    findDateObjectAndSetValue("nextControlDate", "nextControlTime", validationData1.dateTimeVal);

    srv->synchronize();
    QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->validationView,
                              "extTextInputs",
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

        if (globalSpecificIndex == (0) || globalSpecificIndex == (2)) {
            while (srv->existsAndVisible(spix::ItemPath(deleteCommentPath + "globalDeleteButton_" + std::to_string(0)))) {
                srv->mouseClick(spix::ItemPath(deleteCommentPath + "globalDeleteButton_" + std::to_string(0)));
                srv->waitPeriod();
            }
        }
        if (globalSpecificIndex == (1) || globalSpecificIndex == (2)) {
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
    std::cout << "Printing report: " << reportFileName.toStdString() << std::endl;

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

    foreach(Tucuxi::Gui::Core::PatientVariate *variate, patientVariateList->getList()) {
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

    foreach(Tucuxi::Gui::Core::CoreMeasure *coreMeasure, coreMeasureList->getList()) {
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

    foreach(Tucuxi::Gui::Core::Target *target, targetList->getList()) {
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

    findEntityTextFieldAndSetValue("analystTitle", analystData1.analystTitle);
    findEntityTextFieldAndSetValue("analystFirstname", analystData1.analystFirstName);
    findEntityTextFieldAndSetValue("analystLastname", analystData1.analystLastName);
    findEntityTextFieldAndSetValue("analystRole", analystData1.analystRole);
    findEntityTextFieldAndSetValue("analystPhone", analystData1.analystPhoneNumber);
    findEntityTextFieldAndSetValue("analystAffiliation", analystData1.analystAffiliation);
    findEntityTextFieldAndSetValue("analystAddress", analystData1.analystAddress);
    findEntityTextFieldAndSetValue("analystCity", analystData1.analystCity);
    findEntityTextFieldAndSetValue("analystPostcode", analystData1.analystPostcode);
    findEntityTextFieldAndSetValue("analystState", analystData1.analystState);
    findEntityTextFieldAndSetValue("analystCountry", analystData1.analystCountry);

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
