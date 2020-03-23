import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2

import guiutils.qml.windows 1.0

Item {
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

//        RowLayout {
//            spacing: 0

//            Rectangle {
//                id: requestsButton
//                Layout.preferredHeight: 50
//                Layout.fillWidth: true

//                function press() {
//                    requests.visible = true;
//                    flow.visible = false;
//                }

//                gradient: Gradient {
//                    GradientStop { position: 0.0; color: "#044ebb" }
//                    GradientStop { position: 1.0; color: "#00b6ff" }
//                }

//                Text {
//                    anchors.fill: parent
//                    anchors.margins: 10

//                    horizontalAlignment: Text.AlignHCenter
//                    verticalAlignment:   Text.AlignVCenter

//                    text: "Requests List"
//                    color: "white"

//                    font.bold: true
//                    font.pixelSize: 30
//                    font.overline:  true
//                    font.underline: true
//                }

//                MouseArea {
//                    id: requestsButtonMouseArea
//                    anchors.fill: parent
//                    onClicked: parent.press()
//                }
//            }

//            Rectangle {
//                id: flowButton
//                Layout.preferredHeight: 50
//                Layout.fillWidth: true

//                function press() {
//                    requests.visible = false;
//                    flow.visible = true;
//                }

//                gradient: Gradient {
//                    GradientStop { position: 0.0; color: "#044ebb" }
//                    GradientStop { position: 1.0; color: "#00b6ff" }
//                }

//                Text {
//                    anchors.fill: parent
//                    anchors.margins: 10

//                    horizontalAlignment: Text.AlignHCenter
//                    verticalAlignment:   Text.AlignVCenter

//                    text: "Request Interpretation"
//                    color: "white"

//                    font.bold: true
//                    font.pixelSize: 30
//                    font.overline:  true
//                    font.underline: true
//                }

//                MouseArea {
//                    id: flowButtonMouseArea
//                    anchors.fill: parent
//                    onClicked: parent.press()
//                }
//            }
//        }

        Flow {
            id: flow
            objectName: "flowView"

            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: true
        }

        RequestList {
            id: requests
            objectName: "requestsView"

            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: false

            onVisibleChanged: if (visible) requestListControler.queryList()
            onRequestSelected: flowButton.press()
        }
    }
}
