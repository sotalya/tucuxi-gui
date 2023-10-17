import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQml
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

Window {
    id: calendarWindow
    visible: false
    modality: Qt.ApplicationModal
    title: "Calendar"
    flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint | Qt.MSWindowsFixedSizeDialogHint
    width: 350
    height: 300

    property alias month: grid.month
    property alias year: grid.year
    property alias locale: grid.locale

    signal accepted(var date)

    function open(date) {
        //console.log(date)
        datePicker.selectedTime = date.getTime()
        showNormal()
    }

    function buttonClick(direction){
        if(direction === "right") {
            year = month === Calendar.December ? year + 1 : year
            month = month === Calendar.December ? Calendar.January : month + 1
        } else {
            year = month === Calendar.January ? year - 1 : year
            month = month === Calendar.January ? Calendar.December : month - 1
        }
        datePicker.selectedDate.setMonth(month)
        datePicker.selectedDate.setYear(year)
    }

    Page {
        background: Rectangle { color: "#F9F9F9" }
        anchors.centerIn : parent

        width: calendarWindow.width

        Frame {
            anchors.fill: parent
            background: Rectangle {
                color: "#FFFFFF"
                border.color: "grey"
            }
            ColumnLayout {
                id: datePicker
                anchors.centerIn: parent

                property var selectedTime: 0
                property date selectedDate: new Date(selectedTime)

                DayOfWeekRow {
                    locale: grid.locale
                    Layout.fillWidth: true
                }

                MonthGrid {
                    id: grid
                    month: Calendar.January
                    year: 1900
                    locale: grid.locale
                    Layout.fillWidth: true

                    delegate: Text {
                        id: dateText
                        property MonthGrid control: grid
                        property bool isCurrentItem: model.date.getTime() === datePicker.selectedTime
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        opacity: model.month === control.month ? 1 : 0.5
                        text: day
                        font: control.font
                        Rectangle {
                            id: dateSelector
                            anchors.fill: parent
                            anchors.margins: -4
                            visible: isCurrentItem
                            color: "#0078D7"
                            z: -2
                        }
                    }

                    onClicked: function (date) {
                        datePicker.selectedTime = date.getTime()
                    }
                }
            }
        }
        header: RowLayout {
            Layout.fillWidth:  true
            Layout.fillHeight:  true
            Button {
                width: 20
                text: "<"
                onClicked: buttonClick("left")
            }

            ColumnLayout{
                Text {
                    Layout.fillWidth:  true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                    text: Qt.formatDate(datePicker.selectedDate, "yyyy")
                }
                Text {
                    Layout.fillWidth:  true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                    text: Qt.formatDate(datePicker.selectedDate, "MMMM")
                }
            }

            Button {
                width: 20
                text: ">"
                onClicked: buttonClick("right")
            }
        }

        footer: RowLayout {
            Layout.fillWidth:  true
            Layout.fillHeight:  true
            spacing: 10

            Button {
                Layout.fillWidth:  true
                text: "Ok"
                onClicked: {
                    accepted(new Date(datePicker.selectedTime))
                    close()
                }
            }
            Button {
                Layout.fillWidth:  true
                text: "Cancel"
                onClicked: {
                    close()
                }
            }
        }
    }
}
