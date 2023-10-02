import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

ComboBox {
//    Layout.fillWidth: true

    property bool isOk: true
    property var doValidation: 0
    function validate()
    {
        isOk = !doValidation || (doValidation && doValidation())
        return isOk
    }
}
