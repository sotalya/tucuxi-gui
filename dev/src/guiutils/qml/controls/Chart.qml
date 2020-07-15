import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0
import "graphing.js" as Graphing
import "annotations.js" as Annotations
import "trails.js" as Trails
import "clock.js" as Clock

Canvas {
    id: canvas
    objectName: "chartView"

    property alias mouseArea: mouseArea
    signal currentTimeChanged(date adate);
    property date date: new Date();

    property var dosages: interpretation.drugResponseAnalysis.treatment.dosages.objlist;
    property var measures: interpretation.drugResponseAnalysis.treatment.measures.objlist;
    property var targets: interpretation.drugResponseAnalysis.treatment.targets.objlist;
    property var pvars: interpretation.drugResponseAnalysis.treatment.covariates.objlist;

    //Canvas properties
    renderStrategy: Canvas.Threaded
    renderTarget: Canvas.FramebufferObject

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
    property real scaleMax: 2e1
    property real scaleMin: 2e-3

    property bool isOffsetXEnabled: true
    property bool isOffsetYEnabled: true

    property var screenConversions: ({})

    property string police: "sans-serif"
    property bool hasMeasures: false
    property bool hasPatientVariates: false;
    property bool hasTargets: false

    property real highlightthresh: 15

    property var currentPoints: []
    property var closestPred: ({})
    property var currentMeasure: null

    property var colors: [];
    property var popcolors: [];
    property var aprcolors: [];
    property var apocolors: [];
    property var adjcolors: [];

    property var revcolors: [];
    property string nographtext: "Welcome to Tucuxi! Graphs of predicted concentrations will be shown here.";
    property var nographdrugtext: interpretationController.getGraphDrugData();

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
    property var closest: [0,0,0]
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

    //ToDo: remove default unit
    property string unit: "ug/l"
    property real unitefforder: 1

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

        colors = ["#bfbfbf", "#21B035", "#cc0066", "#cc0066", "#8fd6f7", "#e8a45c", "#64a0e0", "lightgreen", "pink", "black"];
        popcolors = ["#0B360E", "#17701E", "#2CD438"];
        aprcolors = ["#E84229", "#B53A70", "#FF4200"];
        apocolors = ["#4A24B5", "#484db5", "#5168B5"];
        adjcolors = ["#323232", "#969696", "#646464"];
        revcolors = [];

        timestart = Date.now();

        closest = [0,0,0];
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

    Connections {
        target: graphInformationSelection
        onHasBeenModified: {
            canvas.requestPaint();
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
            canvas.requestPaint();
        }
    }

    Connections {
        target: reportTab
        onToggleShow: {
            canvas.requestPaint();
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
            canvas.requestPaint();
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
            dosages.current = dosages[index];
            requestPaint();
        }
    }

    Connections {
        target: targetTab
        onCurrentIndexChanged: {
            requestPaint();
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

    function predUpdate(prediction, predType) {
        if (!prediction.predictive.predictionData.isValid) {
            // YTA : I removed this log
            //console.log("!!! " + predType + " data are not valid");
            requestPaint(); // Needed to erase old curve
            return;
        }
/*
var times = prediction.predictive.predictionData.times();
if (times.length > 0) {
    var xs = Graphing.atime2screen(times[0]);
    var xe = Graphing.atime2screen(times[times.length-1]);
    console.log("Display data for " + predType + ": From " + new Date(times[0]*1000) + " (" + xs + ") to " + new Date(times[times.length-1]*1000) + "(" + xe + ")");
}
*/
        prediction.X = prediction.predictive.predictionData.times();
        prediction.Y = prediction.predictive.predictionData.values();
        prediction.predictive.predictionData.displayTooltip = true;
        prediction.predictive.predictionData.closestPoint = {
            currentindex: 0
        }
        requestPaint();
    }


    function updatePercs(pairs) {
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
        requestPaint();
    }


    onUnitChanged: {
        Graphing.updateUnits();
        requestPaint();
    }

    Component.onCompleted: {
        loadImage("qrc:/icons/flow/dosages_disabled_mini.png");
        loadImage("qrc:/icons/flow/covariates_disabled_mini.png");
        loadImage("qrc:/icons/flow/targets_disabled_mini.png");
        //        var yu = isImageLoading("qrc:/icons/flow/dosages_disabled_mini.png");
        //        var yu2 = isImageLoading("qrc:/icons/flow/covariates_disabled_mini.png");
        //        console.log("IMAGE LOADING: " + yu);
        Graphing.updateUnits();
        requestPaint();
    }
    onImageLoaded: {
        //        var yes = isImageLoaded("qrc:/icons/flow/dosages_disabled_mini.png");
        //        console.log("IMAGE LOADED: " + yes);
    }

    onPaint: {
        updateChartDimensions();
        Graphing.step();
        Annotations.step();
        Clock.showClock();
    }

    Timer {
        id: clocktima
        interval: 1000; running: true; repeat: true
        onTriggered: date = new Date();
    }

    //ToDo: horizontal offset and zoom only
    MouseArea {
        id: mouseArea
        anchors.fill: parent

        hoverEnabled: true

        property bool isMouseOver: false
        property bool hasShifted: false
        property double pressX
        property int tooltipX: -1
        property int tooltipY: -1

        onDoubleClicked: {
            Graphing.manualextents();
            tooltipX = -1;  // Clear tooltip
            canvas.requestPaint();
        }

        onClicked: {
        }

        onReleased: {
            closestPred.selected = !closestPred.selected && closestPred.highlight;
            canvas.requestPaint();
        }

        onPressed: {
            pressX = mouse.x;
            tooltipX = mouse.x;
            tooltipY = mouse.y;
        }

        onExited: {
//            canvas.requestPaint();
        }

        onPositionChanged: {
            if (pressed && containsMouse) {
                var dx = mouse.x - pressX
                canvas.isOffsetXEnabled ? Graphing.shift(dx) : 0;
                pressX = mouse.x;
                tooltipX = -1; // Clear tooltip
            }
            isMouseOver = mouse.x >= canvas.topLeftX && mouse.x <= canvas.topRightX &&
                          mouse.y >= canvas.topLeftY && mouse.y <= canvas.bottomLeftY;
        }

        onWheel: {
            // For most mice, 120 equals one step of the wheel.
            Graphing.zoom(wheel.angleDelta.y/120);
            tooltipX = -1; // Clear tooltip
        }

        CheckBox {
            id: popPCB
            onClicked: { graphInformationSelection.setVisible(0, this.checked) }

            TooltipArea {
                text : ToolTips.chart.popVisible
            }
        }
        CheckBox {
            id: popPerCB
            onClicked: { graphInformationSelection.setVisible(1, this.checked) }
            TooltipArea {
                text : ToolTips.chart.popPercVisible
            }
        }
        CheckBox {
            id: aprPCB
            onClicked: { graphInformationSelection.setVisible(2, this.checked) }
            TooltipArea {
                text : ToolTips.chart.aprioriVisible
            }
        }
        CheckBox {
            id: aprPerCB
            onClicked: { graphInformationSelection.setVisible(3, this.checked) }
            TooltipArea {
                text : ToolTips.chart.aprioriPercVisible
            }
        }
        CheckBox {
            id: apoPCB
            onClicked: { graphInformationSelection.setVisible(4, this.checked) }
            TooltipArea {
                text : ToolTips.chart.apostVisible
            }
        }
        CheckBox {
            id: apoPerCB
            onClicked: { graphInformationSelection.setVisible(5, this.checked) }
            TooltipArea {
                text : ToolTips.chart.apostPercVisible
            }
        }
        CheckBox {
            id: revCB
            onClicked: { graphInformationSelection.setVisible(8, this.checked) }
            TooltipArea {
                text : ToolTips.chart.revVisible
            }
        }
        CheckBox {
            id: adjCB
            onClicked: { graphInformationSelection.setVisible(9, this.checked) }
            TooltipArea {
                text : ToolTips.chart.adjVisible
            }
        }
    }

    function resume() {
        Trails.setTargetSize(0);
        overlay.fadeout = true;
    }

    function wait() {
        overlay.fadeout = false;
        overlay.visible = true;
        Trails.setTargetSize(1);
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
            Trails.init();
            Trails.setTargetSize(0);
        }
        onPaint: {
            Trails.loop();
        }
    }
}