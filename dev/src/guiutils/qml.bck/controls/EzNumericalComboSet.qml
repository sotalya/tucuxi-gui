import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

RowLayout {
    property string labeltext: ""
    property alias box: sb_spinbox

    Rectangle {
        Layout.fillWidth: true
        color: "transparent"
        implicitWidth: TableStyle.cell.colwidth; implicitHeight: TableStyle.cell.height * 1.2
        Layout.maximumWidth: TableStyle.cell.colwidth
        Layout.minimumWidth: 75
       EzText {
           id: txt_fname
           text: labeltext
           horizontalAlignment: Text.AlignRight
           anchors.fill: parent
       }
    }
    Image {
        Layout.preferredHeight: TableStyle.cell.height * 1.2
        Layout.preferredWidth: TableStyle.cell.height * 1.2
        source: txtfld_fname.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
    }
    SpinBox {
        id: sb_spinbox
        Layout.fillWidth: true
        decimals: 3
        Layout.preferredHeight: TableStyle.cell.height * 1.2
        Layout.preferredWidth: TableStyle.cell.colwidth
        Layout.minimumHeight: TableStyle.cell.height * 1.2
        Layout.minimumWidth: TableStyle.cell.colwidth
        horizontalAlignment: Qt.AlignHCenter
        style: EzTextFieldStyle {}
        minimumValue: 0
        maximumValue: 100000
    }
}
