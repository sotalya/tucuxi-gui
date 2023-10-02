import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import guiutils.qml.styles
import guiutils.qml.controls


Button {

    property string tooltipText: ""

    ToolTip {
        visible: (show_tooltip) ? parent.hovered : false
        text: (show_tooltip) ? tooltipText : ""
        background: Rectangle {
            color: Style.tooltip.color
            border.color: Style.tooltip.border_color
        }
    }

}
