import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {
    id: root
    width:  64
    height: 64

    property var icon
    property var icon_hover
    property var icon_disable

    signal pressed()

    Item {
        id: impl
        anchors.fill: parent

        Rectangle {
            id: background
            anchors.fill: parent
            color:        Style.toolbar.background.button
            border.color: Style.toolbar.border.color
            border.width: Style.toolbar.border.width
        }

        Image {
            id: forground
            anchors.fill: parent
            anchors.margins: 4
            source: root.icon
            antialiasing: true
            mipmap: true
            fillMode: Image.PreserveAspectFit
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onClicked: root.pressed()

            onEntered: {
                background.color = Style.toolbar.background.button_hover
                forground.source = root.icon_hover;
            }

            onExited: {
                background.color = Style.toolbar.background.button
                forground.source = root.icon;
            }
        }
    }
}
