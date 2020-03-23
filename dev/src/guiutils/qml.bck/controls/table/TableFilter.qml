import QtQuick 2.5
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {
    id: root
    visible: false

    implicitWidth: TableStyle.filter.width
    implicitHeight: TableStyle.filter.height

    default property alias content: content.data

    signal clear()

    Rectangle {
        id: filterFrame
        anchors.fill: parent

        radius: TableStyle.filter.radius
        color: TableStyle.filter.backgroundColor

        RowLayout {
            anchors.fill: parent
            anchors.margins: TableStyle.filter.margin

            RowLayout {
                id: content
            }

            Text {
                Layout.preferredWidth: contentWidth
                Layout.fillHeight: true

                verticalAlignment: Text.AlignVCenter

                text: "Clear"
                color: TableStyle.filter.button.foregroundColor
                font.underline: true

                MouseArea {
                    anchors.fill: parent
                    onClicked: clear()
                }
            }

            Text {
                Layout.preferredWidth: contentWidth
                Layout.fillHeight: true

                verticalAlignment: Text.AlignVCenter

                text: "Close"
                color: TableStyle.filter.button.foregroundColor
                font.underline: true

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.visible = false
                }
            }
        }
    }
}
