import QtQuick 2.5
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.0

import ezechiel 1.0
import guiutils.qml.styles 1.0

Item
{
    property var title
    property var icon
    property var iconHover

    property string tooltipText : ""

    signal selected();

    width: Style.splash.buttons.width + Style.splash.buttons.margin
    height: Style.splash.buttons.height + Style.splash.buttons.margin


    ToolTip.visible: (show_tooltip) ? (btnMouseArea.containsMouse && (tooltipText != "")) : false
    ToolTip.text: tooltipText

    Image {
        id: btnBox
        anchors.centerIn: parent
        width: Style.splash.buttons.width
        height: Style.splash.buttons.height
        fillMode: Image.PreserveAspectFit
        source: btnMouseArea.containsMouse ? Style.splash.buttons.background.hover : Style.splash.buttons.background.standard
    }

    MouseArea {
        id: btnMouseArea
        objectName: "mouseArea"
        anchors.fill: btnBox
        hoverEnabled: true
        onClicked: parent.selected()
    }

    Image {
        id: btnImage
        anchors.horizontalCenter: btnBox.horizontalCenter
        anchors.top: btnBox.top
        anchors.topMargin: btnBox.height * 0.2
        width: btnBox.width / 2
        height: btnBox.height / 2 - 20
        source: btnMouseArea.containsMouse ? parent.iconHover : parent.icon
        fillMode: Image.PreserveAspectFit
        antialiasing: true
        mipmap: true
    }

    Text {
        anchors.horizontalCenter: btnImage.horizontalCenter
        anchors.top: btnImage.bottom
        text: parent.title
        elide: Text.ElideRight
        color: btnMouseArea.containsMouse ? Style.splash.buttons.font.color.hover : Style.splash.buttons.font.color.standard
        font.family:    Style.splash.buttons.font.family
        font.pixelSize: Style.splash.buttons.font.size
        font.bold: true
    }
}
