import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

Rectangle {
    property alias label: label
    Layout.preferredWidth: 85
    Layout.fillHeight: true
    border.width: 0
    radius: 1

    Text {
        id: label
        anchors.fill: parent
        anchors.margins: 5
        elide: Text.ElideRight
        font.family: Style.table.font.row
        font.pixelSize: Style.table.size.row
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
    }
}
