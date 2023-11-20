import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

Rectangle {
    property alias image: image
    property alias mousearea: mousearea
    Layout.preferredWidth: 25
    Layout.preferredHeight: 25
    border.width: 0
    radius: 1
    color: Style.table.background.header

    property string tooltipText : ""

    Image {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        antialiasing: true
        mipmap: true


        ToolTip {
            background: Rectangle {
                color: Style.tooltip.color
                border.color: Style.tooltip.border_color
            }
            visible: (show_tooltip) ? (mousearea.containsMouse && tooltipText != "") : false
            text: tooltipText
        }

        MouseArea {
            id: mousearea
            anchors.fill: parent
            hoverEnabled: true
        }
    }

}
