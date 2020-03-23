import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQml 2.2

import guiutils.qml.styles 1.0

Item {
    id: root
    implicitWidth: daysInput.implicitWidth + dmSeparator.implicitWidth + monthsInput.implicitWidth + mySeparator.implicitWidth + yearsInput.implicitWidth - 20
    implicitHeight: Math.max(daysInput.height, dmSeparator.height, monthsInput.height, mySeparator.height, yearsInput.height)

    property var date: new Date()
    property string separator: "."

    signal editingFinished()

    Item {
        id: impl
        anchors.fill: parent

//        TextFieldStyleBackground {
//            anchors.fill: parent
//            hasFocus: daysInput.activeFocus || monthsInput.activeFocus || yearsInput.activeFocus
//        }

        TextField {
            id: daysInput
            anchors.left: parent.left

            implicitWidth: 26
            style: EzTextFieldStyle { background: EzBorderLessBackgroundStyle {}}
            font.family: TableStyle.fontfamily
            font.pixelSize: TableStyle.textpxsize

            text: impl.format(root.date.getDate())
            validator: IntValidator { bottom: 1; top: 31}

            onFocusChanged: if (activeFocus) selectAll()
            onEditingFinished: {
                root.date = new Date(yearsInput.text, monthsInput.text - 1, daysInput.text);
                root.editingFinished();
            }
        }

        Text {
            id: dmSeparator
            anchors.left: daysInput.right
            anchors.leftMargin: -5
            anchors.verticalCenter: daysInput.verticalCenter
            text: separator
            font.family: TableStyle.fontfamily
            font.pixelSize: TableStyle.textpxsize
        }

        TextField {
            id: monthsInput
            anchors.left: dmSeparator.right
            anchors.leftMargin: -5

            implicitWidth: 26
            style: EzTextFieldStyle { background: EzBorderLessBackgroundStyle {}}
            font.family: TableStyle.fontfamily
            font.pixelSize: TableStyle.textpxsize

            text: impl.format(root.date.getMonth() + 1)
            validator: IntValidator { bottom: 1; top: 12}

            onFocusChanged: if (activeFocus) selectAll()
            onEditingFinished: {
                root.date = new Date(yearsInput.text, monthsInput.text - 1, daysInput.text);
                root.editingFinished();
            }
        }

        Text {
            id: mySeparator
            anchors.left: monthsInput.right
            anchors.leftMargin: -5
            anchors.verticalCenter: monthsInput.verticalCenter
            text: separator
            font.family: TableStyle.fontfamily
            font.pixelSize: TableStyle.textpxsize
        }

        TextField {
            id: yearsInput
            anchors.left: mySeparator.right
            anchors.right: parent.right
            anchors.leftMargin: -5

            implicitWidth: 50 + calendarInput.width
            style: EzTextFieldStyle { background: EzBorderLessBackgroundStyle {}}
            font.family: TableStyle.fontfamily
            font.pixelSize: TableStyle.textpxsize

            text: root.date.getFullYear()
            validator: IntValidator { bottom: 1900; top: 2999}

            onFocusChanged: if (activeFocus) selectAll()
            onEditingFinished: {
                root.date = new Date(yearsInput.text, monthsInput.text - 1, daysInput.text);
                root.editingFinished();
            }
        }

        Image {
            id: calendarInput
            anchors.right: yearsInput.right
            anchors.top: yearsInput.top
            anchors.bottom: yearsInput.bottom
            anchors.margins: 5
            width: height

            source: "qrc:/images/calendar.png"
            fillMode: Image.PreserveAspectFit

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (calendarInstance.sourceComponent === null)
                        calendarInstance.sourceComponent = calendarDialog;
                    calendarInstance.item.date = root.date;
                    calendarInstance.item.show();
                }
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

        Component {
            id: calendarDialog

            Window {
                id: root
                width: 320
                height: 320

                flags: Qt.Dialog | Qt.FramelessWindowHint
                modality: Qt.ApplicationModal

                property alias date: calendar.selectedDate

                signal accepted(var date)
                signal rejected()

                Rectangle {
                    anchors.fill: parent
                    border.color: "lightgrey"
                    border.width: 3
                }

                Calendar {
                    id: calendar
                    anchors.centerIn: parent
                    onClicked: root.accepted(date)
                }

                Component.onCompleted: {
                    accepted.connect(close);
                    rejected.connect(close);
                }
            }
        }

        Loader {
            id: calendarInstance
        }

        Connections {
            ignoreUnknownSignals: true
            target: calendarInstance.item
            onAccepted: {
                root.date = date;
                root.editingFinished();
            }
        }

        function format(data) {
            return data.toString().length === 1 ? "0" + data : data;
        }
    }
}
