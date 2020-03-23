import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {
    id: root
    implicitWidth: hoursInput.implicitWidth + hmColumn.implicitWidth + minutesInput.implicitWidth
    implicitHeight: Math.max(hoursInput.height, hmColumn.height, minutesInput.height)

    property var date: new Date()

    property string separator: ":"

    signal editingFinished()

    Item {
        id: impl
        anchors.fill: parent

//        TextFieldStyleBackground {
//            anchors.fill: parent
//            hasFocus: hoursInput.activeFocus || minutesInput.activeFocus
//        }

        TextField {
            id: hoursInput
            anchors.left: parent.left

            style: EzTextFieldStyle {background: EzBorderLessBackgroundStyle {}}
            implicitWidth: 26

            text: impl.format(date.getHours())
            validator: IntValidator { bottom: 0; top: 23}

            onFocusChanged: if (activeFocus) selectAll()
            onEditingFinished: {
                root.hours = parseInt(text);
                root.editingFinished();
            }
        }

        Text {
            id: hmColumn
            anchors.left: hoursInput.right
            anchors.leftMargin: -5
            anchors.verticalCenter: hoursInput.verticalCenter
            text: separator
        }

        TextField {
            id: minutesInput
            anchors.left: hmColumn.right
            anchors.leftMargin: -5
            anchors.right: parent.right

            style: EzTextFieldStyle {background: EzBorderLessBackgroundStyle {}}
            implicitWidth: 26

            text: impl.format(date.getMinutes())
            validator: IntValidator { bottom: 0; top: 59}

            onFocusChanged: if (activeFocus) selectAll()
            onEditingFinished: {
                root.minutes = parseInt(text);
                root.editingFinished();
            }
        }

//        Component {
//            id: borderlessStyle

//            TextFieldStyle {
//                background: Rectangle {
//                    anchors.fill: parent
//                    color: "transparent"
//                }
//            }
//        }

        function format(data) {
            return data.toString().length === 1 ? "0" + data : data;
        }
    }
}
