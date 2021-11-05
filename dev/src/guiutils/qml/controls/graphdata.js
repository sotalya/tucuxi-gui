///////////////////////////////////////////////////////////////////////////////
// File: graphdata.js
// Author : Yann Thoma
// Date   : 20.08.2021
//
// Description : This file embeds classes used by the graph rendering engine.
//               Filling a GraphFullData object allows to give all information
//               required to draw a canvas.
//
///////////////////////////////////////////////////////////////////////////////

// A list of percentiles.
// Usually we should have 7 items in it:
// Percentiles 5, 10, 25, 50, 75, 90, 95
//
// Each item shall be a GraphPercentileData
//
class GraphPercentileDataList {

    constructor() {
        this.plist = [];
        this.isValid = false;
    }

    isEmpty() {
        return this.plist.length == 0;
    }

    size() {
        return this.plist.length;
    }

    objat(index) {
        return this.plist[index];
    }

    append(data) {
        this.plist.push(data);
    }
}

// A GraphFancyPoint represents a concentration value at a certain time
// It is not strictly required by graphing.js, but could be useful 
// maybe for a controller
class GraphFancyPoint {

    constructor(times, values) {
        this.times = times;
        this.values = values;
    }

    times() {
        return this.times;
    }

    values() {
        return this.values;
    }
}

// GraphPredictionData embeds all information related to a prediction.
//
// It shall supply the times as a vector, and the values also as a vector
//
// Various methods allow to get statistical values of a specific index
// in time. Here it is hardcoded, but it should depend on the values
// stored in the object.
//
class GraphPredictionData {
    constructor(time, value) {
        this.time = time;
        this.value = value;
        // The indexes at which there are trough concentrations
        this.troughs = [6];
        // The indexes at which there are peaks
        this.peaks = [1, 4];
        this.isValid = false;
        this.selected = false;
    }
    
    times() {
        return this.time;
    }

    values() {
        return this.value;
    }

    meanAt(index) {
     return 10;
    }

    timeAt(index) {
        return this.time[index];
    }

    valueAt(index) {
        return this.value[index];
    }

    aucAt(index) { return 20;}
    auc24At(index) { return 30;}
    cumulatedAucAt(index) { return 40;}
    troughAt(index) { return 50;}
    peakAt(index) { return 60;}
    timeAfterDose(index) { return 70;}
    cycleDuration(index) { return 80;}
}

// A GraphPercentileData embeds the percentile rank and
// a GraphPredictionData for its curve.
//
class GraphPercentileData {
    constructor() {
        this.percentile = 0;
        this.predictionData = null;
    }
}

// A GraphPredictive embeds a prediction, along with its percentiles.
// Typically, a typical patient curve is represented by a GraphPredictive,
// as well as an apriori and a posteriori curve.
//
class GraphPredictive {
    constructor() {
        this.predictionData = new GraphPredictionData();
        this.percentilePairs = new GraphPercentileDataList();
    }
}

// A GraphPredictionResult just embeds a GraphPredictive, for compatibility
// with Qt Objects
class GraphPredictionResult {
    constructor() {
        this.predictive = new GraphPredictive();
    }
}


// This class embeds information about what to show or not on the graph.
// graphing.js uses that to know whether some curves or annotations shall be
// displayed or not.
class GraphInformationSelection {
    constructor() {
        this.presentPopulationPrediction = true;
        this.displayPopulationPrediction = true;

        this.presentAprioriPrediction = true;
        this.displayAprioriPrediction = true;

        this.presentAposterioriPrediction = true;
        this.displayAposterioriPrediction = true;
        
        this.presentPopulationPercentiles = true;
        this.displayPopulationPercentiles = true;
        
        this.presentAprioriPercentiles = true;
        this.displayAprioriPercentiles = true;
        
        this.presentAposterioriPercentiles = true;
        this.displayAposterioriPercentiles = true;
        
        this.presentPossibleAdjustments = true;
        this.displayPossibleAdjustments = true;

        this.presentSelectedAdjustments = true;
        this.displaySelectedAdjustments = true;

        this.presentMeasures = true;
        this.displayMeasures = true;

        this.presentTargets = true;
        this.displayTargets = true;

        this.displayCovariateChange = true;
        this.displayCurrentTime = true;

        this.perc50 = true;
        this.perc25_75 = true;
        this.perc10_90 = true;
        this.perc5_95 = true;
    }
}

// For the graph, the only required information about a covariate is
// its modification date
class GraphCovariate {
    constructor(date = new Date(Date.now() - 12 * 3600 * 1000)) {
        this.date = date;
    }
}

// For a dosage, the graph only needs its start and end time
class GraphDosage {
    constructor(){
        this.applied = new Date(Date.now() - 24 * 3600 * 1000);
        this.endtime = new Date(Date.now() + 24 * 3600 * 1000);
    }
}

// This class represents a value in conjunction with a multiplier coefficient and a unit.
// For the web front end, the multiplier could stay at 1.
class GraphIdentifiableAmount {
    constructor(value, multiplier, unit) {
        this.dbvalue = value;
        this.multiplier = multiplier;
        this.unitstring = unit;
    }
}

// This class represents a measure, being a date and a concentration.
// The concentration embeds a value, a multiplier and a unit.
class GraphMeasure {
    constructor(moment, value, multiplier, unit) {
        this.moment = moment;
        this.concentration = new GraphIdentifiableAmount(value, multiplier, unit);
    }
}

// A GraphTargetMethod represents a type of target.
// The value shall be corresponding to the following enum type:
//
// ResidualTarget = 0,  //! Targeted residual concentration, or targetted concentration at the end of a complete cycle.
// PeakTarget,          //! Targeted peak concentration. WARNING: It does not correspond to the maximum of a cycle, but the value at a specific time after the intake.
// MeanTarget,          //! Targeted mean concentration, currently unused.
// AUCTarget,           //! Targeted AUC concentration, currently unused.
// AUC24Target,           //! Targeted AUC concentration, currently unused.
// CumulativeAUCTarget, //! Targeted cumulative AUC concentration.
// AUCDividedByMicTarget, //! Soon to be supported target
// AUC24DividedByMicTarget, //! Soon to be supported target
// AUCOverMicTarget, //! Soon to be supported target
// AUC24OverMicTarget, //! Soon to be supported target
// PeakDividedByMicTarget, //! Soon to be supported target
// TimeOverMicTarget, //! Soon to be supported target
// ResidualDividedByMicTarget, //! Targeted residual concentration divided by MIC
// UnknownTarget        //! Unknown or invalid target.
class GraphTargetMethod {
    constructor(value = 0) {
        this.value = value;
    }
}

// This class represents a value in conjunction with a multiplier coefficient.
// For the web front end, the multiplier could stay at 1.
class GraphAmount {
    constructor(dbValue, multiplier = 1) {
        this.dbvalue = dbValue;
        this.multiplier = multiplier;
    }
}

// A GraphTarget embeds information about a target.
// It consists of its type represented by a GraphTargetMethod, a best, min and max
// value.
class GraphTarget {
    constructor(type, cmin = 0, cbest = 2, cmax = 10) {
        this.type = new GraphTargetMethod(type);
        this.cbest = new GraphAmount(cbest, 1.0);
        this.cmin = new GraphAmount(cmin, 1.0);
        this.cmax = new GraphAmount(cmax, 1.0);
    }
}



// This class represents a suggested adjustment.
// It shall just embed a GraphPredictioData
class GraphAdjustment {
    constructor() {
        this.predictionData = new GraphPredictionData();
    }

}

// This class represents all the suggested adjustments.
// It is simply a list of GraphAdjustments
class GraphAdjustments {
    constructor() {
        this.alist = [];
        this.isValid = false;
    }
    append(adj) {
        this.alist.push(adj);
    }

    isEmpty() {
        return this.alist.length == 0;
    }

    size() {
        return this.alist.length;
    }

    objat(index) {
        return this.alist[index];
    }

}


class GraphMouseArea {
    constructor() {
        this.mouseX = -1;
        this.mouseY = -1;
        this.containsMouse = false;
        this.isMouseOver = false;
        this.tooltipX = -1;
        this.tooltipY = -1;
    }
}


class GraphFullData {

    constructor(scale = 1) {
        this.scale = scale;

        // The canvas shall be set before this object is used
        this.canvas = null;
        this.annotationsCanvas = null;
        this.clockCanvas = null;
        
        this.colors = ["#bfbfbf",    // Unused
        "#3c4042",    // popPrediction
        "#004375",    // aprioriPrediction
        "#9abfe7",    // Unused
        //"#C80000",    // aposterioriPrediction
        "#269043",    // aposterioriPrediction
        "#e8a45c",    // suggestedAdjustmentLegend
        "#8cda75",    // aposterioriPercentilesLegend
        "#aaaeb2",    // popPercentilesLegend
        "#9abfe7",    // aprioriPercentilesLegend
        "black"       // selectedAdjustmentLegend
        ];
        
        
        this.popcolors = ["#aaaeb2", "#D2D6DA", "#E0E2E6", "#EAEEF0"];
        this.aprcolors = ["#9abfe7", "#9AE8E2", "#B6F4F0", "#DEFEFC"];
        this.apocolors = ["#8cda75", "#8CECA6", "#B4FABA", "#D6FCDA"];
        this.adjcolors = ["#323232", "#969696", "#646464"];
        this.revcolors = [];

        // constant indices
        this.pop = 0;
        this.apr = 1;
        this.apo = 2;
        this.rev = 3;
        this.mea = 4;
        this.tar = 5;
        this.adj = 6;

        // Could be a const
        this.indices = ["pop", "apr", "apo", "rev", "mea", "tar", "adj"];
        
        this.nographtext = "Welcome to Tucuxi! Graphs of predicted concentrations will be shown here.";

        // The following variables are generated by the drawer,
        // So no need to set them prior to the rendering
        this.hasMeasure = false;
        this.hasPatientVariates = false;
        this.hasTargets = true;

        this.popercsP = new GraphPercentileDataList();
        this.aprpercsP = new GraphPercentileDataList();
        this.apopercsP = new GraphPercentileDataList();
        this.adjpercsP = new GraphPercentileDataList();


        this.popP = new GraphPredictionResult();
        this.aprP = new GraphPredictionResult();
        this.apoP = new GraphPredictionResult();
        this.adjP = new GraphPredictionResult();
        
        this.revP = new GraphAdjustments();

        this.measures = [];
        this.targets = [];
        this.pvars = [];
        this.dosages = [];

            //Chart properties
        this.scaleMax = 2e1
        this.scaleMin = 2e-3
        this.police = "sans-serif";

        if (scale == 1) {
            this.fontSize = "12px";
            this.axisTicksFontSize = "10px";
            this.tooltipFontSize = "10px";
        }
        else if (scale == 2) {
            this.fontSize = "24px";
            this.axisTicksFontSize = "20px";
            this.tooltipFontSize = "20px";    
        }
        else if (scale == 4) {
            this.fontSize = "48px";
            this.axisTicksFontSize = "40px";
            this.tooltipFontSize = "40px";    
        }
        this.yFactor = 1.0;
        this.date = new Date();

        this.maxX = 0;
        this.minX = 0;
        this.maxY = 0;
        this.minY = 0;
        this.xRatio = 0;
        this.yRatio = 0;

        this.highlightthresh = 15

        this.currentPoints = []
        this.closestPred = ({})
        this.currentMeasure = null
        this.currentDosage = null

        this.antialiasing = true;


        this.isOffsetXEnabled = true;
        this.isOffsetYEnabled = true;

        //ToDo = remove default unit
        this.unit = "ug/l";
        this.unitefforder = 1;

        this.gInformationSelection = new GraphInformationSelection();

        this.timestart = Date.now();

        //Canvas margins
        this.leftMargin =   75 * this.scale;
        this.rightMargin =  75 * this.scale;
        this.topMargin =    75 * this.scale;
        this.bottomMargin = 75 * this.scale;

        this.targetTabIndex = -1;

        this.mArea = new GraphMouseArea();

    }


	getViewRangeMin() {var newdate = new Date(); return newdate.getTime() / 1000 - 24 * 3600;}
	
    getViewRangeMax() {var newdate = new Date(); return newdate.getTime() / 1000;}

    // This function shall be called before printing the graph,
    // to precalculate values.
    updateChartDimensions(canvas)
    {

        if (this.scale <= 1.5) {
            this.fontSize = "12px";
            this.axisTicksFontSize = "10px";
            this.tooltipFontSize = "10px";
        }
        else if (this.scale <= 2.5) {
            this.fontSize = "24px";
            this.axisTicksFontSize = "20px";
            this.tooltipFontSize = "20px";    
        }
        else if (this.scale <= 4.5) {
            this.fontSize = "48px";
            this.axisTicksFontSize = "40px";
            this.tooltipFontSize = "40px";    
        }

        //Canvas margins
        this.leftMargin =   75 * this.scale;
        this.rightMargin =  75 * this.scale;
        this.topMargin =    75 * this.scale;
        this.bottomMargin = 75 * this.scale;

        this.topLeftX = this.leftMargin;
        this.topLeftY = this.topMargin;

        this.topRightX = canvas.width - this.rightMargin;
        this.topRightY = this.topMargin;

        this.bottomLeftX = this.leftMargin;
        this.bottomLeftY = canvas.height - this.bottomMargin;

        this.bottomRightX = canvas.width - this.rightMargin;
        this.bottomRightY = canvas.height - this.bottomMargin;

        this.plotWidth =  this.topRightX   - this.topLeftX;
        this.plotHeight = this.bottomLeftY - this.topLeftY;

    }

    
}

// This function should be called when a prediction is ready,
// to generate the .X and .Y data
// This function is also available in Chart.qml
function preparePrediction(prediction, predType) {
    if (!prediction.predictive.predictionData.isValid) {
        return;
    }
    prediction.X = prediction.predictive.predictionData.times();
    prediction.Y = prediction.predictive.predictionData.values();
    prediction.predictive.predictionData.displayTooltip = true;
    prediction.predictive.predictionData.closestPoint = {
        currentindex: 0
    }
}

// This function should be called when the percentiles are ready,
// to generate the .X and .Y arrays
// This function is also available in Chart.qml
function preparePercs(pairs) {
    if (!pairs.isValid) {return;}
    if (pairs.size() === 0) {return;}
    for (var i = 0; i < pairs.size(); ++i) {
        if (pairs.objat(i).isValid) {
            pairs.objat(i).X = pairs.objat(i).predictionData.times();
            pairs.objat(i).Y = pairs.objat(i).predictionData.values();
            pairs.objat(i).predictionData.closestPoint = {
                currentindex: 0
            }
        }
    }
}
