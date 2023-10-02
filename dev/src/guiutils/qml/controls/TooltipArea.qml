import QtQuick
//import QtQuick.Controls.Private

// Taken from : https://gist.github.com/webmaster128/d32a278d4037a72dd47a
// Thanks to : https://www.kullo.net/blog/tooltiparea-the-missing-tooltip-component-of-qt-quick/

// TooltipArea.qml
// This file contains private Qt Quick modules that might change in future versions of Qt
// Tested on: Qt 5.4.1 - Qt 5.6.1

MouseArea {
    property string text: ""

    id: root
    anchors.fill: parent
    acceptedButtons: Qt.NoButton
    hoverEnabled: root.enabled

    propagateComposedEvents: true

//   onClicked: mouse.accepted = false;
   onPressed: mouse.accepted = false;
   onReleased: mouse.accepted = false;
   onDoubleClicked: mouse.accepted = false;
   onPositionChanged: mouse.accepted = false;
   onPressAndHold: mouse.accepted = false;

    onExited: Tooltip.hideText()
    onCanceled: Tooltip.hideText()
    onClicked: {
    if (typeof(_root.parent.pressed) !== 'undefined')
        root.parent.pressed = !root.parent.pressed
    }

    Timer {
        interval: 500
        running: root.enabled && root.containsMouse && root.text.length && show_tooltip
        onTriggered: Tooltip.showText(root, Qt.point(root.mouseX, root.mouseY), root.text)
    }
}
