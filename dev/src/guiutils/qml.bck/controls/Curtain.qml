import QtQuick 2.5
import QtQuick.Layouts 1.2
import guiutils.qml.styles 1.0

Rectangle {
    id: root

    property var behinds
    property var windowscaler
    property var container: parent.parent
    property var innertext: ""

//    width: row_curtain.width
//    height: anim_ring.height
    implicitHeight: 64
    implicitWidth: 346
    color: "transparent"
//    x: Math.max((container.parent.parent.parent.width - width)/2,0)
    RowLayout {
        id: row_curtain
        EzText {
            id: eztext_curtain
            Layout.preferredHeight: 25
            Layout.preferredWidth: 250
            Layout.fillWidth: true
            Layout.fillHeight: true
//            height: root.height
            text: innertext
            font.underline: true
//            width: 200
        }
        AnimatedSprite {
            id: anim_ring
            implicitHeight: 50//64
            implicitWidth: 50//64
//            anchors.right: root.right
//            scale: windowscaler.flowscale / container.scale
            frameRate: 50
//            width: 64
//            height: 64
            source: "qrc:/sprites/blueringrotate.png"
            frameWidth: 128
            frameHeight: 128
            frameCount: 40
            running: false
            loops: 1
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 16
        Image {
            Layout.preferredHeight: 16
            Layout.preferredWidth: 16
            source: behinds.suminvalid == behinds.sumtotal? "qrc:/images/check.png" : ""
//            width: 16
//            height: 16
        }
        Image {
            Layout.preferredHeight: 16
            Layout.preferredWidth: 16
            opacity: marea_curtain.containsMouse
            Behavior on opacity { PropertyAnimation {easing.type: Easing.InOutQuad} }
//            width: 16; height: 16
            source: "qrc:/images/help.png"
        }
        }
    }


    MouseArea {
        id: marea_curtain
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            anim_ring.running = true
            anim_ring.reverse = behinds.visible
            behinds.opacity = !behinds.opacity
        }
    }
}

