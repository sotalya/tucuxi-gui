import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Item {
    id: root
    anchors.fill: parent
    property alias leftPanel: ldr_leftpanel.sourceComponent
    property alias rightPanel: ldr_rightpanel.sourceComponent

    Item {
        id: impl
        anchors.fill: parent

        Rectangle {
            id: rect_leftpanel
            width: parent.width/2
            height: parent.height
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            color: "transparent"
            Loader {
                id: ldr_leftpanel
                anchors.fill: parent
//                asynchronous: true
                visible: status == Loader.Ready
                sourceComponent: leftPanel
            }
        }
        Rectangle {
            id: rect_rightpanel
            width: parent.width/2
            height: parent.height
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: "transparent"
            Loader {
                id: ldr_rightpanel
                anchors.fill: parent
//                asynchronous: true
                visible: status == Loader.Ready
                sourceComponent: rightPanel
            }
        }
    }
}
