//@@license@@

#include <QQuickWindow>

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


extern SpixGTest* srv;

extern int waitTime1;
extern int waitTimeLong;


TEST(ReportTest, TestCefepime1)
{
    srv->startNewPatient();

    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Cefepime"));
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod(waitTimeLong);
/*
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(waitTimeLong);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(waitTimeLong);
    */

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));



    srv->waitPeriod(waitTimeLong);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
    srv->waitPeriod(waitTime1);
    srv->synchronize();
    srv->printReport("report_cefepime1.pdf");

    srv->synchronize();

    CHECKSPIXERRORS;
}

TEST(ReportTest, TestImatinib1)
{
    srv->startNewPatient();

    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Imatinib"));
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));

    srv->waitPeriod(waitTimeLong);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
    srv->waitPeriod(waitTime1);

    srv->printReport("report_imatinib1.pdf");

    srv->synchronize();

    CHECKSPIXERRORS;
}


TEST(ReportTest, TestImatinibMeasures)
{
    srv->startNewPatient();

    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Apixaban"));
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(waitTimeLong);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/measureButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/measuresView/addMeasure"));
    srv->waitPeriod(waitTime1);

//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);
//    srv->waitPeriod(waitTime1);
//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);

    srv->synchronize();
    auto idItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleIdField");
    idItem->setProperty("text", "Sample 1");
    auto valueItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueEntry");
    valueItem->setProperty("text", 100000.0);
    srv->waitPeriod(waitTimeLong);

    srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));
    srv->waitPeriod(waitTime1);


    srv->mouseClick(spix::ItemPath("mainWindow/measuresView/addMeasure"));
    srv->waitPeriod(waitTime1);

//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);
//    srv->waitPeriod(waitTime1);
//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);

    srv->synchronize();
    {
        auto idItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleIdField");
        idItem->setProperty("text", "Sample 1");
        auto valueItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueEntry");
        valueItem->setProperty("text", 100000.0);
    }
    srv->waitPeriod(waitTimeLong);

    srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod(waitTimeLong);
/*
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(waitTimeLong);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(waitTimeLong);
    */

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));

    srv->waitPeriod(waitTimeLong);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
    srv->waitPeriod(waitTime1);

    srv->printReport("report_imatinib_measures.pdf");

    srv->synchronize();

    CHECKSPIXERRORS;
}


void testReportDrugModel(QString drugName)
{
    srv->startNewPatient();

    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod(waitTime1);
    srv->selectDrugInList(drugName, 0);
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));


    // Now let's add a measure
    srv->waitPeriod(waitTimeLong);
    srv->synchronize();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/measureButton"));
    srv->waitPeriod(waitTimeLong);
    return;
    srv->mouseClick(spix::ItemPath("mainWindow/measuresView/addMeasure"));
    srv->waitPeriod(waitTime1);

//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);
//    srv->waitPeriod(waitTime1);
//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);

    srv->synchronize();
    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueInput");
    item->setProperty("value", 10000.0);
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));
    srv->waitPeriod(waitTime1);

    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod(waitTimeLong);
    /*
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(waitTime1);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod(waitTime1);
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(waitTime1);
    */
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));

    srv->waitPeriod(waitTimeLong);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod(waitTime1);
    srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
    srv->waitPeriod(waitTime1);

    QString reportName = QString("report_%1.pdf").arg(drugName.data());
    srv->printReport(reportName);

    srv->synchronize();

    CHECKSPIXERRORS;
}


TEST(ReportTest, TestRifampicin)
{
    testReportDrugModel("Rifampicin");
}

#include <QQmlApplicationEngine>

class GenericReportTest : public ::testing::TestWithParam<std::tuple<QString, int, std::string> > {
protected:
    void testReportDrugModel()
    {
        auto param = GetParam();
        auto drugName = std::get<2>(param);
        std::cout << "Starting drug " << drugName << std::endl;
        srv->startNewPatient();

        srv->waitPeriod(waitTime1);
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
        srv->waitPeriod(waitTime1);
//        QString selectDrug = QString("mainWindow/flowView/drugList_%1").arg(drugName.data());
//        srv->mouseClick(spix::ItemPath(selectDrug.toUtf8().constData()));
        srv->synchronize();

//        auto rootContext = srv->m_mainWindowController->getRootContext();
//        auto engine = srv->m_mainWindowController->engine;
//        QObject *root = engine->rootObjects().at(0);
//        std::cout << "Root : " << root->objectName().toStdString() << std::endl;
//        QObject *list = srv->getObjectByName(root, "drugListView");

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView,
                                  "setExtCurrentActiveSubstance",
                                  Q_ARG(QVariant, QVariant::fromValue(std::get<0>(param))));
        srv->synchronize();
        srv->waitPeriod(waitTime1);

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView,
                                  "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(std::get<1>(param))));
        srv->synchronize();
        srv->waitPeriod(waitTime1);

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
        srv->waitPeriod(waitTime1);
        srv->synchronize();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
        srv->waitPeriod(waitTime1);
        srv->synchronize();
        srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));
        srv->waitPeriod(waitTime1);

        QDateTime dateTime;
        dateTime.setDate(QDate(2021,07,27));
        dateTime.setTime(QTime(17, 0, 0));
//        srv->addMeasure(100.0, dateTime);
//        srv->waitPeriod(waitTime1);

//        srv->mouseClick(spix::ItemPath("mainWindow/flowView/measureButton"));
//        srv->mouseClick(spix::ItemPath("mainWindow/measuresView/addMeasure"));
//        srv->waitPeriod(waitTime1);

//    //    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);
//    //    srv->waitPeriod(waitTime1);
//    //    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);

//        srv->synchronize();
//        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueInput");
//        item->setProperty("value", 100000.0);
//        srv->waitPeriod(waitTime1);

//        srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));
//        srv->waitPeriod(waitTime1);


//        srv->waitPeriod(waitTime1);

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
        srv->waitPeriod(waitTime1);
        /*
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
        for(int i = 0; i < 10; i++) {
            srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
        }
        srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
        srv->waitPeriod(waitTime1);
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
        srv->waitPeriod(waitTime1);

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
        for(int i = 0; i < 10; i++) {
            srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
        }
        srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
        srv->waitPeriod(waitTime1);
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
        srv->waitPeriod(waitTime1);

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
        for(int i = 0; i < 10; i++) {
            srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
        }
        srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
        srv->waitPeriod(waitTime1);
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

        srv->waitPeriod(waitTime1);
        */

        srv->synchronize();
        int nbAdjs = srv->getNbProposedAdjustments();
        if (nbAdjs > 0) {
            srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));
        }
        else {
            srv->mouseClick(spix::ItemPath("mainWindow/flowView/addAdjustment"));
        }
        srv->waitForSync();

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
        srv->waitPeriod(waitTime1);
        srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
        srv->waitPeriod(waitTime1);

        srv->waitForSync();

        QString reportName = QString("report_%1.pdf").arg(std::get<2>(param).data());

        srv->printReport(reportName);

        srv->synchronize();

        CHECKSPIXERRORS;
    }

};

TEST_P(GenericReportTest, TestDrug)
{
    testReportDrugModel();
}

#ifdef  DRUGMODELTESTS
INSTANTIATE_TEST_SUITE_P(
    ReportFullTest,
    GenericReportTest,
    ::testing::Values(
        std::tuple<int, int, std::string>{0, 0, "Active moiety name"},
        std::tuple<int, int, std::string>{1, 0, "Active moiety name 2"},
        std::tuple<int, int, std::string>{0, 0, "Apixaban"},
        std::tuple<int, int, std::string>{1, 0, "Bictegravir"},
        std::tuple<int, int, std::string>{2, 0, "Busulfan"},
        std::tuple<int, int, std::string>{3, 0, "Caffeine"},
        std::tuple<int, int, std::string>{4, 0, "Cefepime"},
        std::tuple<int, int, std::string>{5, 0, "Daptomycin"},
        std::tuple<int, int, std::string>{6, 0, "Darunavir"},
        std::tuple<int, int, std::string>{7, 0, "Dolutegravir"},
        std::tuple<int, int, std::string>{8, 0, "Doravirine"},
        std::tuple<int, int, std::string>{9, 0, "Gentamicin"},
        std::tuple<int, int, std::string>{10, 0, "Hydroxychloroquine"},
        std::tuple<int, int, std::string>{11, 0, "Imatinib"},
        std::tuple<int, int, std::string>{12, 0, "Lopinavir"},
        std::tuple<int, int, std::string>{13, 0, "Meropenem"},
        std::tuple<int, int, std::string>{14, 0, "Piperacillin"},
        std::tuple<int, int, std::string>{15, 0, "Ponatinib"},
        std::tuple<int, int, std::string>{16, 0, "Rifampicin"},
        std::tuple<int, int, std::string>{17, 0, "Tacrolimus"},
        std::tuple<int, int, std::string>{18, 0, "Teicoplanin"},
        //std::tuple<int, int, std::string>{19, 0, "Test 2 analytes Erlang"},
        std::tuple<int, int, std::string>{19, 0, "Tobramycin"},
        std::tuple<int, int, std::string>{20, 0, "Vancomycin0"},
        std::tuple<int, int, std::string>{20, 1, "Vancomycin1"},
        std::tuple<int, int, std::string>{20, 2, "Vancomycin2"},
        std::tuple<int, int, std::string>{20, 3, "Vancomycin3"},
        std::tuple<int, int, std::string>{20, 4, "Vancomycin4"},
        std::tuple<int, int, std::string>{20, 5, "Vancomycin5"},
        std::tuple<int, int, std::string>{20, 6, "Vancomycin6"}//,
        //std::tuple<int, int, std::string>{21, 0, "test MM 1comp"},
        //std::tuple<int, int, std::string>{22, 0, "test MM 2comp macro"},
        //std::tuple<int, int, std::string>{23, 0, "test MM 2comp micro"},
        //std::tuple<int, int, std::string>{24, 0, "test2analytes"}
        ));
#else // DRUGMODELTESTS
INSTANTIATE_TEST_SUITE_P(
    ReportFullTest,
    GenericReportTest,
    ::testing::Values(
        //Struct of the tuple -> Drugs, Model, TestName
        std::tuple<QString, int, std::string>{"Apixaban",           0, "Apixaban"},
        std::tuple<QString, int, std::string>{"Bictegravir",        0, "Bictegravir"},
        std::tuple<QString, int, std::string>{"Busulfan",           0, "Busulfan"},
        std::tuple<QString, int, std::string>{"Caffeine",           0, "Caffeine"},
        std::tuple<QString, int, std::string>{"Cefepime",           0, "Cefepime"},
        std::tuple<QString, int, std::string>{"Daptomycin",         0, "Daptomycin"},
        std::tuple<QString, int, std::string>{"Darunavir",          0, "Darunavir"},
        std::tuple<QString, int, std::string>{"Dolutegravir",       0, "Dolutegravir"},
        std::tuple<QString, int, std::string>{"Doravirine",         0, "Doravirine"},
        std::tuple<QString, int, std::string>{"Gentamicin",         0, "Gentamicin"},
        std::tuple<QString, int, std::string>{"Hydroxychloroquine", 0, "Hydroxychloroquine"},
        std::tuple<QString, int, std::string>{"Imatinib",           0, "Imatinib"},
        std::tuple<QString, int, std::string>{"Lopinavir",          0, "Lopinavir"},
        std::tuple<QString, int, std::string>{"Meropenem",          0, "Meropenem"},
        std::tuple<QString, int, std::string>{"Piperacillin",       0, "Piperacillin"},
        std::tuple<QString, int, std::string>{"Ponatinib",          0, "Ponatinib"},
        std::tuple<QString, int, std::string>{"Rifampicin",         0, "Rifampicin"},
        std::tuple<QString, int, std::string>{"Tacrolimus",         0, "Tacrolimus"},
        std::tuple<QString, int, std::string>{"Teicoplanin",        0, "Teicoplanin"},
        std::tuple<QString, int, std::string>{"Tobramycin",         0, "Tobramycin"},
        std::tuple<QString, int, std::string>{"Vancomycin",         0, "Vancomycin0"},
        std::tuple<QString, int, std::string>{"Vancomycin",         1, "Vancomycin1"},
        std::tuple<QString, int, std::string>{"Vancomycin",         2, "Vancomycin2"},
        std::tuple<QString, int, std::string>{"Vancomycin",         3, "Vancomycin3"},
        std::tuple<QString, int, std::string>{"Vancomycin",         4, "Vancomycin4"},
        std::tuple<QString, int, std::string>{"Vancomycin",         5, "Vancomycin5"},
        std::tuple<QString, int, std::string>{"Vancomycin",         6, "Vancomycin6"}//,
        ));
#endif // DRUGMODELTESTS
