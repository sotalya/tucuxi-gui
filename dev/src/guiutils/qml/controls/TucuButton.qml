import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0


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
