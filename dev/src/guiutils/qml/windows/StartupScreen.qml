import QtQuick 2.5
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3

import ezechiel 1.0
import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

Rectangle
{
    signal optionSelected(int option);

    Image {
        width : parent.width
        height: parent.height
        source: Style.splash.image
    }

    ColumnLayout
    {
        anchors.fill: parent

        // Logo
        Image {
            Layout.fillWidth: true
            Layout.maximumHeight: Style.splash.logo.height
            fillMode: Image.PreserveAspectFit
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            antialiasing: true
            mipmap: true
            source: Style.splash.logo.source
        }

        // Buttons
        RowLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.minimumHeight: Style.splash.buttons.height

            StartupButton {
                title: "New Patient"
                objectName: "newPatient"
                icon: Style.splash.buttons.icon.add
                iconHover: Style.splash.buttons.icon.add_hover
                onSelected: optionSelected(StartupWindow.NewPatient)
                tooltipText: ToolTips.startupScreen.newPatient
            }
            StartupButton {
                // title: "Patients"
                title: "Load interpretation"
                icon: Style.splash.buttons.icon.folder
                iconHover: Style.splash.buttons.icon.folder_hover
                onSelected: optionSelected(StartupWindow.LoadRequest)
                tooltipText: ToolTips.startupScreen.load
            }
            StartupButton {
                title: "Pending requests"
                icon: Style.splash.buttons.icon.requests
                iconHover: Style.splash.buttons.icon.requests_hover
                onSelected: optionSelected(StartupWindow.PendingRequestGivenPatient)
                // onSelected: mainWindowController.goToPendingRequests()
                tooltipText: ToolTips.startupScreen.pending
            }
            StartupButton {
                title: "Shutdown"
                icon: Style.splash.buttons.icon.shutdown
                iconHover: Style.splash.buttons.icon.shutdown_hover
                onSelected: optionSelected(StartupWindow.Shutdown)
                tooltipText: ToolTips.startupScreen.quit
            }
        }

        // Links
        RowLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.maximumHeight: 80
            visible: false

            Image {
                //anchors.top: parent.top
                //anchors.bottom: parent.bottom
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icons/splash/link.png"
            }
            Image {
                //anchors.top: parent.top
                //anchors.bottom: parent.bottom
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icons/splash/mail.png"
            }
            Image {
                //anchors.top: parent.top
                //anchors.bottom: parent.bottom
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icons/splash/facebook.png"
            }
        }

        // Copyright
        Text {
            Layout.fillWidth: true
            Layout.preferredHeight: Style.splash.copyright.height
            horizontalAlignment: Text.AlignHCenter
            text: "Copyright (c) Tucuxi SA - 2019"
            font.family: Style.splash.copyright.family
            fontSizeMode: Text.Fit
            minimumPixelSize: Style.splash.copyright.size
            font.pixelSize: Style.splash.copyright.size
            color: Style.splash.copyright.color
        }
    }
}
