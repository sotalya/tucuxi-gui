
function step()
{
    var ctx = canvas.getContext('2d');
    //the min max is restricted to the window after scaling. We have to apply the scale to the values
    //then project the values onto the drawn space. So you have to know and use the scale and offset first.
    //console.log("painting!")
    //Init context
    initContext(ctx);
    ctx.save();
    extents(ctx);
    //    console.log(maxX + " " + minX);

    var adjTabShowPop = graphInformationSelection.presentAposterioriPrediction && !hasPatientVariates && !hasMeasures;
    var adjTabShowApr = graphInformationSelection.presentAposterioriPrediction && hasPatientVariates && !hasMeasures;

    //Draw the content
    //dont draw any curves if we cant even draw the population curves
    if (popP) {
        if (popP.predictive.predictionData.isValid) {
            if ((graphInformationSelection.presentPopulationPercentiles || adjTabShowPop) && graphInformationSelection.displayPopulationPercentiles) {
                if (popercsP.isValid) {
                    drawPercentiles(ctx, popercsP, colors[7], popcolors);
                }
            }
            
            //draw apriori if indicated in show
            if (aprP) {
                if (aprP.predictive.predictionData.isValid) {
                    if ((graphInformationSelection.presentAprioriPercentiles || adjTabShowApr) && graphInformationSelection.displayAprioriPercentiles) {
                        if (aprpercsP.isValid) {
                            drawPercentiles(ctx, aprpercsP, colors[8], aprcolors);
                        }
                    }
                }
            }

            //draw aposteriori if indicated in show and we have measures
            if (apoP) {
                if (apoP.predictive.predictionData.isValid) {
                    if (graphInformationSelection.presentAposterioriPercentiles && hasMeasures && graphInformationSelection.displayAposterioriPercentiles) {
                        if (apopercsP.isValid) {
                            drawPercentiles(ctx, apopercsP, colors[6], apocolors);
                        }
                    }
                }
            }
        }
    }


    //Draw the content
    //dont draw any curves if we cant even draw the population curves
    if (popP) {
        if (popP.predictive.predictionData.isValid) {

            //draw pop curves if indicated in show or measurestab with no measures
            if ((graphInformationSelection.presentPopulationPrediction || adjTabShowPop) && graphInformationSelection.displayPopulationPrediction) {
                drawPop(ctx, colors, popcolors);
            }

            //draw apriori if indicated in show
            if (aprP) {
                if (aprP.predictive.predictionData.isValid) {

                    if ((graphInformationSelection.presentAprioriPrediction || adjTabShowApr) && graphInformationSelection.displayAprioriPrediction) {
                        drawApr(ctx, colors, aprcolors);
                    }
                }
            }

            //draw aposteriori if indicated in show and we have measures
            if (apoP) {
                if (apoP.predictive.predictionData.isValid) {

                    if (graphInformationSelection.presentAposterioriPrediction && hasMeasures && graphInformationSelection.displayAposterioriPrediction) {
                        drawApo(ctx, colors, apocolors);
                    }
                }
            }
        }
    }


    //verify compatibility between drugmodel constraint and covariate
    if (aprP.predictive.predictionData.isValid){
        checkAndDisplayDomain(ctx, aprP)
    }

    if (adjP) {
        if (graphInformationSelection.presentSelectedAdjustment && graphInformationSelection.displaySelectedAdjustment) {

            if (adjpercsP.isValid) {

                if ((apoP) && (apoP.predictive.predictionData.isValid)) {
                    drawPercentilesAdjustments(ctx, adjpercsP, apocolors[0], apocolors);
                }
                else {
                    drawPercentilesAdjustments(ctx, adjpercsP, aprcolors[0], aprcolors);
                }
            }

            if ((apoP) && (apoP.predictive.predictionData.isValid)) {
                drawAdjustment(ctx, colors[4]);
            }
            else {
                drawAdjustment(ctx, colors[2]);

            }
        }
    }

    //draw adjustments if we have, and indicated in show
    if (revP && revP.isValid) {
        if (graphInformationSelection.presentPossibleAdjustments && graphInformationSelection.displayPossibleAdjustments) {
            drawReverse(ctx, colors, revcolors);
        }
    }

    //draw measures
    if (graphInformationSelection.presentMeasures && graphInformationSelection.displayMeasures) {
        drawMeasures(ctx);
        ctx.restore();
        ctx.save();
    }

    //draw targets if have, and indicated in show
    if (hasTargets && graphInformationSelection.presentTargets && graphInformationSelection.displayTargets) {
        if (apoP && apoP.predictive.predictionData.isValid) {
            drawTargets(ctx, apoP.X, apoP.predictive.predictionData);
        } else if (aprP && aprP.predictive.predictionData.isValid) {
            drawTargets(ctx, aprP.X, aprP.predictive.predictionData);
        } else if (popP && popP.predictive.predictionData.isValid) {
            drawTargets(ctx, popP.X, popP.predictive.predictionData);
        } else if (adjP && adjP.predictive.predictionData.isValid) {
            drawTargets(ctx, adjP.X, adjP.predictive.predictionData);
        } else if (revP && revP.size()>0) {
            var predData = revP.objat(0);
            if (predData.predictionData.isValid) {
                drawTargets(ctx, predData.X, predData.predictionData);
            }
        }
        else {
            var tmin = ascreen2time(bottomLeftX)
            var tmax = ascreen2time(bottomRightX)
            drawTargets(ctx, [tmin, tmax], null);
        }
        ctx.restore();
        ctx.save();
    }

    //clear the border to draw non plot area
    clearBorder(ctx);

    //draw non plot area (axes, legends, etc)
    drawNonPlotArea(ctx, colors, pop);

    //draw descriptions
//    if (!graphInformationSelection.presentPopulationPrediction &&
//        !graphInformationSelection.presentAprioriPrediction &&
//        !graphInformationSelection.presentAposterioriPrediction) {
//        if (nographdrugtext) {
//            drawSoftwareDescription(ctx, nographdrugtext);
//        }
//        else {
//            drawSoftwareDescription(ctx, nographtext);
//        }
//    }
}

function checkAndDisplayDomain(ctx, pred)
{
    if (!pred.isValidDomain){
        ctx.textAlign = 'end';
        ctx.strokeText(pred.domainMessage, topRightX, topRightY + 10);
    }
}

function updateUnits()
{
    for (var i = 0; i < measures.length; ++i) {
        measures[i].unitstring = unit;
    }
    for (var i = 0; i < targets.length; ++i) {
        targets[i].cmin.unitstring = unit;
        targets[i].cmax.unitstring = unit;
        targets[i].cbest.unitstring = unit;
        targets[i].tmin.unitstring = unit;
        targets[i].tmax.unitstring = unit;
        targets[i].tbest.unitstring = unit;
    }
    interpretationController.defaultUnit = unit;
}

function getAdjustmentFilter(filterMax)
{
    if (graphInformationSelection.presentSelectedAdjustment) {
        var limit = adjustmentTabController.adjustmentDate/1000;
        if (filterMax) return function(d) { return d < limit; }
        return function(d) { return d > limit; }
    }
    return
}

function drawPop(ctx, colors, popcolors)
{
    drawCurve(ctx, popP.predictive.predictionData, popcolors[0], getAdjustmentFilter(true));
    ctx.restore();
    ctx.save();
}

function drawApr(ctx, colors, aprcolors)
{
    drawCurve(ctx, aprP.predictive.predictionData, colors[2], getAdjustmentFilter(true));
    ctx.restore();
    ctx.save();
}

function drawApo(ctx, colors, apocolors)
{
    drawCurve(ctx, apoP.predictive.predictionData, colors[4], getAdjustmentFilter(true));
    ctx.restore();
    ctx.save();
}

function drawReverse(ctx, colors, revcolors)
{
    for (var i = 0; i < revP.size(); ++i)
    {
        // Display all adjustments if less than 6 adjustement are proposed
        // otherwise only display the currently selected one.
        if (revP.size()<=6 || revP.objat(i).predictionData.selected) {
            drawCurve(ctx, revP.objat(i).predictionData, colors[5], getAdjustmentFilter(false));
        }

        // Display the first 5 adjustments, and the selected one
        //if (i <= 4 || revP.objat(i).predictionData.selected) {
        //    drawCurve(ctx, revP.objat(i).predictionData, colors[5], getAdjustmentFilter(false));
        //}
        ctx.restore();
        ctx.save();
    }
}

function drawAdjustment(ctx, color)
{
    drawCurve(ctx, adjP.predictive.predictionData, color, getAdjustmentFilter(false));
    ctx.restore();
    ctx.save();
}

function drawPercentiles(ctx, pairs, color, colors)
{
    if (pairs.size() < 1) {return;}
    colorRegionBtwCurves(ctx, pairs.objat(0).predictionData,  pairs.objat(3).predictionData, pairs.objat(0).X, pairs.objat(0).Y, pairs.objat(3).Y, colors[2], getAdjustmentFilter(true));
    ctx.restore();
    ctx.save();
    colorRegionBtwCurves(ctx, pairs.objat(1).predictionData,  pairs.objat(2).predictionData, pairs.objat(0).X, pairs.objat(1).Y, pairs.objat(2).Y, colors[1], getAdjustmentFilter(true));
    ctx.restore();
    ctx.save();
    for (var i = 0; i < pairs.size(); ++i) {
        drawCurve(ctx, pairs.objat(i).predictionData, color, getAdjustmentFilter(true));
        ctx.restore();
        ctx.save();
    }
}

function drawPercentilesAdjustments(ctx, pairs, color, colors)
{
    if (pairs.size() < 1) {return;}
    for (var i = 0; i < pairs.size(); ++i) {
        drawCurve(ctx, pairs.objat(i).predictionData, color, false);
        ctx.restore();
        ctx.save();
    }
    colorRegionBtwCurves(ctx, pairs.objat(0).predictionData,  pairs.objat(3).predictionData, pairs.objat(0).X, pairs.objat(0).Y, pairs.objat(3).Y, colors[2], false);
    ctx.restore();
    ctx.save();
    colorRegionBtwCurves(ctx, pairs.objat(1).predictionData,  pairs.objat(2).predictionData, pairs.objat(0).X, pairs.objat(1).Y, pairs.objat(2).Y, colors[1], false);
    ctx.restore();
    ctx.save();
}

function drawNonPlotArea(ctx, colors, index)
{
    //Draw the plot
    drawAxis(ctx);
    ctx.restore();
    ctx.save();
    drawAxisTicks(ctx);
    drawAxisLabels(ctx);
    //Draw the labels
    drawLegends(ctx, colors);
}

function drawSoftwareDescription(ctx, text)
{
    ctx.fillText(text, 100, 50);
}

function colorRegionBtwCurves(ctx, predDataL, predDataU, dataX, dataY, dataYY, color, filter)
{
    ctx.globalAlpha = 0.2;
    ctx.globalAlpha = 1.0;
    ctx.lineWidth   = 2.0;// / scalex;
    ctx.lineCap     = "round";
    ctx.lineJoin    = "round";
    ctx.beginPath();

    if (dataX === undefined || dataY === undefined) {
        console.log("No data avaialable in colorRegionBtwCurves");
        return;
    }

    var x1, x2, y1, y2;
    var j = 0;
    x1 = atime2screen(dataX[j]);
    y1 = acxn2screen(dataY[j]);
    ctx.moveTo(x1, y1);
    for (; j < dataX.length - 1; j++) {
        if (filter && !filter(dataX[j+1])) {
            break;
        }
        x2 = atime2screen(dataX[j+1]);
        y2 = acxn2screen(dataY[j+1]);
        ctx.lineTo(x2, y2);
    }
    ctx.lineTo(x2, y2);
    x1 = atime2screen(dataX[j]);
    y1 = acxn2screen(dataYY[j]);
    ctx.lineTo(x1, y1);
    for (; j > -1; j--) {
        x2 = atime2screen(dataX[j-1]);
        y2 = acxn2screen(dataYY[j-1]);
        ctx.lineTo(x2, y2);
    }
    ctx.closePath();
    ctx.fillStyle = color;
    ctx.fill();
}

function initContext(ctx)
{
    //Clear the canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    hasMeasures = measures && (measures.length > 0);
    hasPatientVariates = pvars && (pvars.length > 0);
    hasTargets = targets && (targets.length > 0);

    //Default settings
    ctx.font        = "12px " + police;
    ctx.globalAlpha = 1.0;
    ctx.lineWidth   = 1.0;
    ctx.strokeStyle = "black";
    ctx.fillStyle   = "black";
}

function manualextents(ctx)
{
    interpretationController.resetViewRange();
    minX = interpretationController.getViewRangeMin();
    maxX = interpretationController.getViewRangeMax();
}

function extents(ctx)
{
    minX = interpretationController.getViewRangeMin()
    maxX = interpretationController.getViewRangeMax()

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

    if (popercsP) {
        if (!popercsP.isEmpty()) {
            for (var i = 0; i < popercsP.size(); ++i) {
                populationpercmax = Math.max(populationpercmax, Math.max.apply(Math, popercsP.objat(i).Y));
                populationpercmin = Math.min(populationpercmin, Math.min.apply(Math, popercsP.objat(i).Y));
            }
        }
    }

    if (aprpercsP) {
        if (!aprpercsP.isEmpty()) {
            for (var i = 0; i < aprpercsP.size(); ++i) {
                aprioripercmax = Math.max(aprioripercmax, Math.max.apply(Math, aprpercsP.objat(i).Y));
                aprioripercmin = Math.min(aprioripercmin, Math.min.apply(Math, aprpercsP.objat(i).Y));
            }
        }
    }

    if (apopercsP) {
        if (!apopercsP.isEmpty()) {
            for (var i = 0; i < apopercsP.size(); ++i) {
                aposterioripercmax = Math.max(aposterioripercmax, Math.max.apply(Math, apopercsP.objat(i).Y));
                aposterioripercmin = Math.min(aposterioripercmin, Math.min.apply(Math, apopercsP.objat(i).Y));
            }
        }
    }

    if (adjpercsP) {
        if (!adjpercsP.isEmpty()) {
            for (var i = 0; i < adjpercsP.size(); ++i) {
                adjustmentpercmax = Math.max(adjustmentpercmax, Math.max.apply(Math, adjpercsP.objat(i).Y));
                adjustmentpercmin = Math.min(adjustmentpercmin, Math.min.apply(Math, adjpercsP.objat(i).Y));
            }
        }
    }

    if (revP) {
        if (!revP.isEmpty()) {
            for (var i = 0; i < revP.size(); ++i) {
                if (revP.size()<=5 || revP.objat(i).predictionData.selected) {
                    reversepercmax = Math.max(reversepercmax, Math.max.apply(Math, revP.objat(i).Y));
                    reversepercmin = Math.min(reversepercmin, Math.min.apply(Math, revP.objat(i).Y));
                }
            }
        }
    }


    // Find the maximum Y value. Could be a function of ChartData...
    maxY = 0;
    if (popP)
        maxY = Math.max(maxY, Math.max.apply(Math, popP.Y));
    if (aprP)
        maxY = Math.max(maxY, Math.max.apply(Math, aprP.Y));
    if (apoP)
        maxY = Math.max(maxY, Math.max.apply(Math, apoP.Y));
    if (adjP)
        maxY = Math.max(maxY, Math.max.apply(Math, adjP.Y));
    maxY = Math.max(maxY, populationpercmax, aprioripercmax, aposterioripercmax, adjustmentpercmax, reversepercmax);

    // In case we have no other data, use target data to define the Y range...
    if (maxY == 0) {
        for (var targetIndex = 0; targetIndex < targets.length; ++targetIndex) {
            var targetY = targets[targetIndex].cmax.dbvalue * targets[targetIndex].cmax.multiplier;
            if (targetY > maxY) maxY = targetY;
        }
    }

    var lg10 = Math.pow(10, Math.ceil(Math.log(1.1 * maxY) / Math.LN10) - 1);
    maxY = Math.ceil(1.1 / lg10 * maxY) * lg10;

    xRatio = plotWidth  / (maxX - minX);
    yRatio = plotHeight / (maxY - minY);

    //                console.log("maxX: " + maxX);
    //                console.log("minX: " + minX);
    //                console.log("maxY: " + maxY);
    //                console.log("minY: " + minY);
}

function drawCurve(ctx, predData, color, filter)
{
    var dataX = predData.times();
    var dataY = predData.values();

    //Settings
    ctx.strokeStyle = color;
    ctx.lineWidth   = 2.0;
    ctx.lineCap     = "butt";
    ctx.lineJoin    = "round";

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    //Draw the curve
    //    ctx.path = "";
    ctx.beginPath();
    var isFuture = false;
    var highlight = predData === canvas.closestPred && canvas.closestPred.highlight;

    // This variable is used to know if we need to move before drawing the line.
    var continueLine = false;

    for (var i = 0; i < dataX.length - 2; i++)
    {
        if (!filter || filter(dataX[i+1]))
        {
            var x1 = atime2screen(dataX[i]);
            var y1 = acxn2screen(dataY[i]);
            var x2 = atime2screen(dataX[i+1]);
            var y2 = acxn2screen(dataY[i+1]);

            if (highlight) {
                ctx.lineWidth   = 4.0;
            }

            if (predData.selected) {
                ctx.lineWidth   = 6.0;
                ctx.strokeStyle = color + 1;
            }

            if (dataX[i] > date.getTime() / 1000) {
                if (!isFuture) {
                    ctx.stroke();
                    ctx.beginPath();
                    ctx.globalAlpha = 0.6;
                    continueLine = false;
                } else {
                    isFuture = true;
                }
            }

            if (x1 > topLeftX && x2 < canvas.bottomRightX) {
                if (!continueLine) {
                    ctx.moveTo(x1, y1);
                    continueLine = true;
                }
                ctx.lineTo(x2, y2);
            }
        }
        else {
            continueLine = false;
        }
    }
    if (highlight) {
        ctx.lineWidth   = 4.0;
    }
    if (predData.selected) {
        ctx.lineWidth   = 6.0;
        ctx.strokeStyle = color + 1;
    }

    ctx.stroke();
}

function drawMeasures(ctx)
{
    //Settings
    ctx.fillStyle = "red";
    ctx.lineWidth = 1;// / scalex;

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    var mouseX = ascreen2time(mouseArea.mouseX);
    var mouseY = ascreen2acxn(mouseArea.mouseY);

    //Draw the samples
    for (var i = 0; i < measures.length; ++i) {//measures.length; ++i) {
        var x = atime2screen(measures[i].moment.getTime() / 1000);
        var y = acxn2screen(measures[i].concentration.dbvalue * measures[i].concentration.multiplier);

        //console.log("measurex: " + x)
        //console.log("measurey: " + y)
        ctx.beginPath();
        ctx.arc(x, y, 5, 0, 2 * Math.PI, true);
        ctx.fill();
        ctx.stroke();

        if (checkPointProximity(x, y, mouseX, mouseY, true) && mouseArea.isMouseOver) {
            currentMeasure = {
                x: x,
                y: y,
                value: measures[i].concentration.dbvalue.toFixed(2) + " " + measures[i].concentration.unitstring,
                time: formatDate(measures[i].moment),
                color: "red"
            }
        }
    }
}

function drawTargets(ctx, times, predData)
{
    //Settings
    if (!targets) return;
    ctx.fillStyle = "blue";
    ctx.lineWidth = 1;

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    var mouseX = ascreen2time(mouseArea.mouseX);
    var mouseY = ascreen2acxn(mouseArea.mouseY);

    // console.log("Nb targets = " + targets.length);

    for (var targetIndex = 0; targetIndex < targets.length; ++targetIndex)
    {   
        var ttpe = targets[targetIndex].type.value;
        var y_mean = acxn2screen(targets[targetIndex].cbest.dbvalue * targets[targetIndex].cbest.multiplier);
        var y_max = acxn2screen(targets[targetIndex].cmax.dbvalue * targets[targetIndex].cmax.multiplier);
        var y_min = acxn2screen(targets[targetIndex].cmin.dbvalue * targets[targetIndex].cmin.multiplier);
        var first = atime2screen(times[0]);
        var last = atime2screen(times[times.length - 1]);


        if (ttpe !== 3) {
            ctx.globalAlpha = 0.05;
            if (targetTab.index === targetIndex) {
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
            for (var i = 0; i < troughs.length; ++i) {
                ctx.beginPath();
                var t = atime2screen(predData.timeAt(troughs[i]));
                if (isFinite(t) && !isNaN(t) && !isNaN(y_mean)) {
                    var v = y_mean;
                    var leftgrd = ctx.createLinearGradient(t, y_mean, t - 20, y_mean);
                    leftgrd.addColorStop(0,"black");
                    leftgrd.addColorStop(1,"white");
                    var rightgrd = ctx.createLinearGradient(t, y_mean, t + 20, y_mean);
                    rightgrd.addColorStop(0,"black");
                    rightgrd.addColorStop(1,"white");
                    ctx.fillStyle = leftgrd
                    ctx.fillRect(t - 20, y_min, 20, y_max - y_min);
                    ctx.fillStyle = rightgrd
                    ctx.fillRect(t, y_min, 20, y_max - y_min);
                    ctx.stroke();

                    var startY = bottomLeftY;

                    ctx.globalAlpha = 1;
                    ctx.beginPath();
                    // Bottom arrow
                    var arrowWidth = 3;
                    var arrowHeight = 4;
                    var diamondWidth = 4;
                    var diamondHeight = 5;
                    var crossSize = 6;
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
            for (var i = 0; i < peaks.length; ++i) {
                ctx.beginPath();
                var t = atime2screen(predData.timeAt(peaks[i]));
                var v = y_mean;
                var leftgrd = ctx.createLinearGradient(t, y_mean, t - 20, y_mean);
                leftgrd.addColorStop(0,"black");
                leftgrd.addColorStop(1,"white");
                var rightgrd = ctx.createLinearGradient(t, y_mean, t + 20, y_mean);
                rightgrd.addColorStop(0,"black");
                rightgrd.addColorStop(1,"white");
                ctx.fillStyle = leftgrd
                ctx.fillRect(t - 20, y_min, 20, y_max - y_min);
                ctx.fillStyle = rightgrd
                ctx.fillRect(t, y_min, 20, y_max - y_min);
                ctx.stroke();

                var startY = bottomLeftY;

                ctx.globalAlpha = 1;
                ctx.beginPath();
                // Bottom arrow
                var arrowWidth = 3;
                var arrowHeight = 4;
                var diamondWidth = 4;
                var diamondHeight = 5;

                var crossSize = 6;
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

function clearBorder(ctx)
{
    ctx.clearRect(0, 0, leftMargin, plotHeight + topMargin);
    ctx.clearRect(0, bottomLeftY, plotWidth + leftMargin, bottomMargin);
    ctx.clearRect(topRightX, topRightY, rightMargin, plotHeight + bottomMargin);
    ctx.clearRect(topLeftX, 0, plotWidth + rightMargin, topMargin);
}

function drawAxis(ctx)
{
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

function acxn2screen(c)
{
    return  (-(c - minY) * yRatio) + bottomLeftY;
}

function ascreen2acxn(p)
{
    return minY - ((p - bottomLeftY) / yRatio);
}

function atime2screen(t)
{
    return ((t - minX) * xRatio) + bottomLeftX;
}

function ascreen2time(p)
{
    return minX + ((p - bottomLeftX) / xRatio);
}

function drawAxisTicks(ctx)
{
    //Settings
    ctx.textAlign    = "left";
    ctx.textBaseline = "middle";
    ctx.font         = "10px " + police;

    //ToDo: Ignore filtered curves in the calculation of max and min, and move it canvas properties

    var tickSize = 10;
    var tickSpacingy = 0.2;

    //Draw y-ticks
    var down = -1.0;
    var up = 1.0;
    var down_i = Math.ceil(down / tickSpacingy);
    var up_i = Math.floor(up / tickSpacingy);
    var remy = down_i * tickSpacingy - down;
    var firstTickY = -1.0 + remy;

    var tickOffsetY = bottomLeftY - 1.0 - firstTickY;
    var tickIncrementY = tickSpacingy * plotHeight / 2;

    var nbTicksY = up_i - down_i + 1;
    if (nbTicksY > 21)
        nbTicksY = 21;

    var order = (2 - Math.log(maxY - minY) / Math.LN10);
    if (order >= 1 && order < 4) {
        unitefforder = 1e3;
        unit = "ng/l";
    }
    if (order >= 4 && order < 7) {
        unitefforder = 1e6;
        unit = "pg/l";
    }
    if (order > -5 && order < -2) {
        unitefforder = 1e-3;
        unit = "mg/l";
    }
    if (order > -8 && order < -5) {
        unitefforder = 1e-6;
        unit = "g/l";
    }
    if (order < 1 && order >= -2) {
        unitefforder = 1;
        unit = "ug/l";
    }

    for (var i = 0; i < nbTicksY; i++) {
        var y = tickOffsetY - i * tickIncrementY;
        var tickScaleY = ((i + down_i) % 10) ? 0.5 : 1;

        ctx.beginPath();
        ctx.moveTo(bottomLeftX, y);
        ctx.lineTo(bottomLeftX - tickSize * tickScaleY, y);
        ctx.stroke();


        var val = minY + (0.5 * i * tickSpacingy) * (maxY - minY);
        //use the range of Y to calculate the required units to show max 1000 as max Y
        ctx.fillText(Math.max(0,Math.round(unitefforder * val)), bottomLeftX - tickSize * 5.5, y);
    }

    //Draw x-ticks
    var tickSpacingx;
    var ticktimes = [];
    var earliest = ascreen2time(bottomLeftX);
    var latest = ascreen2time(canvas.width);
    var days = (latest - earliest) / 86400.0;
    var date = new Date(earliest * 1000);
    //    console.log("days: " + days);
    //    console.log("earliest: " + earliest);
    //    console.log("latest: " + latest);
    //    console.log("date: " + date.toString());
    if (days > 2 && days < 7) {
        //1 tick / 12 hr
        ticktimes = chopTimeAxis(function(d) {
            d.setMilliseconds(0);
            d.setSeconds(0);
            d.setMinutes(0);
            return d.setHours(Math.ceil(d.getHours() / 12) * 12 % 24);
        }, 43200);
    } else {
        if (days < 2) {
            if (days > 1) {
                //1 tick / 4 hr
                ticktimes = chopTimeAxis(function(d) {
                    d.setMilliseconds(0);
                    d.setSeconds(0);
                    d.setMinutes(0);
                    return d.setHours(Math.ceil(d.getHours() / 4) * 4 % 24);
                }, 14400);
            } else {
                if (days > 0.25) {
                    //1 tick / hr
                    ticktimes = chopTimeAxis(function(d) {
                        d.setMilliseconds(0);
                        d.setSeconds(0);
                        d.setMinutes(0);
                        return d.setHours(Math.ceil(d.getHours()) % 24);
                    }, 3600);
                } else {
                    if (days > 0.1) {
                        //1 tick / every 30 minutes
                        ticktimes = chopTimeAxis(function(d) {
                            d.setMilliseconds(0);
                            d.setSeconds(0);
                            return d.setMinutes(Math.ceil(d.getMinutes() / 30) * 30 % 60);
                        }, 1800);
                    } else {
                        //1 tick / every 15 minutes
                        ticktimes = chopTimeAxis(function(d) {
                            d.setMilliseconds(0);
                            d.setSeconds(0);
                            return d.setMinutes(Math.ceil(d.getMinutes() / 15) * 15 % 60);
                        }, 900);
                    }

                }

            }
        } else {
            //1 tick / every day
            if (days < 25) {
                ticktimes = chopTimeAxis(function(d) {
                    d.setMilliseconds(0);
                    d.setSeconds(0);
                    d.setMinutes(0);
                    return d.setHours(Math.ceil(d.getHours() / 24) * 24 % 24);
                }, 86400);
            } else {
                //1 tick / every week
                ticktimes = chopTimeAxis(function(d) {
                    d.setMilliseconds(0);
                    d.setSeconds(0);
                    d.setMinutes(0);
                    return d.setHours(Math.ceil(d.getHours() / 24) * 24 % 24);
                }, 604800);
            }
        }
    }

    for (i = 0; i < ticktimes.length; i++) {
        ctx.beginPath();
        ctx.moveTo(atime2screen(ticktimes[i]), bottomLeftY);
        ctx.lineTo(atime2screen(ticktimes[i]), bottomLeftY + tickSize);
        ctx.stroke();

        var date = new Date(ticktimes[i] * 1000);
        //        console.log(date.toString());
        ctx.translate(atime2screen(ticktimes[i]), bottomLeftY + tickSize * 1.5);
        ctx.rotate(Math.PI/5);
        ctx.fillText(formatDate(date), 0, 0);
        ctx.rotate(-Math.PI/5);
        ctx.translate(-atime2screen(ticktimes[i]), -(bottomLeftY + tickSize * 1.5));
    }

    ctx.restore();
    ctx.save();
}

function chopTimeAxis(fxn, interval)
{
    var early = ascreen2time(bottomLeftX) * 1000;
    var late = ascreen2time(canvas.width - bottomLeftX);
    var date = new Date(early);
    var tick = fxn(date);
    tick = tick / 1000;
    var ticktimes = [];
    while (tick < late) {
        if (tick * 1000 >= early) {
            ticktimes.push(tick);
        }
        tick = tick + interval;
    }
    return ticktimes;
}

function drawAxisLabels(ctx)
{
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

function drawLegends(ctx, colors)
{
    //Settings
    ctx.textAlign    = "left";
    ctx.textBaseline = "Alphabetic";
    ctx.font         = "12px " + police;

    //Constants
    var populationText  = "Population";
    var popPercText     = "Pop. percentiles (5-25-75-95)"
    var aprioriText     = "A priori";
    var aposterioriText = "A posteriori";
    var reverseText     = "Suggested adjustments";
    var adjustmentText  = "Adjustments";
    var aprPercText		= "Apr. percentiles (5-25-75-95)"
    var apoPercText		= "Apo. percentiles (5-25-75-95)"

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

    var adjTabShowPop = graphInformationSelection.presentAposterioriPrediction && !hasPatientVariates && !hasMeasures;
    var adjTabShowApr = graphInformationSelection.presentAposterioriPrediction && hasPatientVariates && !hasMeasures;


    if (popP) {
        if (popP.predictive.predictionData.isValid) {
            if (graphInformationSelection.presentPopulationPrediction || adjTabShowPop) {
                legends.push( {text: populationText, color: colors[1]} );
                legendsWidth.push(ctx.measureText(populationText).width);
                //legendsWidth.push(ctx.measureText(popPCB.text).width);
            }
            if (popercsP.isValid && graphInformationSelection.presentPopulationPercentiles || adjTabShowPop) {
                legends.push( {text: popPercText, color: colors[7]} );
                legendsWidth.push(ctx.measureText(popPercText).width);
            }
        }
    }
    if (aprP) {
        if (aprP.predictive.predictionData.isValid) {
            if (graphInformationSelection.presentAprioriPrediction || adjTabShowApr) {
                legends.push( {text: aprioriText, color: colors[2]} );
                legendsWidth.push(ctx.measureText(aprioriText).width);
            }
            if (aprpercsP.isValid && graphInformationSelection.presentAprioriPercentiles || adjTabShowApr) {
                legends.push( {text: aprPercText, color: colors[8]} );
                legendsWidth.push(ctx.measureText(aprPercText).width);
            }
        }
    }
    if (apoP) {
        if (apoP.predictive.predictionData.isValid) {
            if (graphInformationSelection.presentAposterioriPrediction) {
                legends.push( {text: aposterioriText, color: colors[4]} );
                legendsWidth.push(ctx.measureText(aposterioriText).width);
            }
            if (apopercsP.isValid && graphInformationSelection.presentAposterioriPercentiles) {
                legends.push( {text: apoPercText, color: colors[6]} );
                legendsWidth.push(ctx.measureText(apoPercText).width);
            }
        }
    }
    if (revP) {
        if (revP.isValid && graphInformationSelection.presentPossibleAdjustments) {
            legends.push( {text: reverseText, color: colors[5]});
            legendsWidth.push(ctx.measureText(reverseText).width);
        }
    }
    if (adjP) {
        if (adjP.predictive.predictionData.isValid && graphInformationSelection.presentSelectedAdjustment) {
            legends.push( {text: adjustmentText, color: colors[9]} );
            legendsWidth.push(ctx.measureText(adjustmentText).width);
        }
    }

    for (var j = 0; j < legendsWidth.length; j++)
        totalLength += legendsWidth[j] + internalSpacing * 4 + colorSize;
    totalLength += externalSpacing * (legends.length - 1);

    var nLines = Math.ceil(totalLength / plotWidth);
    var nLegendsPerLine = legends.length/nLines;
    var nLegendsInLine = 0;
    var lengthPerLine = totalLength / nLines;

    var initialX = leftMargin + (plotWidth / 2) - (lengthPerLine / 2);
    var initialY = topMargin / 2 - boxHeight / 2 - 5;

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


function zoom(nSteps)
{
    var f = 1.0 - nSteps*0.1;
    interpretationController.zoomViewRange(f);
}

function shift(dScreenX)
{
    var dX = -dScreenX/xRatio
    interpretationController.shiftViewRange(dX);
}

function publishImage()
{
    //TODO done hardcode this size, figure it out
    grabToImage(function(result) {interpretationController.getImage(result.image)}, Qt.size(canvas.width,canvas.height));
}
