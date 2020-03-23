import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Window {
    id: root
    title: "Dialog"

    width: TableStyle.dialog.width
    height: TableStyle.dialog.height

    flags: Qt.Dialog | Qt.FramelessWindowHint
    modality: Qt.ApplicationModal

    default property alias content: content.data

    property string acceptText: "Ok"
    property string cancelText: "Cancel"

    signal accepted()
    signal rejected()

    onRejected: close();

    Item {
        id: wrapper
        anchors.fill: parent

        Rectangle {
            id: frame
            anchors.fill: parent
            color: TableStyle.dialog.backgroundColor
            radius: TableStyle.dialog.radius
            border.color: TableStyle.dialog.border.color
            border.width: TableStyle.dialog.border.size
        }

        ColumnLayout {
            anchors.fill: parent

            ColumnLayout {
                id: content
                Layout.margins: TableStyle.dialog.margin
            }

            RowLayout {
                Layout.bottomMargin: TableStyle.dialog.margin
                Layout.leftMargin: TableStyle.dialog.margin
                Layout.rightMargin: TableStyle.dialog.margin

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Text {
                        id: cancelButton
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        width: contentWidth
                        font.family: TableStyle.fontfamily

                        text: cancelText
                        color: TableStyle.dialog.button.foregroundColor
                        font.underline: true

                        MouseArea {
                            anchors.fill: parent
                            onClicked: rejected()
                        }
                    }

                    Text {
                        id: acceptButton
                        anchors.right: cancelButton.left
                        anchors.bottom: parent.bottom
                        anchors.rightMargin: 5
                        width: contentWidth
                        font.family: TableStyle.fontfamily

                        text: acceptText
                        color: TableStyle.dialog.button.foregroundColor
                        font.underline: true

                        MouseArea {
                            anchors.fill: parent
                            onClicked: accepted()
                        }
                    }
                }
            }
        }
    }
}
