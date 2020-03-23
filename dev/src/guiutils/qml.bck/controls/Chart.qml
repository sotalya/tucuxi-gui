import QtQuick 2.5
import ezechiel 1.0

Repeater {
    model: flowModel
    anchors.fill: parent
    Item {
        id: root
        anchors.fill: parent
        implicitWidth: 500
        implicitHeight: 310

        property alias isOffsetXEnabled: canvas.isOffsetXEnabled
        property alias isOffsetYEnabled: canvas.isOffsetYEnabled

        property alias times: canvas.times
        property alias apriori_x: canvas.apriori_x
        property alias apriori_y: canvas.apriori_y
        property alias aposteriori_x: canvas.aposteriori_x
        property alias aposteriori_y: canvas.aposteriori_y

        property alias unit: canvas.unit

//        property alias measures: canvas.measures

        function refresh () {
            canvas.requestPaint();
        }

        Item {
            id: impl
            anchors.fill: parent

            Canvas {
                id: canvas
                anchors.fill: parent

                //Canvas margins
                property int leftMargin:   75
                property int rightMargin:  75
                property int topMargin:    75
                property int bottomMargin: 75

                //Chart coordinates
                property int topLeftX: leftMargin
                property int topLeftY: topMargin

                property int topRightX: canvas.width - rightMargin
                property int topRightY: topMargin

                property int bottomLeftX: leftMargin
                property int bottomLeftY: canvas.height - bottomMargin

                property int bottomRightX: canvas.width - rightMargin
                property int bottomRightY: canvas.height - bottomMargin

                property int plotWidth:  topRightX   - topLeftX
                property int plotHeight: bottomLeftY - topLeftY

                //Chart properties
                property real scale:  1.0
                property real scaleMax: 5.00
                property real scaleMin: 0.05

                property real offsetX: 0.0
                property real offsetY: 0.0
                property bool isOffsetXEnabled: true
                property bool isOffsetYEnabled: true

                property real rotate: 0.0

                property string police: "sans-serif"
                property bool hasAprioriData: false
                property bool hasAposterioriData: false
                property bool hasMeasures: false
                property bool hasReverse: false

                property var currentPoints: []
                property var currentMeasure: null

                //Curves data
                property var times: model.times
                property var population_y: model.populationpts
                property var apriori_x: model.aprioritimes
                property var aposteriori_x: model.aposterioritimes
                property var apriori_y: model.aprioripts
                property var aposteriori_y: model.aposterioripts
                property var aprioripercs_y: model.aprioripercentiles                
                property var reverse_y: model.reverse
                property var reversetime: model.reversetime

                property real maxX: 0
                property real minX: 0
                property real maxY: 0
                property real minY: 0
                property real xRatio: 0
                property real yRatio: 0

                //ToDo: remove default unit
                property string unit: "ug/l"

//                property var measures: []

                //Canvas properties
                renderStrategy: Canvas.Threaded

                onRotateChanged: requestPaint()

                onTimesChanged:     requestPaint()
                onApriori_yChanged:     requestPaint()
                onAposteriori_yChanged: requestPaint()
                onReverse_yChanged: requestPaint()

                onUnitChanged: requestPaint()

//                onMeasuresChanged: requestPaint()

                Component.onCompleted: {
                    requestPaint()
                }

                onPaint: {
                    var ctx = canvas.getContext('2d');

                    console.log("painting!")
                    //Init context
                    initContext(ctx);
                    extents(ctx);

                    //Draw the content
                    if (hasAprioriData) {
                        for (var i = 0; i < aprioripercs_y.length; ++i) {
                            drawCurve(ctx, times, aprioripercs_y[i], "#bfbfbf");
                            console.log("drew apriori percs")
                        }
                        drawCurve(ctx, times, population_y, "green");
                        console.log("drew population");
                        drawCurve(ctx, apriori_x, apriori_y, "#cc0066");
                        console.log("drew apriori")
                    }
                    if (hasAposterioriData) {
                        drawCurve(ctx, aposteriori_x, aposteriori_y, "#60b6f1");
                        console.log("drew aposteriori")
                    }
                    if (hasMeasures) {
                        drawMeasures(ctx);
                    }
                    if (hasReverse) {
                        for (var i = 0; i < reverse_y.length; ++i) {
                            drawCurve(ctx, times, reverse_y[i], "orange");
                            console.log("drew reverse candidates")
                        }
                    }

                    clearBorder(ctx);
                    console.log("cleared border!")

                    //Draw the plot
                    drawAxis(ctx);
                    console.log("drew axes!")
                    drawAxisTicks(ctx);
                    console.log("drew ticks!")
                    drawAxisLabels(ctx);
                    console.log("drew labels!")

                    //Draw the labels
                    drawLegends(ctx);
                    console.log("drew legends!")
                    drawTooltips(ctx);
                    console.log("drew tooltips!")
                }

                function initContext(ctx) {

                    //Clear the canvas
                    ctx.clearRect(0, 0, canvas.width, canvas.height);

                    //Default variables
//                    hasData =  times.length     !== 0 &&
//                               apriori_y.length     !== 0 &&
//                               times.length !== 0 &&
//                               aposteriori_y.length !== 0;
                    hasAprioriData =     times.length     !== 0 &&
                                         apriori_y.length     !== 0;
                    hasAposterioriData = times.length     !== 0 &&
                                         aposteriori_y.length     !== 0;

                    console.log("hasAprioridata! " + hasAprioriData)
                    console.log("hasAposterioridata! " + hasAposterioriData)

                    hasMeasures = measureListModel.hasIndex(0,0)//measures.length   !== 0
                    console.log("hasmeasures: " + hasMeasures)

                    hasReverse = 		reverse_y.length !== 0
                    console.log("hasreverse: " + hasReverse)

                    //Default settings
                    ctx.font        = "12px " + police;
                    ctx.globalAlpha = 1.0;
                    ctx.lineWidth   = 1.0;
                    ctx.strokeStyle = "black";
                    ctx.fillStyle   = "black";
                    ctx.save();
                }

                function extents(ctx) {

                    maxX = Math.max(Math.max.apply(Math, times), Math.max.apply(Math, apriori_x), Math.max.apply(Math, aposteriori_x));
                    minX = Math.min(Math.min.apply(Math, times), Math.min.apply(Math, apriori_x), Math.min.apply(Math, aposteriori_x));
                    var aprioripercmax = 0;
                    var aprioripercmin = 0;
                    for (var i = 0; i < aprioripercs_y.length; ++i) {
                        aprioripercmax = Math.max.apply(Math, aprioripercs_y[i]);
                        aprioripercmin = Math.min.apply(Math, aprioripercs_y[i]);
                    }
                    maxY = Math.max(Math.max.apply(Math, apriori_y), Math.max.apply(Math, aposteriori_y), aprioripercmax);
                    minY = Math.min(Math.min.apply(Math, apriori_y), Math.min.apply(Math, aposteriori_y), aprioripercmin);

                    xRatio = plotWidth  / (maxX - minX);
                    yRatio = plotHeight / (maxY - minY);

                    console.log("maxX: " + maxX);
                    console.log("minX: " + minX);
                    console.log("maxY: " + maxY);
                    console.log("minY: " + minY);
                }

                function drawCurve(ctx, dataX, dataY, color) {

                    //Settings
                    ctx.strokeStyle = color;
                    ctx.lineWidth   = 2.5 / scale;
                    ctx.lineCap     = "round";
                    ctx.lineJoin    = "round";

                    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

                    //Constants

                    var mouseX = (mouseArea.mouseX - topLeftX - offsetX) / scale;

                    //Transformations
                    ctx.translate(topLeftX + offsetX, bottomLeftY - offsetY);
                    ctx.scale(canvas.scale, canvas.scale);
                    ctx.rotate(canvas.rotate);

                    //Draw the curve
                    ctx.beginPath();
                    for (var i = 0; i < dataX.length - 1; i++) {
                        var x1 =  (dataX[i]   - minX) * xRatio;
                        var y1 = -(dataY[i]   - minY) * yRatio;
                        var x2 =  (dataX[i+1] - minX) * xRatio;
                        var y2 = -(dataY[i+1] - minY) * yRatio;

                        ctx.moveTo(x1, y1);
                        ctx.lineTo(x2, y2);

//                        var res = checkLineIntersection(x1, y1, x2, y2, mouseX, offsetY / scale, mouseX, (-plotHeight + offsetY) / scale);
//                        if (res.onLine1 && res.onLine2 && mouseArea.isMouseOver) {
//                            var currentPoint = {
//                                x: res.x,
//                                y: res.y,
//                                value: ((-res.y) / yRatio - minY).toFixed(2) + " mg/l",
//                                time: formatDate(new Date(1000 * (res.x / xRatio + minX))),
//                                color: color
//                            }
//                            currentPoints.push(currentPoint);
//                        }
                    }
                    ctx.stroke();

                    ctx.restore();
                    ctx.save();
                }

                function drawMeasures(ctx) {

                    //Settings
                    ctx.fillStyle = "blue";
                    ctx.lineWidth = 1 / scale;

                    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

                    var mouseX = (mouseArea.mouseX - topLeftX    - offsetX) / scale;
                    var mouseY = (mouseArea.mouseY - bottomLeftY + offsetY) / scale;

                    //Transformations
                    ctx.translate(topLeftX + offsetX, bottomLeftY - offsetY);
                    ctx.scale(canvas.scale, canvas.scale);
                    ctx.rotate(canvas.rotate);

                    //Draw the samples
                    for (var i = 0; i < measureListModel.rowCount(); ++i) {//measures.length; ++i) {

                        console.log("measure time: " + measureListModel.data(measureListModel.index(i,0),MeasureListModel.SampleDateRole))
                        console.log("measure time: " + new Date(measureListModel.data(measureListModel.index(i,0),MeasureListModel.SampleDateRole)).getTime())
                        console.log("measure value: " + measureListModel.data(measureListModel.index(i,0),MeasureListModel.ValueRole))//need to use units to multiply
                        console.log("measure unit: " + measureListModel.data(measureListModel.index(i,0),MeasureListModel.UnitRole))
                        var x =  (new Date(measureListModel.data(measureListModel.index(i,0),MeasureListModel.SampleDateRole)).getTime() / 1000/*measures[i].moment.getTime() / 1000*/ - minX) * xRatio;
                        var y = -(/*measures[i].value */measureListModel.data(measureListModel.index(i,0),MeasureListModel.ValueRole) * 1000 - minY) * yRatio;

                        console.log("measurex: " + x)
                        console.log("measurey: " + y)
                        ctx.beginPath();
                        ctx.arc(x, y, 5 / scale, 0, 2 * Math.PI, true);
                        ctx.fill();
                        ctx.stroke();

                        if (checkPointProximity(x, y, mouseX, mouseY, true) && mouseArea.isMouseOver) {
                            currentMeasure = {
                                x: x,
                                y: y,
                                value: (measureListModel.data(measureListModel.index(i,0),MeasureListModel.ValueRole)).toFixed(2) + " " + measureListModel.data(measureListModel.index(i,0),MeasureListModel.UnitRole),
                                time: formatDate(new Date(measureListModel.data(measureListModel.index(i,0),MeasureListModel.SampleDateRole))),
                                color: "blue"
                            }
                        }
                    }

                    ctx.restore();
                    ctx.save();
                }

                function clearBorder(ctx) {
                    ctx.clearRect(0, 0, leftMargin, plotHeight + topMargin);
                    ctx.clearRect(0, bottomLeftY, plotWidth + leftMargin, bottomMargin);
                    ctx.clearRect(topRightX, topRightY, rightMargin, plotHeight + bottomMargin);
                    ctx.clearRect(topLeftX, 0, plotWidth + rightMargin, topMargin);
                }

                function drawAxis(ctx) {

                    //Constants
                    var arrowSize = 5;
                    var overrun   = 10;

                    //X-Axis
                    ctx.beginPath();
                    ctx.moveTo(bottomLeftX, bottomLeftY);
                    ctx.lineTo(bottomRightX + overrun, bottomRightY);
                    ctx.stroke();

                    //Y-Axis
                    ctx.beginPath();
                    ctx.moveTo(bottomLeftX, bottomLeftY);
                    ctx.lineTo(topLeftX, topLeftY - overrun);
                    ctx.stroke();

                    //X-Arrow
                    ctx.beginPath();
                    ctx.moveTo(bottomRightX + overrun, bottomRightY);
                    ctx.lineTo(bottomRightX + overrun - arrowSize, bottomRightY - arrowSize);
                    ctx.lineTo(bottomRightX + overrun - arrowSize, bottomRightY + arrowSize);
                    ctx.closePath();
                    ctx.stroke();
                    ctx.fill();

                    //Y-Arrow
                    ctx.beginPath();
                    ctx.moveTo(topLeftX, topLeftY - overrun);
                    ctx.lineTo(topLeftX - arrowSize, topLeftY - overrun + arrowSize);
                    ctx.lineTo(topLeftX + arrowSize, topLeftY - overrun + arrowSize);
                    ctx.closePath();
                    ctx.stroke();
                    ctx.fill();

                    ctx.restore();
                    ctx.save();
                }

                function drawAxisTicks(ctx) {

                    //Settings
                    ctx.textAlign    = "left";
                    ctx.textBaseline = "middle";
                    ctx.font         = "10px " + police;

                    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

                    var tickSize = 10;
                    var tickSpacing = 0.1 * Math.pow(10, -Math.floor(Math.log(scale)));

                    //Draw y-ticks
                    var down = -1.0 / scale + offsetY;
                    var up = 1.0 / scale + offsetY;
                    var down_i = Math.ceil(down / tickSpacing);
                    var up_i = Math.floor(up / tickSpacing);
                    var remy = down_i * tickSpacing - down;
                    var firstTickY = -1.0 + remy * scale;

                    var tickOffsetY = bottomLeftY - 1.0 - firstTickY;
                    var tickIncrementY = tickSpacing * scale * plotHeight / 2;

                    var nbTicksY = up_i - down_i + 1;
                    if (nbTicksY > 21)
                        nbTicksY = 21;

                    for (var i = 0; i < nbTicksY; i++) {
                        var y = tickOffsetY - i * tickIncrementY;
                        var tickScaleY = ((i + down_i) % 10) ? 0.5 : 1;

                        ctx.beginPath();
                        ctx.moveTo(bottomLeftX, y);
                        ctx.lineTo(bottomLeftX - tickSize * tickScaleY, y);
                        ctx.stroke();

                        if (hasAprioriData)
                            ctx.fillText((minY + (0.5 * i * tickSpacing - offsetY / plotHeight) * (maxY - minY)).toFixed(2), bottomLeftX - tickSize * 5.5, y);
                    }

                    //Draw x-ticks
                    var left = -1.0 / scale - offsetX;
                    var right = 1.0 / scale - offsetX;
                    var left_i = Math.ceil(left / tickSpacing);
                    var right_i = Math.floor(right / tickSpacing);
                    var rem = left_i * tickSpacing - left;
                    var firstTickX = -1.0 + rem * scale;

                    var tickOffsetX = bottomLeftX + 1.0 + firstTickX;
                    var tickIncrementX = tickSpacing * scale * plotWidth / 2;

                    var nbTicksX = right_i - left_i + 1;
                    if (nbTicksX > 21)
                        nbTicksX = 21;

                    for (i = 0; i < nbTicksX; i++) {
                        var x = tickOffsetX + i * tickIncrementX;
                        var tickscalex = ((i + left_i) % 10) ? 0.5 : 1;

                        ctx.beginPath();
                        ctx.moveTo(x, bottomLeftY);
                        ctx.lineTo(x, bottomLeftY + tickSize * tickscalex);
                        ctx.stroke();

                        if (hasAprioriData) {
                            var date = new Date(1000 * (minX + (0.5 * i * tickSpacing - offsetX / plotWidth) * (maxX - minX)));
                            ctx.translate(x, bottomLeftY + tickSize * 1.5);
                            ctx.rotate(Math.PI/5);
                            ctx.fillText(formatDate(date), 0, 0);
                            ctx.rotate(-Math.PI/5);
                            ctx.translate(-x, -(bottomLeftY + tickSize * 1.5));
                        }
                    }

                    ctx.restore();
                    ctx.save();
                }

                function drawAxisLabels(ctx) {
                    if (!hasAprioriData)
                        return;

                    //Settings
                    ctx.font = "bold 12px " + police;

                    //Draw x-axis label
                    ctx.textAlign    = "left";
                    ctx.textBaseline = "middle";
                    ctx.fillText("Time", leftMargin + plotWidth + 15, topMargin + plotHeight);

                    //Draw y-axis label
                    ctx.textAlign    = "center";
                    ctx.textBaseline = "bottom";
                    ctx.fillText(unit, leftMargin, topMargin - 15);

                    ctx.restore();
                    ctx.save();
                }

                function drawLegends(ctx) {
                    if (!hasAprioriData)
                        return;

                    //Settings
                    ctx.textAlign    = "left";
                    ctx.textBaseline = "Alphabetic";
                    ctx.font         = "12px " + police;

                    //Constants
                    var legends = [];
                    var legendsWidth = [];

                    var populationText  = "Population";
                    var aprioriText     = "A priori";
                    var aprioripercText = "Population Percentiles"
                    var aposterioriText = "A posteriori";

                    var hasPopulation  = false;
                    var hasApriori     = times.length     !== 0 && apriori_y.length     !== 0;
                    var hasAposteriori = times.length !== 0 && aposteriori_y.length !== 0;

                    if (hasPopulation) {
                        legends.push( {text: populationText, color: "green"} );
                        legendsWidth.push(ctx.measureText(populationText).width);
                    }
                    if (hasApriori) {
                        legends.push( {text: aprioriText, color: "#c3562b"} );
                        legendsWidth.push(ctx.measureText(aprioriText).width);
                        legends.push( {text: aprioripercText, color: "#32293a"} );
                        legendsWidth.push(ctx.measureText(aprioripercText).width);
                    }
                    if (hasAposteriori) {
                        legends.push( {text: aposterioriText, color: "#60b6f1"} );
                        legendsWidth.push(ctx.measureText(aposterioriText).width);
                    }

                    var internalSpacing = 5;
                    var externalSpacing = 10;

                    var colorSize = 10;
                    var boxHeight = 20;
                    var boxWidth  = internalSpacing * 3 + colorSize + Math.max.apply(Math, legendsWidth);

                    var initialX = plotWidth / 2 + leftMargin - (boxWidth / 2) * legends.length - (externalSpacing / 2) * (legends.length - 1);
                    var initialY = topMargin / 2;

                    //Draw legends
                    for (var i = 0; i < legends.length; i++) {
                        var x = initialX + (boxWidth + externalSpacing) * i;
                        var y = initialY - boxHeight / 2;

                        ctx.fillStyle = legends[i].color;

                        ctx.beginPath();
                        ctx.rect(x, y, boxWidth, boxHeight);
                        ctx.stroke();

                        ctx.beginPath();
                        ctx.rect(x + internalSpacing, y + (boxHeight - colorSize) / 2, colorSize, colorSize);
                        ctx.stroke();
                        ctx.fill();

                        ctx.fillStyle = "black";
                        ctx.beginPath();
                        ctx.fillText(legends[i].text, x + internalSpacing * 2 + colorSize, y + (boxHeight + colorSize) / 2);
                    }

                    ctx.restore();
                    ctx.save();
                }

                //ToDo: Refactor drawTooltips (too long, not generic enough, possible other tooptips will be added with time)

                function drawTooltips(ctx) {

                    //Settings
                    ctx.font         = "10px " + police;
                    ctx.textAlign    = "left";
                    ctx.textBaseline = "Alphabetic";

                    //Constants
                    var valueText = "Value:";
                    var timeText  = "Time:";

                    var labelsWidth = Math.max(ctx.measureText(valueText).width, ctx.measureText(timeText).width) + 4;
                    var valuesWidth, tooltipWidth, tooltipHeight, x, y;

                    var previousY = [];

                    //Transformations
                    ctx.translate(topLeftX + offsetX, bottomLeftY - offsetY);
                    ctx.rotate(canvas.rotate);

                    for (var i = 0; i < currentPoints.length; ++i) {

                        ctx.beginPath();
                        ctx.arc(currentPoints[i].x * scale, currentPoints[i].y * scale, 4, 0, 2 * Math.PI, true);
                        ctx.fillStyle = currentPoints[i].color;
                        ctx.fill();
                        ctx.stroke();

                        if (!currentMeasure) {
                            valuesWidth = Math.max(ctx.measureText(currentPoints[i].value).width, ctx.measureText(currentPoints[i].time).width) + 2;

                            tooltipWidth = (labelsWidth + valuesWidth);
                            tooltipHeight = 30;

                            x = currentPoints[i].x * scale - tooltipWidth  / 2;
                            y = currentPoints[i].y * scale - tooltipHeight - 10;

                            for (var j = 0; j < previousY.length; ++j) {
                                if (y + tooltipHeight + 5 > previousY[j] && y < previousY[j] + tooltipHeight + 5)
                                    y = previousY[j] - tooltipHeight - 5;
                            }

                            previousY.push(y);

                            ctx.beginPath();
                            ctx.rect(x, y, tooltipWidth, tooltipHeight);

                            ctx.globalAlpha = 0.5;
                            ctx.fill();
                            ctx.globalAlpha = 1.0;
                            ctx.stroke();

                            ctx.fillStyle = "black";
                            ctx.fillText(valueText, x + 2, y + 12);
                            ctx.fillText(currentPoints[i].value, x + labelsWidth, y + 12);
                            ctx.fillText(timeText, x + 2, y + 26);
                            ctx.fillText(currentPoints[i].time, x + labelsWidth, y + 26);
                        }
                    }
                    currentPoints.length = 0;

                    if (currentMeasure) {
                        valuesWidth = Math.max(ctx.measureText(currentMeasure.value).width, ctx.measureText(currentMeasure.time).width) + 2;

                        tooltipWidth = (labelsWidth + valuesWidth);
                        tooltipHeight = 30;

                        x = currentMeasure.x * scale - tooltipWidth  / 2;
                        y = currentMeasure.y * scale - tooltipHeight - 10;

                        ctx.beginPath();
                        ctx.rect(x, y, tooltipWidth, tooltipHeight);

                        ctx.globalAlpha = 0.5;
                        ctx.fillStyle = currentMeasure.color;
                        ctx.fill();
                        ctx.globalAlpha = 1.0;
                        ctx.stroke();

                        ctx.fillStyle = "black";
                        ctx.fillText(valueText, x + 2, y + 12);
                        ctx.fillText(currentMeasure.value, x + labelsWidth, y + 12);
                        ctx.fillText(timeText, x + 2, y + 26);
                        ctx.fillText(currentMeasure.time, x + labelsWidth, y + 26);
                    }
                    currentMeasure = null;

                    ctx.restore();
                    ctx.save();
                }

                function checkLineIntersection(line1StartX, line1StartY, line1EndX, line1EndY, line2StartX, line2StartY, line2EndX, line2EndY) {
                    var denominator, a, b, numerator1, numerator2, result = {
                        x: null,
                        y: null,
                        onLine1: false,
                        onLine2: false
                    };

                    denominator = ((line2EndY - line2StartY) * (line1EndX - line1StartX)) - ((line2EndX - line2StartX) * (line1EndY - line1StartY));
                    if (denominator == 0) {
                        return result;
                    }

                    a = line1StartY - line2StartY;
                    b = line1StartX - line2StartX;
                    numerator1 = ((line2EndX - line2StartX) * a) - ((line2EndY - line2StartY) * b);
                    numerator2 = ((line1EndX - line1StartX) * a) - ((line1EndY - line1StartY) * b);
                    a = numerator1 / denominator;
                    b = numerator2 / denominator;

                    result.x = line1StartX + (a * (line1EndX - line1StartX));
                    result.y = line1StartY + (a * (line1EndY - line1StartY));

                    if (a > 0 && a < 1) {
                        result.onLine1 = true;
                    }

                    if (b > 0 && b < 1) {
                        result.onLine2 = true;
                    }

                    return result;
                }

                function checkPointProximity(x1, y1, x2, y2, useScale) {
                    var tolerance = 10;
                    if (useScale)
                        tolerance /= scale;

                    return Math.abs(x1 - x2) <= tolerance && Math.abs(y1 - y2) <= tolerance;
                }

                function formatDate(date) {
                    var hours = date.getHours();
                    var mins  = date.getMinutes();
                    var sTime = (hours < 10 ? "0" + hours : hours) + ":" + (mins < 10 ? "0" + mins : mins);

                    var day   = date.getDate();
                    var month = date.getMonth() + 1;
                    var year  = date.getFullYear();
                    var sDate = (day < 10 ? "0" + day : day) + "." + (month < 10 ? "0" + month : month) + "." + year;

                    return sTime + " " + sDate;
                }
            }

            //ToDo: horizontal offset and zoom only
            MouseArea {
                id: mouseArea
                anchors.fill: parent

                hoverEnabled: true

                property bool isMouseOver: false

                property double pressX
                property double pressY

                property double previousOffsetX: 0
                property double previousOffsetY: 0

                onDoubleClicked: {
                    canvas.offsetX = 0;
                    canvas.offsetY = 0;
                    previousOffsetX = 0;
                    previousOffsetY = 0;

                    canvas.scale = 1;

                    canvas.requestPaint();
                }

                onPressed: {
                    pressX = mouse.x;
                    pressY = mouse.y;
                }

                onReleased: {
                    previousOffsetX = canvas.offsetX;
                    previousOffsetY = canvas.offsetY;
                }

                onPositionChanged: {
                    if (!canvas.hasAprioriData)
                        return;

                    if (pressed && containsMouse) {
                        canvas.offsetX = canvas.isOffsetXEnabled ? previousOffsetX +  mouse.x - pressX  : 0;
                        canvas.offsetY = canvas.isOffsetYEnabled ? previousOffsetY - (mouse.y - pressY) : 0;
                    }

                    isMouseOver = mouse.x >= canvas.topLeftX && mouse.x <= canvas.topRightX &&
                                  mouse.y >= canvas.topLeftY && mouse.y <= canvas.bottomLeftY;

                    canvas.requestPaint();
                }

                onWheel: {
                    console.log("wheeld")
                    if (!canvas.hasAprioriData)
                        return;

                    if (wheel.angleDelta.y < 0)
                        zoomIn();
                    else
                        zoomOut();

                    canvas.requestPaint();
                }

                function zoomIn() {
                    if (canvas.scale <= canvas.scaleMin)
                        canvas.scale =  canvas.scaleMin;
                    else
                        canvas.scale -= 0.05;
                }

                function zoomOut() {
                    if (canvas.scale >= canvas.scaleMax)
                        canvas.scale =  canvas.scaleMax;
                    else
                        canvas.scale += 0.05;
                }
            }
        }
    }
}
