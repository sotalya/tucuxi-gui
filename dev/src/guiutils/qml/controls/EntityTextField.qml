import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

TextField {
//    Layout.fillWidth:  true
    font.family: Style.form.font.input
    font.pixelSize: Style.form.size.input
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft

    property var doValidation: function() { return true; }





}
