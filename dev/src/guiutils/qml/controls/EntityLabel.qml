import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

Text {
    Layout.preferredWidth: 75
    elide: Text.ElideRight
    color: Style.form.foreground.label
    font.family: Style.form.font.label
    font.pixelSize: Style.form.size.label
    font.bold: false
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
    leftPadding: 3.0

    property string tooltipText : ""

    MouseArea {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
        ToolTip {
            visible : (show_tooltip) ? (mousearea.containsMouse && (tooltipText != "")) : false
            text : tooltipText

            background: Rectangle {
                color: Style.tooltip.color
                border.color: Style.tooltip.border_color
            }
        }
    }

}
