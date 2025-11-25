/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QQuickView>
#include <QDateTime>

namespace Tucuxi {
namespace Gui {
namespace Admin {

class InterpretationRequest;
class Interpretation;

}
}
}

#ifdef CONFIG_GUITEST
class QQmlApplicationEngine;
#endif // CONFIG_GUITEST

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class InterpretationController;
class RequestsController;
class ValidateDrugDialog;
class PartialRequestListModel;

class fromGuiTest: public QObject
{
    Q_OBJECT
public:
    void toMainWindowController(QString filename);
    void toIntController(QDateTime startView, QDateTime endView);
signals:
    void loadInterpretation(QString);
    void extSignalSetView(QDateTime, QDateTime);
};

class MainWindowController : public QQuickView
{
    Q_OBJECT

public:
    explicit MainWindowController(QObject *parent = nullptr);
    ~MainWindowController() Q_DECL_OVERRIDE;

    bool event(QEvent *event) Q_DECL_OVERRIDE;

#ifdef CONFIG_GUITEST
    QQmlContext *m_rootContext;
    QQmlApplicationEngine *engine;

#ifdef CONFIG_GUITEST
    QQuickWindow *getWindow() { return m_window;};
    InterpretationController *getInterpretationController() { return interpretationController;}
    RequestsController *getRequestController() { return requestsController;}
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

    void requestReady(Tucuxi::Gui::Admin::InterpretationRequest *request);

    void interpretationReady(Tucuxi::Gui::Admin::Interpretation *interpretation);

    ///
    /// \brief loadInterpretationFile Loads an interpretation file, and loads the interpretation
    /// \param fileName Full path of the file to open
    /// This function tries to open the interpretation file. If it succeeds, then it creates
    /// the interpretation and emits a signal that will start the GUI interpretation
    ///
    void loadInterpretationFile(const QString &fileName);
    void extSlotSetView(const QDateTime &startView, const QDateTime &endView);//emptySlot();


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

    QObject *requestsView;
    RequestsController *requestsController;

    ValidateDrugDialog *_validateDrug;

};

}
}
}

#endif // MAINWINDOWCONTROLLER_H
