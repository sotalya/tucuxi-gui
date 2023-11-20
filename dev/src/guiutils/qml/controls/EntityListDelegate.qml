import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel 1.0

Item {
    property alias mousearea: mousearea
    property alias rowlayout: rowlayout
    width:  ListView.view.width
    height: 25

    MouseArea {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
        property bool hovered: false
        onEntered: hovered = true
        onExited:  hovered = false
    }

    RowLayout {
        id: rowlayout
        anchors.fill: parent
        spacing: 2
    }
}
