import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

Rectangle {
    property alias label: label
    Layout.preferredHeight: 25
    Layout.preferredWidth: 85
    border.width: 0
    radius: 1
    color: Style.table.background.header

    property string tooltipText : ""

    MouseArea {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
        ToolTip {
            background: Rectangle {
                color: Style.tooltip.color
                border.color: Style.tooltip.border_color
            }
            visible : (show_tooltip) ? (mousearea.containsMouse && (tooltipText != "")) : false
            text : tooltipText
        }
    }

    Text {
        id: label
        anchors.fill: parent
        elide: Text.ElideRight
        color: Style.table.foreground.header
        font.family: Style.table.font.header
        font.pixelSize: Style.table.size.header
        font.bold: false
        font.underline: false
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        leftPadding: 3.0
    }
}

