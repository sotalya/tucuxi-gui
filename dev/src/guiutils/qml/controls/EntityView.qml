import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

Rectangle {
    property alias columnlayout: columnlayout
    Layout.fillWidth: true
    Layout.preferredHeight: 125
    Layout.minimumHeight:   125
    property int rowHeight: 25
    property string frameBackground: Style.form.background.frame
    property string frameColor:      Style.form.border.color
    property int frameWidth:   Style.form.border.width
    property int frameRadius:  Style.form.border.radius
    property int framePadding: 5
    property string labelForeground: Style.form.foreground.label
    property string labelFontFamily: Style.form.font.label
    property string labelFontSize:   Style.form.size.label
    property bool labelFontBold: false
    property int labelVAlign: Text.AlignVCenter
    property int labelHAlign: Text.AlignRight
    property string inputFontFamily: Style.form.font.input
    property string inputFontSize:   Style.form.size.input
    property int inputVAlign: Text.AlignVCenter
    property int inputHAlign: Text.AlignLeft
    property int delegateStatus: Status.invalid

    color:  Style.form.background.frame
    radius: Style.form.border.radius
    border.width: Style.form.border.width
    border.color: Style.form.border.color

    ColumnLayout {
        id: columnlayout
        anchors.fill: parent
        anchors.margins: parent.framePadding
        spacing: 5

    }
}
