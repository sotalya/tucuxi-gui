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


    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            self.exit(true);
        }
    }

    function init(){

        self = this

        popPCB.visible = true
        popPCB.checked = graphInformationSelection.presentPopulationPrediction

        popPerCB.visible = true
        popPerCB.checked = graphInformationSelection.presentPopulationPercentiles

        aprPCB.visible = true
        aprPCB.checked = graphInformationSelection.presentAprioriPrediction

        aprPerCB.visible = true
        aprPerCB.checked = graphInformationSelection.presentAprioriPercentiles

        apoPCB.visible = true
        apoPCB.checked = graphInformationSelection.presentAposterioriPrediction

        apoPerCB.visible = true
        apoPerCB.checked = graphInformationSelection.presentAposterioriPercentiles

        revCB.visible = true
        revCB.checked = graphInformationSelection.presentPossibleAdjustments

        adjCB.visible = true
        adjCB.checked = graphInformationSelection.presentSelectedAdjustment
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
          }

            ColumnLayout {
                spacing: 2
                CheckBox {
                    id: apoPCB
                    text: aposterioriText
                    onClicked: { graphInformationSelection.setAvailable(4, this.checked) }
                    TooltipArea {
                        text : ToolTips.chart.apostVisible
                    }
                }
                CheckBox {
                    id: apoPerCB
                    text: apoPercText
                    onClicked: { graphInformationSelection.setAvailable(5, this.checked) }
                    TooltipArea {
                        text : ToolTips.chart.apostPercVisible
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
                        self.exit(false);
                    }
                }
            }
        }
    }
}
