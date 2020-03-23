import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0


Item {
    //    visible : title.expanded
    //    anchors.top : stack.bottom
    Layout.minimumHeight:   6 // Style.infoBlock.lines.width
    Layout.minimumWidth: parent.width
    //    border.color: Style.infoBlock.lines.color
    //    border.width: Style.infoBlock.lines.width



    Canvas {
        id: ex
        width : parent.width
        height : parent.height//parent.height


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
            ctx.lineWidth = 6;

            //   ctx.fillStyle = "#fff";
            //   ctx.fillRect(0,0,width,height);

            ctx.fillStyle = ex.fillStyle;
            ctx.globalAlpha = ex.alpha
            ctx.beginPath();

            ctx.moveTo(0,0);
            ctx.lineTo(width,0);

            //   ctx.closePath();
            //  ctx.fill();
            ctx.stroke();
            //   ctx.restore();

        }
    }
}
