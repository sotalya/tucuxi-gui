.import "graphing.js" as Graphing;

function step()
{
    var ctx = canvas.getContext("2d");
    if (graphInformationSelection.displayPopulationPrediction && canvas.state !== "validation") {
        annotateDosage(ctx, dosages.current, colors[1]);
    }

    if (graphInformationSelection.displayAprioriPrediction) {
        for (var i = 0; i < pvars.length; ++i) {
            annotateCovariate(ctx, pvars[i], colors[2]);
        }
    }

    canvas.currentPoints = []
    canvas.closestPred = ({})
    if (!mouseArea.containsMouse) {
        return;
    }

    //    ctx.clearRect(0, 0, canvas.width, canvas.height);
    annotatePrediction(ctx, canvas.popP, canvas.pop, canvas.colors[1]);
    annotatePrediction(ctx, aprP, apr, aprcolors[1]);
    annotatePrediction(ctx, apoP, apo, colors[4]);
    annotatePrediction(ctx, adjP, adj, colors[5]);

    if (revP !== null && revP.isValid) {
        if (graphInformationSelection.displayPossibleAdjustments) {
            var pairs = revP;
            for (var i = 0; i < pairs.size(); ++i) {
                findClosestValue(ctx, pairs.objat(i), pairs.objat(i).predictionData, rev, colors[5]);
            }
        }
    }
}

function annotatePrediction(ctx, pred, index, color)
{
    if (pred) {
        if (pred.predictive.predictionData.isValid && isCurveAvailable(index)) {
            findClosestValue(ctx, pred.predictive, pred.predictive.predictionData, index, color);
            if (pred.predictive.percentilePairs.isValid) {
                var pairs = pred.predictive.percentilePairs;
                for (var i = 0; i < pairs.size(); ++i) {
                    findClosestValue(ctx, pairs.objat(i), pairs.objat(i).predictionData, index, color);
                }
            }
            if (isCurveVisible(index)) {
                checkAndDisplayDomain(ctx, pred);
                annotateCurveLoci(ctx, pred.predictive.predictionData, index, color);
            }
        }
    }
}

function checkAndDisplayDomain(ctx, pred)
{
    if (!pred.isValidDomain){
        ctx.fillText(pred.domainMessage, topRightX, topRightY)
    }
}
function annotateDosage(ctx, dosage, color)
{
    if (!dosage) {return;}
    var start = dosage.applied.getTime()/1000;
    var end = dosage.endtime.getTime()/1000;
    var startX = Graphing.atime2screen(start);
    var startY = bottomLeftY;
    var endY = bottomLeftY;
    var endX = Graphing.atime2screen(end);
    //these are for checking if the start and end are within view, if not we draw it differenetly
    var startinview = true;
    var endinview = true;
    if (startX < topLeftX && endX > topLeftX) {
        startX = topLeftX;
        startinview = false;
    }
    if (endX > bottomRightX && startX < bottomRightX) {
        endX = bottomRightX;
        endinview = false;
    }

    ctx.beginPath();
    ctx.strokeStyle = color;
    ctx.fillStyle = color;

    ctx.arc(startX, startY, 5, 0, 2 * Math.PI);
    ctx.arc(endX, endY, 5, 0, 2 * Math.PI);
    ctx.fill();
    if (startinview) {
        ctx.drawImage("qrc:/icons/flow/dosages_disabled_mini.png", startX - 32, bottomLeftY - 32, 32, 32);
    } else {
        //draw a leftarrow
    }

    if (endinview) {
        ctx.drawImage("qrc:/icons/flow/dosages_disabled_mini.png", endX - 32, bottomLeftY - 32, 32, 32);
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

function annotateCovariate(ctx, pvar, color)
{
    var time = pvar.date.getTime()/1000;
    var timeX = Graphing.atime2screen(time);
    var startY = bottomLeftY;

    var tl = topLeftX;
    if (timeX < topLeftX || timeX > bottomRightX) {
        return;
    }

    ctx.beginPath();
    ctx.strokeStyle = color;
    ctx.fillStyle = color;

    ctx.arc(timeX, startY, 5, 0, 2 * Math.PI);
    ctx.fill();
    ctx.drawImage("qrc:/icons/flow/covariates_disabled_mini.png", timeX - 32, bottomLeftY - 32, 32, 32);
    ctx.stroke();
    //    console.log("HELLO");
    //    console.log(start);
    //    console.log(end);
    //    console.log(startX);
    //    console.log(endX);
}

function annotateCurveLoci(ctx, predData, index, color)
{
    var peaks = predData.peaks;
    var troughs = predData.troughs;
    var filter = Graphing.getAdjustmentFilter(true);

    ctx.strokeStyle = color;
    ctx.fillStyle = color;

    for (var i = 0; i < peaks.length; ++i) {
        var t = Graphing.atime2screen(predData.timeAt(peaks[i]));
        var v = Graphing.acxn2screen(predData.valueAt(peaks[i]));
        if (!filter || filter(predData.timeAt(peaks[i]))) {
            if (t > topLeftX && t < bottomRightX) {
                ctx.beginPath();
                ctx.arc(t, v, 2, 0, 2 * Math.PI);
                ctx.fill();
                ctx.stroke();
            }
        }
    }
    for (var i = 0; i < troughs.length; ++i) {
        var t = Graphing.atime2screen(predData.timeAt(troughs[i]));
        var v = Graphing.acxn2screen(predData.valueAt(troughs[i]));
        if (!filter || filter(predData.timeAt(troughs[i]))) {
            if (t > topLeftX && t < bottomRightX) {
                ctx.beginPath();
                ctx.arc(t, v, 2, 0, 2 * Math.PI);
                ctx.fill();
                ctx.stroke();
            }
        }
    }
}

function prepareValueForDisplay(val)
{
  var tmp = val*unitefforder
  return ((tmp > 100) ? Math.round(tmp) : Math.round(tmp*100)/100)
}

function findClosestValue(ctx, predictive, predData, index, color)
{
    if (mouseArea.tooltipX < 0 && mouseArea.tooltipY < 0) {
        return; // Tooltip not wanted...
    }

    var x = predData.times();
    var y = predData.values();
    if (!predData.closestPoint)
        return;
    var current = predData.closestPoint.currentindex;
    var mousexms = Math.max(0,Graphing.ascreen2time(mouseArea.tooltipX));
    var mouseyug = Graphing.ascreen2acxn(mouseArea.tooltipY);

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

    var rat = canvas.xRatio;
    var minxx = canvas.minX;
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
        canvas.currentPoints[index] = currentPoint;
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
    drawTooltips(ctx);
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

function isCurveAvailable(index)
{
    var adjTabShowPop = graphInformationSelection.presentAposterioriPrediction && !hasPatientVariates && !hasMeasures;
    var adjTabShowApr = graphInformationSelection.presentAposterioriPrediction && hasPatientVariates && !hasMeasures;

    var isAvailable = false;
    switch(index) {
        case pop: isAvailable = graphInformationSelection.presentPopulationPrediction || adjTabShowPop; break;
        case apr: isAvailable = graphInformationSelection.presentAprioriPrediction || adjTabShowApr; break;
        case apo: isAvailable = graphInformationSelection.presentAposterioriPrediction; break;
        case rev: isAvailable = graphInformationSelection.presentPossibleAdjustments; break;
        case mea: isAvailable = graphInformationSelection.presentMeasures; break;
        case tar: isAvailable = graphInformationSelection.presentTargets; break;
        case adj: isAvailable = graphInformationSelection.presentSelectedAdjustment; break;
        default: break;
    }
    //console.log("Curve " + index + " is " + (isAvailable ? "available" : "not available"))
    return isAvailable;
}

function isCurveVisible(index)
{
    var isVisible = false;
    if (isCurveAvailable(index)) {
        switch(index) {
            case pop: isVisible = graphInformationSelection.displayPopulationPrediction; break;
            case apr: isVisible = graphInformationSelection.displayAprioriPrediction; break;
            case apo: isVisible = graphInformationSelection.displayAposterioriPrediction; break;
            case rev: isVisible = graphInformationSelection.displayPossibleAdjustments; break;
            case mea: isVisible = graphInformationSelection.displayMeasures; break;
            case tar: isVisible = graphInformationSelection.displayTargets; break;
            case adj: isVisible = graphInformationSelection.displaySelectedAdjustment; break;
            default: break;
        }
    }
    //console.log("Curve " + index + " is " + (isVisible ? "visible" : "not visible"))
    return isVisible;
}

function drawTooltips(ctx)
{
    //Settings
    ctx.font         = "10px " + canvas.police;
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

    for (var i = 0; i < canvas.currentPoints.length; ++i) {
        if (canvas.currentPoints[i]) {
            var filter = Graphing.getAdjustmentFilter(canvas.currentPoints[i].predIndex !== adj);
            var t = canvas.currentPoints[i].x;
            if (!filter || filter(t)) {
                if (isCurveVisible(i) && mouseArea.tooltipX > topLeftX) {
                    x = Graphing.atime2screen(canvas.currentPoints[i].x);
                    y = Graphing.acxn2screen(canvas.currentPoints[i].y);
                    ctx.beginPath();
                    ctx.arc(x, y, 4, 0, 2 * Math.PI, true);
                    ctx.fillStyle = canvas.currentPoints[i].color;
                    ctx.fill();
                    ctx.stroke();

                    if (!canvas.currentMeasure) {
                        valuesWidth = Math.max(ctx.measureText(canvas.currentPoints[i].value).width, ctx.measureText(canvas.currentPoints[i].time).width) + 2;

                        tooltipWidth = (labelsWidth + valuesWidth);
                        tooltipHeight = 9*14+8 + canvas.currentPoints[i].measureTime.length * 14 * 2;
                        x = x - tooltipWidth  / 2;
                        y = y - tooltipHeight - 10;

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
                        var xText = x + 2
                        var yText = y + 12
                        ctx.fillText(timeText, xText, yText);
                        ctx.fillText(canvas.currentPoints[i].time, x + labelsWidth, yText);
                        yText = yText + 14
                        ctx.fillText(timeAfterDoseText, xText, yText);
                        ctx.fillText(canvas.currentPoints[i].timeAfterDose + "h", x + labelsWidth, yText);
                        yText = yText + 14
                        ctx.fillText(valueText, xText, yText);
                        ctx.fillText(canvas.currentPoints[i].value + " " + canvas.unit, x + labelsWidth, yText);
                        yText = yText + 14
                        var mean = canvas.currentPoints[i].mean
                        if (!(mean > 0)) mean = "-"
                        ctx.fillText(averageText, xText, yText);
                        ctx.fillText(mean + " " + canvas.unit, x + labelsWidth, yText);
                        yText = yText + 14
                        var trough = canvas.currentPoints[i].trough
                        if (!(trough > 0)) trough = "-"
                        ctx.fillText(troughText, xText, yText);
                        ctx.fillText(trough + " " + canvas.unit, x + labelsWidth, yText);
                        yText = yText + 14
                        var peak = canvas.currentPoints[i].peak
                        if (!(peak > 0)) peak = "-"
                        ctx.fillText(peakText, xText, yText);
                        ctx.fillText(peak + " " + canvas.unit, x + labelsWidth, yText);
                        yText = yText + 14
                        var auc = canvas.currentPoints[i].auc
                        if (!(auc > 0)) auc = "-"
                        ctx.fillText(aucText + " (" + canvas.currentPoints[i].cycleDuration + "h):" , xText, yText);
                        ctx.fillText(auc + " " + canvas.unit + "*h", x + labelsWidth, yText);
                        yText = yText + 14
                        var auc24 = canvas.currentPoints[i].auc24
                        if (!(auc24 > 0)) auc24 = "-"
                        ctx.fillText(aucText + " (24h):" , xText, yText);
                        ctx.fillText(auc24 + " " + canvas.unit + "*h", x + labelsWidth, yText);
                        yText = yText + 14
                        var cumulatedAuc = canvas.currentPoints[i].cumulatedAuc
                        if (!(cumulatedAuc > 0)) cumulatedAuc = "-"
                        ctx.fillText(cumulatedAucText, xText, yText);
                        ctx.fillText(cumulatedAuc + " " + canvas.unit + "*h", x + labelsWidth, yText);

                        if (canvas.currentPoints[i].measureTime.length > 0) {
                            var measureIndex;
                            for (measureIndex = 0; measureIndex < canvas.currentPoints[i].measureTime.length; measureIndex++) {
                                yText = yText + 14;
                                var textMeasure = "Value at ";
                                var textMeasure2 = formatDate(new Date(canvas.currentPoints[i].measureTime[measureIndex]));
                                ctx.fillText(textMeasure, xText, yText);
                                ctx.fillText(textMeasure2, x + labelsWidth, yText);

                                yText = yText + 14;
                                textMeasure = "is : ";
                                textMeasure2 = prepareValueForDisplay(canvas.currentPoints[i].measurePredicted[measureIndex]) + " " + canvas.unit;
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
