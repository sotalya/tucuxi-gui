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
    height: 400

    property var self

    //Constants
    property var populationText:    "Population";
    property var popPercText:       "Pop. percentiles (5-25-75-95)"
    property var aprioriText:       "A priori";
    property var aprPercText:       "Apr. percentiles (5-25-75-95)"
    property var aposterioriText:   "A posteriori";
    property var apoPercText:       "Apo. percentiles (5-25-75-95)"
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

        RowLayout{
            spacing: 10

            ColumnLayout {
                spacing: 2

                CheckBox {
                    id: popPCB
                    text: populationText
                    onClicked: { graphInformationSelection.setAvailable(0, this.checked) }

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
                spacing: 2

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

            RowLayout {
                spacing: 10
                Layout.preferredWidth: parent
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
