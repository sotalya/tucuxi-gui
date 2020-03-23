import QtQuick 2.5
import QtQuick.Controls.Styles 1.4
import guiutils.qml.styles 1.0

TextFieldStyle {
    background: EzBackgroundStyle {}
    textColor: "black"
    selectedTextColor: "white"
    selectionColor: "orange"
    font.family: TableStyle.fontfamily
    font.pixelSize: TableStyle.textpxsize
}
