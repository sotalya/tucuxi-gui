import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0
import "graphing.js" as Graphing
import "trails.js" as Trails
import "clock.js" as Clock

Canvas {
    id: canvas
    objectName: "chartView"

    property var img_covariates_disabled_mini : "qrc:/icons/flow/covariates_disabled_mini.png";
    property var img_dosages_disabled_mini : "qrc:/icons/flow/dosages_disabled_mini.png";

    property var canvas : this;
    property var annotationsCanvas : overlayAnnotations;
    property var clockCanvas : this;

    property var module: Canvas.FramebufferObject;

    property var iController : interpretationController;
    property var gInformationSelection : graphInformationSelection;
    property var adjustmentController : adjustmentTabController;

    property alias mouseArea: mouseArea
    signal currentTimeChanged(date adate);
    property date date: new Date();

    property var mArea : mouseArea;

    property var dosages: interpretation.drugResponseAnalysis.treatment.dosages.objlist;
    property var measures: interpretation.drugResponseAnalysis.treatment.measures.objlist;
    property var targets: interpretation.drugResponseAnalysis.treatment.targets.objlist;
    property var pvars: interpretation.drugResponseAnalysis.treatment.covariates.objlist;

    property var adjustmentDate : adjustmentController.adjustmentDate;

    //Canvas properties
    renderStrategy: Canvas.Threaded
    renderTarget: Canvas.FramebufferObject

    antialiasing: true

    property int targetTabIndex: -1;

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

    property real scale : 1 // Default scale, only used for the web front end
    //Chart properties
    property real scaleMax: 2e1
    property real scaleMin: 2e-3

    property bool isOffsetXEnabled: true
    property bool isOffsetYEnabled: true

    property string police: "sans-serif"
    property string fontSize: "12px"
    property string axisTicksFontSize: "10px"
    property string tooltipFontSize: "10px"

    property bool hasMeasures: false
    property bool hasPatientVariates: false;
    property bool hasTargets: false

    property real highlightthresh: 15

    property var currentPoints: []
    property var closestPred: ({})
    property var currentMeasure: null
    property var currentDosage : null

    property var colors: [];
    property var popcolors: [];
    property var aprcolors: [];
    property var apocolors: [];
    property var adjcolors: [];

    property var revcolors: [];
    property string nographtext: "Welcome to Tucuxi! Graphs of predicted concentrations will be shown here.";
//    property var nographdrugtext: interpretationController.getGraphDrugData();

    property real timestart: Date.now();

    //indices
    readonly property int pop: 0
    readonly property int apr: 1
    readonly property int apo: 2
    readonly property int rev: 3
    readonly property int mea: 4
    readonly property int tar: 5
    readonly property int adj: 6

    readonly property var indices: ["pop", "apr", "apo", "rev", "mea", "tar", "adj"]
    property var popP: chartData.popPred
    property var aprP: chartData.aprPred
    property var apoP: chartData.apoPred
    property var adjP: chartData.adjPred

    property var popercsP: chartData.popPred.predictive.percentilePairs
    property var aprpercsP: chartData.aprPred.predictive.percentilePairs
    property var apopercsP: chartData.apoPred.predictive.percentilePairs
    property var adjpercsP: chartData.adjPred.predictive.percentilePairs
    property var revP: chartData.revPred.adjustments


    property real maxX: 0
    property real minX: 0
    property real maxY: 0
    property real minY: 0
    property real xRatio: 0
    property real yRatio: 0

    property real yFactor: 1

    //ToDo: remove default unit
    property string unit: "ug/l"
    property real unitefforder: 1

    function rePaint() {
        requestPaint();
        overlayAnnotations.requestPaint();
    }

    function saveGraph(fileName) {
        canvas.save(fileName);
    }


    function reset() {
        //return;
        date = new Date();


        //        panalysis = interpretation.drugResponseAnalysis.treatment;
        //        dosages = interpretation.drugResponseAnalysis.treatment.dosages.objlist;
        //        measures = interpretation.drugResponseAnalysis.treatment.measures.objlist;
        //        targets = interpretation.drugResponseAnalysis.treatment.targets.objlist;
        //        pvars = interpretation.drugResponseAnalysis.treatment.covariates.objlist;
        //
        antialiasing = true;

        //Canvas margins
        leftMargin =   75;
        rightMargin =  75;
        topMargin =    75;
        bottomMargin = 75;

        //Chart coordinates
        updateChartDimensions();

        //Chart properties
        scaleMax = 2e1;
        scaleMin = 2e-3;

        isOffsetXEnabled = true;
        isOffsetYEnabled = true;

        police = "sans-serif";
        hasMeasures = false;
        hasPatientVariates = false;
        hasTargets = false;

        highlightthresh = 15;

        currentPoints = [];
        closestPred = ({});
        currentMeasure = null;
        currentDosage = null;
/*
        colors = ["#bfbfbf",    // Unused
                  "#21B035",    // popPrediction
                  "#cc0066",    // aprioriPrediction
                  "#cc0066",    // Unused
                  "#8fd6f7",    // aposterioriPrediction
                  "#e8a45c",    // suggestedAdjustmentLegend
                  "#64a0e0",    // aposterioriPercentilesLegend
                  "lightgreen", // popPercentilesLegend
                  "pink",       // aprioriPercentilesLegend
                  "black"       // selectedAdjustmentLegend
                ];


        popcolors = ["#0B360E", "#17701E", "#2CD438"];
        aprcolors = ["#E84229", "#B53A70", "#FF4200"];
        apocolors = ["#4A24B5", "#484db5", "#5168B5"];
*/
/*
        colors = ["#bfbfbf",    // Unused
                  "#9f8e6f",    // popPrediction
                  "#e19a00",    // aprioriPrediction
                  "#27ba58",    // Unused
                  "#C80000",    // aposterioriPrediction
                  "#e8a45c",    // suggestedAdjustmentLegend
                  "#fe6d6d",    // aposterioriPercentilesLegend
                  "#b2a283",    // popPercentilesLegend
                  "#efac1c",    // aprioriPercentilesLegend
                  "black"       // selectedAdjustmentLegend
                ];


        // For the next arrays:
        // [0] : Percentile line
        // [1] : Inner percentile fill
        // [2] : Between percentile fill
        // [3] : Outer percentile fill
        popcolors = ["#b2a283", "#D0C0A2", "#E1D6C0", "#F2ECDE"];
        aprcolors = ["#efac1c", "#F2C054", "#F8D98A", "#FEF2C0"];
        apocolors = ["#fe6d6d", "#FE9A9A", "#FEB7B7", "#FED4D4"];
        adjcolors = ["#323232", "#969696", "#646464"];
        revcolors = [];

*/


        colors = ["#bfbfbf",    // Unused
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

        popcolors = ["#aaaeb2", "#D2D6DA", "#E0E2E6", "#EAEEF0"];
        aprcolors = ["#9abfe7", "#9AE8E2", "#B6F4F0", "#DEFEFC"];
        apocolors = ["#8cda75", "#8CECA6", "#B4FABA", "#D6FCDA"];
        adjcolors = ["#323232", "#969696", "#646464"];
        revcolors = [];

        timestart = Date.now();

        //        popP = chartData.popPred;
        //        aprP = chartData.aprPred;
        //        apoP = chartData.apoPred;

        //        popercsP = chartData.popPred.predictive.percentilePairs;
        //        aprpercsP = chartData.aprPred.predictive.percentilePairs;
        //        apopercsP = chartData.apoPred.predictive.percentilePairs;
        //        revP = chartData.revPred.adjustments;

        maxX = 0;
        minX = 0;
        maxY = 0;
        minY = 0;
        xRatio = 0;
        yRatio = 0;
        yFactor = 1.0;

        //ToDo: remove default unit
        unit = "ug/l";
        unitefforder = 1;
    }

    function updateChartDimensions()
    {
        topLeftX = leftMargin;
        topLeftY = topMargin;

        topRightX = canvas.width - rightMargin;
        topRightY = topMargin;

        bottomLeftX = leftMargin;
        bottomLeftY = canvas.height - bottomMargin;

        bottomRightX = canvas.width - rightMargin;
        bottomRightY = canvas.height - bottomMargin;

        plotWidth =  topRightX   - topLeftX;
        plotHeight = bottomLeftY - topLeftY;
    }

    function getViewRangeMin() {
        return interpretationController.getViewRangeMin();
    }

    function getViewRangeMax() {
        return interpretationController.getViewRangeMax();
    }

    function zoomY(nSteps)
    {
        yFactor = yFactor * (1.0 - nSteps * 0.1);
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
        grabToImage(function(result) {interpretationController.getImage(result.image)}, Qt.size(cdata.canvas.width,cdata.canvas.height));
    }

    function manualextents(cdata)
    {
        cdata.iController.resetViewRange();
        cdata.minX = cdata.iController.getViewRangeMin();
        cdata.maxX = cdata.iController.getViewRangeMax();
    }

    function updateUnits()
    {
        /*
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
        */
        interpretationController.defaultUnit = unit;
    }

    Connections {
        target: graphInformationSelection
        onHasBeenModified: {
            canvas.rePaint();
        }
    }

    Connections {
        target: flow
        onChangedTab: {
            switch (flow.currentIndex)
            {
                case 0: canvas.state = "patients"; break;
                case 1: canvas.state = "drugs"; break;
                case 2: canvas.state = "dosages"; break;
                case 3: canvas.state = "covariates"; break;
                case 4: canvas.state = "measures"; break;
                case 5: canvas.state = "targets"; break;
                case 6: canvas.state = "adjustments"; break;
                case 7: canvas.state = "validation"; break;
                case 8: canvas.state = "report"; break;
            }
            canvas.rePaint();
        }
    }

    Connections {
        target: reportTab
        onToggleShow: {
            canvas.rePaint();
        }
        onPublishReport: {
            canvas.save(appPath + "/graph.png");
        }
    }

    Connections {
        target: adjustmentTab
        onCurrentIndexChanged: {
            if (index === -1) {return;}
            if (!revP.isValid || revP.isEmpty()) {return;}
            for (var i = 0; i < canvas.revP.size(); ++i) {
                canvas.revP.objat(i).predictionData.selected = false;
            }
            canvas.revP.objat(index).predictionData.selected = true;
            canvas.rePaint();
        }
    }
    signal curveSelectionEvent(bool selected, int index);


    Connections {
        target: dosageTab
        onCurrentIndexChanged: {
            if (index === -1) {return;}
            for (var j = 0; j < dosages.length; j++) {
                //    console.log(j);
                //    console.log(dosages[j].applied)
                //    console.log(dosages[j].endtime)
            }
            canvas.currentDosage = dosages[index];
            rePaint();
        }
    }

    Connections {
        target: targetTab
        onCurrentIndexChanged: {
            canvas.targetTabIndex = index;
            rePaint();
        }
    }

    onDateChanged: {
        currentTimeChanged(date);
    }

    onPopPChanged: {
        if (popP) {
            predUpdate(popP, "Population");
        }
    }

    onAprPChanged: {
        if (aprP) {
            predUpdate(aprP, "apriori");
        }
    }

    onApoPChanged: {
        if (apoP) {
            predUpdate(apoP, "aposteriori");
        }
    }

    onAdjPChanged: {
        if (adjP) {
            predUpdate(adjP, "adjustment");
        }
    }

    onPopercsPChanged: {
        if (popP) {
            if (popP.predictive.percentilePairs) {
                updatePercs(popP.predictive.percentilePairs);
            }
        }
    }

    onAprpercsPChanged: {
        if (aprP) {
            if (aprP.predictive.percentilePairs) {
                updatePercs(aprP.predictive.percentilePairs);
            }
        }
    }

    onApopercsPChanged: {
        if (apoP) {
            if (apoP.predictive.percentilePairs) {
                updatePercs(apoP.predictive.percentilePairs);
            }
        }
    }

    onAdjpercsPChanged: {
        if (adjP) {
            if (adjP.predictive.percentilePairs) {
                updatePercs(adjP.predictive.percentilePairs);
            }
        }
    }

    onRevPChanged: {
        if (revP) {
            updatePercs(revP);
        }
    }


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

    function predUpdate(prediction, predType) {
        preparePrediction(prediction, predType);
        rePaint();
    }


    function updatePercs(pairs) {
        preparePercs(pairs);
        rePaint();
    }


    onUnitChanged: {
        updateUnits();
        rePaint();
    }

    Component.onCompleted: {
        loadImage("qrc:/icons/flow/dosages_disabled_mini.png");
        loadImage("qrc:/icons/flow/covariates_disabled_mini.png");
        loadImage("qrc:/icons/flow/targets_disabled_mini.png");
        //        var yu = isImageLoading("qrc:/icons/flow/dosages_disabled_mini.png");
        //        var yu2 = isImageLoading("qrc:/icons/flow/covariates_disabled_mini.png");
        //        console.log("IMAGE LOADING: " + yu);
        updateUnits();
        rePaint();
    }
    onImageLoaded: {
        //        var yes = isImageLoaded("qrc:/icons/flow/dosages_disabled_mini.png");
        //        console.log("IMAGE LOADED: " + yes);
    }

    onPaint: {
        updateChartDimensions();
        Graphing.drawGraph(this);
        if (graphInformationSelection.displayCurrentTime) {
            Clock.showClock(this);
        }
        //Graphing.drawAnnotations(this);
    }

    Timer {
        id: clocktima
        interval: 1000; running: true; repeat: true
        onTriggered: date = new Date();
    }

    Canvas {
        id: overlayAnnotations
        z: 2
        objectName: "chartOverlayAnnotationsView"
        visible: true
        anchors.fill: parent
        property var overlay: this
        property var mouseArea: canvas.mouseArea

        property int screen_width: width;
        property int screen_height: height;
        property real radius: 50;
        property real radius_scale: 1;
        property real radius_scale_min: 1;
        property real radius_scale_max: 1.5;
        property int quantity: 25;
        property var particles: [];
        property real fadealpha: 0.05;
        property bool fadeout: false

        onPaint: {
            getContext("2d").clearRect(0, 0, canvas.width, canvas.height);
            Graphing.drawAnnotations(canvas);
        }
    }

    //ToDo: horizontal offset and zoom only
    MouseArea {
        id: mouseArea
        objectName: "chartArea"
        anchors.fill: parent

        hoverEnabled: true

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        property bool isMouseOver: false
        property bool hasShifted: false
        property double pressX
        property int tooltipX: -1
        property int tooltipY: -1

        propagateComposedEvents: true

        onDoubleClicked: {
            manualextents(canvas);
            tooltipX = -1;  // Clear tooltip
            canvas.rePaint();
        }

        onClicked: {

        }

        onReleased: {
            if (mouse.button  == Qt.LeftButton) {
                closestPred.selected = !closestPred.selected && closestPred.highlight;
                overlayAnnotations.requestPaint();
            }
        }

        GraphSettingsDialog {
            id: graphSettingsDialog
            objectName: "graphSettingsDialog"
        }

        onPressed: {
            if (mouse.button  == Qt.RightButton) {
                // Open a dialog to select the objects to show on the graph
                graphSettingsDialog.init()
                graphSettingsDialog.open(true)

            }
            else {
                pressX = mouse.x;
                tooltipX = mouse.x;
                tooltipY = mouse.y;
            }
        }

        onExited: {
//            canvas.requestPaint();
        }

        onPositionChanged: {
            if (pressed && containsMouse) {
                var dx = mouse.x - pressX
                canvas.isOffsetXEnabled ? shift(dx) : 0;
                pressX = mouse.x;
                tooltipX = -1; // Clear tooltip
            }
            isMouseOver = mouse.x >= canvas.topLeftX && mouse.x <= canvas.topRightX &&
                          mouse.y >= canvas.topLeftY && mouse.y <= canvas.bottomLeftY;
            if (graphInformationSelection.displayLiveAnnotations) {
                tooltipX = mouse.x;
                tooltipY = mouse.y;
                overlayAnnotations.requestPaint();
            }
        }

        onWheel: {
            if (wheel.modifiers & Qt.ShiftModifier) {
                zoomY(wheel.angleDelta.y/120);
                rePaint();
            }
            else {
                // For most mice, 120 equals one step of the wheel.
                zoom(wheel.angleDelta.y/120);
                tooltipX = -1; // Clear tooltip
            }
        }
    }

    function resume() {
        Trails.setTargetSize(overlay, 0);
        overlay.fadeout = true;
    }

    function wait() {
        overlay.fadeout = false;
        overlay.visible = true;
        Trails.setTargetSize(overlay, 1);
        waittima.start();
    }
    //    function showClock() {
    //        clocktima.start();
    //    }

    Canvas {
        id: overlay
        z: -1
        objectName: "chartOverlayView"
        visible: false
        anchors.fill: parent
        property var overlay: this
        property var mouseArea: canvas.mouseArea

        property int screen_width: width;
        property int screen_height: height;
        property real radius: 50;
        property real radius_scale: 1;
        property real radius_scale_min: 1;
        property real radius_scale_max: 1.5;
        property int quantity: 25;
        property var particles: [];
        property real fadealpha: 0.05;
        property bool fadeout: false

        Timer {
            id: waittima
            interval: 100/60; running: false; repeat: true
            onTriggered: overlay.requestPaint();
        }


        Component.onCompleted: {
            Trails.init(overlay);
            Trails.setTargetSize(overlay, 0);
        }
        onPaint: {
            Trails.loop(overlay);
        }
    }

}
