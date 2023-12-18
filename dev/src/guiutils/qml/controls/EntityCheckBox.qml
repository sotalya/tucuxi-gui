import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel


CheckBox {
    property alias checkBoxtext : controlText.text

    function setErrorColor(isError){
        if(!isError){
            controlText.color = "black"
            controlCheck.color = "black"
        }
        else if(control.checked){
            controlText.color = "red"
            controlCheck.color = "red"
        }
        else{
            controlText.color = "black"
            controlCheck.color = "black"
        }
    }

    id: control
    checked: false
    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: altCheckBox.leftPadding
        y: parent.height / 2 - height / 2
        border.color: "black"

        Rectangle {
            id: controlCheck
            width: 20
            height: 20
            x: 3
            y: 3
            color: "black"
            visible: control.checked
        }
    }
    contentItem: Text {
        id: controlText
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
