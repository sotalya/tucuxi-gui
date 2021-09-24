


function createPredictionData(cdata, offset) {
    data = new GraphPredictionData([0, 1, 2,3,4,5,6,7,8,9],[0,1,2,3,4,5,6,7,8,9]);
    for(i = 0; i < 10; i++) {
        data.time[i] *=3600;
        data.time[i] += cdata.date.getTime()/1000 - 24 * 3600;
        data.value[i] += offset;
    }
    data.isValid = true;
    return data;
}

function populatePercentiles(cdata, perc, rank, offset) {
    data = new GraphPredictionData([0, 1, 2,3,4,5,6,7,8,9],[0,1,2,3,4,5,6,7,8,9]);
    for(i = 0; i < 10; i++) {
        data.time[i] *=3600;
        data.time[i] += cdata.date.getTime()/1000 - 24 * 3600;
        data.value[i] += offset;
    }
    data.isValid = true;
    p = new GraphPercentileData();
    p.percentile = rank;
    p.predictionData = data;
    p.isValid = true;
    perc.append(p);
}

function test1() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();
    
    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    
    obj.currentDosage = obj.dosages[0];
    
    // Let's add covariates
    obj.pvars.push(new GraphCovariate(new Date(Date.now() - 12 * 3600 * 1000)));
    obj.pvars.push(new GraphCovariate(new Date(Date.now() - 8 * 3600 * 1000)));
    
    // Let's add measures
    obj.measures.push(new GraphMeasure(new Date(Date.now() - 8 * 3600 * 1000), 15, 1, "ug/l"));
    obj.measures.push(new GraphMeasure(new Date(Date.now() - 4 * 3600 * 1000), 12, 1, "ug/l"));
    obj.measures.push(new GraphMeasure(new Date(Date.now() - 12 * 3600 * 1000), 18, 1, "ug/l"));
    
    // Let's add targets
    obj.targets.push(new GraphTarget(0, 2, 4, 6));
    obj.targets.push(new GraphTarget(0, 12, 14, 18));
    
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    
    populatePercentiles(obj, obj.popercsP, 5, 1);
    populatePercentiles(obj, obj.popercsP, 10, 2);
    populatePercentiles(obj, obj.popercsP, 25, 3);
    populatePercentiles(obj, obj.popercsP, 50, 4);
    populatePercentiles(obj, obj.popercsP, 75, 5);
    populatePercentiles(obj, obj.popercsP, 90, 6);
    populatePercentiles(obj, obj.popercsP, 95, 17);
    obj.popercsP.isValid = true;
    
    preparePercs(obj.popercsP);

    obj.revP = new GraphAdjustments();
    adj = new GraphAdjustment();
    adj.predictionData = createPredictionData(obj, 15);
    obj.revP.append(adj);
    adj = new GraphAdjustment();
    adj.predictionData = createPredictionData(obj, 17);
    obj.revP.append(adj);
    adj = new GraphAdjustment();
    adj.predictionData = createPredictionData(obj, 19);
    adj.predictionData.selected = true;
    obj.revP.append(adj);
    obj.revP.isValid = true;


    obj.mArea = new GraphMouseArea();
    obj.mArea.mouseX = 300 * obj.scale;
    obj.mArea.mouseY = 200 * obj.scale;
    obj.mArea.containsMouse = true;
    obj.mArea.isMouseOver = true;
    obj.mArea.tooltipX = 300 * obj.scale;
    obj.mArea.tooltipY = 200 * obj.scale;

    expected = "We shall see a lot of things here";
    return {obj, expected};
}


function test2() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();

    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    
    obj.currentDosage = obj.dosages[0];
    
    // Let's add covariates
    obj.pvars.push(new GraphCovariate(new Date(Date.now() - 12 * 3600 * 1000)));
    obj.pvars.push(new GraphCovariate(new Date(Date.now() - 8 * 3600 * 1000)));
    
    // Let's add measures
    obj.measures.push(new GraphMeasure(new Date(Date.now() - 8 * 3600 * 1000), 15, 1, "ug/l"));
    //obj.measures.push(new GraphMeasure(new Date(Date.now() - 4 * 3600 * 1000), 12, 1, "ug/l"));
    //obj.measures.push(new GraphMeasure(new Date(Date.now() - 12 * 3600 * 1000), 18, 1, "ug/l"));
    
    // Let's add targets
    obj.targets.push(new GraphTarget(0, 2, 4, 6));
    obj.targets.push(new GraphTarget(0, 12, 14, 18));
    
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    
    populatePercentiles(obj, obj.popercsP, 5, 1);
    populatePercentiles(obj, obj.popercsP, 10, 2);
    populatePercentiles(obj, obj.popercsP, 25, 3);
    populatePercentiles(obj, obj.popercsP, 50, 4);
    populatePercentiles(obj, obj.popercsP, 75, 5);
    populatePercentiles(obj, obj.popercsP, 90, 6);
    populatePercentiles(obj, obj.popercsP, 95, 17);
    obj.popercsP.isValid = true;
    
    preparePercs(obj.popercsP);

    obj.revP = new GraphAdjustments();
    adj = new GraphAdjustment();
    adj.predictionData = createPredictionData(obj, 15);
    obj.revP.append(adj);
    adj = new GraphAdjustment();
    adj.predictionData = createPredictionData(obj, 17);
    obj.revP.append(adj);
    adj = new GraphAdjustment();
    adj.predictionData = createPredictionData(obj, 19);
    adj.predictionData.selected = true;
    obj.revP.append(adj);
    obj.revP.isValid = true;


    obj.mArea = new GraphMouseArea();
    obj.mArea.mouseX = 300 * obj.scale;
    obj.mArea.mouseY = 200 * obj.scale;
    obj.mArea.containsMouse = true;
    obj.mArea.isMouseOver = true;
    obj.mArea.tooltipX = 300 * obj.scale;
    obj.mArea.tooltipY = 200 * obj.scale;

    expected = "We shall see a lot of things here";
    return {obj, expected};    
}


function testtarget0() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();
    
    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    
    // Let's add targets
    obj.targets.push(new GraphTarget(0, 2, 4, 6));
    obj.targets.push(new GraphTarget(0, 12, 14, 18));
    
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);

    expected = "Test of Residual target. We shall see the prediction and the residual targets";
    return {obj, expected};    
}


function testtarget1() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();
    
    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    
    // Let's add targets
    obj.targets.push(new GraphTarget(1, 2, 4, 6));
    obj.targets.push(new GraphTarget(1, 12, 14, 18));
    
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);

    expected = "Test of Peak target. We shall see the prediction and the peak targets";
    return {obj, expected};    
}

function testtarget2() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();
    
    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    
    // Let's add targets
    obj.targets.push(new GraphTarget(2, 2, 4, 6));
    obj.targets.push(new GraphTarget(2, 12, 14, 18));
    
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);

    expected = "Test of Mean target. We shall see the prediction and the mean targets";
    return {obj, expected};    
}


function testtarget3() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();
    
    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    
    // Let's add targets
    obj.targets.push(new GraphTarget(3, 2, 4, 6));
    obj.targets.push(new GraphTarget(3, 12, 14, 18));
    
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);

    expected = "Test of AUC target. We shall only see the prediction";
    return {obj, expected};    
}


function testtarget4() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();
    
    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    
    // Let's add targets
    obj.targets.push(new GraphTarget(4, 2, 4, 6));
    obj.targets.push(new GraphTarget(4, 12, 14, 18));
    
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);

    expected = "Test of AUC24 target. We shall only see the prediction";
    return {obj, expected};    
}


function testtarget5() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();

    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    // Let's add targets
    obj.targets.push(new GraphTarget(5, 2, 4, 6));
    obj.targets.push(new GraphTarget(5, 12, 14, 18));
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    expected = "Test of CumulativeAUC target. We shall only see the prediction";
    return {obj, expected};    
}

function testtarget6() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();

    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    // Let's add targets
    obj.targets.push(new GraphTarget(6, 2, 4, 6));
    obj.targets.push(new GraphTarget(6, 12, 14, 18));
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    expected = "Test of AUCDividedByMic target. We shall only see the prediction";
    return {obj, expected};    
}

function testtarget7() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();

    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    // Let's add targets
    obj.targets.push(new GraphTarget(7, 2, 4, 6));
    obj.targets.push(new GraphTarget(7, 12, 14, 18));
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    expected = "Test of AUC24DividedByMic target. We shall only see the prediction";
    return {obj, expected};    
}

function testtarget8() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();

    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    // Let's add targets
    obj.targets.push(new GraphTarget(8, 2, 4, 6));
    obj.targets.push(new GraphTarget(8, 12, 14, 18));
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    expected = "Test of AUCOverMic target. We shall only see the prediction";
    return {obj, expected};    
}

function testtarget9() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();

    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    // Let's add targets
    obj.targets.push(new GraphTarget(9, 2, 4, 6));
    obj.targets.push(new GraphTarget(9, 12, 14, 18));
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    expected = "Test of AUC24OverMic target. We shall only see the prediction";
    return {obj, expected};    
}

function testtarget10() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();

    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    // Let's add targets
    obj.targets.push(new GraphTarget(10, 2, 4, 6));
    obj.targets.push(new GraphTarget(10, 12, 14, 18));
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    expected = "Test of PeakDividedByMic target. We shall only see the prediction";
    return {obj, expected};    
}

function testtarget11() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();

    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    // Let's add targets
    obj.targets.push(new GraphTarget(11, 2, 4, 6));
    obj.targets.push(new GraphTarget(11, 12, 14, 18));
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    expected = "Test of TimeOverMic target. We shall only see the prediction";
    return {obj, expected};    
}

function testtarget12() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();

    // Let's add dosages
    obj.dosages.push(new GraphDosage());
    // Let's add targets
    obj.targets.push(new GraphTarget(12, 2, 4, 6));
    obj.targets.push(new GraphTarget(12, 12, 14, 18));
    obj.popP.predictive.predictionData = createPredictionData(obj, 1);
    preparePrediction(obj.popP);
    expected = "Test of ResidualDividedByMic target. We shall only see the prediction";
    return {obj, expected};    
}


function testempty() {
    // Creation of all the data necessary for a drawing
    var obj = new GraphFullData();


    
    expected = "Test of an empty GraphFullData. The graph shall be empty, with NaN on the Y-axis.";
    return {obj, expected};    
}
