import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {
    property alias columnlayout: columnlayout
    Layout.fillWidth:  true
    Layout.fillHeight: true

    property string headerBackground: Style.table.background.header
    property string headerForeground: Style.table.foreground.header
    property string headerFontFamily: Style.table.font.header
    property string headerFontSize:   Style.table.size.header
    property bool headerFontBold:      true
    property bool headerFontUnderline: false
    property int headerHeight: 25
    property int headerVAlign: Text.AlignVCenter
    property int headerHAlign: Text.AlignHCenter

    property string rowBackground:         Style.table.background.row
    property string rowBackgroundHover:    Style.table.background.row_hover
    property string rowBackgroundSelected: Style.table.background.row_selected
    property string rowForeground:         Style.table.foreground.row
    property string rowForegroundGray:     Style.table.foreground.row_gray
    property string rowForegroundHover:    Style.table.foreground.row_hover
    property string rowForegroundSelected: Style.table.foreground.row_selected
    property string rowFontFamily:  Style.table.font.row
    property string rowFontSize:    Style.table.size.row
    property int rowVAlign: Text.AlignVCenter
    property int rowHAlign: Text.AlignLeft
    property int rowHeight: 25

    property int cellFrameRadius: 1
    property int cellFrameWidth:  0
    property int cellMargin:      5

    ColumnLayout {
        id: columnlayout
        anchors.fill: parent
        anchors.margins: 0
    }
}

