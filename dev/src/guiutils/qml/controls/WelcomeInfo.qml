
import QtQuick 2.7
import QtQuick.Controls 1.4

import QtQuick.Layouts 1.3

import guiutils.qml.controls 1.0

Item {

    ColumnLayout {

        Layout.alignment: Qt.AlignVCenter
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
            Layout.preferredHeight: 100
        }

        Text {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            text: "<center>Welcome to Tucuxi!</center>"
            textFormat: TextEdit.RichText
            font.family: "Helvetica"
            font.pointSize: 18
            color: "black"
            padding : 20
            focus: true
        }

        Image {
            Layout.fillHeight: true
            Layout.preferredWidth: 40
            Layout.alignment: Qt.AlignCenter
            source: "qrc:/icons/logo.png"
            antialiasing: true
            mipmap: true
            fillMode: Image.PreserveAspectFit
        }

        Text {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            text: "<center>
         Graphs of predicted concentrations will be shown here.</center>"
            textFormat: TextEdit.RichText
            font.family: "Helvetica"
            font.pointSize: 18
            color: "black"
            padding : 20
            focus: true
        }
        Image {
            Layout.fillHeight: true
            Layout.preferredWidth: 40
            Layout.alignment: Qt.AlignCenter
            source: "qrc:/icons/logo.png"
            antialiasing: true
            mipmap: true
            fillMode: Image.PreserveAspectFit
        }

        Text {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            text: "<center>Tooltips can be toggled using CTRL+SHIFT+T.<br/> They can be useful to guide you through the software features.</center>"
            textFormat: TextEdit.RichText
            font.family: "Helvetica"
            font.pointSize: 18
            color: "black"
            padding : 20
            focus: true
        }

        Item {
            Layout.fillHeight: true
            Layout.preferredHeight: 100
        }
    }
}
    /*
Text {
    wrapMode: Text.WordWrap
    verticalAlignment: Text.AlignVCenter
    text: "<center>Welcome to Tucuxi!<br/>
                   <img src=\"qrc:/icons/logo.png\" width=\"40\" align=\"center\"/><br/>
 Graphs of predicted concentrations will be shown here.<br/>
                   <div style=\"text-align:center\"><img src=\"qrc:/icons/logo.png\" width=\"40\"/></div> <br/>
                   Tooltips can be toggled using CTRL+SHIFT+T.<br/> They can be useful to guide you through the software features.</center>"
    textFormat: TextEdit.RichText
    font.family: "Helvetica"
    font.pointSize: 18
    color: "black"
    padding : 20
    focus: true
}

}*/
