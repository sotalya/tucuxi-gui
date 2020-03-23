import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0

RowLayout {
    property string labeltext: ""
    property date date: new Date()
    property alias cal: calendar
    property alias hoursbox: sb_hours
    property alias minsbox: sb_minutes
    z:1

    Rectangle {
        id: rect_fname
        Layout.fillWidth: true
        color: "transparent"
        implicitWidth: TableStyle.cell.colwidth; implicitHeight: TableStyle.cell.height * 1.2
        Layout.maximumWidth: TableStyle.cell.colwidth
        Layout.minimumWidth: 75
       EzText {
           id: txt_fname
           text: labeltext + " " + date.toLocaleDateString(Locale.ShortFormat)
           horizontalAlignment: Text.AlignRight
           anchors.fill: parent
       }
    }
    Image {
        id: calendarInput
        Layout.fillWidth: true
        Layout.preferredHeight: TableStyle.cell.height * 1.2
        Layout.preferredWidth: TableStyle.cell.height
        Layout.minimumHeight: TableStyle.cell.height * 1.2
        Layout.minimumWidth: TableStyle.cell.height

        source: "qrc:/images/calendar.png"
        fillMode: Image.PreserveAspectFit

        MouseArea {
        anchors.fill: parent
        onClicked: {
            calendar.visible = !calendar.visible
            rect_fname.visible = !rect_fname.visible
            sb_minutes.visible = !sb_minutes.visible
            sb_hours.visible = !sb_hours.visible
        }
        }
    }
    SpinBox {
        id: sb_hours
        minimumValue: 0
        maximumValue: 23
        decimals: 0
        suffix: "h"
        Layout.fillWidth: true
        Layout.preferredHeight: TableStyle.cell.height * 1.2
        Layout.preferredWidth: 75
        Layout.minimumHeight: TableStyle.cell.height * 1.2
        style: SpinBoxStyle {
           background: EzBackgroundStyle {}
        }
    }

    SpinBox {
        id: sb_minutes
        minimumValue: 0
        maximumValue: 59
        decimals: 0
        suffix: "m"
        Layout.fillWidth: true
        Layout.preferredHeight: TableStyle.cell.height * 1.2
        Layout.preferredWidth: 75
        Layout.minimumHeight: TableStyle.cell.height * 1.2
        style: SpinBoxStyle {
           background: EzBackgroundStyle {}
        }
    }
    Calendar {
        id: calendar
        z: 1
        visible: false
        Layout.fillWidth: true
        Layout.preferredHeight: TableStyle.cell.height * 1.2
        Layout.preferredWidth: TableStyle.cell.height
        Layout.minimumHeight: 250
        Layout.minimumWidth: 150
    }
}
