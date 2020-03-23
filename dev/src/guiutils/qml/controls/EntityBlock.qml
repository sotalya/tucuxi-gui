import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Item {

    height: 50
    width : parent.width

    property alias label : title.label
    property alias expanded : title.expanded



    default property alias content: stack.children

    ColumnLayout {

        width: parent.width

        // Tentative. To be replaced by a custom expandable header
        EntityTitle {
            width: parent.width
            label.text: "Coucou"
            id: title

        }


        Rectangle{
            id: stack
            visible: title.expanded
            //width: 200
            //height: 40
            anchors.top : title.bottom
        }
        /*
    Item {
        visible: title.expanded
        id : stack
        anchors.top : title.bottom

    }
*/

        Rectangle {
            visible : title.expanded
            anchors.top : stack.bottom
            Layout.minimumHeight:   2
            Layout.minimumWidth: parent.width
            border.color: "black"
            border.width: 2
        }


    }

}

