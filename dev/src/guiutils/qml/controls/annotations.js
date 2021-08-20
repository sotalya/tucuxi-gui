.import "graphing.js" as Graphing;

function step(cdata)
{
    var ctx = canvas.getContext("2d");

    ctx.lineWidth = 1;
    ctx.globalAlpha = 1.0;

    if (cdata.gInformationSelection.displayPopulationPrediction && canvas.state !== "validation") {
        annotateDosage(cdata, ctx, cdata.dosages.current, colors[1]);
    }

    if (cdata.gInformationSelection.displayAprioriPrediction) {
        if (cdata.gInformationSelection.displayCovariateChange) {
            for (var i = 0; i < cdata.pvars.length; ++i) {
                annotateCovariate(cdata, ctx, cdata.pvars[i], cdata.colors[2]);
            }
        }
    }

    canvas.currentPoints = []
    canvas.closestPred = ({})
    if (!mouseArea.containsMouse) {
        return;
    }

    //    ctx.clearRect(0, 0, canvas.width, canvas.height);
    annotatePrediction(cdata, ctx, cdata.popP, cdata.pop, cdata.colors[1]);
    annotatePrediction(cdata, ctx, cdata.aprP, cdata.apr, cdata.colors[2]);
    annotatePrediction(cdata, ctx, cdata.apoP, cdata.apo, cdata.colors[4]);
    annotatePrediction(cdata, ctx, cdata.adjP, cdata.adj, cdata.adjcolors[1]);

    if (cdata.revP !== null && cdata.revP.isValid) {
        if (cdata.gInformationSelection.displayPossibleAdjustments) {
            var pairs = cdata.revP;
            for (var i = 0; i < pairs.size(); ++i) {
                findClosestValue(ctx, pairs.objat(i), pairs.objat(i).predictionData, rev, cdata.colors[5]);
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
    var startX = Graphing.atime2screen(cdata, start);
    var startY = cdata.bottomLeftY;
    var endY = cdata.bottomLeftY;
    var endX = Graphing.atime2screen(cdata, end);
    //these are for checking if the start and end are within view, if not we draw it differenetly
    var startinview = true;
    var endinview = true;
    if (startX < cdata.topLeftX && endX > cdata.topLeftX) {
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

    ctx.arc(startX, startY, 5, 0, 2 * Math.PI);
    ctx.arc(endX, endY, 5, 0, 2 * Math.PI);
    ctx.fill();
    if (startinview) {
        ctx.drawImage("qrc:/icons/flow/dosages_disabled_mini.png", startX - 32, cdata.bottomLeftY - 32, 32, 32);
    } else {
        //draw a leftarrow
    }

    if (endinview) {
        ctx.drawImage("qrc:/icons/flow/dosages_disabled_mini.png", endX - 32, cdata.bottomLeftY - 32, 32, 32);
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
    var timeX = Graphing.atime2screen(cdata, time);
    var startY = cdata.bottomLeftY;

    var tl = cdata.topLeftX;
    if (timeX < cdata.topLeftX || timeX > cdata.bottomRightX) {
        return;
    }

    ctx.beginPath();
    ctx.strokeStyle = color;
    ctx.fillStyle = color;

    ctx.arc(timeX, startY, 5, 0, 2 * Math.PI);
    ctx.fill();
    ctx.drawImage("qrc:/icons/flow/covariates_disabled_mini.png", timeX - 32, cdata.bottomLeftY - 32, 32, 32);
    ctx.stroke();
    //    console.log("HELLO");
    //    console.log(start);
    //    console.log(end);
    //    console.log(startX);
    //    console.log(endX);
}

function annotateCurveLoci(cdata, ctx, predData, index, color)
{
    var peaks = predData.peaks;
    var troughs = predData.troughs;
    var filter = Graphing.getAdjustmentFilter(cdata, true);

    ctx.strokeStyle = color;
    ctx.fillStyle = color;

    for (var i = 0; i < peaks.length; ++i) {
        var t = Graphing.atime2screen(cdata, predData.timeAt(peaks[i]));
        var v = Graphing.acxn2screen(cdata, predData.valueAt(peaks[i]));
        if (!filter || filter(predData.timeAt(peaks[i]))) {
            if (t > cdata.topLeftX && t < cdata.bottomRightX) {
                ctx.beginPath();
                ctx.arc(t, v, 2, 0, 2 * Math.PI);
                ctx.fill();
                ctx.stroke();
            }
        }
    }
    for (var i = 0; i < troughs.length; ++i) {
        var t = Graphing.atime2screen(cdata, predData.timeAt(troughs[i]));
        var v = Graphing.acxn2screen(cdata, predData.valueAt(troughs[i]));
        if (!filter || filter(predData.timeAt(troughs[i]))) {
            if (t > topLeftX && t < cdata.bottomRightX) {
                ctx.beginPath();
                ctx.arc(t, v, 2, 0, 2 * Math.PI);
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
    if (mouseArea.tooltipX < 0 && mouseArea.tooltipY < 0) {
        return; // Tooltip not wanted...
    }

    var x = predData.times();
    var y = predData.values();
    if (!predData.closestPoint)
        return;
    var current = predData.closestPoint.currentindex;
    var mousexms = Math.max(0,Graphing.ascreen2time(cdata, mouseArea.tooltipX));
    var mouseyug = Graphing.ascreen2acxn(cdata, mouseArea.tooltipY);

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
        value: prepareValueForDisplay(val),
        time: formatDate(new Date(1000 * mousexms)),
        color: color,
        predIndex: index,
        mean: prepareValueForDisplay(predData.meanAt(current)),
        auc: prepareValueForDisplay(predData.aucAt(current)),
        auc24: prepareValueForDisplay(predData.auc24At(current)),
        cumulatedAuc: prepareValueForDisplay(predData.cumulatedAucAt(current)),
        trough: prepareValueForDisplay(predData.troughAt(current)),
        peak: prepareValueForDisplay(predData.peakAt(current)),
        timeAfterDose: predData.timeAfterDose(current),
        cycleDuration: predData.cycleDuration(current),
        measurePredicted : pointAtMeasure,
        measureTime : timeAtMeasure
    }
    //    console.log(mouseArea.tooltipX);
    //    console.log(topLeftX);
/*
    //set the closest prediction so we know which to highlight
    predData.closestPoint = currentPoint;
    predData.highlight = false;
    if (!canvas.closestPred.closestPoint|| currentPoint.diffY < canvas.closestPred.closestPoint.diffY) {
        if (currentPoint.diffY < canvas.highlightthresh) {
            predData.highlight = true;
        }
        canvas.closestPred = predData;
    }
*/
    if (predData.displayTooltip) {
        cdata.currentPoints[index] = currentPoint;
    }

    //    if (Math.abs(diffY) < canvas.highlightthresh) {
    //        currentPoint.highlight = true;
    //        if (predData.selected && mouseArea.pressed) {
    //            predData.pliable = true;
    //        } else {
    //            predData.pliable = false;
    //        }

    //    } else {
    //        currentPoint.highlight = false;
    //    }
    drawTooltips(cdata, ctx);
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

function isCurveAvailable(cdata, index)
{
    if (!cdata.gInformationSelection) {
        console.trace();
    }

    var adjTabShowPop = cdata.gInformationSelection.presentAposterioriPrediction && !cdata.hasPatientVariates && !cdata.hasMeasures;
    var adjTabShowApr = cdata.gInformationSelection.presentAposterioriPrediction && cdata.hasPatientVariates && !cdata.hasMeasures;

    var isAvailable = false;
    switch(index) {
        case pop: isAvailable = cdata.gInformationSelection.presentPopulationPrediction || adjTabShowPop; break;
        case apr: isAvailable = cdata.gInformationSelection.presentAprioriPrediction || adjTabShowApr; break;
        case apo: isAvailable = cdata.gInformationSelection.presentAposterioriPrediction; break;
        case rev: isAvailable = cdata.gInformationSelection.presentPossibleAdjustments; break;
        case mea: isAvailable = cdata.gInformationSelection.presentMeasures; break;
        case tar: isAvailable = cdata.gInformationSelection.presentTargets; break;
        case adj: isAvailable = cdata.gInformationSelection.presentSelectedAdjustment; break;
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
            case pop: isVisible = cdata.gInformationSelection.displayPopulationPrediction; break;
            case apr: isVisible = cdata.gInformationSelection.displayAprioriPrediction; break;
            case apo: isVisible = cdata.gInformationSelection.displayAposterioriPrediction; break;
            case rev: isVisible = cdata.gInformationSelection.displayPossibleAdjustments; break;
            case mea: isVisible = cdata.gInformationSelection.displayMeasures; break;
            case tar: isVisible = cdata.gInformationSelection.displayTargets; break;
            case adj: isVisible = cdata.gInformationSelection.displaySelectedAdjustment; break;
            default: break;
        }
    }
    //console.log("Curve " + index + " is " + (isVisible ? "visible" : "not visible"))
    return isVisible;
}

function drawTooltips(cdata, ctx)
{
    //Settings
    ctx.font         = "10px " + cdata.police;
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

    var labelsWidth = ctx.measureText(timeAfterDoseText).width + 4;
    var valuesWidth, tooltipWidth, tooltipHeight, x, y;

    var previousY = [];

    for (var i = 0; i < cdata.currentPoints.length; ++i) {
        if (cdata.currentPoints[i]) {
            var filter = Graphing.getAdjustmentFilter(cdata.currentPoints[i].predIndex !== adj);
            var t = cdata.currentPoints[i].x;
            var a = cdata
            if (!filter || filter(t)) {
                if (isCurveVisible(i) && mouseArea.tooltipX > cdata.topLeftX) {
                    x = Graphing.atime2screen(cdata, cdata.currentPoints[i].x);
                    y = Graphing.acxn2screen(cdata, cdata.currentPoints[i].y);
                    ctx.beginPath();
                    ctx.arc(x, y, 4, 0, 2 * Math.PI, true);
                    ctx.fillStyle = cdata.currentPoints[i].color;
                    ctx.fillStyle = "#e6e6e6";
                    ctx.fill();
                    ctx.stroke();

                    if (!cdata.currentMeasure) {
                        valuesWidth = Math.max(ctx.measureText(cdata.currentPoints[i].value).width, ctx.measureText(cdata.currentPoints[i].time).width) + 2;

                        tooltipWidth = (labelsWidth + valuesWidth);
                        tooltipHeight = 9*14+8 + cdata.currentPoints[i].measureTime.length * 14 * 2;
                        x = x - tooltipWidth  / 2;
                        y = y - tooltipHeight - 10;

                        for (var j = 0; j < previousY.length; ++j) {
                            if (y + tooltipHeight + 5 > previousY[j] && y < previousY[j] + tooltipHeight + 5)
                                y = previousY[j] - tooltipHeight - 5;
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
                        var yText = y + 12
                        ctx.fillText(timeText, xText, yText);
                        ctx.fillText(cdata.currentPoints[i].time, x + labelsWidth, yText);
                        yText = yText + 14
                        ctx.fillText(timeAfterDoseText, xText, yText);
                        ctx.fillText(cdata.currentPoints[i].timeAfterDose + "h", x + labelsWidth, yText);
                        yText = yText + 14
                        ctx.fillText(valueText, xText, yText);
                        ctx.fillText(cdata.currentPoints[i].value + " " + canvas.unit, x + labelsWidth, yText);
                        yText = yText + 14
                        var mean = canvas.currentPoints[i].mean
                        if (!(mean > 0)) mean = "-"
                        ctx.fillText(averageText, xText, yText);
                        ctx.fillText(mean + " " + cdata.unit, x + labelsWidth, yText);
                        yText = yText + 14
                        var trough = cdata.currentPoints[i].trough
                        if (!(trough > 0)) trough = "-"
                        ctx.fillText(troughText, xText, yText);
                        ctx.fillText(trough + " " + cdata.unit, x + labelsWidth, yText);
                        yText = yText + 14
                        var peak = cdata.currentPoints[i].peak
                        if (!(peak > 0)) peak = "-"
                        ctx.fillText(peakText, xText, yText);
                        ctx.fillText(peak + " " + cdata.unit, x + labelsWidth, yText);
                        yText = yText + 14
                        var auc = cdata.currentPoints[i].auc
                        if (!(auc > 0)) auc = "-"
                        ctx.fillText(aucText + " (" + cdata.currentPoints[i].cycleDuration + "h):" , xText, yText);
                        ctx.fillText(auc + " " + cdata.unit + "*h", x + labelsWidth, yText);
                        yText = yText + 14
                        var auc24 = cdata.currentPoints[i].auc24
                        if (!(auc24 > 0)) auc24 = "-"
                        ctx.fillText(aucText + " (24h):" , xText, yText);
                        ctx.fillText(auc24 + " " + cdata.unit + "*h", x + labelsWidth, yText);
                        yText = yText + 14
                        var cumulatedAuc = cdata.currentPoints[i].cumulatedAuc
                        if (!(cumulatedAuc > 0)) cumulatedAuc = "-"
                        ctx.fillText(cumulatedAucText, xText, yText);
                        ctx.fillText(cumulatedAuc + " " + cdata.unit + "*h", x + labelsWidth, yText);

                        if (cdata.currentPoints[i].measureTime.length > 0) {
                            var measureIndex;
                            for (measureIndex = 0; measureIndex < cdata.currentPoints[i].measureTime.length; measureIndex++) {
                                yText = yText + 14;
                                var textMeasure = "Value at ";
                                var textMeasure2 = formatDate(new Date(cdata.currentPoints[i].measureTime[measureIndex]));
                                ctx.fillText(textMeasure, xText, yText);
                                ctx.fillText(textMeasure2, x + labelsWidth, yText);

                                yText = yText + 14;
                                textMeasure = "is : ";
                                textMeasure2 = prepareValueForDisplay(cdata.currentPoints[i].measurePredicted[measureIndex]) + " " + canvas.unit;
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
    if (canvas.currentMeasure) {
        valuesWidth = Math.max(ctx.measureText(canvas.currentMeasure.value).width, ctx.measureText(canvas.currentMeasure.time).width) + 2;

        tooltipWidth = (labelsWidth + valuesWidth);
        tooltipHeight = 30;

        var x = canvas.currentMeasure.x * canvas.scalex - tooltipWidth  / 2;
        var y = canvas.currentMeasure.y * canvas.scaley - tooltipHeight - 10;

        ctx.beginPath();
        ctx.rect(x, y, tooltipWidth, tooltipHeight);

        ctx.globalAlpha = 0.5;
        ctx.fillStyle = canvas.currentMeasure.color;
        ctx.fill();
        ctx.globalAlpha = 1.0;
        ctx.stroke();

        ctx.fillStyle = "black";
        ctx.fillText(valueText, x + 2, y + 12);
        ctx.fillText(canvas.currentMeasure.value, x + labelsWidth, y + 12);
        ctx.fillText(timeText, x + 2, y + 26);
        ctx.fillText(canvas.currentMeasure.time, x + labelsWidth, y + 26);
    }
    canvas.currentMeasure = null;
*/
    ctx.restore();
    ctx.save();
}
