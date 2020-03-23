import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

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
