import QtQuick 2.5

import ezechiel 1.0

Canvas {
    id: canvas
    objectName: "canvas"
        anchors.fill: parent
        implicitWidth: 500
        implicitHeight: 310

        function refresh () {
            canvas.requestPaint();
        }


        states: [
        State {
          name: "patients"
          PropertyChanges { target: canvas; show: [0,0,0,0,0,0]}
        },
        State {
          name: "drugs"
          PropertyChanges { target: canvas; show: [0,0,0,0,0,0]}
        },
        State {
          name: "dosages"
          PropertyChanges { target: canvas; show: [1,0,0,0,0,0]}
        },
        State {
          name: "covariates"
          PropertyChanges { target: canvas; show: [0,1,0,0,0,0]}
        },
        State {
          name: "measures"
          PropertyChanges { target: canvas; show: [1,0,1,0,1,0]}
        },
        State {
          name: "targets"
          PropertyChanges { target: canvas; show: [0,0,1,0,1,1]}
        },
        State {
          name: "adjustments"
          PropertyChanges { target: canvas; show: [0,0,1,1,1,1]}
        },
        State {
          name: "report"
          PropertyChanges { target: canvas; show: reportshow }
        },
        State {
          name: "validation"
          PropertyChanges { target: canvas; show: [1,1,1,1,1,1]}
        }

        ]

    antialiasing: true

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
    property bool hasPopulationData: false
    property bool hasAprioriData: false
    property bool hasAposterioriData: false
    property bool hasMeasures: false
    property bool hasReverse: false
    property bool hasTargets: false

    property var currentPoints: []
    property var currentMeasure: null

    //Curves data
    property var population_x:  []
    property var population_y:  []
    property var apriori_x:     []
    property var apriori_y:     []
    property var aposteriori_x: []
    property var aposteriori_y: []
    property real timestart: Date.now();

    property var populationpercs_x:  []
    property var populationpercs_y:  []
    property var aprioripercs_x:     []
    property var aprioripercs_y:     []
    property var aposterioripercs_x: []
    property var aposterioripercs_y: []

    property var reverse_x: []
    property var reverse_y: []

    property real maxX: 0
    property real minX: 0
    property real maxY: 0
    property real minY: 0
    property real xRatio: 0
    property real yRatio: 0

    function publishImage() {
    grabToImage(function(result) {interpretationController.getImage(result.image)})
    }

    //ToDo: remove default unit
    property string unit: "ug/l"
    property var unitefforder: [1, 1, 1, 1, 1, 1]
    onUnitChanged: {
    for (var i = 0; i < measureListModel.rowCount(); ++i) {
        measureListModel.setData(measureListModel.index(i,0), unit, MeasureListModel.UnitRole);
    }
    interpretationController.setDefaultMeasureUnit(unit);
    requestPaint();
    }

    property var show: [0, 0, 0, 0, 0, 0]
    onShowChanged: console.log("changed show to " + show);
    property var reportshow: [1, 1, 1, 1, 1, 1]
    onReportshowChanged: console.log("changed show to " + reportshow);

    //indices
    property int pop: 0
    property int apr: 1
    property int apo: 2
    property int rev: 3
    property int mea: 4
    property int tar: 5

    Connections {
    target: flow
    onChangedTab: {
        root.state = root.states[flow.currentIndex].name;
        refresh()
    }
    }

    Connections {
    target: populationPredictionModel
    onModelReset: {
        canvas.population_x = populationPredictionModel.data(populationPredictionModel.index(0, 0), PredictionModel.TimeRole);
        canvas.population_y = populationPredictionModel.data(populationPredictionModel.index(0, 0), PredictionModel.DataRole);
    }
    }

    Connections {
    target: aPrioriPredictionModel
    onModelReset: {
        canvas.apriori_x = aPrioriPredictionModel.data(aPrioriPredictionModel.index(0, 0), PredictionModel.TimeRole);
        canvas.apriori_y = aPrioriPredictionModel.data(aPrioriPredictionModel.index(0, 0), PredictionModel.DataRole);
    }
    }

    Connections {
    target: aPosterioriPredictionModel
    onModelReset: {
        canvas.aposteriori_x = aPosterioriPredictionModel.data(aPosterioriPredictionModel.index(0, 0), PredictionModel.TimeRole);
        canvas.aposteriori_y = aPosterioriPredictionModel.data(aPosterioriPredictionModel.index(0, 0), PredictionModel.DataRole);
    }
    }

    Connections {
    target: populationPercentilesModel
    onModelReset: {
        canvas.populationpercs_x = populationPercentilesModel.data(populationPercentilesModel.index(0, 0), PercentilesModel.TimeListRole);
        canvas.populationpercs_y = populationPercentilesModel.data(populationPercentilesModel.index(0, 0), PercentilesModel.DataListRole);
    }
    }

    Connections {
    target: aPrioriPercentilesModel
    onModelReset: {
        canvas.aprioripercs_x = aPrioriPercentilesModel.data(aPrioriPercentilesModel.index(0, 0), PercentilesModel.TimeListRole);
        canvas.aprioripercs_y = aPrioriPercentilesModel.data(aPrioriPercentilesModel.index(0, 0), PercentilesModel.DataListRole);
    }
    }

    Connections {
    target: aPosterioriPercentilesModel
    onModelReset: {
        canvas.aposterioripercs_x = aPosterioriPercentilesModel.data(aPosterioriPercentilesModel.index(0, 0), PercentilesModel.TimeListRole);
        canvas.aposterioripercs_y = aPosterioriPercentilesModel.data(aPosterioriPercentilesModel.index(0, 0), PercentilesModel.DataListRole);
    }
    }

    Connections {
    target: reversePredictionModel
    onModelReset: {
        canvas.reverse_x = reversePredictionModel.data(reversePredictionModel.index(0, 0), ReverseModel.TimeListRole);
        canvas.reverse_y = reversePredictionModel.data(reversePredictionModel.index(0, 0), ReverseModel.DataListRole);
    }
    }

    //Canvas properties
    renderStrategy: Canvas.Threaded
    renderTarget: Canvas.FramebufferObject


    onRotateChanged: requestPaint()

    onPopulation_xChanged:  requestPaint()
    onApriori_yChanged:     requestPaint()
    onAposteriori_yChanged: requestPaint()
    onReverse_yChanged:     requestPaint()

    Component.onCompleted: {
    requestPaint()
    }

    onPaint: {
    var ctx = canvas.getContext('2d');
    var stt = root.state;
    var shw = show;
    var rshw = reportshow;

    //console.log("painting!")
    //Init context
    initContext(ctx);
    ctx.save();
    extents(ctx);
    var colors = ["#bfbfbf", "#21B035", "#cc0066", "#cc0066", "#60b6f1", "orange", "blue", "lightgreen", "pink"];
    var popcolors = ["#0B360E", "#17701E", "#2CD438"];
    var aprcolors = ["#E84229", "#B53A70", "#FF4200"];
    var apocolors = ["#4A24B5", "#5848B5", "#5168B5"];
    var nographtext = "Welcome to Tucuxi! Graphs of predicted concentrations will be shown here.";
    var nographdrugtext = interpretationController.getGraphDrugData();

    //Draw the content
    if (hasPopulationData) {
        if (show[pop]) {
        for (var i = 0; i < populationpercs_y.length; ++i) {
        drawCurve(ctx, population_x, populationpercs_y[i], colors[7]);
        ctx.restore();
        ctx.save();
        }

        drawCurve(ctx, population_x, population_y, popcolors[0]);
        ctx.restore();
        ctx.save();

        colorRegionBtwCurves(ctx, population_x, populationpercs_y[0], populationpercs_y[3], popcolors[2]);
        ctx.restore();
        ctx.save();
        colorRegionBtwCurves(ctx, population_x, populationpercs_y[1], populationpercs_y[2], popcolors[1]);
        ctx.restore();
        ctx.save();
        addToCurrentPoints(ctx,population_x, population_y, colors[1], pop);
        drawNonPlotArea(ctx, colors, pop);
        }
        if (hasAprioriData) {
        if (show[apr]) {
            for (var i = 0; i < aprioripercs_y.length; ++i) {
            drawCurve(ctx, population_x, aprioripercs_y[i], colors[8]);
            ctx.restore();
            ctx.save();
            }
            drawCurve(ctx, apriori_x, apriori_y, colors[2]);
            ctx.restore();
            ctx.save();

            colorRegionBtwCurves(ctx, population_x, aprioripercs_y[0], aprioripercs_y[3], aprcolors[2]);
            ctx.restore();
            ctx.save();
            colorRegionBtwCurves(ctx, population_x, aprioripercs_y[1], aprioripercs_y[2], aprcolors[1]);
            ctx.restore();
            ctx.save();

            // ToDo ///////////////////////////////////////////////////
            // The addToCurrentPoints
            // needs to be optimized and extended to all three curves
            // (population, apriori, aposteriori)
            // ////////////////////////////////////////////////////////

            addToCurrentPoints(ctx,apriori_x, apriori_y, colors[2], apr);
            drawNonPlotArea(ctx, colors, apr);
        }
        if (hasMeasures) {
            if (hasAposterioriData) {
            if (show[apo]) {
                for (var i = 0; i < aposterioripercs_y.length; ++i) {
                drawCurve(ctx, population_x, aposterioripercs_y[i], colors[6]);
                ctx.restore();
                ctx.save();
                //console.log("drew apriori percs")
                }
                drawCurve(ctx, aposteriori_x, aposteriori_y, colors[4]);
                ctx.restore();
                ctx.save();

                //                    colorRegionBtwCurves(ctx, population_x, aposterioripercs_y[0], aposterioripercs_y[3], apocolors[2]);
                //                    ctx.restore();
                //                    ctx.save();
                //                    colorRegionBtwCurves(ctx, population_x, aposterioripercs_y[1], aposterioripercs_y[2], apocolors[1]);
                //                    ctx.restore();
                //                    ctx.save();

                addToCurrentPoints(ctx,aposteriori_x, aposteriori_y, colors[4], apo);
                //console.log("drew aposteriori")
                drawMeasures(ctx);
                ctx.restore();
                ctx.save();
                drawNonPlotArea(ctx, colors, apo);
            }
            }
        }

        if (hasReverse && show[rev]) {
            for (var i = 0; i < reverse_y.length; ++i) {
            drawCurve(ctx, reverse_x[i], reverse_y[i], colors[5]);
            ctx.restore();
            ctx.save();
            //console.log("drew reverse candidates")
            }
            drawNonPlotArea(ctx, colors, rev);
        }

        if (hasTargets && show[tar]) {
            drawTargets(ctx);
            ctx.restore();
            ctx.save();
        }

        }
    }
    if (!show[pop] && !show[apr] && !show[apo]) {
        if (nographdrugtext) {
        drawSoftwareDescription(ctx, nographdrugtext);
        }
        else {
        drawSoftwareDescription(ctx, nographtext);
        }
    }

    }

    function drawNonPlotArea(ctx, colors, index) {
        clearBorder(ctx);
        //Draw the plot
        drawAxis(ctx);
        ctx.restore();
        ctx.save();
        drawAxisTicks(ctx, index);
        drawAxisLabels(ctx);

        //Draw the labels
        drawLegends(ctx, colors);
        drawTooltips(ctx, index);
    }

    function drawSoftwareDescription(ctx, text) {

    ctx.fillText(text, 100, 50);
    }

    function colorRegionBtwCurves(ctx, dataX, dataY, dataYY, color) {

    ctx.globalAlpha = 0.2;
    ctx.lineWidth   = 2.0 / scale;
    ctx.lineCap     = "round";
    ctx.lineJoin    = "round";

    ctx.translate(topLeftX + offsetX, bottomLeftY);// - offsetY);
    ctx.scale(canvas.scale, canvas.scale);
    ctx.rotate(canvas.rotate);
    ctx.beginPath();
//                var dataX = population_x;
//                var dataY = population_y;
//                var dataYY = populationpercs_y[0];
    var x1, x2, y1, y2;
    var j = 0;
    x1 =  (dataX[j]   - minX) * xRatio;
    y1 = -(dataY[j]   - minY) * yRatio;
    x2 =  (dataX[j+1] - minX) * xRatio;
    y2 = -(dataY[j+1] - minY) * yRatio;
    ctx.moveTo(x1, y1);
    for (; j < dataX.length - 1; j++) {
    x1 =  (dataX[j]   - minX) * xRatio;
    y1 = -(dataY[j]   - minY) * yRatio;
    x2 =  (dataX[j+1] - minX) * xRatio;
    y2 = -(dataY[j+1] - minY) * yRatio;
    ctx.lineTo(x2, y2);
//                console.log(x2, y2);

    }
//                console.log("switch now");
    ctx.lineTo(x2, y2);
    x1 =  (dataX[j]   - minX) * xRatio;
    y1 = -(dataYY[j]   - minY) * yRatio;
    ctx.lineTo(x1, y1);
//                console.log(x1, y1);
    for (; j > -1; j--) {
    x1 =  (dataX[j]   - minX) * xRatio;
    y1 = -(dataYY[j]   - minY) * yRatio;
    x2 =  (dataX[j-1] - minX) * xRatio;
    y2 = -(dataYY[j-1] - minY) * yRatio;
    ctx.lineTo(x2, y2);
//                console.log(x1, y1);

    }
    ctx.closePath();
    ctx.fillStyle = color;
    ctx.fill();
//                    ctx.stroke();

    }

    function initContext(ctx) {

    //Clear the canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    //Default variables
//                    hasData =  times.length     !== 0 &&
//                               apriori_y.length     !== 0 &&
//                               times.length !== 0 &&
//                               aposteriori_y.length !== 0;
    hasPopulationData =  population_x.length     !== 0
    hasAprioriData = apriori_y.length     !== 0;
    hasAposterioriData = aposteriori_y.length     !== 0;

    //console.log("hasAprioridata! " + hasAprioriData)
    //console.log("hasAposterioridata! " + hasAposterioriData)

    hasMeasures = measureListModel.hasIndex(0,0)//measures.length   !== 0
    //console.log("hasmeasures: " + hasMeasures)

    hasReverse = 		reverse_y.length !== 0
    //console.log("hasreverse: " + hasReverse)
    hasTargets = targetListModel.hasIndex(0,0)

    //Default settings
    ctx.font        = "12px " + police;
    ctx.globalAlpha = 1.0;
    ctx.lineWidth   = 1.0;
    ctx.strokeStyle = "black";
    ctx.fillStyle   = "black";
    }

    function extents(ctx) {

    //X max

    maxX = Math.max(Math.max.apply(Math, population_x), Math.max.apply(Math, apriori_x), Math.max.apply(Math, aposteriori_x));
    minX = Math.min(Math.min.apply(Math, population_x), Math.min.apply(Math, apriori_x), Math.min.apply(Math, aposteriori_x));


    //Y max
    var aprioripercmax = 0;
    var aprioripercmin = 0;
    var aposterioripercmax = 0;
    var aposterioripercmin = 0;
    var populationpercmax = 0;
    var populationpercmin = 0;

    if (show[pop]) {
        for (var i = 0; i < populationpercs_y.length; ++i) {
        aprioripercmax = Math.max(aprioripercmax, Math.max.apply(Math, populationpercs_y[i]));
        aprioripercmin = Math.min(aprioripercmin, Math.min.apply(Math, populationpercs_y[i]));
        }
    }

    if (show[apr]) {
        for (var i = 0; i < aprioripercs_y.length; ++i) {
        aprioripercmax = Math.max(aprioripercmax, Math.max.apply(Math, aprioripercs_y[i]));
        aprioripercmin = Math.min(aprioripercmin, Math.min.apply(Math, aprioripercs_y[i]));
        }
    }

    if (show[apo]) {
        for (var i = 0; i < aposterioripercs_y.length; ++i) {
        aposterioripercmax = Math.max(aprioripercmax, Math.max.apply(Math, aposterioripercs_y[i]));
        aposterioripercmin = Math.min(aprioripercmin, Math.min.apply(Math, aposterioripercs_y[i]));
        }
    }

    maxY = Math.max(Math.max.apply(Math, population_y), Math.max.apply(Math, apriori_y), Math.max.apply(Math, aposteriori_y), aprioripercmax, aposterioripercmax, populationpercmax);
    minY = 0;//Math.min(Math.min.apply(Math, population_y), Math.min.apply(Math, apriori_y), Math.min.apply(Math, aposteriori_y), aprioripercmin, aposterioripercmin, populationpercmin);

    maxY = Math.pow(10, Math.ceil(Math.log(maxY) / Math.LN10));

    xRatio = plotWidth  / (maxX - minX);
    yRatio = plotHeight / (maxY - minY);

//                console.log("maxX: " + maxX);
//                console.log("minX: " + minX);
//                console.log("maxY: " + maxY);
//                console.log("minY: " + minY);
    }

    function drawCurve(ctx, dataX, dataY, color) {

    //Settings
    ctx.strokeStyle = color;
    ctx.lineWidth   = 2.0 / scale;
    ctx.lineCap     = "round";
    ctx.lineJoin    = "round";

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    //Constants


    //Transformations
    ctx.translate(topLeftX + offsetX, bottomLeftY);// - offsetY);
    ctx.scale(canvas.scale, canvas.scale);
    ctx.rotate(canvas.rotate);

    //Draw the curve
    ctx.path = "";
    //               ctx.beginPath();
    for (var i = 0; i < dataX.length - 2; i++) {
        var x1 =  (dataX[i]   - minX) * xRatio;
        var y1 = -(dataY[i]   - minY) * yRatio;
        var x2 =  (dataX[i+1] - minX) * xRatio;
        var y2 = -(dataY[i+1] - minY) * yRatio;
//                    var x3 =  (dataX[i+2] - minX) * xRatio;
//                    var y3 = -(dataY[i+2] - minY) * yRatio;

//                    var xc = (x2 + x3) / 2;
//                    var yc = (y2 + y3) / 2;

//                    if (i == 0) {
//                         ctx.path = ctx.path + "M " + x1 + " " + y1;
//                    }

        //using svg to smoothen the curve
//                    ctx.path = ctx.path + " S " + x2 + " " + y2 + " " + xc + " " + yc;
        ctx.moveTo(x1, y1);
        ctx.lineTo(x2, y2);

    }
//                ctx.shadowBlur = 0.5;
//                ctx.shadowColor = color;
    ctx.stroke();

    }

    function addToCurrentPoints(ctx, dataX, dataY, color, index) {
    for (var i = 0; i < dataX.length - 2; i++) {

        var x1 =  (dataX[i]   - minX) * xRatio;
        var y1 = -(dataY[i]   - minY) * yRatio;
        var x2 =  (dataX[i+1] - minX) * xRatio;
        var y2 = -(dataY[i+1] - minY) * yRatio;

        var mouseX = (mouseArea.mouseX - topLeftX - offsetX) / scale;

        var res = checkLineIntersection(x1, y1, x2, y2, mouseX, 0/*offsetY / scale*/, mouseX, (-plotHeight/* + offsetY*/) / scale);
        if (res.onLine1 && res.onLine2 && mouseArea.isMouseOver) {
        var currentPoint = {
            x: res.x,
            y: res.y,
            value: Math.max(0,Math.round(unitefforder[index] * ((-res.y) / yRatio - minY))),
            time: formatDate(new Date(1000 * (res.x / xRatio + minX))),
            color: color
        }
        currentPoints.push(currentPoint);
        }
    }
    }

    function drawMeasures(ctx) {

    if (!show[apo]) {return;}
    //Settings
    ctx.fillStyle = "blue";
    ctx.lineWidth = 1 / scale;

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    var mouseX = (mouseArea.mouseX - topLeftX    - offsetX) / scale;
    var mouseY = (mouseArea.mouseY - bottomLeftY/* + offsetY*/) / scale;

    //Transformations
    ctx.translate(topLeftX + offsetX, bottomLeftY/* - offsetY*/);
    ctx.scale(canvas.scale, canvas.scale);
    ctx.rotate(canvas.rotate);

    //Draw the samples
    for (var i = 0; i < measureListModel.rowCount(); ++i) {//measures.length; ++i) {

        //console.log("measure time: " + measureListModel.data(measureListModel.index(i,0),MeasureListModel.SampleDateRole))
        //console.log("measure time: " + new Date(measureListModel.data(measureListModel.index(i,0),MeasureListModel.SampleDateRole)).getTime())
        //console.log("measure value: " + measureListModel.data(measureListModel.index(i,0),MeasureListModel.ValueRole))//need to use units to multiply
        //console.log("measure unit: " + measureListModel.data(measureListModel.index(i,0),MeasureListModel.UnitRole))
        var x =  (new Date(measureListModel.data(measureListModel.index(i,0),MeasureListModel.SampleDateRole)).getTime() / 1000/*measures[i].moment.getTime() / 1000*/ - minX) * xRatio;
        var y = -(/*measures[i].value */measureListModel.data(measureListModel.index(i,0),MeasureListModel.ValueRole) * 1000 - minY) * yRatio;

        //console.log("measurex: " + x)
        //console.log("measurey: " + y)
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

    }

    function drawTargets(ctx) {
    if (!show[tar]) {return;}
    //Settings
    ctx.fillStyle = "blue";
    ctx.lineWidth = 1 / scale;

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    var mouseX = (mouseArea.mouseX - topLeftX    - offsetX) / scale;
    var mouseY = (mouseArea.mouseY - bottomLeftY/* + offsetY*/) / scale;

    //Transformations
    ctx.translate(topLeftX + offsetX, bottomLeftY/* - offsetY*/);
    ctx.scale(canvas.scale, canvas.scale);
    ctx.rotate(canvas.rotate);

    //Draw the samples
    for (var i = 0; i < targetListModel.rowCount(); ++i) {//targets.length; ++i) {

        //console.log("target time: " + targetListModel.data(targetListModel.index(i,0),TargetListModel.SampleDateRole))
        //console.log("target time: " + new Date(targetListModel.data(targetListModel.index(i,0),TargetListModel.SampleDateRole)).getTime())
        //console.log("target value: " + targetListModel.data(targetListModel.index(i,0),TargetListModel.ValueRole))//need to use units to multiply
        //console.log("target unit: " + targetListModel.data(targetListModel.index(i,0),TargetListModel.UnitRole))
//                    var x_mean =  (new Date(targetListModel.data(targetListModel.index(i,0),TargetListModel.TBestRole)).getTime() / 1000/*targets[i].moment.getTime() / 1000*/ - minX) * xRatio;
        var y_tmean = -(/*targets[i].value */targetListModel.data(targetListModel.index(i,0),TargetListModel.CBestValueRole) - minY) * yRatio;
        var y_tmax = -(/*targets[i].value */targetListModel.data(targetListModel.index(i,0),TargetListModel.CMaxValueRole) - minY) * yRatio;
        var y_tmin = -(/*targets[i].value */targetListModel.data(targetListModel.index(i,0),TargetListModel.CMinValueRole) - minY) * yRatio;

        //console.log("measurex: " + x)
        //console.log("measurey: " + y)
        var first = (population_x[0] - minX) * xRatio;
        var last = (population_x[population_x.length - 1] - minX) * xRatio;
        ctx.globalAlpha = 0.2;
        ctx.beginPath();
        ctx.moveTo(first, y_tmean);
        ctx.lineTo(last, y_tmean);
        ctx.stroke();
        var highgrd = ctx.createLinearGradient(0, y_tmean, 0, y_tmax);
        highgrd.addColorStop(0,"black");
        highgrd.addColorStop(1,"white");
        var lowgrd = ctx.createLinearGradient(0, y_tmean, 0, y_tmin);
        lowgrd.addColorStop(0,"black");
        lowgrd.addColorStop(1,"white");
        ctx.fillStyle = highgrd
        ctx.fillRect(first, y_tmean, last - first, y_tmax - y_tmean);
        ctx.fillStyle = lowgrd
        ctx.fillRect(first, y_tmin, last - first, y_tmean - y_tmin);

//                    ctx.arc(x_mean, y_mean, 5 / scale, 0, 2 * Math.PI, true);
//                    ctx.fill();

//                    if (checkPointProximity(x_mean, y_mean, mouseX, mouseY, true) && mouseArea.isMouseOver) {
//                        currentTarget = {
//                            x: x_mean,
//                            y: y_mean,
//                            value: (targetListModel.data(targetListModel.index(i,0),TargetListModel.CBestRole)).toFixed(2) + " " + targetListModel.data(targetListModel.index(i,0),TargetListModel.UnitRole),
//                            time: formatDate(new Date(targetListModel.data(targetListModel.index(i,0),TargetListModel.TBestRole))),
//                            color: "blue"
//                        }
//                    }
    }
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

    }

    function drawAxisTicks(ctx, index) {

    //Settings
    ctx.textAlign    = "left";
    ctx.textBaseline = "middle";
    ctx.font         = "10px " + police;

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    var tickSize = 10;
    var tickSpacing = 0.5 * Math.pow(10, -Math.floor(Math.log(scale)));

    //Draw y-ticks
    var down = -1.0 / scale;// + offsetY;
    var up = 1.0 / scale;// + offsetY;
    var down_i = Math.ceil(down / tickSpacing);
    var up_i = Math.floor(up / tickSpacing);
    var remy = down_i * tickSpacing - down;
    var firstTickY = -1.0 + remy * scale;

    var tickOffsetY = bottomLeftY - 1.0 - firstTickY;
    var tickIncrementY = tickSpacing * scale * plotHeight / 2;

    var nbTicksY = up_i - down_i + 1;
    if (nbTicksY > 21)
        nbTicksY = 21;

    var order = (2 - Math.log(maxY - minY) / Math.LN10);
    unitefforder[index] = 1;
    if (order >= 1 && order < 4) {
        unitefforder[index] = 1e3;
        unit = "ng/l";
    }
    if (order >= 4 && order < 7) {
        unitefforder[index] = 1e6;
        unit = "pg/l";
    }
    if (order > -5 && order < -2) {
        unitefforder[index] = 1e-3;
        unit = "mg/l";
    }
    if (order > -8 && order < -5) {
        unitefforder[index] = 1e-6;
        unit = "g/l";
    }

    for (var i = 0; i < nbTicksY; i++) {
        var y = tickOffsetY - i * tickIncrementY;
        var tickScaleY = ((i + down_i) % 10) ? 0.5 : 1;

        ctx.beginPath();
        ctx.moveTo(bottomLeftX, y);
        ctx.lineTo(bottomLeftX - tickSize * tickScaleY, y);
        ctx.stroke();


        if (hasAprioriData)
        var val = minY + (0.5 * i * tickSpacing) * (maxY - minY);
        //use the range of Y to calculate the required units to show max 1000 as max Y
        ctx.fillText(Math.max(0,Math.round(unitefforder[index] * val)), bottomLeftX - tickSize * 5.5, y);
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

    function drawLegends(ctx, colors) {
    if (!hasAprioriData)
        return;

    //Settings
    ctx.textAlign    = "left";
    ctx.textBaseline = "Alphabetic";
    ctx.font         = "12px " + police;

    //Constants
    var populationText  = "Population";
    var popPercText     = "Pop. percentiles (10-25-75-90)"
    var aprioriText     = "A priori";
    var aposterioriText = "A posteriori";
    var reverseText     = "Adjustments";
    var aprPercText		= "Apr. percentiles (10-25-75-90)"
    var apoPercText		= "Apo. percentiles (10-25-75-90)"

    var internalSpacing = 5;
    var externalSpacing = 10;

    var colorSize = 10;
    var boxHeight = 20;

    //Variables
    var legends      = [];
    var legendsWidth = [];

    var totalLength = 0;

    // ToDo ///////////////////////////////////////////////////////
    // Add has... checks for population and percentiles of each type
    // ////////////////////////////////////////////////////////////

    if (hasAprioriData && show[pop]) {
        legends.push( {text: populationText, color: colors[1]} );
        legendsWidth.push(ctx.measureText(populationText).width + internalSpacing * 3 + colorSize);
        legends.push( {text: popPercText, color: colors[7]} );
        legendsWidth.push(ctx.measureText(popPercText).width + internalSpacing * 3 + colorSize);
    }
    if (hasAprioriData && show[apr]) {
        legends.push( {text: aprioriText, color: colors[2]} );
        legendsWidth.push(ctx.measureText(aprioriText).width + internalSpacing * 3 + colorSize);
        legends.push( {text: aprPercText, color: colors[8]} );
        legendsWidth.push(ctx.measureText(aprPercText).width + internalSpacing * 3 + colorSize);
    }
    if (hasAposterioriData && show[apo]) {
        legends.push( {text: aposterioriText, color: colors[4]} );
        legendsWidth.push(ctx.measureText(aposterioriText).width + internalSpacing * 3 + colorSize);
        legends.push( {text: apoPercText, color: colors[6]} );
        legendsWidth.push(ctx.measureText(apoPercText).width + internalSpacing * 3 + colorSize);
    }
    if (hasReverse && show[rev]) {
        legends.push( {text: reverseText, color: colors[5]} );
        legendsWidth.push(ctx.measureText(reverseText).width + internalSpacing * 3 + colorSize);
    }

    for (var j = 0; j < legendsWidth.length; j++)
        totalLength += legendsWidth[j];
    totalLength += externalSpacing * (legends.length - 1);

    var initialX = leftMargin + (plotWidth / 2) - (totalLength / 2);
    var initialY = topMargin / 2 - boxHeight / 2 - 5;

    var x = initialX;
    var y = initialY;

    //Draw legends
    for (var i = 0; i < legends.length; i++) {
        var boxWidth  = legendsWidth[i];

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

        x += boxWidth + externalSpacing;
    }

    ctx.restore();
    ctx.save();
    }

    //ToDo: Refactor drawTooltips (too long, not generic enough, possible other tooptips will be added with time)

    function drawTooltips(ctx, index) {

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
    ctx.translate(topLeftX + offsetX, bottomLeftY/* - offsetY*/);
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
//                        console.log("at index" + index + " " + unitefforder[index] + " currentpoint" + currentPoints[i].value )
        ctx.fillText(currentPoints[i].value + " " + unit, x + labelsWidth, y + 12);
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
        //console.log("wheeld")
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
