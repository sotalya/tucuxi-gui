import QtQuick 2.5

Item {
    id: root
    anchors.fill: parent
    default property alias content: content.data
    Rectangle {
        id: rect_pformmain
        anchors.fill: parent
        color: "transparent"
        Rectangle {
            id: content

        }
    }
}
