import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

import guiutils.qml.controls 1.0

// Changed to Window instead of Item for Gui tests
Window {
    id: mainWindow
    objectName: "mainWindow"
    width: 1366                         // instead of 1366x768 (see "MainWindow.qml"), JRT 21.20.2022
    height: 768
    //visibility: Window.FullScreen;    // Not quite working, JRT 21.02.2022

    readonly property int minimumWidth:  960
    readonly property int minimumHeight: 540

    readonly property int contentPreferredWidth:  width
    readonly property int contentPreferredHeight: height - applicationBar.height

    readonly property int contentMinimumWidth:  minimumWidth
    readonly property int contentMinimumHeight: minimumHeight - applicationBar.height


    Component.onCompleted: {
        resetReqState();
    }

    function resetReqState () {
      //  state = "Flow"
    }

    function showRequests() {
    //    state = "Request";
        stack.currentIndex = 1;
        stack2.currentIndex = 1;
    }

    function showFlow() {
  //      state = "Flow";
        stack.currentIndex = 1;
        stack2.currentIndex = 0;
    }

    function showLaunch() {
        stack.currentIndex = 0;
        stack.currentIndex = 0;
    }
/*
    states: [
        State {
            name: "Flow"
            PropertyChanges {target : stack2; currentIndex: 0}
        },
        State {
            name: "Requests"
            PropertyChanges {target : stack2; currentIndex: 1}
        }
    ]
*/
    StackLayout {
        id : stack
        anchors.fill: parent
        Layout.fillHeight: true
        Layout.fillWidth: true
        currentIndex: 0

        StartupScreen {
            id : launch
            objectName : "launchView"
            Layout.fillHeight: true
            Layout.fillWidth: true
            visible : true
        }


        ColumnLayout {
            id : standard
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 0
            visible : false

            ApplicationBar {
                id: applicationBar
                objectName: "applicationBarView"
                Layout.fillWidth: true
            }

            StackLayout {

                id : stack2

                width : parent.width

                Flow {
                    id: flow
                    objectName: "flowView"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                RequestList {
                    id: requests
                    objectName: "requestsView"
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    onVisibleChanged: if (visible) requestListController.queryList()
//                    onRequestSelected: flowButton.press()
                }
            }
        }

    }
}
