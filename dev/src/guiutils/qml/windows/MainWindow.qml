import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import QtQml

import guiutils.qml.controls

// Changed to Window instead of Item for Gui tests
Item {
    id: mainWindow
    objectName: "mainWindow"
    width:  1366
    height: 768

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

//                RequestList {
//                    id: requests
//                    objectName: "requestsView"
//                    Layout.fillWidth: true
//                    Layout.fillHeight: true

//                    onVisibleChanged: if (visible) requestListController.queryList()
////                    onRequestSelected: flowButton.press()
//                }
            }
        }

    }
}
