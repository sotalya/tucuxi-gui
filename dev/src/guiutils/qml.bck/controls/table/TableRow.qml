import QtQuick 2.5
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {
    id: root
//    implicitWidth: TableStyle.row.width
//    implicitHeight: TableStyle.row.height
    width: wrapper.width
    height: wrapper.height


    default property alias content: content.data

    property string removeText: "Remove"
    property string editText: "Edit"

    signal remove()
    signal edit()

    Rectangle {
        id: wrapper
        color: "transparent"
        width: buttonBox.width + content.width
        height: TableStyle.row.height

        MouseArea {
            id: marea_tablerow
           anchors.fill: parent
           hoverEnabled: true
//           onContainsMouseChanged: containsMouse ? wrapper.color = "red" : wrapper.color = "transparent"
           onDoubleClicked: edit()
           onClicked: root.ListView.view.currentIndex = index
        }

        RowLayout {
            id: content
            spacing: TableStyle.row.spacing
        }

        Item {
            id: buttonBox
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
//            anchors.margins: TableStyle.row.margin

            width: removeButton.width
            height: parent.height

            Image {
                id: removeButton
                anchors.right: parent.right

                width: TableStyle.header.height
                height: TableStyle.header.height
                source: "qrc:/images/remove.png"

                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                visible: root.ListView.isCurrentItem

                MouseArea {
                    anchors.fill: parent
                    onClicked: remove()
                }
            }

//            Text {
//                id: editButton
//                anchors.right: removeButton.left
//                anchors.margins: 5

//                width: contentWidth
//                height: parent.height

//                horizontalAlignment: Text.AlignRight
//                verticalAlignment: Text.AlignVCenter

//                text: editText
//                color: TableStyle.row.button.foregroundColor
//                font.underline: true
//                font.family: TableStyle.fontfamily

//                visible: root.ListView.isCurrentItem

//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: edit()
//                }
//            }
        }
    }
}
