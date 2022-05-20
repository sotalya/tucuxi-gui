import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

TextField {
//    Layout.fillWidth:  true
    font.family: Style.form.font.input
    font.pixelSize: Style.form.size.input
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft

    property var doValidation: function() { return true; }





}
