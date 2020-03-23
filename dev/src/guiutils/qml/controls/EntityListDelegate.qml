import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

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
