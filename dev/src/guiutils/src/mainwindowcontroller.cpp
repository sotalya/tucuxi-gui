#include <iostream>
#include "mainwindowcontroller.h"

#include <QApplication>
#include <QQuickItem>
#include <QQmlContext>
#include <QFileDialog>

#ifdef CONFIG_GUITEST
#include <QQmlApplicationEngine>
#endif // CONFIG_GUITEST

#include "startupwindow.h"
#include "core/utils/connect.h"
#include "guiutils/src/controllers/interpretationcontroller.h"
#include "admin/src/dal/interpretationrequest.h"

#include "guiutils/src/requestscontroller.h"
#include "core/settings.h"
#include "apputils/src/chartdataexporter.h"

#ifdef CONFIG_CONNECTED
#include "rest/restrequestsclient.h"
#include "guiutils/src/demorequestfileclient.h"
#else
#include "demorequestsclient.h"
#include "guiutils/src/demorequestfileclient.h"
#endif

#include "guiutils/src/widgets/validatedrugdialog.h"
#include "admin/src/interpretationxmlimport.h"

#include "guiutils/src/unavailablefunctionality.h"

MainWindowController::MainWindowController(QObject *parent) :
    currentView(HOME),
    launchView(nullptr),
    requestsView(nullptr),
    requestsController(nullptr)
{
    Q_UNUSED(parent);
    _validateDrug = new ValidateDrugDialog();

    requestsController = new RequestsController(this);

#ifdef CONFIG_CONNECTED
    QString requestDefaultDir = QCoreApplication::applicationDirPath() + "/requests/list.xml";
    QString listFile = SETTINGS.get(ezechiel::core::Module::GUI, "listFile").toString();
    //if (listFile.isEmpty()) {
    //    listFile = requestDefaultDir;
    //}
    if (listFile.isEmpty())
        requestsController->setClient(new RestRequestsClient(this));
    else {
        DemoRequestFileClient *client = new DemoRequestFileClient(this);
        client->setListFile(listFile);
        requestsController->setClient(client);
    }
#else
//    requestsController->setClient(new DemoRequestsClient(this));
    QString listFile = ":/requests/List.xml";
    DemoRequestFileClient *client = new DemoRequestFileClient(this);
    client->setListFile(listFile);
    requestsController->setClient(client);

#endif

#ifdef CONFIG_GUITEST
    engine = new QQmlApplicationEngine();
    m_rootContext = engine->rootContext();

    // Unfruitful tentatives to retrieve the close signal
    // disconnect(engine, SIGNAL(quit()), qApp, SLOT(quit()));
    // CONNECT(engine, SIGNAL(quit()), this, SLOT(goToQuit()));
#endif // CONFIG_GUITEST

#ifdef CONFIG_DEMO
    getRootContext()->setContextProperty("show_tooltip", true);
    getRootContext()->setContextProperty("CONFIG_DEMO", true);
#else
    getRootContext()->setContextProperty("show_tooltip", false);
    getRootContext()->setContextProperty("CONFIG_DEMO", false);
#endif // CONFIG_DEMO


    // The constructor does a lot of initialization
    interpretationController = new InterpretationController(this);

    // Here we bind the models in order to avoir QML warnings during the QML loading
    interpretationController->bindModelsToRootContext(getRootContext());
    bindModelsToRootContext();

#ifdef CONFIG_GUITEST
    engine->load(QUrl(QStringLiteral("qrc:/windows/MainWindowGuiTest.qml")));
    if (engine->rootObjects().isEmpty())
        return;

    QObject *root = engine->rootObjects().at(0);
    m_window =qobject_cast<QQuickWindow*>(root);
    if (!m_window)
    {
        qFatal("Error: No window found!");
    }
    m_window->setFlags(Qt::Window
                           | Qt::WindowMinimizeButtonHint
                           | Qt::WindowMaximizeButtonHint);
    m_window->show();
#else
    // Then we load the QML
    setSource(QUrl("qrc:/windows/MainWindow.qml"));
#endif // CONFIG_GUITEST


    // And finally the last initializations of the controller, done after the
    // loading of all QML files
#ifdef CONFIG_GUITEST
    interpretationController->initAfterQmlLoading(getRootObject(), getRootContext());
#else
    interpretationController->initAfterQmlLoading(dynamic_cast<QQuickItem *>(getRootObject()), getRootContext());
#endif



    retrieveViewsFromQml();
    initViewConnections();


    getRootContext()->setContextProperty("mainWindowController", this);
    getRootContext()->setContextProperty("appPath", QApplication::applicationDirPath());


//    QString requestFile = SETTINGS.get(ezechiel::core::Module::GUI, "requestFile").toString();
//    loadRequestFile(requestFile);
}

MainWindowController::~MainWindowController()
{
    interpretationController->deleteLater();

}


QQmlContext* MainWindowController::getRootContext() {
#ifdef CONFIG_GUITEST
    return m_rootContext;
#else
    return rootContext();
#endif // CONFIG_TEST
}

QObject* MainWindowController::getRootObject()
{
#ifdef CONFIG_GUITEST
    return engine->rootObjects().at(0);
#else
    return rootObject();
#endif // CONFIG_GUITEST
}

void MainWindowController::bindModelsToRootContext()
{
    getRootContext()->setContextProperty("requestListModel",      requestsController->model());
    getRootContext()->setContextProperty("requestListController", requestsController);
}

void MainWindowController::retrieveViewsFromQml()
{
    mainView = getRootObject();
    Q_ASSERT(mainView);

    launchView = getRootObject()->findChild<QObject *>("launchView");
    Q_ASSERT(launchView);

    requestsView = getRootObject()->findChild<QObject *>("requestsView");
    Q_ASSERT(requestsView);

}

#include <QAction>

void MainWindowController::initViewConnections() {

    // To detect errors with Qml
    CONNECT(this, SIGNAL(statusChanged(QQuickView::Status)), this, SLOT(monitorStatus(QQuickView::Status)));

    CONNECT(launchView, SIGNAL(optionSelected(int)), this, SLOT(processStartupWindow(int)));

    //Rest specific
    CONNECT(requestsView,      SIGNAL(requestSelected(QString, QString, QString)),         requestsController, SLOT(queryRequest(QString, QString, QString)));
    CONNECT(requestsController,      SIGNAL(requestReady(InterpretationRequest*)),         this, SLOT(requestReady(InterpretationRequest*)));
}

void MainWindowController::requestReady(InterpretationRequest *request)
{

    interpretationController->startInterpretationRequest(request);
    currentView = FLOW;
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(mainView, "showFlow"));
}

#include "admin/src/dal/interpretation.h"

void MainWindowController::interpretationReady(Interpretation *interpretation)
{
    interpretationController->loadInterpretation(interpretation);
    currentView = FLOW;
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(mainView, "showFlow"));
}


void MainWindowController::monitorStatus(QQuickView::Status status)
{
    if (status == QQuickView::Error) {
        QString message;
        foreach(QQmlError error, errors()) {
            message += error.description() + "\n";
        }

        QMessageBox::critical(nullptr, tr("Error"), message);
    }
}

void MainWindowController::goToHome()
{
    if (currentView == FLOW)
        if (!interpretationController->acceptAndQuit())
            return;

    currentView = HOME;
    // Then go to home page
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(mainView, "showLaunch"));
}

void MainWindowController::goToPendingRequests()
{
    if (currentView == FLOW)
        if (!interpretationController->acceptAndQuit())
            return;

    currentView = PENDINGREQUESTS;
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(mainView, "showRequests"));

}

void MainWindowController::goToNewInterpretation()
{
    if (currentView == FLOW)
        if (!interpretationController->acceptAndQuit())
            return;

    currentView = FLOW;

    interpretationController->startNewPatient();
    CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(mainView, "showFlow"));
    //    resetReqState();
}

void MainWindowController::goToQuit()
{
    if (currentView == FLOW) {
        if (interpretationController->acceptAndQuit()) {
         //   delete interpretationController;
         //   interpretationController = 0;
            qApp->exit(EXIT_SUCCESS);
        }
    }
}

bool MainWindowController::event(QEvent *event)
{
    if (event->type() == QEvent::Close) {

        if (currentView == FLOW) {
            if (!interpretationController->acceptAndQuit()) {
                event->ignore();
                return true;
            }
            else {

           //     delete interpretationController;
             //   interpretationController = 0;
                qApp->exit(EXIT_SUCCESS);
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if (e) {
            if ((e->key() == Qt::Key_F2) && (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))) {
                _validateDrug->exec();
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if (e) {
            if ((e->key() == Qt::Key_T) && (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))) {
                bool showToolTip = getRootContext()->contextProperty("show_tooltip").toBool();
                getRootContext()->setContextProperty("show_tooltip", !showToolTip);
            }
        }
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if (e) {
            if ((e->key() == Qt::Key_O) && (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))) {
                goToLoadRequest();
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if (e) {
            if ((e->key() == Qt::Key_E) && (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))) {
                interpretationController->saveStatistics();
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if (e) {
            if ((e->key() == Qt::Key_A) && (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))) {
                interpretationController->showStatistics(EXPORT_POPULATION);
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if (e) {
            if ((e->key() == Qt::Key_S) && (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))) {
                interpretationController->showStatistics(EXPORT_APRIORI);
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if (e) {
            if ((e->key() == Qt::Key_D) && (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))) {
                interpretationController->showStatistics(EXPORT_APOSTERIORI);
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if (e) {
            if ((e->key() == Qt::Key_F) && (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))) {
                interpretationController->showStatistics(EXPORT_ADJUSTMENTS);
            }
        }
    }

    return QQuickView::event(event);
}


void MainWindowController::goToLoadInterpretation()
{
#ifndef CONFIG_DEMO
    QString dirPath;

    // Settings to keep track of the previous path
    dirPath = SETTINGS.get(ezechiel::core::Module::GUI,"interpretationDirPath", "").toString();

    QString fileName = QFileDialog::getOpenFileName(QApplication::activeWindow(), tr("Open Interpretation File"),
                                                    dirPath,
                                                    tr("Tucuxi Interpretation File (*.tui)"));
    if (fileName.isEmpty())
        return;

    dirPath = QFileInfo(fileName).absoluteDir().absolutePath();
    SETTINGS.set(ezechiel::core::Module::GUI,"interpretationDirPath", dirPath);

    loadInterpretationFile(fileName);
#else
    UnavailableFunctionality::displayMessage();
#endif // CONFIG_DEMO
}

void MainWindowController::loadInterpretationFile(const QString &fileName)
{
#ifndef CONFIG_DEMO
    if (!fileName.isEmpty()) {
        QFile f(fileName);
        if (!f.open(QFile::ReadOnly)) {
            std::cout << "Error";
        }
        QByteArray fileContent;
        fileContent = f.readAll();

        InterpretationXmlImport importer;
        Interpretation *interpretation = importer.load(fileContent);
        if (interpretation != nullptr) {
            emit interpretationReady(interpretation);
        }
        else {
            QMessageBox::warning(nullptr, "Error while loading file", "The interpretation file could not be loaded");
        }
    }
#else
    UnavailableFunctionality::displayMessage();
#endif // CONFIG_DEMO
}

void MainWindowController::goToLoadRequest()
{
    QString dirPath;

    // Settings to keep track of the previous path
    dirPath = SETTINGS.get(ezechiel::core::Module::GUI,"requestDirPath", "").toString();

    QString fileName = QFileDialog::getOpenFileName(QApplication::activeWindow(), tr("Open Request File"),
                                                    dirPath,
                                                    tr("XML Request File (*.xml)"));
    if (fileName.isEmpty())
        return;

    dirPath = QFileInfo(fileName).absoluteDir().absolutePath();
    SETTINGS.set(ezechiel::core::Module::GUI,"requestDirPath", dirPath);

    loadRequestFile(fileName);

}

void MainWindowController::loadRequestFile(const QString &fileName)
{
#ifndef CONFIG_DEMO
    if (!fileName.isEmpty()) {
        QFile f(fileName);
        if (!f.open(QFile::ReadOnly)) {
            std::cout << "Error";
        }
        QString fileContent;
        fileContent = f.readAll();

        SimpleBuilder requestBuilder;
        InterpretationRequest *ir = requestBuilder.buildRequest(fileContent);
        if (ir != nullptr) {
            emit requestReady(ir);
        }
        else {
            QMessageBox::warning(nullptr, "Error while loading file", QString("Error while loading file %1. See the logs for details.").arg(fileName));
        }
    }
#else
    UnavailableFunctionality::displayMessage();
#endif // CONFIG_DEMO
}

void MainWindowController::processStartupWindow(int choice) {
    switch (choice) {
    case StartupWindow::PendingRequests:
        goToPendingRequests();
        break;
    case StartupWindow::PendingRequestGivenPatient:
        goToPendingRequests();
        break;
    case StartupWindow::NewPatient:
        goToNewInterpretation();
        break;
    case StartupWindow::LoadRequest:
        goToLoadInterpretation();
        break;
    case StartupWindow::Shutdown:
        qApp->exit(EXIT_SUCCESS);
        break;

    }
}
