import QtQuick 2.5

Item {
    id: root

    property bool hasFocus: hasOwnProperty("control") && control.activeFocus

    Rectangle {
        anchors.fill: parent
        anchors.bottomMargin: -1
        color: "#44ffffff"
        radius: baserect.radius
    }

    Rectangle {
        id: baserect
        anchors.fill: parent

        radius: height * 0.16
        border.color: hasFocus ? "#47b" : "#999"

        gradient: Gradient {
            GradientStop {color: "#e0e0e0" ; position: 0}
            GradientStop {color: "#fff" ; position: 0.1}
            GradientStop {color: "#fff" ; position: 1}
        }
    }
}
