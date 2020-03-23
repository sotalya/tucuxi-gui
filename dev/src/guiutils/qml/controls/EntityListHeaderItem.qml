import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

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

