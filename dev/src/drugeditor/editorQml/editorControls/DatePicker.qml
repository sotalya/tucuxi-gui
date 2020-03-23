import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQml 2.2

import guiutils.qml.styles 1.0

//ToDo: fix the height (slightly above the a basic text field)

Item {
    id: root
    implicitWidth: daysInput.implicitWidth + dmSeparator.implicitWidth + monthsInput.implicitWidth + mySeparator.implicitWidth + yearsInput.implicitWidth - 20
    implicitHeight: Math.max(daysInput.height, dmSeparator.height, monthsInput.height, mySeparator.height, yearsInput.height)

    property var date: new Date()
    property string separator: "."

    property string fontFamily: Style.form.font.input
    property int fontSize:      Style.form.size.input

    signal editingFinished()


    Item {
        id: impl
        anchors.fill: parent

        Item {
            id: defaultStyle
            anchors.fill: parent

            property bool hasFocus: daysInput.activeFocus || monthsInput.activeFocus || yearsInput.activeFocus

            Rectangle {
                anchors.fill: parent
                anchors.bottomMargin: -1
                color: "#44ffffff"
                radius: baserect.radius
            }

            Rectangle {
                id: baserect
                anchors.fill: parent

                radius: height * 0.16
                border.color: defaultStyle.hasFocus ? "#47b" : "#999"

                gradient: Gradient {
                    GradientStop {color: "#e0e0e0" ; position: 0}
                    GradientStop {color: "#fff" ; position: 0.1}
                    GradientStop {color: "#fff" ; position: 1}
                }
            }
        }

        TextField {
            id: daysInput
            anchors.left: parent.left

            implicitWidth: 26
            style: borderlessStyle
            font.family:    root.fontFamily
            font.pixelSize: root.fontSize

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
            font.family:    root.fontFamily
            font.pixelSize: root.fontSize
        }

        TextField {
            id: monthsInput
            anchors.left: dmSeparator.right
            anchors.leftMargin: -5

            implicitWidth: 26
            style: borderlessStyle
            font.family:    root.fontFamily
            font.pixelSize: root.fontSize

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
            font.family:    root.fontFamily
            font.pixelSize: root.fontSize
        }

        TextField {
            id: yearsInput
            anchors.left: mySeparator.right
            anchors.right: parent.right
            anchors.leftMargin: -5

            implicitWidth: 50 + calendarInput.width
            style: borderlessStyle
            font.family:    root.fontFamily
            font.pixelSize: root.fontSize

            text: root.date.getFullYear()
            validator: IntValidator { bottom: 1850; top: 2999}

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
            mipmap: true
            source: "qrc:/icons/buttons/calendar.png"
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

        Component {
            id: borderlessStyle

            TextFieldStyle {
                background: Rectangle {
                    anchors.fill: parent
                    anchors.margins: 0
                    color: "transparent"
                }
            }
        }

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
