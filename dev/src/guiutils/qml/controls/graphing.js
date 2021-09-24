
function drawGraph(cdata)
{
    var ctx = cdata.canvas.getContext('2d');

    //the min max is restricted to the window after scaling. We have to apply the scale to the values
    //then project the values onto the drawn space. So you have to know and use the scale and offset first.
    //console.log("painting!")
    //Init context
    initContext(cdata, ctx);
    ctx.save();
    extents(cdata, ctx);
    //    console.log(maxX + " " + minX);

    var adjTabShowPop = cdata.gInformationSelection.presentAposterioriPrediction && !cdata.hasPatientVariates && !cdata.hasMeasures;
    var adjTabShowApr = cdata.gInformationSelection.presentAposterioriPrediction && cdata.hasPatientVariates && !cdata.hasMeasures;

    //Draw the content
    //dont draw any curves if we cant even draw the population curves
    if (cdata.popP) {
        if (cdata.popP.predictive.predictionData.isValid) {
            if (cdata.gInformationSelection.presentPopulationPercentiles || adjTabShowPop) {
                if (cdata.popercsP.isValid) {
                    drawPercentiles(cdata, ctx, cdata.popercsP, cdata.colors[7], cdata.popcolors);
                }
            }
            
            //draw apriori if indicated in show
            if (cdata.aprP) {
                if (cdata.aprP.predictive.predictionData.isValid) {
                    if (cdata.gInformationSelection.presentAprioriPercentiles || adjTabShowApr) {
                        if (cdata.aprpercsP.isValid) {
                            drawPercentiles(cdata, ctx, cdata.aprpercsP, cdata.colors[8], cdata.aprcolors);
                        }
                    }
                }
            }

            //draw aposteriori if indicated in show and we have measures
            if (cdata.apoP) {
                if (cdata.apoP.predictive.predictionData.isValid) {
                    if (cdata.gInformationSelection.presentAposterioriPercentiles && cdata.hasMeasures) {
                        if (cdata.apopercsP.isValid) {
                            drawPercentiles(cdata, ctx, cdata.apopercsP, cdata.colors[6], cdata.apocolors);
                        }
                    }
                }
            }
        }
    }


    //Draw the content
    //dont draw any curves if we cant even draw the population curves
    if (cdata.popP) {
        if (cdata.popP.predictive.predictionData.isValid) {

            //draw pop curves if indicated in show or measurestab with no measures
            if ((cdata.gInformationSelection.presentPopulationPrediction || adjTabShowPop) && cdata.gInformationSelection.displayPopulationPrediction) {
                drawPop(cdata, ctx, cdata.colors, cdata.popcolors);
            }

            //draw apriori if indicated in show
            if (cdata.aprP) {
                if (cdata.aprP.predictive.predictionData.isValid) {

                    if ((cdata.gInformationSelection.presentAprioriPrediction || adjTabShowApr) && cdata.gInformationSelection.displayAprioriPrediction) {
                        drawApr(cdata, ctx, cdata.colors, cdata.aprcolors);
                    }
                }
            }

            //draw aposteriori if indicated in show and we have measures
            if (cdata.apoP) {
                if (cdata.apoP.predictive.predictionData.isValid) {

                    if (cdata.gInformationSelection.presentAposterioriPrediction && cdata.hasMeasures && cdata.gInformationSelection.displayAposterioriPrediction) {
                        drawApo(cdata, ctx, cdata.colors, cdata.apocolors);
                    }
                }
            }
        }
    }


    //verify compatibility between drugmodel constraint and covariate
    if (cdata.aprP.predictive.predictionData.isValid){
        checkAndDisplayDomain(cdata, ctx, cdata.aprP)
    }

    if (cdata.adjP) {
        if (cdata.gInformationSelection.presentSelectedAdjustment && cdata.gInformationSelection.displaySelectedAdjustment) {

            if (cdata.adjpercsP.isValid) {

                if ((cdata.apoP) && (cdata.apoP.predictive.predictionData.isValid)) {
                    drawPercentilesAdjustments(cdata, ctx, cdata.adjpercsP, cdata.apocolors[0], cdata.apocolors);
                }
                else {
                    drawPercentilesAdjustments(cdata, ctx, cdata.adjpercsP, cdata.aprcolors[0], cdata.aprcolors);
                }
            }

            if ((cdata.apoP) && (cdata.apoP.predictive.predictionData.isValid)) {
                drawAdjustment(cdata, ctx, cdata.colors[4]);
            }
            else {
                drawAdjustment(cdata, ctx, cdata.colors[2]);

            }
        }
    }

    //draw adjustments if we have, and indicated in show
    if (cdata.revP && cdata.revP.isValid) {
        if (cdata.gInformationSelection.presentPossibleAdjustments && cdata.gInformationSelection.displayPossibleAdjustments) {
            drawReverse(cdata, ctx, cdata.colors, cdata.revcolors);
        }
    }

    //draw measures
    if (cdata.gInformationSelection.presentMeasures && cdata.gInformationSelection.displayMeasures) {
        drawMeasures(cdata, ctx);
        ctx.restore();
        ctx.save();
    }

    //draw targets if have, and indicated in show
    if (cdata.hasTargets && cdata.gInformationSelection.presentTargets && cdata.gInformationSelection.displayTargets) {
        if (cdata.apoP && cdata.apoP.predictive.predictionData.isValid) {
            drawTargets(cdata, ctx, cdata.apoP.X, cdata.apoP.predictive.predictionData);
        } else if (cdata.aprP && cdata.aprP.predictive.predictionData.isValid) {
            drawTargets(cdata, ctx, cdata.aprP.X, cdata.aprP.predictive.predictionData);
        } else if (cdata.popP && cdata.popP.predictive.predictionData.isValid) {
            drawTargets(cdata, ctx, cdata.popP.X, cdata.popP.predictive.predictionData);
        } else if (cdata.adjP && cdata.adjP.predictive.predictionData.isValid) {
            drawTargets(cdata, ctx, cdata.adjP.X, cdata.adjP.predictive.predictionData);
        } else if (cdata.revP && cdata.revP.size()>0) {
            var predData = cdata.revP.objat(0);
            if (predData.predictionData.isValid) {
                drawTargets(cdata, ctx, predData.X, predData.predictionData);
            }
        }
        else {
            var tmin = ascreen2time(cdata, cdata.bottomLeftX)
            var tmax = ascreen2time(cdata, cdata.bottomRightX)
            drawTargets(cdata, ctx, [tmin, tmax], null);
        }
        ctx.restore();
        ctx.save();
    }

    //clear the border to draw non plot area
    clearBorder(cdata, ctx);

    //draw non plot area (axes, legends, etc)
    drawNonPlotArea(cdata, ctx, cdata.colors, cdata.pop);

    //draw descriptions
//    if (!graphInformationSelection.presentPopulationPrediction &&
//        !graphInformationSelection.presentAprioriPrediction &&
//        !graphInformationSelection.presentAposterioriPrediction) {
//        if (nographdrugtext) {
//            drawSoftwareDescription(cdata, ctx, nographdrugtext);
//        }
//        else {
//            drawSoftwareDescription(cdata, ctx, nographtext);
//        }
//    }
}

function checkAndDisplayDomain(cdata, ctx, pred)
{
    if (!pred.isValidDomain){
        ctx.textAlign = 'end';
        ctx.strokeText(pred.domainMessage, cdata.topRightX, cdata.topRightY + 10);
    }
}


function getAdjustmentFilter(cdata, filterMax)
{
    if (cdata.gInformationSelection.presentSelectedAdjustment) {
        var limit = cdata.adjustmentDate/1000;
        if (filterMax) return function(d) { return d < limit; }
        return function(d) { return d > limit; }
    }
    return
}

function drawPop(cdata, ctx, colors, popcolors)
{
    drawCurve(cdata, ctx, cdata.popP.predictive.predictionData, colors[1], getAdjustmentFilter(cdata, true));
    ctx.restore();
    ctx.save();
}

function drawApr(cdata, ctx, colors, aprcolors)
{
    drawCurve(cdata, ctx, cdata.aprP.predictive.predictionData, colors[2], getAdjustmentFilter(cdata, true));
    ctx.restore();
    ctx.save();
}

function drawApo(cdata, ctx, colors, apocolors)
{
    drawCurve(cdata, ctx, cdata.apoP.predictive.predictionData, colors[4], getAdjustmentFilter(cdata, true));
    ctx.restore();
    ctx.save();
}

function drawReverse(cdata, ctx, colors, revcolors)
{
    for (var i = 0; i < cdata.revP.size(); ++i)
    {
        // Display all adjustments if less than 6 adjustement are proposed
        // otherwise only display the currently selected one.
        if (cdata.revP.size()<=6 || cdata.revP.objat(i).predictionData.selected) {
            drawCurve(cdata, ctx, cdata.revP.objat(i).predictionData, colors[5], getAdjustmentFilter(cdata, false));
        }

        // Display the first 5 adjustments, and the selected one
        //if (i <= 4 || revP.objat(i).predictionData.selected) {
        //    drawCurve(cdata, ctx, revP.objat(i).predictionData, colors[5], getAdjustmentFilter(cdata, false));
        //}
        ctx.restore();
        ctx.save();
    }
}

function drawAdjustment(cdata, ctx, color)
{
    drawCurve(cdata, ctx, cdata.adjP.predictive.predictionData, color, getAdjustmentFilter(cdata, false));
    ctx.restore();
    ctx.save();
}

function drawPercentiles(cdata, ctx, pairs, color, colors)
{
    var displayedCurves = [];

    if (pairs.size() < 1){
        return;
    }
    if (cdata.gInformationSelection.perc5_95){
        colorRegionBtwCurves(cdata, ctx, pairs.objat(0).predictionData,  pairs.objat(6).predictionData, pairs.objat(0).X, pairs.objat(0).Y, pairs.objat(6).Y, colors[3], getAdjustmentFilter(cdata, true));
        ctx.restore();
        ctx.save();
        displayedCurves.push(pairs.objat(0))
        displayedCurves.push(pairs.objat(6))
    }

    if (cdata.gInformationSelection.perc10_90){
        colorRegionBtwCurves(cdata, ctx, pairs.objat(1).predictionData,  pairs.objat(5).predictionData, pairs.objat(0).X, pairs.objat(1).Y, pairs.objat(5).Y, colors[2], getAdjustmentFilter(cdata, true));
        ctx.restore();
        ctx.save();
        displayedCurves.push(pairs.objat(1))
        displayedCurves.push(pairs.objat(5))
    }

    if (cdata.gInformationSelection.perc25_75){
        colorRegionBtwCurves(cdata, ctx, pairs.objat(2).predictionData,  pairs.objat(4).predictionData, pairs.objat(0).X, pairs.objat(2).Y, pairs.objat(4).Y, colors[1], getAdjustmentFilter(cdata, true));
        ctx.restore();
        ctx.save();
        displayedCurves.push(pairs.objat(2))
        displayedCurves.push(pairs.objat(4))
    }

    if (cdata.gInformationSelection.perc50){
        displayedCurves.push(pairs.objat(3))
    }

    for (var i = 0; i < displayedCurves.length; ++i) {
        drawCurve(cdata, ctx, displayedCurves[i].predictionData, color, getAdjustmentFilter(cdata, true));
        ctx.restore();
        ctx.save();
    }
}

function drawPercentilesAdjustments(cdata, ctx, pairs, color, colors)
{
    var displayedCurves = [];

    if (pairs.size() < 1) {return;}
    if (cdata.gInformationSelection.perc5_95){
        colorRegionBtwCurves(cdata, ctx, pairs.objat(0).predictionData,  pairs.objat(6).predictionData, pairs.objat(0).X, pairs.objat(0).Y, pairs.objat(6).Y, colors[3], false);
        ctx.restore();
        ctx.save();
        displayedCurves.push(pairs.objat(0))
        displayedCurves.push(pairs.objat(6))
    }

    if (cdata.gInformationSelection.perc10_90){
        colorRegionBtwCurves(cdata, ctx, pairs.objat(1).predictionData,  pairs.objat(5).predictionData, pairs.objat(0).X, pairs.objat(1).Y, pairs.objat(5).Y, colors[2], false);
        ctx.restore();
        ctx.save();
        displayedCurves.push(pairs.objat(1))
        displayedCurves.push(pairs.objat(5))
    }

    if (cdata.gInformationSelection.perc25_75){
        colorRegionBtwCurves(cdata, ctx, pairs.objat(2).predictionData,  pairs.objat(4).predictionData, pairs.objat(0).X, pairs.objat(2).Y, pairs.objat(4).Y, colors[1], false);
        ctx.restore();
        ctx.save();
        displayedCurves.push(pairs.objat(2))
        displayedCurves.push(pairs.objat(4))
    }

    if (cdata.gInformationSelection.perc50){
        displayedCurves.push(pairs.objat(3))
    }

    for (var i = 0; i < displayedCurves.length; ++i) {
        drawCurve(cdata, ctx, displayedCurves[i].predictionData, color, getAdjustmentFilter(cdata, true));
        ctx.restore();
        ctx.save();
    }

    return;
}

function drawNonPlotArea(cdata, ctx, colors, index)
{
    //Draw the plot
    drawAxis(cdata, ctx);
    ctx.restore();
    ctx.save();
    drawAxisTicks(cdata, ctx);
    drawAxisLabels(cdata, ctx);
    //Draw the labels
    drawLegends(cdata, ctx, colors);
}

function drawSoftwareDescription(cdata, ctx, text)
{
    ctx.fillText(text, 100, 50);
}

function colorRegionBtwCurves(cdata, ctx, predDataL, predDataU, dataX, dataY, dataYY, color, filter)
{
    ctx.globalAlpha = 0.2;
    ctx.globalAlpha = 1.0;
    ctx.lineWidth   = 2.0 * cdata.scale;// / scalex;
    ctx.lineCap     = "round";
    ctx.lineJoin    = "round";
    ctx.beginPath();

    if (dataX === undefined || dataY === undefined) {
        console.log("No data available in colorRegionBtwCurves");
        return;
    }

    var x1, x2, y1, y2;
    var j = 0;
    x1 = atime2screen(cdata, dataX[j]);
    y1 = acxn2screen(cdata, dataY[j]);
    ctx.moveTo(x1, y1);
    for (; j < dataX.length - 1; j++) {
        if (filter && !filter(dataX[j+1])) {
            break;
        }
        x2 = atime2screen(cdata, dataX[j+1]);
        y2 = acxn2screen(cdata, dataY[j+1]);
        ctx.lineTo(x2, y2);
    }
    ctx.lineTo(x2, y2);
    x1 = atime2screen(cdata, dataX[j]);
    y1 = acxn2screen(cdata, dataYY[j]);
    ctx.lineTo(x1, y1);
    for (; j > -1; j--) {
        x2 = atime2screen(cdata, dataX[j-1]);
        y2 = acxn2screen(cdata, dataYY[j-1]);
        ctx.lineTo(x2, y2);
    }
    ctx.closePath();
    ctx.fillStyle = color;
    ctx.fill();
}

function initContext(cdata, ctx)
{
    //Clear the canvas
    ctx.clearRect(0, 0, cdata.canvas.width, cdata.canvas.height);

    cdata.hasMeasures = cdata.measures && (cdata.measures.length > 0);
    cdata.hasPatientVariates = cdata.pvars && (cdata.pvars.length > 0);
    cdata.hasTargets = cdata.targets && (cdata.targets.length > 0);

    //Default settings
    ctx.font        = cdata.fontSize + " " + cdata.police;
    ctx.globalAlpha = 1.0;
    ctx.lineWidth   = 1.0;
    ctx.strokeStyle = "black";
    ctx.fillStyle   = "black";
}


function extents(cdata, ctx)
{
    // /!\ var has a weird scope: it spreads in all the function, meaning that 2 for loops declaring a "var i" variable
    // will trigger a "i is already defined" error in eslint. The "let" keyword to declare variable has a more
    // natural scope (only spreads in inner blocks) but may not be supported by QT.
    var i;

    cdata.minX = cdata.getViewRangeMin();
    cdata.maxX = cdata.getViewRangeMax();

    //Y max
    var aprioripercmax = 0;
    var aprioripercmin = 0;
    var aposterioripercmax = 0;
    var aposterioripercmin = 0;
    var populationpercmax = 0;
    var populationpercmin = 0;
    var adjustmentpercmax = 0;
    var adjustmentpercmin = 0;
    var reversepercmax = 0;
    var reversepercmin = 0;

    if (cdata.popercsP) {
        if (!cdata.popercsP.isEmpty()) {
            for (i = 0; i < cdata.popercsP.size(); ++i) {
                populationpercmax = Math.max(populationpercmax, Math.max.apply(Math, cdata.popercsP.objat(i).Y));
                populationpercmin = Math.min(populationpercmin, Math.min.apply(Math, cdata.popercsP.objat(i).Y));
            }
        }
    }

    if (cdata.aprpercsP) {
        if (!cdata.aprpercsP.isEmpty()) {
            for (i = 0; i < cdata.aprpercsP.size(); ++i) {
                aprioripercmax = Math.max(aprioripercmax, Math.max.apply(Math, cdata.aprpercsP.objat(i).Y));
                aprioripercmin = Math.min(aprioripercmin, Math.min.apply(Math, cdata.aprpercsP.objat(i).Y));
            }
        }
    }

    if (cdata.apopercsP) {
        if (!cdata.apopercsP.isEmpty()) {
            for (i = 0; i < cdata.apopercsP.size(); ++i) {
                aposterioripercmax = Math.max(aposterioripercmax, Math.max.apply(Math, cdata.apopercsP.objat(i).Y));
                aposterioripercmin = Math.min(aposterioripercmin, Math.min.apply(Math, cdata.apopercsP.objat(i).Y));
            }
        }
    }

    if (cdata.adjpercsP) {
        if (!cdata.adjpercsP.isEmpty()) {
            for (i = 0; i < cdata.adjpercsP.size(); ++i) {
                adjustmentpercmax = Math.max(adjustmentpercmax, Math.max.apply(Math, cdata.adjpercsP.objat(i).Y));
                adjustmentpercmin = Math.min(adjustmentpercmin, Math.min.apply(Math, cdata.adjpercsP.objat(i).Y));
            }
        }
    }

    if (cdata.revP) {
        if (!cdata.revP.isEmpty()) {
            for (i = 0; i < cdata.revP.size(); ++i) {
                if (cdata.revP.size()<=5 || cdata.revP.objat(i).predictionData.selected) {
                    reversepercmax = Math.max(reversepercmax, Math.max.apply(Math, cdata.revP.objat(i).Y));
                    reversepercmin = Math.min(reversepercmin, Math.min.apply(Math, cdata.revP.objat(i).Y));
                }
            }
        }
    }


    // Find the maximum Y value. Could be a function of ChartData...
    var maxY = 0;
    if (cdata.popP)
        maxY = Math.max(maxY, Math.max.apply(Math, cdata.popP.Y));
    if (cdata.aprP)
        maxY = Math.max(maxY, Math.max.apply(Math, cdata.aprP.Y));
    if (cdata.apoP)
        maxY = Math.max(maxY, Math.max.apply(Math, cdata.apoP.Y));
    if (cdata.adjP)
        maxY = Math.max(maxY, Math.max.apply(Math, cdata.adjP.Y));
    cdata.maxY = Math.max(maxY, populationpercmax, aprioripercmax, aposterioripercmax, adjustmentpercmax, reversepercmax);

    // In case we have no other data, use target data to define the Y range...
    if (cdata.maxY == 0) {
        for (var targetIndex = 0; targetIndex < cdata.targets.length; ++targetIndex) {
            var targetY = cdata.targets[targetIndex].cmax.dbvalue * cdata.targets[targetIndex].cmax.multiplier;
            if (targetY > cdata.maxY) cdata.maxY = targetY;
        }
    }

    // Modify maxY with respect to the manual y factor
    cdata.maxY = cdata.maxY * cdata.yFactor;

    var lg10 = Math.pow(10, Math.ceil(Math.log(1.1 * cdata.maxY) / Math.LN10) - 1);
    cdata.maxY = Math.ceil(1.1 / lg10 * cdata.maxY) * lg10;


    cdata.xRatio = cdata.plotWidth  / (cdata.maxX - cdata.minX);
    cdata.yRatio = cdata.plotHeight / (cdata.maxY - cdata.minY);

    //                console.log("maxX: " + maxX);
    //                console.log("minX: " + minX);
    //                console.log("maxY: " + maxY);
    //                console.log("minY: " + minY);
}

function drawCurve(cdata, ctx, predData, color, filter)
{
    var dataX = predData.times();
    var dataY = predData.values();

    //Settings
    ctx.strokeStyle = color;
    ctx.lineWidth   = 2.0 * cdata.scale;
    ctx.lineCap     = "butt";
    ctx.lineJoin    = "round";

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    //Draw the curve
    //    ctx.path = "";
    ctx.beginPath();
    var isFuture = false;
    var highlight = predData === cdata.canvas.closestPred && cdata.canvas.closestPred.highlight;

    // This variable is used to know if we need to move before drawing the line.
    var continueLine = false;

    for (var i = 0; i < dataX.length - 2; i++)
    {
        if (!filter || filter(dataX[i+1]))
        {
            var x1 = atime2screen(cdata, dataX[i]);
            var y1 = acxn2screen(cdata, dataY[i]);
            var x2 = atime2screen(cdata, dataX[i+1]);
            var y2 = acxn2screen(cdata, dataY[i+1]);

            if (highlight) {
                ctx.lineWidth   = 4.0 * cdata.scale;
            }

            if (predData.selected) {
                ctx.lineWidth   = 6.0 * cdata.scale;
                ctx.strokeStyle = color + 1;
            }

            if (dataX[i] > cdata.date.getTime() / 1000) {
                if (!isFuture) {
                    ctx.stroke();
                    ctx.beginPath();
                    ctx.globalAlpha = 0.6;
                    continueLine = false;
                } else {
                    isFuture = true;
                }
            }
            //console.log(x2 + " " + x1 + " " + cdata.topLeftX + " " + cdata.bottomRightX);
            if (x2 > cdata.topLeftX && x1 <= cdata.bottomRightX) {
                if (!continueLine) {
                    ctx.moveTo(x1, y1);
                    continueLine = true;
                }
                //console.log("LineTo(" + x2, ", " + y2);
                ctx.lineTo(x2, y2);
            }
        }
        else {
            continueLine = false;
        }
    }
    if (highlight) {
        ctx.lineWidth   = 4.0 * cdata.scale;
    }
    if (predData.selected) {
        ctx.lineWidth   = 6.0 * cdata.scale;
        ctx.strokeStyle = color + 1;
    }

    ctx.stroke();
}

function drawMeasures(cdata, ctx)
{
    //Settings
    ctx.fillStyle = "red";
    ctx.lineWidth = 1 * cdata.scale;// / scalex;

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    var mouseX = ascreen2time(cdata, cdata.mArea.mouseX);
    var mouseY = ascreen2acxn(cdata, cdata.mArea.mouseY);

    //Draw the samples
    for (var i = 0; i < cdata.measures.length; ++i) {//measures.length; ++i) {
        var x = atime2screen(cdata, cdata.measures[i].moment.getTime() / 1000);
        var y = acxn2screen(cdata, cdata.measures[i].concentration.dbvalue * cdata.measures[i].concentration.multiplier);

        //console.log("measurex: " + x)
        //console.log("measurey: " + y)
        ctx.beginPath();
        ctx.arc(x, y, 5 * cdata.scale, 0, 2 * Math.PI, true);
        ctx.fill();
        ctx.stroke();

        if (checkPointProximity(x, y, mouseX, mouseY, true) && cdata.mArea.isMouseOver) {
            cdata.currentMeasure = {
                x: x,
                y: y,
                value: cdata.measures[i].concentration.dbvalue.toFixed(2) + " " + cdata.measures[i].concentration.unitstring,
                time: formatDate(cdata.measures[i].moment), //formatDateInLine(cdata.measures[i].moment),
                color: "red"
            }
        }
    }
}

function drawTargets(cdata, ctx, times, predData)
{
    //Settings
    var targets = cdata.targets;
    if (!targets) return;

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    var mouseX = ascreen2time(cdata, cdata.mArea.mouseX);
    var mouseY = ascreen2acxn(cdata, cdata.mArea.mouseY);

    // console.log("Nb targets = " + targets.length);

    for (var targetIndex = 0; targetIndex < targets.length; ++targetIndex)
    {
        // Settings
        ctx.fillStyle = "blue";
        ctx.lineWidth = 1 * cdata.scale;
        
        var i, t, v, leftgrd, rightgrd, startY, crossSize, gradientSize;
        var ttpe = targets[targetIndex].type.value;
        var y_mean = acxn2screen(cdata, targets[targetIndex].cbest.dbvalue * targets[targetIndex].cbest.multiplier);
        var y_max = acxn2screen(cdata, targets[targetIndex].cmax.dbvalue * targets[targetIndex].cmax.multiplier);
        var y_min = acxn2screen(cdata, targets[targetIndex].cmin.dbvalue * targets[targetIndex].cmin.multiplier);
        var first = atime2screen(cdata, times[0]);
        var last = atime2screen(cdata, times[times.length - 1]);


        if (ttpe < 3) {
            ctx.globalAlpha = 0.05;
            if (cdata.targetTabIndex === targetIndex) {
                ctx.globalAlpha = 0.1;
            }
            ctx.beginPath();
            ctx.moveTo(first, y_mean);
            ctx.lineTo(last, y_mean);
            ctx.stroke();
            //        var highgrd = ctx.createLinearGradient(0, y_mean, 0, y_max);
            //        highgrd.addColorStop(0,"black");
            //        highgrd.addColorStop(1,"white");
            //        var lowgrd = ctx.createLinearGradient(0, y_mean, 0, y_min);
            //        lowgrd.addColorStop(0,"black");
            //        lowgrd.addColorStop(1,"white");
            //        ctx.fillStyle = highgrd
            ctx.fillRect(first, y_mean, last - first, y_max - y_mean);
            //        ctx.fillStyle = lowgrd
            ctx.fillRect(first, y_min, last - first, y_mean - y_min);
            ctx.stroke();
        }
        ctx.globalAlpha = 0.2;

        if (ttpe === 0 && predData !== null) {
            var troughs = predData.troughs;
            for (i = 0; i < troughs.length; ++i) {
                ctx.beginPath();
                t = atime2screen(cdata, predData.timeAt(troughs[i]));
                if (isFinite(t) && !isNaN(t) && !isNaN(y_mean)) {
                    gradientSize = 20 * cdata.scale;
                    leftgrd = ctx.createLinearGradient(t, y_mean, t - gradientSize, y_mean);
                    leftgrd.addColorStop(0,"black");
                    leftgrd.addColorStop(1,"white");
                    rightgrd = ctx.createLinearGradient(t, y_mean, t + gradientSize, y_mean);
                    rightgrd.addColorStop(0,"black");
                    rightgrd.addColorStop(1,"white");
                    ctx.fillStyle = leftgrd
                    ctx.fillRect(t - gradientSize, y_min, gradientSize, y_max - y_min);
                    ctx.fillStyle = rightgrd
                    ctx.fillRect(t, y_min, gradientSize, y_max - y_min);
                    ctx.stroke();

                    startY = cdata.bottomLeftY;

                    ctx.globalAlpha = 1;
                    ctx.beginPath();
                    // Bottom arrow
                    /*
                    var arrowWidth = 3 * cdata.scale;
                    var arrowHeight = 4 * cdata.scale;
                    var diamondWidth = 4 * cdata.scale;
                    var diamondHeight = 5 * cdata.scale;
                    */
                    crossSize = 6 * cdata.scale;
                    ctx.moveTo(t, y_mean + crossSize);
                    ctx.lineTo(t, y_mean - crossSize);
                    ctx.moveTo(t - crossSize, y_mean);
                    ctx.lineTo(t + crossSize, y_mean);
                    /*
                    ctx.moveTo(t,y_min);
                    ctx.lineTo(t-arrowWidth,y_min-arrowHeight);
                    ctx.lineTo(t+arrowWidth,y_min-arrowHeight);
                    ctx.lineTo(t,y_min);
                    // Middle square
                    ctx.moveTo(t,y_min-arrowHeight);
                    ctx.lineTo(t,y_mean+diamondHeight);
                    ctx.lineTo(t-diamondWidth,y_mean);
                    ctx.lineTo(t,y_mean-diamondHeight);
                    ctx.lineTo(t+diamondWidth,y_mean);
                    ctx.lineTo(t,y_mean+diamondHeight);
                    // Top arrow
                    ctx.moveTo(t,y_mean-diamondHeight);
                    ctx.lineTo(t,y_max+arrowHeight);
                    ctx.lineTo(t-arrowWidth,y_max+arrowHeight);
                    ctx.lineTo(t,y_max);
                    ctx.lineTo(t+arrowWidth,y_max+arrowHeight);
                    ctx.lineTo(t,y_max+arrowHeight);
                    */
             //       ctx.drawImage("qrc:/icons/flow/targets_disabled_mini.png", t - 16, v - 16, 32, 32);
                    ctx.stroke();
                    ctx.globalAlpha = 0.2;
                }
            }
            // console.log("target is residual");
        }

        if (ttpe === 1 && predData !== null) {
            var peaks = predData.peaks;
            for (i = 0; i < peaks.length; ++i) {
                gradientSize = 20 * cdata.scale;
                ctx.beginPath();
                t = atime2screen(cdata, predData.timeAt(peaks[i]));
                leftgrd = ctx.createLinearGradient(t, y_mean, t - gradientSize, y_mean);
                leftgrd.addColorStop(0,"black");
                leftgrd.addColorStop(1,"white");
                rightgrd = ctx.createLinearGradient(t, y_mean, t + gradientSize, y_mean);
                rightgrd.addColorStop(0,"black");
                rightgrd.addColorStop(1,"white");
                ctx.fillStyle = leftgrd;
                ctx.fillRect(t - gradientSize, y_min, gradientSize, y_max - y_min);
                ctx.fillStyle = rightgrd
                ctx.fillRect(t, y_min, gradientSize, y_max - y_min);
                ctx.stroke();

                startY = cdata.bottomLeftY;

                ctx.globalAlpha = 1;
                ctx.beginPath();
                // Bottom arrow
                /*
                var arrowWidth = 3 * cdata.scale;
                var arrowHeight = 4 * cdata.scale;
                var diamondWidth = 4 * cdata.scale;
                var diamondHeight = 5 * cdata.scale;
                */

                crossSize = 6 * cdata.scale;
                ctx.moveTo(t, y_mean + crossSize);
                ctx.lineTo(t, y_mean - crossSize);
                ctx.moveTo(t - crossSize, y_mean);
                ctx.lineTo(t + crossSize, y_mean);
                /*
                ctx.moveTo(t,y_min);
                ctx.lineTo(t-arrowWidth,y_min-arrowHeight);
                ctx.lineTo(t+arrowWidth,y_min-arrowHeight);
                ctx.lineTo(t,y_min);
                // Middle square
                ctx.moveTo(t,y_min-arrowHeight);
                ctx.lineTo(t,y_mean+diamondHeight);
                ctx.lineTo(t-diamondWidth,y_mean);
                ctx.lineTo(t,y_mean-diamondHeight);
                ctx.lineTo(t+diamondWidth,y_mean);
                ctx.lineTo(t,y_mean+diamondHeight);
                // Top arrow
                ctx.moveTo(t,y_mean-diamondHeight);
                ctx.lineTo(t,y_max+arrowHeight);
                ctx.lineTo(t-arrowWidth,y_max+arrowHeight);
                ctx.lineTo(t,y_max);
                ctx.lineTo(t+arrowWidth,y_max+arrowHeight);
                ctx.lineTo(t,y_max+arrowHeight);
                */
//                ctx.drawImage("qrc:/icons/flow/targets_disabled_mini.png", t - 16, v - 16, 32, 32);
                ctx.stroke();
                ctx.globalAlpha = 0.2;
            }
            // console.log("target is peak");
        }

        if (ttpe === 2) {
            // Mean/AUC
            // Do not draw something else
            // console.log("target is mean/AUX");
        }
    }
}

function clearBorder(cdata, ctx)
{
    ctx.clearRect(0, 0, cdata.leftMargin, cdata.plotHeight + cdata.topMargin);
    ctx.clearRect(0, cdata.bottomLeftY, cdata.plotWidth + cdata.leftMargin, cdata.bottomMargin);
    ctx.clearRect(cdata.topRightX, cdata.topRightY, cdata.rightMargin, cdata.plotHeight + cdata.bottomMargin);
    ctx.clearRect(cdata.topLeftX, 0, cdata.plotWidth + cdata.rightMargin, cdata.topMargin);
}

function drawAxis(cdata, ctx)
{
    //Constants
    var arrowSize = 5 * cdata.scale;
    var overrun   = 10 * cdata.scale;

    //X-Axis
    ctx.beginPath();
    ctx.moveTo(cdata.bottomLeftX, cdata.bottomLeftY);
    ctx.lineTo(cdata.bottomRightX + overrun, cdata.bottomRightY);
    ctx.stroke();

    //Y-Axis
    ctx.beginPath();
    ctx.moveTo(cdata.bottomLeftX, cdata.bottomLeftY);
    ctx.lineTo(cdata.topLeftX, cdata.topLeftY - overrun);
    ctx.stroke();

    //X-Arrow
    ctx.beginPath();
    ctx.moveTo(cdata.bottomRightX + overrun, cdata.bottomRightY);
    ctx.lineTo(cdata.bottomRightX + overrun - arrowSize, cdata.bottomRightY - arrowSize);
    ctx.lineTo(cdata.bottomRightX + overrun - arrowSize, cdata.bottomRightY + arrowSize);
    ctx.closePath();
    ctx.stroke();
    ctx.fill();

    //Y-Arrow
    ctx.beginPath();
    ctx.moveTo(cdata.topLeftX, cdata.topLeftY - overrun);
    ctx.lineTo(cdata.topLeftX - arrowSize, cdata.topLeftY - overrun + arrowSize);
    ctx.lineTo(cdata.topLeftX + arrowSize, cdata.topLeftY - overrun + arrowSize);
    ctx.closePath();
    ctx.stroke();
    ctx.fill();
}

function acxn2screen(cdata, c)
{
    //console.log(c);
    //console.log(cdata.minY);
    //console.log(cdata.yRatio);
    //console.log(cdata.bottomLeftY);
    return  (-(c - cdata.minY) * cdata.yRatio) + cdata.bottomLeftY;
}

function ascreen2acxn(cdata, p)
{
    return cdata.minY - ((p - cdata.bottomLeftY) / cdata.yRatio);
}

function atime2screen(cdata, t)
{
    return ((t - cdata.minX) * cdata.xRatio) + cdata.bottomLeftX;
}

function ascreen2time(cdata, p)
{
    return cdata.minX + ((p - cdata.bottomLeftX) / cdata.xRatio);
}

function drawAxisTicks(cdata, ctx)
{
    //Settings
    ctx.textAlign    = "left";
    ctx.textBaseline = "middle";
    ctx.font         = cdata.axisTicksFontSize + " " + cdata.police;

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    var tickSize = 10 * cdata.scale;
    var dateTickSize = 30 * cdata.scale;
    var tickSpacingy = 0.2 * cdata.scale;

    //Draw y-ticks
    var down = -1.0;
    var up = 1.0;
    var down_i = Math.ceil(down / tickSpacingy);
    var up_i = Math.floor(up / tickSpacingy);
    var remy = down_i * tickSpacingy - down;
    var firstTickY = -1.0 + remy;

    var tickOffsetY = cdata.bottomLeftY - 1.0 - firstTickY;
    var tickIncrementY = tickSpacingy * cdata.plotHeight / 2;

    var nbTicksY = up_i - down_i + 1;
    if (nbTicksY > 21)
        nbTicksY = 21;

    var order = (2 - Math.log(cdata.maxY - cdata.minY) / Math.LN10);
    if (order >= 1 && order < 4) {
        cdata.unitefforder = 1e3;
        cdata.unit = "ng/l";
    }
    if (order >= 4 && order < 7) {
        cdata.unitefforder = 1e6;
        cdata.unit = "pg/l";
    }
    if (order > -5 && order < -2) {
        cdata.unitefforder = 1e-3;
        cdata.unit = "mg/l";
    }
    if (order > -8 && order < -5) {
        cdata.unitefforder = 1e-6;
        cdata.unit = "g/l";
    }
    if (order < 1 && order >= -2) {
        cdata.unitefforder = 1;
        cdata.unit = "ug/l";
    }

    for (var i = 0; i < nbTicksY; i++) {
        var y = tickOffsetY - i * tickIncrementY;
        var tickScaleY = ((i + down_i) % 10) ? 0.5 : 1;

        ctx.beginPath();
        ctx.moveTo(cdata.bottomLeftX, y);
        ctx.lineTo(cdata.bottomLeftX - tickSize * tickScaleY, y);
        ctx.stroke();


        var val = cdata.minY + (0.5 * i * tickSpacingy) * (cdata.maxY - cdata.minY);
        //use the range of Y to calculate the required units to show max 1000 as max Y
        ctx.fillText(Math.max(0,Math.round(cdata.unitefforder * val)), cdata.bottomLeftX - tickSize * 5.5, y);
    }

    //Draw x-ticks
    var ticktimes = [];
    var earliest = ascreen2time(cdata, cdata.bottomLeftX);
    var latest = ascreen2time(cdata, cdata.canvas.width);
    var axisMaximumWidth = latest - earliest;
    var date = new Date(earliest * 1000);
    var hourHalfWidth = 14 * cdata.scale;
    var dateHalfWidth = 2 * hourHalfWidth;


    var _1week = false;

    var maximumTimeInterval = 0;
    var timeIntervalDependingOnPrecision = 0;
    var ticks = [];
    var maximumTicks;
    var oldDate;

    //1 tick / 15 minutes
    timeIntervalDependingOnPrecision = chop(cdata, 900, ticks, false, 15) // Maximum number of interval corresponding to precision (15 minutes)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / 30 minutes
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 1800, ticks, false, 30)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / hr
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 3600, ticks, false, 1)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / 4 hr
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 14400, ticks, true, 4)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / 6 hr
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 21600, ticks, true, 6)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / 8 hr
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 28800, ticks, true, 8)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / 12 hr
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 43200, ticks, true, 12)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / 24 hr
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 86400, ticks, true, 24)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / 48 hr
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 172800, ticks, true, 24)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / 72 hr
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 259200, ticks, true, 24)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    maximumTimeInterval = (maximumTimeInterval > ticktimes.length) ? maximumTimeInterval : ticktimes.length

    //1 tick / week
    ticks = []
    timeIntervalDependingOnPrecision = chop(cdata, 604800, ticks, true, 24)
    oldDate = new Date(ticks[0] * 1000);
    maximumTicks = (proportionnalInterval(ticks, oldDate) === 1) ? (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * dateHalfWidth) + 10) : (atime2screen(cdata, ticks[ticks.length - 1]) - cdata.bottomLeftX) / ((2 * hourHalfWidth) + 10)
    ticktimes = ((timeIntervalDependingOnPrecision > maximumTimeInterval) && (timeIntervalDependingOnPrecision <= maximumTicks)) ? ticks : ticktimes
    _1week = (ticks === ticktimes) ? true : _1week;


    var oldDateUsed = false
    var cumulInterDateSize = 0
    var interdatesize = 0
    oldDate = new Date(ticktimes[0] * 1000)

    for (i = 0; i < ticktimes.length; i++) {

        //Draw the ticks on the axis
        ctx.beginPath();
        ctx.moveTo(atime2screen(cdata, ticktimes[i]), cdata.bottomLeftY);
        ctx.lineTo(atime2screen(cdata, ticktimes[i]), cdata.bottomLeftY + tickSize);
        ctx.stroke();

        ctx.translate(atime2screen(cdata, ticktimes[i]), cdata.bottomLeftY + tickSize * 1.5);

        date = new Date(ticktimes[i] * 1000);

        ctx.fillText(formatHour(date), -hourHalfWidth, 1)

        // Compute the exact position of the date on the axis
        // The date can be used for multiple ticks for example
        if (i > 0){
            interdatesize = i > 0 ? atime2screen(cdata, ticktimes[i]) - atime2screen(cdata, ticktimes[i - 1]) : atime2screen(cdata, ticktimes[i]);

            if (oldDate.getDate() !== date.getDate()){
                if (oldDateUsed){
                    ctx.fillText(formatDay(oldDate), - ((cumulInterDateSize + interdatesize) / 2) - dateHalfWidth, 30 * cdata.scale);
                    cumulInterDateSize = 0
                }
                else if(_1week){
                    if (i === ticktimes.length - 1){
                        ctx.fillText(formatDay(date), - dateHalfWidth, 15 * cdata.scale);
                    }
                    ctx.fillText(formatDay(oldDate), - (interdatesize) - dateHalfWidth, 15 * cdata.scale);
                }
                else{
                    ctx.fillText(formatDay(oldDate), - (interdatesize/2) - dateHalfWidth, 30 * cdata.scale);
                }

                oldDateUsed = false
            }
            else{
                cumulInterDateSize += interdatesize
                if (i === ticktimes.length - 1){
                    if (new Date(ticktimes[i] * 1000).getDate() !== new Date(ticktimes[i - 2] * 1000).getDate()){
                        if ((atime2screen(cdata, ticktimes[i]) - atime2screen(cdata, ticktimes[i - 1])) > ((2 * dateHalfWidth) + 10)){
                                ctx.fillText(formatDay(date), - (cumulInterDateSize / 2) - dateHalfWidth, 30 * cdata.scale);
                        }
                    }
                    else{
                        ctx.fillText(formatDay(date), - (cumulInterDateSize / 2) - dateHalfWidth, 30 * cdata.scale);
                    }
                }
                oldDateUsed = true
            }
        }
        else if(_1week && ticktimes.length === 1){
            ctx.fillText(formatDay(oldDate), - (interdatesize) - dateHalfWidth, 15 * cdata.scale);
        }

        oldDate = date

        ctx.translate(-atime2screen(cdata, ticktimes[i]), -(cdata.bottomLeftY + tickSize * 1.5));

        // Increase length of ticks at the beggining and the end of a day
        if (!oldDateUsed){
            if ((i === 0 && date.getHours() !== 0) || _1week){ // eslint-disable-line

            }
            else{
                ctx.beginPath();
                ctx.moveTo(atime2screen(cdata, ticktimes[i]), cdata.bottomLeftY + 20 * cdata.scale);
                ctx.lineTo(atime2screen(cdata, ticktimes[i]), cdata.bottomLeftY + 20 * cdata.scale + dateTickSize);
                ctx.stroke();
            }
        }
    }

    ctx.restore();
    ctx.save();
}

function proportionnalInterval(ticktimes, oldDate)
{
    // This function compute for each duration of a DAY the number of ticks.
    // The smallest number is kept as long as it is not at the edge.
    // This number is used for the display rate

    var smallestChangeOfDate = 0;
    var numberChangeOfDate = 0;
    var changeOfDate = 0;
    var once = true
    for (var i = 0; i < ticktimes.length; i++) {
        if(i > 0){
            var date = new Date(ticktimes[i] * 1000);
            if (date.getDate() === oldDate.getDate()){
                numberChangeOfDate++;
                if (numberChangeOfDate > 1 && i === ticktimes.length - 1){
                    smallestChangeOfDate = (smallestChangeOfDate > numberChangeOfDate || once) ? numberChangeOfDate : smallestChangeOfDate
                }
            }
            else{
                numberChangeOfDate++;
                if (i !== 1 || i !== ticktimes.length - 1){
                    smallestChangeOfDate = (smallestChangeOfDate > numberChangeOfDate || once) ? numberChangeOfDate : smallestChangeOfDate
                }
                numberChangeOfDate = 0;
                once = false;
                changeOfDate++;
            }
            oldDate = date
        }
    }
    oldDate = new Date();

    smallestChangeOfDate = changeOfDate === 1 ? smallestChangeOfDate + 1 : smallestChangeOfDate

    if (ticktimes.length === 1){
        smallestChangeOfDate = 1;
    }

    return smallestChangeOfDate;
}

function chop(cdata, interval, ticktimes, isHour, time){
    // This function is used to determine the x coordinate (time)
    // of the ticks corresponding to the precision needed
    // Return : The maximum number of ticks

    var early = ascreen2time(cdata, cdata.bottomLeftX) * 1000;
    var late = ascreen2time(cdata, cdata.canvas.width - cdata.bottomLeftX);
    var date = new Date(early);

    date.setMilliseconds(0);
    date.setSeconds(0);
    date.setMinutes(0);
    if (isHour){
        date.setHours(Math.ceil(date.getHours() / time) * time % 24)
    }
    else{
        date.setMinutes(Math.ceil(date.getMinutes() / time) * time % 60);
    }

    var nbOfTimeInterval = 0;
    var tick = date;

    tick = tick / 1000;
    while (tick < late) {
        if (tick * 1000 >= early) {
            ticktimes.push(tick);
        }
        tick = tick + interval;
        nbOfTimeInterval++;
    }
    return nbOfTimeInterval;
}

function drawAxisLabels(cdata, ctx)
{
    //Settings
    ctx.font = "bold " + cdata.fontSize + " " + cdata.police;

    //Draw x-axis label
    ctx.textAlign    = "left";
    ctx.textBaseline = "middle";
    ctx.fillText("Time", cdata.leftMargin + cdata.plotWidth + 15 * cdata.scale, cdata.topMargin + cdata.plotHeight);

    //Draw y-axis label
    ctx.textAlign    = "center";
    ctx.textBaseline = "bottom";
    ctx.fillText(cdata.unit, cdata.leftMargin, cdata.topMargin - 15 * cdata.scale);

    ctx.restore();
    ctx.save();
}

function drawLegends(cdata, ctx, colors)
{
    //Settings
    ctx.textAlign    = "left";
    ctx.textBaseline = "Alphabetic";
    ctx.font         = cdata.fontSize + " " + cdata.police;

    //Constants
    var populationText  = "Typical patient";
    var popPercText     = "Typical patient percentiles"
    var aprioriText     = "A priori";
    var aposterioriText = "A posteriori";
    var reverseText     = "Suggested adjustments";
    var adjustmentText  = "Adjustments";
    var aprPercText		= "A priori percentiles"
    var apoPercText		= "A posteriori percentiles"

    var internalSpacing = 5 * cdata.scale;
    var externalSpacing = 10 * cdata.scale;

    var colorSize = 10 * cdata.scale;
    var boxHeight = 20 * cdata.scale;

    //Variables
    var legends      = [];
    var legendsWidth = [];

    var totalLength = 0;

    // ToDo ///////////////////////////////////////////////////////
    // Add has... checks for population and percentiles of each type
    // ////////////////////////////////////////////////////////////

    var adjTabShowPop = cdata.gInformationSelection.presentAposterioriPrediction && !cdata.hasPatientVariates && !cdata.hasMeasures;
    var adjTabShowApr = cdata.gInformationSelection.presentAposterioriPrediction && cdata.hasPatientVariates && !cdata.hasMeasures;

    apoPercText += findEnablePercentiles(cdata)
    aprPercText += findEnablePercentiles(cdata)
    popPercText += findEnablePercentiles(cdata)

    if (cdata.popP) {
        if (cdata.popP.predictive.predictionData.isValid) {
            if (cdata.gInformationSelection.presentPopulationPrediction || adjTabShowPop) {
                legends.push( {text: populationText, color: cdata.colors[1]} );
                legendsWidth.push(ctx.measureText(populationText).width);
                //legendsWidth.push(ctx.measureText(popPCB.text).width);
            }
            if (cdata.popercsP.isValid && cdata.gInformationSelection.presentPopulationPercentiles || adjTabShowPop) {
                legends.push( {text: popPercText, color: cdata.colors[7]} );
                legendsWidth.push(ctx.measureText(popPercText).width);
            }
        }
    }
    if (cdata.aprP) {
        if (cdata.aprP.predictive.predictionData.isValid) {
            if (cdata.gInformationSelection.presentAprioriPrediction || adjTabShowApr) {
                legends.push( {text: aprioriText, color: cdata.colors[2]} );
                legendsWidth.push(ctx.measureText(aprioriText).width);
            }
            if (cdata.aprpercsP.isValid && cdata.gInformationSelection.presentAprioriPercentiles || adjTabShowApr) {
                legends.push( {text: aprPercText, color: cdata.colors[8]} );
                legendsWidth.push(ctx.measureText(aprPercText).width);
            }
        }
    }
    if (cdata.apoP) {
        if (cdata.apoP.predictive.predictionData.isValid) {
            if (cdata.gInformationSelection.presentAposterioriPrediction) {
                legends.push( {text: aposterioriText, color: cdata.colors[4]} );
                legendsWidth.push(ctx.measureText(aposterioriText).width);
            }
            if (cdata.apopercsP.isValid && cdata.gInformationSelection.presentAposterioriPercentiles) {
                legends.push( {text: apoPercText, color: cdata.colors[6]} );
                legendsWidth.push(ctx.measureText(apoPercText).width);
            }
        }
    }
    if (cdata.revP) {
        if (cdata.revP.isValid && cdata.gInformationSelection.presentPossibleAdjustments) {
            legends.push( {text: reverseText, color: cdata.colors[5]});
            legendsWidth.push(ctx.measureText(reverseText).width);
        }
    }
    if (cdata.adjP) {
        if (cdata.adjP.predictive.predictionData.isValid && cdata.gInformationSelection.presentSelectedAdjustment) {
            legends.push( {text: adjustmentText, color: cdata.colors[9]} );
            legendsWidth.push(ctx.measureText(adjustmentText).width);
        }
    }

    for (var j = 0; j < legendsWidth.length; j++)
        totalLength += legendsWidth[j] + internalSpacing * 4 + colorSize;
    totalLength += externalSpacing * (legends.length - 1);

    var nLines = Math.ceil(totalLength / cdata.plotWidth);
    var nLegendsPerLine = legends.length/nLines;
    var nLegendsInLine = 0;
    var lengthPerLine = totalLength / nLines;

    var initialX = cdata.leftMargin + (cdata.plotWidth / 2) - (lengthPerLine / 2);
    var initialY = cdata.topMargin / 2 - boxHeight / 2 - 5;

    var x = initialX;
    var y = initialY;

    //Draw legends
    for (var i = 0; i < legends.length; i++) {
        var boxWidth  = legendsWidth[i] + internalSpacing * 4 + colorSize;

        ctx.fillStyle = legends[i].color;

        ctx.beginPath();
        ctx.rect(x, y, boxWidth, boxHeight);
        ctx.stroke();

        ctx.beginPath();
        ctx.rect(x + internalSpacing, y + (boxHeight - colorSize) / 2, colorSize, colorSize);
        ctx.stroke();
        ctx.fill();

//        if (legends[i].cb) {
//            legends[i].cb.visible = true
//            legends[i].cb.x = x + internalSpacing * 2 + colorSize
//            legends[i].cb.y = y + (boxHeight - cbSize) / 2
//        }

        ctx.fillStyle = "black";
        ctx.beginPath();
        ctx.fillText(legends[i].text, x + internalSpacing * 3 + colorSize, y + (boxHeight + colorSize) / 2);

        x += boxWidth + externalSpacing;
        nLegendsInLine++;
        if (nLegendsInLine >= nLegendsPerLine) {
            nLegendsInLine = 0;
            x = initialX;
            y += boxHeight + externalSpacing;
        }
    }

    ctx.restore();
    ctx.save();
}

function checkLineIntersection(line1StartX, line1StartY, line1EndX, line1EndY, line2StartX, line2StartY, line2EndX, line2EndY)
{
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

function checkPointProximity(x1, y1, x2, y2, useScale)
{
    var tolerance = 10;
    return Math.abs(x1 - x2) <= tolerance && Math.abs(y1 - y2) <= tolerance;
}

function formatHour(date)
{
    var hours = date.getHours();
    var mins  = date.getMinutes();
    var sTime = (hours < 10 ? "0" + hours : hours) + ":" + (mins < 10 ? "0" + mins : mins);

    return sTime;
}

function formatDay(date)
{
    var day   = date.getDate();
    var month = date.getMonth() + 1;
    var year  = date.getFullYear();
    var sDate = (day < 10 ? "0" + day : day) + "." + (month < 10 ? "0" + month : month) + "." + year;

    return sDate;
}

function formatDate(date)
{
    var hours = date.getHours();
    var mins  = date.getMinutes();
    var sTime = (hours < 10 ? "0" + hours : hours) + ":" + (mins < 10 ? "0" + mins : mins);

    var day   = date.getDate();
    var month = date.getMonth() + 1;
    var year  = date.getFullYear();
    var sDate = (day < 10 ? "0" + day : day) + "." + (month < 10 ? "0" + month : month) + "." + year;

    return sTime + " " + sDate;
}

function findEnablePercentiles(cdata)
{
    var perc5Text       = "5"
    var perc10Text      = "10"
    var perc25Text      = "25"
    var perc50Text      = "50"
    var perc75Text      = "75"
    var perc90Text      = "90"
    var perc95Text      = "95"

    var percTextTab     = [0,0,0,0,0,0,0];
    var percString = " (";
    var lastPercentile = false;


    if (cdata.gInformationSelection.perc5_95){
        percTextTab[0] = perc5Text
        percTextTab[6] = perc95Text
    }

    if (cdata.gInformationSelection.perc10_90){
        percTextTab[1] = perc10Text
        percTextTab[5] = perc90Text
    }

    if (cdata.gInformationSelection.perc25_75){
        percTextTab[2] = perc25Text
        percTextTab[4] = perc75Text
    }

    if (cdata.gInformationSelection.perc50){
        percTextTab[3] = perc50Text
    }

    for (var i = 0; i < percTextTab.length; i++){
        if (percTextTab[i] !== 0){
            percString += percTextTab[i]
            percString += "-"
            lastPercentile = true;
        }
    }

    if (!lastPercentile){
        return ""
    }

    percString = percString.slice(0, -1) + ")";

    return percString
}





/////////////////////////////////////////////////////////
// Previously in annotations.js
/////////////////////////////////////////////////////////

function drawAnnotations(cdata)
{
    var i;
    var ctx = cdata.canvas.getContext("2d");

    ctx.lineWidth = 1;
    ctx.globalAlpha = 1.0;

    if (cdata.gInformationSelection.displayPopulationPrediction && cdata.state !== "validation") {
        annotateDosage(cdata, ctx, cdata.currentDosage, cdata.colors[1]);
    }

    if (cdata.gInformationSelection.displayAprioriPrediction) {
        if (cdata.gInformationSelection.displayCovariateChange) {
            for (i = 0; i < cdata.pvars.length; ++i) {
                annotateCovariate(cdata, ctx, cdata.pvars[i], cdata.colors[2]);
            }
        }
    }

    cdata.currentPoints = []
    cdata.closestPred = ({})
    if (!cdata.mArea.containsMouse) {
        return;
    }

    //    ctx.clearRect(0, 0, cdata.canvas.width, cdata.canvas.height);
    annotatePrediction(cdata, ctx, cdata.popP, cdata.pop, cdata.colors[1]);
    annotatePrediction(cdata, ctx, cdata.aprP, cdata.apr, cdata.colors[2]);
    annotatePrediction(cdata, ctx, cdata.apoP, cdata.apo, cdata.colors[4]);
    annotatePrediction(cdata, ctx, cdata.adjP, cdata.adj, cdata.adjcolors[1]);

    if (cdata.revP !== null && cdata.revP.isValid) {
        if (cdata.gInformationSelection.displayPossibleAdjustments) {
            var pairs = cdata.revP;
            for (i = 0; i < pairs.size(); ++i) {
                findClosestValue(cdata, ctx, pairs.objat(i), pairs.objat(i).predictionData, cdata.rev, cdata.colors[5]);
            }
        }
    }
}

function annotatePrediction(cdata, ctx, pred, index, color)
{
    if (pred) {
        if (pred.predictive.predictionData.isValid && isCurveAvailable(cdata, index)) {
            findClosestValue(cdata, ctx, pred.predictive, pred.predictive.predictionData, index, color);
            if (pred.predictive.percentilePairs.isValid) {
                var pairs = pred.predictive.percentilePairs;
                for (var i = 0; i < pairs.size(); ++i) {
                    findClosestValue(cdata, ctx, pairs.objat(i), pairs.objat(i).predictionData, index, color);
                }
            }
            if (isCurveVisible(cdata, index)) {
                annotateCurveLoci(cdata, ctx, pred.predictive.predictionData, index, color);
            }
        }
    }
}

function annotateDosage(cdata, ctx, dosage, color)
{
    if (!dosage) {return;}
    var start = dosage.applied.getTime()/1000;
    var end = dosage.endtime.getTime()/1000;
    var startX = atime2screen(cdata, start);
    var startY = cdata.bottomLeftY;
    var endY = cdata.bottomLeftY;
    var endX = atime2screen(cdata, end);
    //these are for checking if the start and end are within view, if not we draw it differenetly
    var startinview = true;
    var endinview = true;
    // console.log(start + " " + end + " " + startX + " " + endX + " " + cdata.topLeftX + " ");
    // Here the -1 is to avoid some imprecision in atime2screen calculation
    if ((startX < cdata.topLeftX - 1) && endX > cdata.topLeftX) {
        startX = cdata.topLeftX;
        startinview = false;
    }
    if (endX > cdata.bottomRightX && startX < cdata.bottomRightX) {
        endX = cdata.bottomRightX;
        endinview = false;
    }

    ctx.beginPath();
    ctx.strokeStyle = color;
    ctx.fillStyle = color;

    ctx.arc(startX, startY, 5 * cdata.scale, 0, 2 * Math.PI);
    ctx.arc(endX, endY, 5 * cdata.scale, 0, 2 * Math.PI);
    ctx.fill();
    var imageSize = 32 * cdata.scale;
    if (startinview) {
        ctx.drawImage(cdata.img_dosages_disabled_mini, startX - imageSize, cdata.bottomLeftY - imageSize, imageSize, imageSize);
    } else {
        //draw a leftarrow
    }

    if (endinview) {
        ctx.drawImage(cdata.img_dosages_disabled_mini, endX - imageSize, cdata.bottomLeftY - imageSize, imageSize, imageSize);
    } else {
        //draw a rightarrow
    }

    ctx.stroke();
    //    console.log("HELLO");
    //    console.log(start);
    //    console.log(end);
    //    console.log(startX);
    //    console.log(endX);
}

function annotateCovariate(cdata, ctx, pvar, color)
{
    var time = pvar.date.getTime()/1000;
    var timeX = atime2screen(cdata, time);
    var startY = cdata.bottomLeftY;

    var tl = cdata.topLeftX;
    if (timeX < cdata.topLeftX || timeX > cdata.bottomRightX) {
        return;
    }

    ctx.beginPath();
    ctx.strokeStyle = color;
    ctx.fillStyle = color;

    ctx.arc(timeX, startY, 5 * cdata.scale, 0, 2 * Math.PI);
    ctx.fill();
    var imageSize = 32 * cdata.scale;
    ctx.drawImage(cdata.img_covariates_disabled_mini, timeX - imageSize, cdata.bottomLeftY - imageSize, imageSize, imageSize);
    ctx.stroke();
    //    console.log("HELLO");
    //    console.log(start);
    //    console.log(end);
    //    console.log(startX);
    //    console.log(endX);
}

function annotateCurveLoci(cdata, ctx, predData, index, color)
{
    var i, t, v;
    var peaks = predData.peaks;
    var troughs = predData.troughs;
    var filter = getAdjustmentFilter(cdata, true);

    ctx.strokeStyle = color;
    ctx.fillStyle = color;

    for (i = 0; i < peaks.length; ++i) {
        t = atime2screen(cdata, predData.timeAt(peaks[i]));
        v = acxn2screen(cdata, predData.valueAt(peaks[i]));
        if (!filter || filter(predData.timeAt(peaks[i]))) {
            if (t > cdata.topLeftX && t < cdata.bottomRightX) {
                ctx.beginPath();
                ctx.arc(t, v, 2 * cdata.scale, 0, 2 * Math.PI);
                ctx.fill();
                ctx.stroke();
            }
        }
    }
    for (i = 0; i < troughs.length; ++i) {
        t = atime2screen(cdata, predData.timeAt(troughs[i]));
        v = acxn2screen(cdata, predData.valueAt(troughs[i]));
        if (!filter || filter(predData.timeAt(troughs[i]))) {
            if (t > cdata.topLeftX && t < cdata.bottomRightX) {
                ctx.beginPath();
                ctx.arc(t, v, 2 * cdata.scale, 0, 2 * Math.PI);
                ctx.fill();
                ctx.stroke();
            }
        }
    }
}

function prepareValueForDisplay(cdata, val)
{
  var tmp = val*cdata.unitefforder
  return ((tmp > 100) ? Math.round(tmp) : Math.round(tmp*100)/100)
}

function findClosestValue(cdata, ctx, predictive, predData, index, color)
{
    if (cdata.mArea.tooltipX < 0 && cdata.mArea.tooltipY < 0) {
        return; // Tooltip not wanted...
    }

    var x = predData.times();
    var y = predData.values();
    if (!predData.closestPoint)
        return;
    var current = predData.closestPoint.currentindex;
    var mousexms = Math.max(0,ascreen2time(cdata, cdata.mArea.tooltipX));
    var mouseyug = ascreen2acxn(cdata, cdata.mArea.tooltipY);

    while (current < x.length && x[current] < mousexms ) {
        current++;
    }
    while (current > 0 && x[current] > mousexms) {
        current--;
    }
    var y1 = y[current];
    var y2 = y[current + 1];
    var dy = y2 - y1;

    var x1 = x[current];
    var x2 = x[current + 1];
    var dx = x2 - x1;

    var rat = cdata.xRatio;
    var minxx = cdata.minX;
    var val = y1 + (mousexms - x1)/dx * dy;

    var pointAtMeasure = [];
    var timeAtMeasure = [];
    if (predictive.pointsAtMeasures) {
        if (predictive.pointsAtMeasures.size() > 0) {
            var measureIndex;
            for (measureIndex = 0; measureIndex < predictive.pointsAtMeasures.size(); measureIndex ++) {
                pointAtMeasure[measureIndex] = predictive.pointsAtMeasures.at(measureIndex).value;
                timeAtMeasure[measureIndex] = predictive.pointsAtMeasures.at(measureIndex).time * 1000;
            }
        }
    }

    var currentPoint = {
        currentindex: current,
        pred: predData,
        diffY: Math.abs(mouseyug - val),
        x: mousexms,
        y: val,
        value: prepareValueForDisplay(cdata, val),
        time: formatDate(new Date(1000 * mousexms)),
        color: color,
        predIndex: index,
        mean: prepareValueForDisplay(cdata, predData.meanAt(current)),
        auc: prepareValueForDisplay(cdata, predData.aucAt(current)),
        auc24: prepareValueForDisplay(cdata, predData.auc24At(current)),
        cumulatedAuc: prepareValueForDisplay(cdata, predData.cumulatedAucAt(current)),
        trough: prepareValueForDisplay(cdata, predData.troughAt(current)),
        peak: prepareValueForDisplay(cdata, predData.peakAt(current)),
        timeAfterDose: predData.timeAfterDose(current),
        cycleDuration: predData.cycleDuration(current),
        measurePredicted : pointAtMeasure,
        measureTime : timeAtMeasure
    }
    //    console.log(cdata.mArea.tooltipX);
    //    console.log(topLeftX);
/*
    //set the closest prediction so we know which to highlight
    predData.closestPoint = currentPoint;
    predData.highlight = false;
    if (!cdata.canvas.closestPred.closestPoint|| currentPoint.diffY < cdata.canvas.closestPred.closestPoint.diffY) {
        if (currentPoint.diffY < cdata.canvas.highlightthresh) {
            predData.highlight = true;
        }
        cdata.canvas.closestPred = predData;
    }
*/
    if (predData.displayTooltip) {
        cdata.currentPoints[index] = currentPoint;
    }

    //    if (Math.abs(diffY) < cdata.canvas.highlightthresh) {
    //        currentPoint.highlight = true;
    //        if (predData.selected && cdata.mArea.pressed) {
    //            predData.pliable = true;
    //        } else {
    //            predData.pliable = false;
    //        }

    //    } else {
    //        currentPoint.highlight = false;
    //    }
    drawTooltips(cdata, ctx);
}

function isCurveAvailable(cdata, index)
{

    var adjTabShowPop = cdata.gInformationSelection.presentAposterioriPrediction && !cdata.hasPatientVariates && !cdata.hasMeasures;
    var adjTabShowApr = cdata.gInformationSelection.presentAposterioriPrediction && cdata.hasPatientVariates && !cdata.hasMeasures;

    var isAvailable = false;
    switch(index) {
        case cdata.pop: isAvailable = cdata.gInformationSelection.presentPopulationPrediction || adjTabShowPop; break;
        case cdata.apr: isAvailable = cdata.gInformationSelection.presentAprioriPrediction || adjTabShowApr; break;
        case cdata.apo: isAvailable = cdata.gInformationSelection.presentAposterioriPrediction; break;
        case cdata.rev: isAvailable = cdata.gInformationSelection.presentPossibleAdjustments; break;
        case cdata.mea: isAvailable = cdata.gInformationSelection.presentMeasures; break;
        case cdata.tar: isAvailable = cdata.gInformationSelection.presentTargets; break;
        case cdata.adj: isAvailable = cdata.gInformationSelection.presentSelectedAdjustment; break;
        default: break;
    }
    //console.log("Curve " + index + " is " + (isAvailable ? "available" : "not available"))
    return isAvailable;
}

function isCurveVisible(cdata, index)
{
    var isVisible = false;
    if (isCurveAvailable(cdata, index)) {
        switch(index) {
            case cdata.pop: isVisible = cdata.gInformationSelection.displayPopulationPrediction; break;
            case cdata.apr: isVisible = cdata.gInformationSelection.displayAprioriPrediction; break;
            case cdata.apo: isVisible = cdata.gInformationSelection.displayAposterioriPrediction; break;
            case cdata.rev: isVisible = cdata.gInformationSelection.displayPossibleAdjustments; break;
            case cdata.mea: isVisible = cdata.gInformationSelection.displayMeasures; break;
            case cdata.tar: isVisible = cdata.gInformationSelection.displayTargets; break;
            case cdata.adj: isVisible = cdata.gInformationSelection.displaySelectedAdjustment; break;
            default: break;
        }
    }
    //console.log("Curve " + index + " is " + (isVisible ? "visible" : "not visible"))
    return isVisible;
}

function drawTooltips(cdata, ctx)
{
    //Settings
    ctx.font         = cdata.tooltipFontSize + " " + cdata.police;
    ctx.textAlign    = "left";
    ctx.textBaseline = "Alphabetic";

    //Constants
    var timeText = "Time:";
    var timeAfterDoseText = "T after dose:";
    var valueText = "C predicted:";
    var averageText = "C average:";
    var troughText = "C trough:";
    var peakText = "C peak:";
    var aucText = "AUC";
    var cumulatedAucText = "Cum. AUC";

    var labelsWidth = ctx.measureText(timeAfterDoseText).width + 4 * cdata.scale;
    var valuesWidth, tooltipWidth, tooltipHeight, x, y;

    var previousY = [];

    for (var i = 0; i < cdata.currentPoints.length; ++i) {
        if (cdata.currentPoints[i]) {
            var filter = getAdjustmentFilter(cdata, cdata.currentPoints[i].predIndex !== cdata.adj);
            var t = cdata.currentPoints[i].x;
            if (!filter || filter(t)) {
                if (isCurveVisible(cdata, i) && cdata.mArea.tooltipX > cdata.topLeftX) {
                    x = atime2screen(cdata, cdata.currentPoints[i].x);
                    y = acxn2screen(cdata, cdata.currentPoints[i].y);
                    ctx.beginPath();
                    ctx.arc(x, y, 4 * cdata.scale, 0, 2 * Math.PI, true);
                    ctx.fillStyle = cdata.currentPoints[i].color;
                    ctx.fillStyle = "#e6e6e6";
                    ctx.fill();
                    ctx.stroke();

                    if (!cdata.currentMeasure) {
                        valuesWidth = Math.max(ctx.measureText(cdata.currentPoints[i].value).width, ctx.measureText(cdata.currentPoints[i].time).width) + 2;

                        tooltipWidth = (labelsWidth + valuesWidth);
                        tooltipHeight = (9*14+8 + cdata.currentPoints[i].measureTime.length * 14 * 2) * cdata.scale;
                        x = x - tooltipWidth  / 2;
                        y = y - tooltipHeight - 10 * cdata.scale;

                        for (var j = 0; j < previousY.length; ++j) {
                            if (y + tooltipHeight + 5 * cdata.scale > previousY[j] && y < previousY[j] + tooltipHeight + 5 * cdata.scale)
                                y = previousY[j] - tooltipHeight - 5 * cdata.scale;
                        }

                        previousY.push(y);

                        ctx.beginPath();
                        ctx.rect(x, y, tooltipWidth, tooltipHeight);

                        //ctx.globalAlpha = 0.5;
                        ctx.globalAlpha = 1.0;
                        ctx.fill();
                        ctx.globalAlpha = 1.0;
                        ctx.stroke();

                        ctx.fillStyle = "black";
                        var xText = x + 2
                        var yText = y + 12 * cdata.scale
                        ctx.fillText(timeText, xText, yText);
                        ctx.fillText(cdata.currentPoints[i].time, x + labelsWidth, yText);
                        yText = yText + 14 * cdata.scale
                        ctx.fillText(timeAfterDoseText, xText, yText);
                        ctx.fillText(cdata.currentPoints[i].timeAfterDose + "h", x + labelsWidth, yText);
                        yText = yText + 14 * cdata.scale
                        ctx.fillText(valueText, xText, yText);
                        ctx.fillText(cdata.currentPoints[i].value + " " + cdata.unit, x + labelsWidth, yText);
                        yText = yText + 14 * cdata.scale
                        var mean = cdata.currentPoints[i].mean
                        if (!(mean > 0)) mean = "-"
                        ctx.fillText(averageText, xText, yText);
                        ctx.fillText(mean + " " + cdata.unit, x + labelsWidth, yText);
                        yText = yText + 14 * cdata.scale
                        var trough = cdata.currentPoints[i].trough
                        if (!(trough > 0)) trough = "-"
                        ctx.fillText(troughText, xText, yText);
                        ctx.fillText(trough + " " + cdata.unit, x + labelsWidth, yText);
                        yText = yText + 14 * cdata.scale
                        var peak = cdata.currentPoints[i].peak
                        if (!(peak > 0)) peak = "-"
                        ctx.fillText(peakText, xText, yText);
                        ctx.fillText(peak + " " + cdata.unit, x + labelsWidth, yText);
                        yText = yText + 14 * cdata.scale
                        var auc = cdata.currentPoints[i].auc
                        if (!(auc > 0)) auc = "-"
                        ctx.fillText(aucText + " (" + cdata.currentPoints[i].cycleDuration + "h):" , xText, yText);
                        ctx.fillText(auc + " " + cdata.unit + "*h", x + labelsWidth, yText);
                        yText = yText + 14 * cdata.scale
                        var auc24 = cdata.currentPoints[i].auc24
                        if (!(auc24 > 0)) auc24 = "-"
                        ctx.fillText(aucText + " (24h):" , xText, yText);
                        ctx.fillText(auc24 + " " + cdata.unit + "*h", x + labelsWidth, yText);
                        yText = yText + 14 * cdata.scale
                        var cumulatedAuc = cdata.currentPoints[i].cumulatedAuc
                        if (!(cumulatedAuc > 0)) cumulatedAuc = "-"
                        ctx.fillText(cumulatedAucText, xText, yText);
                        ctx.fillText(cumulatedAuc + " " + cdata.unit + "*h", x + labelsWidth, yText);

                        if (cdata.currentPoints[i].measureTime.length > 0) {
                            var measureIndex;
                            for (measureIndex = 0; measureIndex < cdata.currentPoints[i].measureTime.length; measureIndex++) {
                                yText = yText + 14 * cdata.scale;
                                var textMeasure = "Value at ";
                                var textMeasure2 = formatDate(new Date(cdata.currentPoints[i].measureTime[measureIndex]));
                                ctx.fillText(textMeasure, xText, yText);
                                ctx.fillText(textMeasure2, x + labelsWidth, yText);

                                yText = yText + 14 * cdata.scale;
                                textMeasure = "is : ";
                                textMeasure2 = prepareValueForDisplay(cdata, cdata.currentPoints[i].measurePredicted[measureIndex]) + " " + cdata.unit;
                                ctx.fillText(textMeasure, xText, yText);
                                ctx.fillText(textMeasure2, x + labelsWidth, yText);
                            }
                        }
                    }
                }
            }
        }
    }
/*
    //currentPoints.length = 0;
    if (cdata.canvas.currentMeasure) {
        valuesWidth = Math.max(ctx.measureText(cdata.canvas.currentMeasure.value).width, ctx.measureText(cdata.canvas.currentMeasure.time).width) + 2;

        tooltipWidth = (labelsWidth + valuesWidth);
        tooltipHeight = 30;

        var x = cdata.canvas.currentMeasure.x * cdata.canvas.scalex - tooltipWidth  / 2;
        var y = cdata.canvas.currentMeasure.y * cdata.canvas.scaley - tooltipHeight - 10;

        ctx.beginPath();
        ctx.rect(x, y, tooltipWidth, tooltipHeight);

        ctx.globalAlpha = 0.5;
        ctx.fillStyle = cdata.canvas.currentMeasure.color;
        ctx.fill();
        ctx.globalAlpha = 1.0;
        ctx.stroke();

        ctx.fillStyle = "black";
        ctx.fillText(valueText, x + 2, y + 12);
        ctx.fillText(cdata.canvas.currentMeasure.value, x + labelsWidth, y + 12);
        ctx.fillText(timeText, x + 2, y + 26);
        ctx.fillText(cdata.canvas.currentMeasure.time, x + labelsWidth, y + 26);
    }
    cdata.canvas.currentMeasure = null;
*/
    ctx.restore();
    ctx.save();
}
