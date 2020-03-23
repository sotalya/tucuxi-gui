import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0

RowLayout {
    property string labeltext: ""
    property alias box: cb_box

    Rectangle {
        id: rect_fname
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
    ComboBox {
        id: cb_box
    }
}
