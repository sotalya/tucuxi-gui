import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    title: "Adjustment parameters"
    width: 600
    height: 350

    property var self

    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            if (self.apply()) {
                self.exit(true);
            }
        }
    }

    function init(quantity, interval, route, tinf, appliedDate, endDate, routeNames)
    {
        self = this

        doseSpinBox.decimals = 2;
        doseSpinBox.setRealValue(quantity.dbvalue);
        doseSpinBox.suffix = " " + quantity.unitstring;
        doseSpinBox.doValidation = function() { return getQuantity() > 0 }

        intervalSpinBox.setRealValue(interval);
        intervalSpinBox.doValidation = function() { return getInterval () > 0 }

        //routeComboBox.model = routeNames;
        //routeComboBox.currentIndex = route.value;
        routeText.text = route.description

        infusionRow.visible = route.label === "INFUSION"
        infusionSpinBox.setRealValue(tinf);

        var validateStartDate = function() {
            return getAppliedDate() >= adjustmentTabController.adjustmentDate
        }

        var validateEndDate = function() {
            return getAppliedDate() <= getEndDate()
        }

        appliedTimeInput.setDate(appliedDate)
        appliedDateInput.date = appliedDate
        stoppedTimeInput.setDate(endDate)
        stoppedDateInput.date = endDate

        appliedTimeInput.doValidation = validateStartDate
        appliedDateInput.doValidation = validateStartDate
        stoppedTimeInput.doValidation = validateEndDate
        stoppedDateInput.doValidation = validateEndDate

        showOverlappingMessage(false)

        validate()

//        routeComboBox.dialog = this;
//        refresh()
    }

    function getQuantity()     { return doseSpinBox.getRealValue() }
    function getInterval()     { return intervalSpinBox.getRealValue() }
    function getInfusion()     { return infusionSpinBox.getRealValue() }
    //function getRoute()       { return routeComboBox.currentIndex }

    function getAppliedDate()  { return new Date(appliedDateInput.date.getFullYear(),
                                    appliedDateInput.date.getMonth(),
                                    appliedDateInput.date.getDate(),
                                    appliedTimeInput.date.getHours(),
                                    appliedTimeInput.date.getMinutes())
                               }
    function getEndDate()      { return new Date(stoppedDateInput.date.getFullYear(),
                                    stoppedDateInput.date.getMonth(),
                                    stoppedDateInput.date.getDate(),
                                    stoppedTimeInput.date.getHours(),
                                    stoppedTimeInput.date.getMinutes())
                               }
    function validate()
    {
        // Fix problem with TimePicker values not used...
        // ... force a change of focus to ensure TimePicker values are actually used
        appliedDateInput.focus = true

        var bOk = doseSpinBox.validate()
        bOk = intervalSpinBox.validate() && bOk
        bOk = appliedDateInput.validate() && bOk
        bOk = stoppedDateInput.validate() && bOk
        bOk = appliedTimeInput.validate() && bOk
        bOk = stoppedTimeInput.validate() && bOk
        return bOk
    }

    function showOverlappingMessage(bShow) {
        overlappingLabel.visible = bShow
    }

/*
    function refresh()
    {
        infusionRow.visible = this.getRoute() === 1;
    }
*/

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

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Dose:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.adjustmentDialog.dose
                }
                EntitySpinBox {
                    id: doseSpinBox
                    objectName: "doseSpinBoxAdj"
                    Layout.preferredWidth: 250
                    horizontalAlignment: Text.AlignLeft
                    onEditingFinished: { self.validate() }
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Interval:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.adjustmentDialog.interval
                }
                EntitySpinBox {
                    id: intervalSpinBox
                    objectName: "intervalSpinBoxAdj"
                    Layout.preferredWidth: 250
                    horizontalAlignment: Text.AlignLeft
                    suffix: " hours"
                    onEditingFinished: { self.validate() }
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Route:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.adjustmentDialog.route
                }
    /*
                EntityComboBox {
                    id: routeComboBox
                    property var dialog
                    Layout.preferredWidth: 300
                    model: [] // Empty at the beginning. It is filled when the drug model is defined
                    onCurrentIndexChanged: {
                        dialog.refresh()
                    }

                }
    */
                EntityTextField {
                    id: routeText
                    readOnly: true
                    Layout.preferredWidth: 250
                }
            }

            RowLayout {
                id: infusionRow
                spacing: 2
                EntityLabel {
                    text: "Infusion:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.adjustmentDialog.infusion
                }
                EntitySpinBox {
                    id: infusionSpinBox
                    suffix: " minutes"
                    horizontalAlignment: Text.AlignLeft
                    Layout.preferredWidth: 250
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    Layout.preferredWidth: 100
                    text: "From:"
                    tooltipText: ToolTips.adjustmentDialog.from
                }
                DatePicker {
                    id: appliedDateInput
                    onEditingFinished: { self.validate() }
                }
                EntityLabel {
                    text: "at:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 50
                }
                TimePicker {
                    id: appliedTimeInput
                    onEditingFinished: { self.validate() }
                }
                EntityLabel {
                }
            }

            RowLayout {
                spacing: 2
                visible: true

                EntityLabel {
                    Layout.preferredWidth: 100
                    text: "To:"
                    tooltipText: ToolTips.adjustmentDialog.to
                }
                DatePicker {
                    id: stoppedDateInput
                    onEditingFinished: { self.validate() }
                }
                EntityLabel {
                    text: "at:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 50
                }
                TimePicker {
                    id: stoppedTimeInput
                    onEditingFinished: { self.validate() }
                }
                EntityLabel {
                }
            }

            EntityLabel {
                id: overlappingLabel
                Layout.preferredWidth: 200
                text: "Dates overlapping an other dosages"
                visible: false
                color: "red"
            }

            RowLayout {
                spacing: 10
                Button {
                    id: acceptBtn
                    objectName: "okAdj"
                    text: "Ok"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        if (self.apply()) {
                            self.exit(true);
                        }
                    }
                }
                Button {
                    id: applyBtn
                    objectName: "applyAdj"
                    text: "Apply"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        self.apply()
                    }
                }
                Button {
                    id: cancelBtn
                    objectName: "cancelAdj"
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
