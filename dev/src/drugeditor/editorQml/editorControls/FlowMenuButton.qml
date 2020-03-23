import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import drugeditor.qml.controls 1.0
import guiutils.qml.styles 1.0

Item {
    id: root
    // This width is bigger by 20 because Canvas delta is 10
    Layout.preferredWidth:  parent.height+20
    Layout.preferredHeight: parent.height

    property var icon
    property var icon_disabled
    property var icon_hovered
    property var icon_selected

    property int index
    property int currentIndex: -1

    property int status: Status.invalid

    property bool isCurrent: index === currentIndex
    property bool isEnabled: true

    property alias legend: legend.text

    onIsEnabledChanged: background.requestPaint()
    onIsCurrentChanged: background.requestPaint()

    // In order to differentiate the last tab
    property bool isLast : false

    signal pressed()

    Item {
        id: impl
        anchors.fill: parent


        Canvas {
            id: background
            anchors.fill: parent

            property int delta: 10

            property variant fillStyle: {
                if (mouseArea.hovered)
                    return Style.flow.background.button_hover;
                if (root.isCurrent)
                    return Style.flow.background.button_selected;
                if (!root.isEnabled)
                    return Style.flow.background.button_disabled;

                return Style.flow.background.button;
            }

            property variant strokeStyle: {
                if (mouseArea.hovered)
                    return Style.flow.border.color_hover;
                if (root.isCurrent)
                    return Style.flow.border.color_selected;
                if (!root.isEnabled)
                    return Style.flow.border.color_disabled;

                return Style.flow.border.color;
            }

            onPaint: {
                var ctx = background.getContext("2d");
                ctx.save();
                ctx.clearRect(0,0,background.width, background.height);
                ctx.strokeStyle = background.strokeStyle;
                ctx.lineWidth = Style.flow.border.width;

                ctx.fillStyle = background.fillStyle;
                ctx.globalAlpha = background.alpha
                ctx.beginPath();
                ctx.moveTo(0,0);
                if (root.isLast) {
                    ctx.lineTo(width,0);
                    ctx.lineTo(width,height);
                }
                else {
                    ctx.lineTo(width-delta,0);
                    ctx.lineTo(width-delta,height/2-delta);
                    ctx.lineTo(width,height/2);
                    ctx.lineTo(width-delta,height/2+delta);
                    ctx.lineTo(width-delta,height);
                }
                ctx.lineTo(0,height);
                ctx.lineTo(0,height/2+delta);
                ctx.lineTo(delta,height/2);
                ctx.lineTo(0,height/2-delta);

                ctx.closePath();
                ctx.fill();
                ctx.stroke();
                ctx.restore();
            }
        }

/*
        Rectangle {
            id: background
            anchors.fill: parent
            color: {
                if (mouseArea.hovered)
                    return Style.flow.background.button_hover;
                if (root.isCurrent)
                    return Style.flow.background.button_selected;
                if (!root.isEnabled)
                    return Style.flow.background.button_disabled;

                return Style.flow.background.button;
            }
            border.color: {
                if (mouseArea.hovered)
                    return Style.flow.border.color_hover;
                if (root.isCurrent)
                    return Style.flow.border.color_selected;
                if (!root.isEnabled)
                    return Style.flow.border.color_disabled;

                return Style.flow.border.color;
            }
            border.width: Style.flow.border.width
        }
*/
        Image {
            id: icon
            anchors.fill: parent
            anchors.margins: 25
            source: {
                if (mouseArea.hovered)
                    return root.icon_hovered;
                if (root.isCurrent)
                    return root.icon_selected;
                if (!root.isEnabled)
                    return root.icon_disabled;

                return root.icon;
            }
            antialiasing: true
            mipmap: true
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: statusIcon
            anchors.top:   parent.top
            anchors.right: parent.right
            anchors.topMargin:   2
            anchors.rightMargin: 12
            width:  38
            height: 38
            visible: root.isEnabled
            source: {
                if (root.status === Status.invalid)
                    return "qrc:/icons/states/invalid.png";
                if (root.status === Status.valid)
                    return "qrc:/icons/states/valid.png";

                return "qrc:/icons/states/complete.png";
            }
            antialiasing: true
            mipmap: true
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: legend
            anchors.horizontalCenter: icon.horizontalCenter
            anchors.top:              icon.bottom
            anchors.topMargin:        -2
            elide: Text.ElideRight
            color: {
                if (mouseArea.hovered)
                    return Style.flow.foreground.button_hover;
                if (root.isCurrent)
                    return Style.flow.foreground.button_selected;
                if (!root.isEnabled)
                    return Style.flow.foreground.button_disabled;

                return Style.flow.foreground.button;
            }
            font.family:    Style.flow.font.button
            font.pixelSize: Style.flow.size.button
            font.bold: true
            verticalAlignment:   Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true

            property bool hovered: false

            onClicked: {
                if (root.isEnabled)
                    root.pressed()
                background.requestPaint();
            }
            onEntered: { hovered = true;  background.requestPaint(); }
            onExited:  { hovered = false; background.requestPaint(); }
        }
    }
}
