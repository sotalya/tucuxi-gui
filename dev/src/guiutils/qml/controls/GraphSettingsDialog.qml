import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    title: "Graph settings parameter"
    width: 600
    height: 450
    minimumHeight: 450
    property var self

    //Constants
    property var populationText:    "Population";
    property var popPercText:       "Pop. percentiles"
    property var aprioriText:       "A priori";
    property var aprPercText:       "Apr. percentiles"
    property var aposterioriText:   "A posteriori";
    property var apoPercText:       "Apo. percentiles"
    property var reverseText:       "Suggested adjustments";
    property var adjustmentText:    "Adjustments";
    property var targetText:        "Target";
    property var measureText:       "Measure";

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
        popPCB.checked = graphInformationSelection.presentPopulationPrediction
        currentPopPred = popPCB.checked //Keep current value for cancel

        popPerCB.visible = true
        popPerCB.checked = graphInformationSelection.presentPopulationPercentiles
        currentPopPerc = popPerCB.checked

        aprPCB.visible = true
        aprPCB.checked = graphInformationSelection.presentAprioriPrediction
        currentAprPred = aprPCB.checked

        aprPerCB.visible = true
        aprPerCB.checked = graphInformationSelection.presentAprioriPercentiles
        currentAprPerc = aprPerCB.checked

        apoPCB.visible = true
        apoPCB.checked = graphInformationSelection.presentAposterioriPrediction
        currentApoPred = apoPCB.checked

        apoPerCB.visible = true
        apoPerCB.checked = graphInformationSelection.presentAposterioriPercentiles
        currentApoPerc = apoPerCB.checked

        targetCB.visible = true
        targetCB.checked = graphInformationSelection.presentTargets
        currentTargets = targetCB.checked

        measureCB.visible = true
        measureCB.checked = graphInformationSelection.presentMeasures
        currentMeasures = measureCB.checked

        revCB.visible = true
        revCB.checked = graphInformationSelection.presentPossibleAdjustments
        currentPossAdj = revCB.checked

        adjCB.visible = true
        adjCB.checked = graphInformationSelection.presentSelectedAdjustment
        currentSelAdj = adjCB.checked

        currentPerc50 = graphInformationSelection.perc50
        currentPerc25_75 = graphInformationSelection.perc25_75
        currentPerc10_90 = graphInformationSelection.perc10_90
        currentPerc5_95 = graphInformationSelection.perc5_95
    }

    function restore(){
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

        graphInformationSelection.perc50 = currentPerc50
        graphInformationSelection.perc25_75 = currentPerc25_75
        graphInformationSelection.perc10_90 = currentPerc10_90
        graphInformationSelection.perc5_95 = currentPerc5_95
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
                id: generalGraphSettingsGB
                title: qsTr("General Settings")
                width: graphSettingsGB.width

                ColumnLayout{
                    spacing: 1
                    Text {
                        id: name
                        text: qsTr("Percentiles")
                    }

                    RowLayout {
                        spacing: 2
                        CheckBox {
                            id : perc5_95CB
                            checked: graphInformationSelection.perc5_95
                            text:"5 - 95"
                            property var old: !graphInformationSelection.perc5_95
                            onClicked: {
                                graphInformationSelection.setPercentile(old, 0)
                                old = !old
                            }
                        }
                        CheckBox {
                            id : perc10_90CB
                            checked: graphInformationSelection.perc10_90
                            text:"10 - 90"
                            property var old: !graphInformationSelection.perc10_90
                            onClicked: {
                                graphInformationSelection.setPercentile(old, 1)
                                old = !old
                            }
                        }
                        CheckBox {
                            id : perc25_75CB
                            checked: graphInformationSelection.perc25_75
                            text:"25 - 75"
                            property var old: !graphInformationSelection.perc25_75
                            onClicked: {
                                graphInformationSelection.setPercentile(old, 2)
                                old = !old
                            }
                        }
                        CheckBox {
                            id : perc50CB
                            checked: graphInformationSelection.perc50
                            text:"50"
                            property var old: !graphInformationSelection.perc50
                            onClicked: {
                                graphInformationSelection.setPercentile(old, 3)
                                old = !old
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
                    text: "Save and Quit"
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