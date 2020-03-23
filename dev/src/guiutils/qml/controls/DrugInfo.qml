import QtQuick 2.5
import QtQuick.Controls 1.4

Text {
    wrapMode: Text.WordWrap
    textFormat: TextEdit.RichText
    font.family: "Helvetica"
    font.pointSize: 11
    color: "black"
    focus: true
    text: drugTabController.drugHtmlDescription;
}

