import QtQuick 2.0
import QtQuick.Controls 2.0

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

ToolTip {
    property string tooltipText: ""
    visible: (show_tooltip) ? parent.hovered : false
    text: (show_tooltip) ? tooltipText : ""
    background: Rectangle {
        color: Style.tooltip.color
        border.color: Style.tooltip.border_color
    }
}
