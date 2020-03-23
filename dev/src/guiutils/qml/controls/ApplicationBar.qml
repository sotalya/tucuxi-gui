import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

Rectangle {
    width:  1280
    height: 45
    color: Style.toolbar.background.toolbar

    //Toolbar content
    RowLayout {
        anchors.fill: parent
        spacing: 0

        Text {
            Layout.preferredWidth: 100
            horizontalAlignment: Qt.AlignHCenter
            Layout.fillHeight: true
            text: "Tucuxi"
            font.pixelSize: 32
            color: "white"
        }

        Item {
            width: 10
        }

        Image {
            Layout.fillHeight: true
            Layout.preferredWidth: 40
            Layout.alignment: Qt.AlignVCenter
            source: "qrc:/icons/logo.png"
            antialiasing: true
            mipmap: true
            fillMode: Image.PreserveAspectFit
        }

        Item {
            Layout.fillWidth:  true
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
            id: saveReqAction
            Layout.preferredWidth:  parent.height
            Layout.preferredHeight: parent.height
            icon:       "qrc:/icons/toolbar/save.png"
            icon_hover: "qrc:/icons/toolbar/save_hover.png"
            onPressed: interpretationController.saveInterpretation()
            Shortcut {
                sequence: "Ctrl+S"
                onActivated: interpretationController.saveInterpretation()
            }
            tooltipText: ToolTips.applicationBar.save
        }

        ApplicationBarButton {
            id: newAction
            Layout.preferredWidth:  parent.height
            Layout.preferredHeight: parent.height
            icon:       "qrc:/icons/toolbar/folder_patient_add.png"
            icon_hover: "qrc:/icons/toolbar/folder_patient_add_hover.png"
            onPressed: {
                mainWindowController.goToHome()
            }

            tooltipText: ToolTips.applicationBar.home

            //        hoverEnabled : true
            //        ToolTip.delay: 1000
            //        ToolTip.timeout: 5000
            //        ToolTip.visible: hovered
            //        ToolTip.text: qsTr("Go back to home screen")
        }

        ApplicationBarButton {
            id: reqAction
            Layout.preferredWidth:  parent.height
            Layout.preferredHeight: parent.height
            icon:       "qrc:/icons/toolbar/requests.png"
            icon_hover: "qrc:/icons/toolbar/requests_hover.png"
            onPressed: mainWindowController.goToPendingRequests()

            tooltipText: ToolTips.applicationBar.pending
        }

        ApplicationBarButton {
            id: shutdownAction
            objectName: "shutDownAction"
            Layout.preferredWidth:  parent.height
            Layout.preferredHeight: parent.height
            icon:       "qrc:/icons/toolbar/shutdown.png"
            icon_hover: "qrc:/icons/toolbar/shutdown_hover.png"
            onPressed: mainWindowController.goToQuit()

            tooltipText: ToolTips.applicationBar.quit
        }
    }
}
