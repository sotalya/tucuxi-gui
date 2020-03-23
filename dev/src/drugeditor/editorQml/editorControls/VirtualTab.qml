import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

import ezechiel 1.0


FlowTab {
    id: virtualRoot
    width: 800
    height:600

    property int index:  -1
    property int status: Status.invalid


    signal currentIndexChanged(string view,int index)
    signal mainIndexChanged(int index)
    signal currentStatusChanged(int status)
    signal reset()


    signal textChanged(string propertyName, var newValue)
    //used when an Editor is added or removed; `index == -1` when added
    signal entityChanged(string listViewName, int index)

    onIndexChanged:  mainIndexChanged(index)//currentIndexChanged(index)



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



}
