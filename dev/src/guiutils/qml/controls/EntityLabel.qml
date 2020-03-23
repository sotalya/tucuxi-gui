import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

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
