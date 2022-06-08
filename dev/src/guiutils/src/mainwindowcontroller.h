//@@license@@

#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QQuickView>

class InterpretationController;
class RequestsController;
class InterpretationRequest;
class ValidateDrugDialog;
class Interpretation;

#ifdef CONFIG_GUITEST
class QQmlApplicationEngine;
#endif // CONFIG_GUITEST


class fromGuiTest: public QObject
{
    Q_OBJECT
public:
    void toMainWindowController(QString filename);
    void toIntController();
signals:
    void loadInterpretation(QString);
    void extSetViewRange();
};

class MainWindowController : public QQuickView
{
    Q_OBJECT

public:
    explicit MainWindowController(QObject *parent = nullptr);
    virtual ~MainWindowController() Q_DECL_OVERRIDE;

    bool event(QEvent *event) Q_DECL_OVERRIDE;

#ifdef CONFIG_GUITEST
    QQmlContext *m_rootContext;
    QQmlApplicationEngine *engine;

#ifdef CONFIG_GUITEST
    QQuickWindow *getWindow() { return m_window;};
    InterpretationController *getInterpretationController() { return interpretationController;}
#endif // CONFIG_GUITEST

#endif // CONFIG_GUITEST
    QQmlContext *getRootContext();

    QObject *getRootObject();
public slots:

    void monitorStatus(QQuickView::Status status);

    void processStartupWindow(int choice);

    void goToHome();
    void goToPendingRequests();
    void goToNewInterpretation();
    void goToQuit();

    ///
    /// \brief goToLoadRequest Starts the process of loading a request
    /// It opens a dialog to select the file, and then calls loadRequestFile()
    ///
    void goToLoadRequest();

    void goToLoadInterpretation();

    void requestReady(InterpretationRequest *request);

    void interpretationReady(Interpretation *interpretation);

    void loadInterpretationFile(const QString &fileName);
    void emptySlot();

//    void signal1();

protected:

#ifdef CONFIG_GUITEST
    QQuickWindow *m_window;
#endif // CONFIG_GUITEST

    enum {
        HOME,
        FLOW,
        PENDINGREQUESTS
    } currentView;

    InterpretationController *interpretationController;

    QObject *testObj;

    QObject *mainView;
    QObject *launchView;

    void bindModelsToRootContext();
    void retrieveViewsFromQml();
    void initViewConnections();

    ///
    /// \brief loadRequestFile Loads a request file, and starts the interpretation
    /// \param fileName Full path of the file to open
    /// This function tries to open the request file. If it succeeds, then it creates
    /// the interpretation and emits a signal that will start the GUI interpretation
    ///
    void loadRequestFile(const QString &fileName);


    ///
    /// \brief loadInterpretationFile Loads an interpretation file, and loads the interpretation
    /// \param fileName Full path of the file to open
    /// This function tries to open the interpretation file. If it succeeds, then it creates
    /// the interpretation and emits a signal that will start the GUI interpretation
    ///
//    void loadInterpretationFile(const QString &fileName);

    QObject *requestsView;
    RequestsController *requestsController;

    ValidateDrugDialog *_validateDrug;

};

#endif // MAINWINDOWCONTROLLER_H
