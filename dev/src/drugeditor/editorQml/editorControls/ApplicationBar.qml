import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {
    id: root
    width:  1280
    height: 64

    Item {
        id: impl
        anchors.fill: parent

        //Toolbar background
        Rectangle {
            id: background
            anchors.fill: parent
            color: Style.toolbar.background.toolbar
        }

        //Toolbar content
        RowLayout {
            anchors.fill: parent
            spacing: 0

            Item {
                id: logoFrame
                Layout.fillWidth:  true
                Layout.fillHeight: true

                Image {
                    anchors.top:    parent.top
                    anchors.bottom: parent.bottom
                    anchors.left:   parent.left
                    anchors.topMargin:    10
                    anchors.bottomMargin: 10
                    anchors.leftMargin:   10
                    source: "qrc:/icons/logo.png"
                    antialiasing: true
                    mipmap: true
                    fillMode: Image.PreserveAspectFit
                }
            }
//            ApplicationBarButton {
//                id: publishReport
//                Layout.preferredWidth:  root.height
//                Layout.preferredHeight: root.height
//                icon:       "qrc:/icons/toolbar/file.png"
//                icon_hover: "qrc:/icons/toolbar/file_hover.png"
//                onPressed: interpretationController.publishReport()
//            }



            ApplicationBarButton {
                id: undoAction
                Layout.preferredWidth:  root.height
                Layout.preferredHeight: root.height
                icon:       "qrc:/icons/toolbar/file.png"
                icon_hover: "qrc:/icons/toolbar/file_hover.png"
                Text {
                    anchors.fill: parent
                    id: undoTxt
                    text: qsTr("undo")
                    color: "white"
                }
                onPressed: {
                    undoRedoController.undo()
                }
            }

            ApplicationBarButton {
                id: redoAction
                Layout.preferredWidth:  root.height
                Layout.preferredHeight: root.height
                icon:       "qrc:/icons/toolbar/file.png"
                icon_hover: "qrc:/icons/toolbar/file_hover.png"
                Text {
                    anchors.fill: parent
                    id: redoTxt
                    text: qsTr("redo")
                    color: "white"
                }
                onPressed: {
                    undoRedoController.redo()
                }
            }

            ApplicationBarButton {
                id: newAction
                Layout.preferredWidth:  root.height
                Layout.preferredHeight: root.height
                icon:       "qrc:/icons/toolbar/folder_patient_add.png"
                icon_hover: "qrc:/icons/toolbar/folder_patient_add_hover.png"
                onPressed: {
//                    interpretationController.newInterpretation()
//                    interpretationController.resetReqState()
                }
            }

            ApplicationBarButton {
                id: reqAction
                Layout.preferredWidth:  root.height
                Layout.preferredHeight: root.height
                icon:       "qrc:/icons/toolbar/requests.png"
                icon_hover: "qrc:/icons/toolbar/requests_hover.png"
//                onPressed: nothing...
            }

            ApplicationBarButton {
                id: shutdownAction
                Layout.preferredWidth:  root.height
                Layout.preferredHeight: root.height
                icon:       "qrc:/icons/toolbar/shutdown.png"
                icon_hover: "qrc:/icons/toolbar/shutdown_hover.png"
                onPressed: undoRedoController.close()
            }
        }
    }
}
