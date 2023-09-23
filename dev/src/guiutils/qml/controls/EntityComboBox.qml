import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

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
