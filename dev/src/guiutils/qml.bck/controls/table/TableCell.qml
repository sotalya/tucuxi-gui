import QtQuick 2.5
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {
    id: root
    width: rect_cell.width
    height: rect_cell.height
//    Component.onCompleted: console.log("cell wh: " + width + " " + height)

    property bool isCurrent: false
    property bool editMode: false

    property alias display: displayContent.data
    property alias edit: editContent.data

    Rectangle {
        id: rect_cell
//        anchors.fill: parent
        width: TableStyle.cell.colwidth
        onWidthChanged: {
            displayContent.width = width
//            console.log("did it chng: " + width)
        }
        height: displayContent.height
//        Component.onCompleted: console.log("cell wh: " + width + " " + height)

        radius: TableStyle.cell.radius
        color: "transparent"
//        color: isCurrent ? TableStyle.cell.highlightedcolor : "transparent"

        RowLayout {
            id: displayContent
            height: TableStyle.cell.height
//            anchors.margins: TableStyle.cell.margin
            visible: !editMode || !editContent.children.length
            Component.onCompleted: {
//                console.log("dispcontsims wh: " + displayContent.width + " " + displayContent.height)
                if (displayContent.width > TableStyle.cell.colwidth) {
                    TableStyle.cell.colwidth = displayContent.width * 1.2
//                    console.log("changed colwidth: " + displayContent.width)
                }
            }
        }

        RowLayout {
            id: editContent
            height: TableStyle.cell.height
//            anchors.margins: TableStyle.cell.margin
            visible: editMode
        }
    }
}
