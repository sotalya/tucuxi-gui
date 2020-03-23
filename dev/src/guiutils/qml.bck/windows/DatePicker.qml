import QtQuick 2.5
import QtQml 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

Window {
    id: root
    width: 320
    height: 320
    color: "white"
    modality: Qt.ApplicationModal
    flags: Qt.FramelessWindowHint

    signal dateChosen(date date)
    property date date: new Date()

    Rectangle {
        id: datePickerContainer
        anchors.fill: parent
        anchors.margins: 20
    }

    Calendar {
        id: datePickerCalendar
        anchors.fill: datePickerContainer
        onClicked: {
            root.date = date
            root.dateChosen(date)
            root.close()
        }
    }
}
