import QtQuick 2.5
import QtQuick.Controls.Private 1.0

// This file contains private Qt Quick modules that might change in future versions of Qt

MouseArea {
    id: root
    property string text: ""

    anchors.fill: parent
    hoverEnabled: root.enabled

    onExited: Tooltip.hideText()
    onCanceled: Tooltip.hideText()

    Timer {
        interval: 1000
        running: root.enabled && root.containsMouse && root.text.length
        onTriggered: Tooltip.showText(root, Qt.point(root.mouseX, root.mouseY), root.text)
    }
}
