import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

DialogBase {
    title: "Graph settings parameter"
    width: 600
    height: 600
    property var self

    //Constants
    property string populationText:    "Typical patient prediction";
    property string popPercText:       "Typical patient percentiles"
    property string aprioriText:       "A priori prediction";
    property string aprPercText:       "A priori percentiles"
    property string aposterioriText:   "A posteriori prediction";
    property string apoPercText:       "A posteriori percentiles"
    property string reverseText:       "Suggested adjustments";
    property string adjustmentText:    "Adjustments";
    property string targetText:        "Targets";
    property string measureText:       "Measures";
    property string currentTimeText:   "Current time";
    property string covariateChangeText: "Covariate change";
    property string liveAnnotationsText: "Live annotations";

    property var currentPopPred;
    property var currentPopPerc;
    property var currentAprPred;
    property var currentAprPerc;
    property var currentApoPred;
    property var currentApoPerc;
    property var currentTargets;
    property var currentMeasures;
    property var currentPossAdj;
    property var currentSelAdj;

    property var currentPerc50;
    property var currentPerc25_75;
    property var currentPerc10_90;
    property var currentPerc5_95;

    property var currentCurrentTime;
    property var currentCovariateChange;
    property var currentLiveAnnotations;

    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            graphInformationSelection.saveSettings()
            self.exit(true);
        }
    }

    function init(){

        self = this

        graphSettingsGB.title = graphInformationSelection.getCurrentTab() + " settings"

        popPCB.visible = true
        popPCB.checked = graphInformationSelection.displayPopulationPrediction
        currentPopPred = popPCB.checked //Keep current value for cancel

        popPerCB.visible = true
        popPerCB.checked = graphInformationSelection.displayPopulationPercentiles
        currentPopPerc = popPerCB.checked

        aprPCB.visible = true
        aprPCB.checked = graphInformationSelection.displayAprioriPrediction
        currentAprPred = aprPCB.checked

        aprPerCB.visible = true
        aprPerCB.checked = graphInformationSelection.displayAprioriPercentiles
        currentAprPerc = aprPerCB.checked

        apoPCB.visible = true
        apoPCB.checked = graphInformationSelection.displayAposterioriPrediction
        currentApoPred = apoPCB.checked

        apoPerCB.visible = true
        apoPerCB.checked = graphInformationSelection.displayAposterioriPercentiles
        currentApoPerc = apoPerCB.checked

        targetCB.visible = true
        targetCB.checked = graphInformationSelection.displayTargets
        currentTargets = targetCB.checked

        measureCB.visible = true
        measureCB.checked = graphInformationSelection.displayMeasures
        currentMeasures = measureCB.checked

        revCB.visible = true
        revCB.checked = graphInformationSelection.displayPossibleAdjustments
        currentPossAdj = revCB.checked

        adjCB.visible = true
        adjCB.checked = graphInformationSelection.displaySelectedAdjustment
        currentSelAdj = adjCB.checked


        perc5_95CB.checked = graphInformationSelection.perc5_95
        currentPerc5_95 = perc5_95CB.checked

        perc10_90CB.checked = graphInformationSelection.perc10_90
        currentPerc10_90 = perc10_90CB.checked

        perc25_75CB.checked = graphInformationSelection.perc25_75
        currentPerc25_75 = perc25_75CB.checked

        perc50CB.checked = graphInformationSelection.perc50
        currentPerc50 = perc50CB.checked

        currentTimeCB.checked = graphInformationSelection.displayCurrentTime
        currentCurrentTime = currentTimeCB.checked

        covariateChangeCB.checked = graphInformationSelection.displayCovariateChange
        currentCovariateChange = covariateChangeCB.checked

        liveAnnotationsCB.checked = graphInformationSelection.displayLiveAnnotations
        currentLiveAnnotations = liveAnnotationsCB.checked

    }

    function restore(){
        graphInformationSelection.setPercentile(3, currentPerc50)
        graphInformationSelection.setPercentile(2, currentPerc25_75)
        graphInformationSelection.setPercentile(1, currentPerc10_90)
        graphInformationSelection.setPercentile(0, currentPerc5_95)

        graphInformationSelection.setAvailable(9, currentSelAdj)
        graphInformationSelection.setAvailable(8, currentPossAdj)
        graphInformationSelection.setAvailable(7, currentTargets)
        graphInformationSelection.setAvailable(6, currentMeasures)
        graphInformationSelection.setAvailable(5, currentApoPerc)
        graphInformationSelection.setAvailable(4, currentApoPred)
        graphInformationSelection.setAvailable(3, currentAprPerc)
        graphInformationSelection.setAvailable(2, currentAprPred)
        graphInformationSelection.setAvailable(1, currentPopPerc)
        graphInformationSelection.setAvailable(0, currentPopPred)

        graphInformationSelection.displayCurrentTime = currentCurrentTime
        graphInformationSelection.displayCovariateChange = currentCovariateChange
        graphInformationSelection.displayLiveAnnotations = currentLiveAnnotations
    }



    GridLayout {
        anchors.fill: parent
        rows: 3
        columns: 3

        Rectangle {
            Layout.row: 0
            Layout.column: 0
            Layout.fillWidth:  true
            Layout.fillHeight:  true
        }
        Rectangle {
            Layout.row: 2
            Layout.column: 2
            Layout.fillWidth:  true
            Layout.fillHeight:  true
        }

        ColumnLayout {
            Layout.row: 1
            Layout.column:1
            width: parent.parent.width - 100
            height: parent.parent.height - 50

            GroupBox {
                id: graphSettingsGB
                width: parent.width
                height: parent.height
                RowLayout{
                    height: parent.height
                    ColumnLayout {
                        width: parent.width
                        height: parent.height
                        CheckBox {
                            id: popPCB
                            text: populationText
                            onCheckedChanged: { graphInformationSelection.setAvailable(0, this.checked) }

                            TooltipArea {
                                text : ToolTips.chart.popVisible
                            }
                        }

                        CheckBox {
                            id: popPerCB
                            text: popPercText
                            onClicked: { graphInformationSelection.setAvailable(1, this.checked) }
                            TooltipArea {
                                text : ToolTips.chart.popPercVisible
                            }
                        }

                        CheckBox {
                            id: aprPCB
                            text: aprioriText
                            onClicked: { graphInformationSelection.setAvailable(2, this.checked) }
                            TooltipArea {
                                text : ToolTips.chart.aprioriVisible
                            }
                        }
                        CheckBox {
                            id: aprPerCB
                            text: aprPercText
                            onClicked: { graphInformationSelection.setAvailable(3, this.checked) }
                            TooltipArea {
                                text : ToolTips.chart.aprioriPercVisible
                            }
                        }
                        CheckBox {
                            id: apoPCB
                            text: aposterioriText
                            onClicked: { graphInformationSelection.setAvailable(4, this.checked) }
                            TooltipArea {
                                text : ToolTips.chart.apostVisible
                            }
                        }
                    }

                    ColumnLayout {

                        CheckBox {
                            id: apoPerCB
                            text: apoPercText
                            onClicked: { graphInformationSelection.setAvailable(5, this.checked) }
                            TooltipArea {
                                text : ToolTips.chart.apostPercVisible
                            }
                        }
                        CheckBox {
                            id: measureCB
                            text: measureText
                            onClicked: { graphInformationSelection.setAvailable(6, this.checked) }
                            TooltipArea {
                                text : ToolTips.chart.apostVisible
                            }
                        }
                        CheckBox {
                            id: targetCB
                            text: targetText
                            onClicked: { graphInformationSelection.setAvailable(7, this.checked) }
                            TooltipArea {
                                text : ToolTips.chart.apostVisible
                            }
                        }
                        CheckBox {
                            id: revCB
                            text: reverseText
                            onClicked: { graphInformationSelection.setAvailable(8, this.checked) }
                            TooltipArea {
                                text : ToolTips.chart.revVisible
                            }
                        }
                        CheckBox {
                            id: adjCB
                            text: adjustmentText
                            onClicked: { graphInformationSelection.setAvailable(9, this.checked) }
                            TooltipArea {
                                text : ToolTips.chart.adjVisible
                            }
                        }
                    }
                }
            }
            GroupBox {
                id: percentilesGraphSettingsGB
                title: qsTr("Percentiles")
                width: graphSettingsGB.width

                ColumnLayout{
                    spacing: 1

                    RowLayout {
                        spacing: 2
                        CheckBox {
                            id : perc5_95CB
                            text:"5 - 95"
                            onClicked: {
                                graphInformationSelection.setPercentile(0, this.checked)
                            }
                        }
                        CheckBox {
                            id : perc10_90CB
                            text:"10 - 90"
                            onClicked: {
                                graphInformationSelection.setPercentile(1, this.checked)
                            }
                        }
                        CheckBox {
                            id : perc25_75CB
                            text:"25 - 75"
                            onClicked: {
                                graphInformationSelection.setPercentile(2, this.checked)
                            }
                        }
                        CheckBox {
                            id : perc50CB
                            text:"50"
                            onClicked: {
                                graphInformationSelection.setPercentile(3, this.checked)
                            }
                        }
                    }
                }
            }
            GroupBox {
                id: generalGraphSettingsGB
                title: qsTr("General Settings")
                width: graphSettingsGB.width

                ColumnLayout{
                    spacing: 1

                    RowLayout {
                        CheckBox {
                            id: currentTimeCB
                            text: currentTimeText
                            onClicked: { graphInformationSelection.displayCurrentTime = this.checked; }
                            TooltipArea {
                                text : ToolTips.chart.displayCurrentTime
                            }
                        }
                        CheckBox {
                            id: covariateChangeCB
                            text: covariateChangeText
                            onClicked: { graphInformationSelection.displayCovariateChange = this.checked; }
                            TooltipArea {
                                text : ToolTips.chart.displayCovariateChange
                            }
                        }
                        CheckBox {
                            id: liveAnnotationsCB
                            text: liveAnnotationsText
                            onClicked: { graphInformationSelection.displayLiveAnnotations = this.checked; }
                            TooltipArea {
                                text : ToolTips.chart.displayLiveAnnotations
                            }
                        }
                    }
                }
            }

            RowLayout {
                spacing: 10
                width: graphSettingsGB.width
                Button {
                    id: acceptBtn
                    text: "Save and Close"
                    objectName: "okDosage"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        graphInformationSelection.saveSettings()
                        self.exit(true);
                    }
                }
                Rectangle{
                    Layout.preferredWidth: 125
                }

                Button {
                    id: cancelBtn
                    text: "Cancel"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        restore();
                        self.exit(false);
                    }
                }
            }
        }
    }
}
