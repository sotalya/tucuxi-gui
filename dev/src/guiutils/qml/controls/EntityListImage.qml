import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {
    property alias image: image
    property alias mousearea: mousearea
    Layout.preferredWidth: 25
    Layout.preferredHeight: 25
    border.width: 0
    radius: 1
    //color: Style.table.background.header

    property string tooltipText : ""


    Image {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        antialiasing: true
        mipmap: true

        MouseArea {
            id: mousearea
            anchors.fill: parent

            ToolTip {
                background: Rectangle {
                    color: Style.tooltip.color
                    border.color: Style.tooltip.border_color
                }
                visible : (show_tooltip) ? (mousearea.containsMouse && (tooltipText != "")) : false
                text : tooltipText
            }
        }
    }
}
