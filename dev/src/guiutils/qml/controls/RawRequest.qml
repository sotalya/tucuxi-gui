import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2

Item {
    id: flick

    width: parent.width
    height: parent.height

    ColumnLayout {
        TextArea {
            id: clinicals

            width: flick.width
            Layout.preferredWidth: flick.width
            height: 200 //parent.height

            focus: true
            wrapMode: TextEdit.Wrap
            textFormat: TextEdit.RichText
            font.family: "Arial"
            font.pointSize: 11
            text: (interpretationController.clinicalsHtml != "") ? interpretationController.clinicalsHtml : "Weird. The request file seems empty. Well, if you are dealing with the demo version that's kind of normal :-)" ;
        }

        Button {
            id : showSource;
            text: flickable.visible? "Hide pending request source" : "Display pending request source";
            onClicked: flickable.visible = !flickable.visible;
        }

        Flickable {
            id: flickable
            width: flick.width
            Layout.preferredWidth: flick.width
            height: flick.height-200
            Layout.preferredHeight: flick.height-200
            visible : false

            TextArea.flickable: TextArea {
                id: raw
                focus: true
                wrapMode: TextEdit.Wrap
                textFormat: TextEdit.PlainText
                font.family: "Courier"
                font.pointSize: 11
                text: (interpretationController.rawRequest != "") ? interpretationController.rawRequest : "Weird. The request file seems empty. Well, if you are dealing with the demo version that's kind of normal :-)" ;

            }

            ScrollBar.vertical: ScrollBar { }
        }
    }

}
