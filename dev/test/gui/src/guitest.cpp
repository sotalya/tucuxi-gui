#include <QQuickWindow>

#include <QTest>
#include <QQmlApplicationEngine>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>

#include "guitest.h"
#include "guiutils/src/controllers/interpretationcontroller.h"


class SpixGTest;
SpixGTest* srv;

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
        srv->waitPeriod(2);
    }
    mouseClick(spix::ItemPath("mainWindow/launchView/newPatient/mouseArea"));
    srv->waitPeriod();
}

void SpixGTest::addMeasure(double value, QDateTime date)
{
    srv->waitPeriod(5);

    srv->mouseClick(spix::ItemPath("mainWindow/flowView/measureButton"));
    srv->mouseClick(spix::ItemPath("mainWindow/measuresView/addMeasure"));
    srv->waitPeriod();

//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);
//    srv->waitPeriod();
//    srv->enterKey(spix::ItemPath("measureDialog/valueInput"), spix::KeyCodes::Num_1, 0);

    srv->synchronize();
    auto item = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("measureValueInput");
    item->setProperty("value", value * 100.0);
    srv->waitPeriod(5);

    auto dateItem = srv->m_mainWindowController->getRootObject()->findChild<QObject*>("sampleTimeInput");
    dateItem->setProperty("date", date);

    srv->mouseClick(spix::ItemPath("measureDialog/okMeasure"));
    srv->waitPeriod();


    srv->waitPeriod(5);

}

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
