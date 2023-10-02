import QtQuick
import QtQuick.Controls

import guiutils.qml.styles
import guiutils.qml.controls

ToolTip {
    property string tooltipText: ""
    visible: (show_tooltip) ? parent.hovered : false
    text: (show_tooltip) ? tooltipText : ""
    background: Rectangle {
        color: Style.tooltip.color
        border.color: Style.tooltip.border_color
    }
}
