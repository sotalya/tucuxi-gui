import QtQuick 2.5
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {
    id: root

//    implicitWidth: TableStyle.header.width
//    implicitHeight: TableStyle.header.height
//    Component.onCompleted: console.log("hdr wh: " + width + " " + height)
    width: rowlyt_tablehdr.width
    height: rowlyt_tablehdr.height

    property ListModel headerData

    signal add()
    signal sort(int role, int order)
    signal filter(int role)

    RowLayout {
        id: rowlyt_tablehdr
//        anchors.fill: parent
        height: repeater.height
        spacing: TableStyle.header.spacing
//        Component.onCompleted: console.log("rlyt wh: " + width + " " + height)

        Repeater {
            id: repeater
            height: 25
            model: headerData
            delegate: headerDelegate
//            Component.onCompleted: console.log("rptr wh: " + width + " " + height)
        }

        Rectangle {
            id: rect_addbtn
            Layout.preferredWidth: TableStyle.header.height
            Layout.preferredHeight: TableStyle.header.height

            radius: TableStyle.header.radius
            color: TableStyle.header.backgroundColor

            Image {
                id: addIcon
                anchors.fill: parent
                anchors.margins:2

                source: "qrc:/images/add.png"
                fillMode: Image.PreserveAspectFit
                antialiasing: true

                MouseArea {
                    anchors.fill: parent
                    onClicked: add()
                }
            }
        }
    }

    Component {
        id: headerDelegate

        Rectangle {
//            Layout.fillWidth: index == repeater.count - 1
//            Layout.preferredWidth: model.columnWidth
//            Layout.preferredHeight: TableStyle.header.height
//            Component.onCompleted: console.log("hdr del wh: " + width + " " + height)
            width: TableStyle.cell.colwidth
            height: TableStyle.header.height

            radius: TableStyle.header.radius
            color: TableStyle.header.backgroundColor

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: {
                    delegateSortButton.visible = true
                    delegateFilterButton.visible = true
                }

                onExited: {
                    delegateSortButton.visible = false
                    delegateFilterButton.visible = false
                }
            }

            Text {
                id: delegateText
                anchors.centerIn: parent
                text: model.columnName
                font.bold: true
                font.family: TableStyle.fontfamily
                minimumPixelSize:TableStyle.textminpxsize
                font.pixelSize: TableStyle.textpxsize
                color: TableStyle.header.foregroundColor
                font.underline: true
            }

            Image {
                id: delegateSortButton
                anchors.left: delegateText.right
                anchors.verticalCenter: delegateText.verticalCenter
                anchors.margins: 5
                height: delegateText.contentHeight / 2

                source: "qrc:/images/sort.png"
                fillMode: Image.PreserveAspectFit
                antialiasing: true
                visible: false
                rotation: 180

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var order = parent.rotation === 180 ? 1 : 0;
                        parent.rotation = order ? 0 : 180;
                        sort(model.columnRole, !order);
                    }
                }
            }

            Image {
                id: delegateFilterButton
                anchors.left: delegateSortButton.right
                anchors.verticalCenter: delegateSortButton.verticalCenter
                anchors.margins: 5
                height: delegateText.contentHeight / 2

                source: "qrc:/images/filter.png"
                fillMode: Image.PreserveAspectFit
                antialiasing: true
                visible: false

                MouseArea {
                    anchors.fill: parent
                    onClicked: filter(model.columnRole)
                }
            }
        }
    }
}
