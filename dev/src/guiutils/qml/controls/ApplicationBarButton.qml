import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import guiutils.qml.styles

Rectangle {
    width:  64
    height: 64
    color:        Style.toolbar.background.button
    border.color: Style.toolbar.border.color
    border.width: Style.toolbar.border.width

    property var icon
    property var icon_hover
    property var icon_disable

    signal pressed()


    property string tooltipText : ""
    ToolTip.visible: (show_tooltip) ? (btnMouseArea.containsMouse && (tooltipText != "")) : false
    ToolTip.text: tooltipText

    Image {
        id: forground
        anchors.fill: parent
        anchors.margins: 4
        source: icon
        antialiasing: true
        mipmap: true
        fillMode: Image.PreserveAspectFit
    }

    MouseArea {
        id : btnMouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: parent.pressed()

        onEntered: {
            parent.color = Style.toolbar.background.button_hover
            forground.source = icon_hover;
        }

        onExited: {
            parent.color = Style.toolbar.background.button
            forground.source = icon;
        }
    }
}
