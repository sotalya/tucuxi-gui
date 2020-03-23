import QtQuick 2.5
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {
    id: root

    default property alias content: content.data

    property var filters
    property var model
    property Component delegate

    property alias currentIndex: listView.currentIndex
    property alias currentItem: listView.currentItem
    property alias count: listView.count

    ColumnLayout {
        id: clyt_table
        anchors.fill: parent
        spacing: TableStyle.spacing
        clip: true

        ColumnLayout {
            id: content
            clip: true
        }

        ListView {
            id: listView
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            Component.onCompleted: {
                forceLayout();
//                console.log("lv wh: " + width + " " + height + "count: " + ListView.count)
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
            snapMode: ListView.SnapToItem

            model: root.model
            delegate: root.delegate
            highlight: highlight
            highlightFollowsCurrentItem: true

            spacing: TableStyle.spacing
            clip: true
        }
    }

    Component.onCompleted: {
        for (var i = 0; i < root.filters.length; ++i)
            model.setFilter(root.filters[i]);
//        console.log("Table wh: " + width + " " + height)
        TableStyle.cell.colwidth = TableStyle.cell.colwidth + 1
    }

    Component {
        id: highlight
        Rectangle {
            width: listView.currentItem.width; height: listView.currentItem.height
            color: "lightsteelblue"; radius: 5
            border.width: 2
            border.color: "black"
            y: listView.currentIndex.y

            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }
        }
    }

}
