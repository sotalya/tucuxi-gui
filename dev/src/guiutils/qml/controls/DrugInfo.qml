import QtQuick
import QtQuick.Controls

Text {
    wrapMode: Text.WordWrap
    textFormat: TextEdit.RichText
    font.family: "Helvetica"
    font.pointSize: 11
    color: "black"
    focus: true
    text: drugTabController.drugHtmlDescription;
}

