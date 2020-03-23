import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Item {
    property alias label: label

    Layout.fillWidth: true
    Layout.preferredHeight: 40
    //    border.width: 0
    //    radius: 1
    //    color: Style.table.background.header

    property bool expanded : true

    property bool showStatus : false
    property int status : Status.invalid

    property string tooltipText : ""

    onExpandedChanged: {
        ex.requestPaint();
        background.requestPaint();
    }


    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (parent.expanded)
                parent.expanded = false;
            else
                parent.expanded = true;
        }

        hoverEnabled: true

        ToolTip {
            background: Rectangle {
                color: Style.tooltip.color
                border.color: Style.tooltip.border_color
            }
            visible : (show_tooltip) ? (parent.containsMouse && (tooltipText != "")) : false
            text : tooltipText
        }

    }

    Canvas {
        id: background
        anchors.fill: parent

        property int delta: 10

        property variant fillStyle: {
            //        if (mouseArea.hovered)
            return Style.flow.background.button_hover;
            //      if (parent.isCurrent)
            //        return Style.flow.background.button_selected;
            //  if (!parent.isEnabled)
            //       return Style.flow.background.button_disabled;

            return Style.flow.background.button;
        }

        property variant strokeStyle: {
            //    if (mouseArea.hovered)
            //        return Style.flow.border.color_hover;
            //    if (parent.isCurrent)
            //        return Style.flow.border.color_selected;
            //    if (!parent.isEnabled)
            //        return Style.flow.border.color_disabled;

            return Style.flow.border.color;
        }

        onPaint: {
            var ctx = background.getContext("2d");
            //   ctx.save();
            ctx.clearRect(0,0,background.width, background.height);
            ctx.strokeStyle = background.strokeStyle;
            ctx.lineWidth = Style.flow.border.width;

            //    ctx.fillStyle = "#aaa";
            //    ctx.fillRect(0,0,width,height);

            ctx.fillStyle = background.fillStyle;
            ctx.globalAlpha = background.alpha
            ctx.beginPath();


            ctx.moveTo(0,height-delta);
            if (parent.expanded) {

                ctx.lineTo(20, height-delta);
                ctx.lineTo(30, height);
                ctx.lineTo(40, height-delta);
            }
            ctx.lineTo(width, height-delta);

            //   ctx.closePath();
            //  ctx.fill();
            ctx.stroke();
            //   ctx.restore();

        }
    }

    RowLayout {

        //anchors.fill : parent;
        width : parent.width

        Canvas {
            id: ex
            width : 50
            height : 30//parent.height

            property int delta: 10

            property variant fillStyle: {
                //        if (mouseArea.hovered)
                return Style.flow.background.button_hover;
                //      if (parent.isCurrent)
                //        return Style.flow.background.button_selected;
                //  if (!parent.isEnabled)
                //       return Style.flow.background.button_disabled;

                return Style.flow.background.button;
            }

            property variant strokeStyle: {
                //    if (mouseArea.hovered)
                //        return Style.flow.border.color_hover;
                //    if (parent.isCurrent)
                //        return Style.flow.border.color_selected;
                //    if (!parent.isEnabled)
                //        return Style.flow.border.color_disabled;

                return Style.flow.border.color;
            }

            onPaint: {
                var ctx = ex.getContext("2d");
                //   ctx.save();
                ctx.clearRect(0,0,ex.width, ex.height);
                ctx.strokeStyle = ex.strokeStyle;
                ctx.lineWidth = Style.flow.border.width;
                ctx.lineWidth = 2;

                //   ctx.fillStyle = "#fff";
                //   ctx.fillRect(0,0,width,height);

                ctx.fillStyle = ex.fillStyle;
                ctx.globalAlpha = ex.alpha
                ctx.beginPath();


                if (parent.parent.expanded) {
                    ctx.moveTo(10,height-delta);
                    ctx.lineTo(20, height-2*delta);
                    ctx.lineTo(30, height-delta);
                }
                else {
                    ctx.moveTo(10,delta);
                    ctx.lineTo(20, 2*delta);
                    ctx.lineTo(30, delta);


                }

                //   ctx.closePath();
                //  ctx.fill();
                ctx.stroke();
                //   ctx.restore();

            }
        }

        Text {
            id: label
            height : 30
            //            anchors.fill: parent
            elide: Text.ElideLeft
            color: Style.infoBlock.foreground.header
            font.family: Style.infoBlock.font.header
            font.pixelSize: Style.infoBlock.size.header
            font.bold: false
            font.capitalization: Font.AllUppercase
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }

        Item {
            Layout.fillWidth: true
        }


        Image {
            id: statussIcon
            //anchors.top:   parent.top
            //anchors.right: parent.right
            //anchors.topMargin:   2
            //anchors.rightMargin: 12

            property int imageSize : 30

            Layout.fillWidth: true
            Layout.preferredWidth: imageSize
            Layout.preferredHeight: imageSize
            Layout.minimumWidth: imageSize
            Layout.maximumWidth: imageSize
            Layout.minimumHeight: imageSize
            Layout.maximumHeight: imageSize

            width:  imageSize
            height: imageSize
            clip: true
            visible: parent.parent.showStatus // parent.isEnabled
            property int status: parent.parent.status
            source: {
                if (status === Status.invalid)
                    return "qrc:/icons/states/invalid.png";
                if (status === Status.valid)
                    return "qrc:/icons/states/valid.png";

                return "qrc:/icons/states/complete.png";
            }
            antialiasing: true
            //  mipmap: true
            // fillMode: Image.PreserveAspectFit
        }
    }

}
