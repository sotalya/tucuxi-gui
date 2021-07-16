#include <QQuickWindow>

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


extern SpixGTest* srv;



TEST(ReportTest, TestCefepime1)
{
    srv->startNewPatient();

    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Cefepime"));
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod();
/*
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(5);
    */
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));



    srv->waitPeriod(10);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
    srv->waitPeriod();
    srv->m_mainWindowController->getInterpretationController()->setReportFileName("report_cefepime1.pdf");
    srv->mouseClick(spix::ItemPath("mainWindow/reportView/printButton"));

    srv->waitPeriod(10);

    srv->synchronize();

    CHECKSPIXERRORS;
}

TEST(ReportTest, TestImatinib1)
{
    srv->startNewPatient();

    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Imatinib"));
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod();
/*
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(5);
    */
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));



    srv->waitPeriod(10);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
    srv->waitPeriod();
    srv->m_mainWindowController->getInterpretationController()->setReportFileName("report_imatinib1.pdf");
    srv->mouseClick(spix::ItemPath("mainWindow/reportView/printButton"));

    srv->waitPeriod(10);

    srv->synchronize();

    CHECKSPIXERRORS;
}


TEST(ReportTest, TestImatinibMeasures)
{
    srv->startNewPatient();

    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugList_Imatinib"));
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/measureButton"));
    srv->mouseClick(spix::ItemPath("mainWindow/measuresView/addMeasure"));
    srv->waitPeriod();

//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);
//    srv->waitPeriod();
//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);

    srv->synchronize();
    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueInput");
    item->setProperty("value", 100000.0);
    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));
    srv->waitPeriod();


    srv->mouseClick(spix::ItemPath("mainWindow/measuresView/addMeasure"));
    srv->waitPeriod();

//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);
//    srv->waitPeriod();
//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);

    srv->synchronize();
    {
        auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueInput");
        item->setProperty("value", 10000.0);
    }
    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod();
/*
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(5);
    */
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));



    srv->waitPeriod(10);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
    srv->waitPeriod();
    srv->m_mainWindowController->getInterpretationController()->setReportFileName("report_imatinib_measures.pdf");
    srv->mouseClick(spix::ItemPath("mainWindow/reportView/printButton"));

    srv->waitPeriod(10);

    srv->synchronize();

    CHECKSPIXERRORS;
}


void testReportDrugModel(std::string drugName)
{
    srv->startNewPatient();

    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
    srv->waitPeriod();
    QString selectDrug = QString("mainWindow/flowView/drugList_%1").arg(drugName.data());
    srv->mouseClick(spix::ItemPath(selectDrug.toUtf8().constData()));
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
    srv->waitPeriod();
    /*
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
    srv->waitPeriod();

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
    for(int i = 0; i < 10; i++) {
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
    }
    srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
    srv->waitPeriod();
    srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

    srv->waitPeriod(5);
    */
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));



    srv->waitPeriod(10);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
    srv->waitPeriod();


    QString reportName = QString("report_%1.pdf").arg(drugName.data());

    srv->m_mainWindowController->getInterpretationController()->setReportFileName(reportName);
    srv->mouseClick(spix::ItemPath("mainWindow/reportView/printButton"));

    srv->waitPeriod(10);

    srv->synchronize();

    CHECKSPIXERRORS;
}


TEST(ReportTest, TestRifampicin)
{
    testReportDrugModel("Rifampicin");
}


QObject *getObjectByName(QObject *root, std::string name)
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

#include <QQmlApplicationEngine>

class GenericReportTest : public ::testing::TestWithParam<std::tuple<int, int, std::string> > {
protected:
    void testReportDrugModel()
    {
        auto param = GetParam();
        srv->startNewPatient();

        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/drugButton"));
        srv->waitPeriod();
//        QString selectDrug = QString("mainWindow/flowView/drugList_%1").arg(drugName.data());
//        srv->mouseClick(spix::ItemPath(selectDrug.toUtf8().constData()));
        srv->synchronize();

//        auto rootContext = srv->m_mainWindowController->getRootContext();
//        auto engine = srv->m_mainWindowController->engine;
//        QObject *root = engine->rootObjects().at(0);
//        std::cout << "Root : " << root->objectName().toStdString() << std::endl;
//        QObject *list = getObjectByName(root, "drugListView");

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentActiveSubstance",
                                  Q_ARG(QVariant, QVariant::fromValue(std::get<0>(param))));
        srv->synchronize();
        srv->waitPeriod();

        QMetaObject::invokeMethod(srv->m_mainWindowController->getInterpretationController()->drugsView, "setExtCurrentDrugModel",
                                  Q_ARG(QVariant, QVariant::fromValue(std::get<1>(param))));
        srv->synchronize();
        srv->waitPeriod();

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/dosageButton"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/addDosage"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("dosageDialog/okDosage"));

        srv->waitPeriod(5);

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentButton"));
        srv->waitPeriod();
        /*
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
        for(int i = 0; i < 10; i++) {
            srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
        }
        srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "01/10/2020");
        srv->waitPeriod();
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
        srv->waitPeriod(5);

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"));
        for(int i = 0; i < 10; i++) {
            srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Backspace, 0);
        }
        srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), "02/10/2020");
        srv->waitPeriod();
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atDateSettingInput/wholeDate"), spix::KeyCodes::Enter, 0); // Enter
        srv->waitPeriod();

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"));
        for(int i = 0; i < 10; i++) {
            srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Backspace, 0);
        }
        srv->inputText(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), "09:09");
        srv->waitPeriod();
        srv->enterKey(spix::ItemPath("mainWindow/flowView/adjustmentsView/atMinutesSettingInput"), spix::KeyCodes::Enter, 0); // Enter

        srv->waitPeriod(5);
        */
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/adjustmentsView/selectAdjustment_0"));



        srv->waitPeriod(10);

        srv->mouseClick(spix::ItemPath("mainWindow/flowView/validationButton"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/validationView/validateInterpretation"));
        srv->waitPeriod();
        srv->mouseClick(spix::ItemPath("mainWindow/flowView/reportButton"));
        srv->waitPeriod();


        QString reportName = QString("report_%1.pdf").arg(std::get<2>(param).data());

        srv->m_mainWindowController->getInterpretationController()->setReportFileName(reportName);
        srv->mouseClick(spix::ItemPath("mainWindow/reportView/printButton"));

        srv->waitPeriod(10);

        srv->synchronize();

        CHECKSPIXERRORS;
    }

};

TEST_P(GenericReportTest, TestDrug)
{
    testReportDrugModel();
}

INSTANTIATE_TEST_SUITE_P(
        ReportFullTest,
        GenericReportTest,
        ::testing::Values(
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
            std::tuple<int, int, std::string>{12, 0, "Meropenem"},
            std::tuple<int, int, std::string>{13, 0, "Piperacillin"},
            std::tuple<int, int, std::string>{14, 0, "Rifampicin"},
            std::tuple<int, int, std::string>{15, 0, "Tacrolimus"},
            std::tuple<int, int, std::string>{17, 0, "Tobramycin0"},
            std::tuple<int, int, std::string>{18, 0, "Vancomycin1"},
            std::tuple<int, int, std::string>{18, 1, "Vancomycin2"},
            std::tuple<int, int, std::string>{18, 2, "Vancomycin3"},
            std::tuple<int, int, std::string>{18, 3, "Vancomycin4"},
            std::tuple<int, int, std::string>{18, 4, "Vancomycin5"},
            std::tuple<int, int, std::string>{18, 5, "Vancomycin6"},
            std::tuple<int, int, std::string>{18, 6, "Vancomycin7"}
            ));

