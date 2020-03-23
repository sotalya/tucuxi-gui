import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {
    id: flowTabroot
    width:  800
    height: 600

    property alias leftPanel:  leftPanel.sourceComponent
    property alias middlePanel: middlePanel.sourceComponent
    property alias rightPanel: rightPanel.sourceComponent

    property real ratio: 2

    Item {
        id: impl
        anchors.fill: parent

        Rectangle {
            id: leftRectangle
            width: parent.width / ratio
            height: parent.height
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            color: "transparent"

            Loader {
                id: leftPanel
                anchors.fill: parent
                visible: status == Loader.Ready
                sourceComponent: leftPanel
            }
        }

        Rectangle {
            width: parent.width / ratio
            height: parent.height
            anchors.left: leftRectangle.right
//            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: "transparent"

            Loader {
                id: middlePanel
                anchors.fill: parent
                visible: status == Loader.Ready
                sourceComponent: leftPanel
            }
        }

        Rectangle {
            width: parent.width / ratio
            height: parent.height
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: "transparent"

            Loader {
                id: rightPanel
                anchors.fill: parent
                visible: status == Loader.Ready
                sourceComponent: leftPanel
            }
        }
    }
}
