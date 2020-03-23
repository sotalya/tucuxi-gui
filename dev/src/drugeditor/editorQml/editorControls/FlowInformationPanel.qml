import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {
    id: root
    width:  256
    height: 128

    property string drugName:         ""

    Item {
        id: impl
        anchors.fill: parent
        anchors.centerIn: parent
//        property int topMargin:    28
//        property int bottomMargin: 28
//        property int leftMargin:   50

        property int iconSpacing: 5

        //Drug label
        Text {
            id: drugNameLabel
            anchors.bottom:  parent.bottom
            anchors.left:    parent.left
            anchors.bottomMargin: impl.bottomMargin
            anchors.leftMargin:   impl.leftMargin
            text: root.drugName === "" ? "select a drug..." : root.drugName
            elide: Text.ElideRight
            color: Style.flow.foreground.information
            font.family:    Style.flow.font.information
            font.pixelSize: Style.flow.size.information
            font.bold:   true
            font.italic: root.drugName === ""
            verticalAlignment:   Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        //Drug icon
        Image {
            id: drugIcon
            anchors.verticalCenter: drugNameLabel.verticalCenter
            anchors.right:          drugNameLabel.left
            anchors.rightMargin: impl.iconSpacing
            width:  32
            height: 32
            source:  "qrc:/icons/other/drug.png";
            antialiasing: true
            mipmap:       true
            fillMode: Image.PreserveAspectFit
        }
    }
}
