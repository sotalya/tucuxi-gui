import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    title: "Dosage definition"
    width: 600
    height: 400

    property var self

    property var stdTreatment

    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            if (self.apply()) {
                self.exit(true);
            }
        }
    }

    function init(quantity, interval, route, tinf, appliedDate, endDate, hasEndDate, isAtSteadyState, routeNames, disableAtSteadyState,
                  standardTreatment, drugModel)
    {
        self = this

        stdTreatment = standardTreatment;
        doseSpinBox.decimals = 2;
        doseSpinBox.setRealValue(quantity.dbvalue);
        doseSpinBox.suffix = " " + quantity.unitstring;
        doseSpinBox.to = drugModel.doses.toDose.dbvalue * Math.pow(10, doseSpinBox.decimals);
        doseSpinBox.stepSize = drugModel.doses.stepDose.dbvalue * Math.pow(10, doseSpinBox.decimals);
        doseSpinBox.from = drugModel.doses.fromDose.dbvalue * Math.pow(10, doseSpinBox.decimals);

        doseSpinBox.doValidation = function() { return getQuantity() > 0 }

        intervalSpinBox.setRealValue(interval);
        intervalSpinBox.doValidation = function() { return getInterval () > 0 }

        //routeComboBox.model = routeNames;
        //routeComboBox.currentIndex = route.value;
        routeText.text = route.description

        infusionRow.visible = route.label === "INFUSION"
        infusionSpinBox.setRealValue(tinf);

        var validateDates = function() {
            return getAppliedDate() <= getEndDate()
        }

        appliedTimeInput.setDate(appliedDate)
        appliedTimeInput.doValidation = validateDates

        appliedDateInput.date = appliedDate
        appliedDateInput.doValidation = validateDates

        stoppedTimeInput.setDate(endDate)
        stoppedTimeInput.doValidation = validateDates

        stoppedDateInput.date = endDate
        stoppedDateInput.doValidation = validateDates

        if (disableAtSteadyState) {
            atSteadyState.checked = false
            atSteadyState.enabled = false
        }
        else {
            atSteadyState.checked = isAtSteadyState
            atSteadyState.enabled = true
        }

        showOverlappingMessage(false)

//        noEndDateRadio.checked = !hasEndDate
//        endDateRadio.checked = hasEndDate

        validate()

//        routeComboBox.dialog = this;
//        refresh()
    }

    function getQuantity()    { return doseSpinBox.getRealValue() }
    function getInterval()    { return intervalSpinBox.getRealValue() }
    function getInfusion()    { return infusionSpinBox.getRealValue() }
    //function getRoute()       { return routeComboBox.currentIndex }
    function getAppliedDate() { return new Date(appliedDateInput.date.getFullYear(),
                                   appliedDateInput.date.getMonth(),
                                   appliedDateInput.date.getDate(),
                                   appliedTimeInput.date.getHours(),
                                   appliedTimeInput.date.getMinutes())
                              }
    function getEndDate()     { return new Date(stoppedDateInput.date.getFullYear(),
                                   stoppedDateInput.date.getMonth(),
                                   stoppedDateInput.date.getDate(),
                                   stoppedTimeInput.date.getHours(),
                                   stoppedTimeInput.date.getMinutes())
                              }


    function calculateEndDate()     {
        return dosageTabController.getNewEndTime(getAppliedDate());
    }

    function updateEndDate() {
        if (stdTreatment.isFixedDuration) {
            var newEndDate = calculateEndDate();
            stoppedDateInput.date = newEndDate;
            stoppedTimeInput.setDate(newEndDate);
        }
    }

    function isAtSteadyState() { return atSteadyState.checked }
    function hasEndDate()      { return true /* !noEndDateRadio.checked */ }

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
                    tooltipText: ToolTips.dosageDialog.dose
                }
                EntitySpinBox {
                    id: doseSpinBox
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
                    tooltipText: ToolTips.dosageDialog.interval
                }
                EntitySpinBox {
                    id: intervalSpinBox
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
                    tooltipText: ToolTips.dosageDialog.route
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
                    tooltipText: ToolTips.dosageDialog.infusion
                }
                EntitySpinBox {
                    id: infusionSpinBox
                    suffix: " minutes"
                    horizontalAlignment: Text.AlignLeft
                    Layout.preferredWidth: 250
                }
            }

            RowLayout {
                CheckBox {
                    id: atSteadyState
                    text: "At steady state?"
                    checked: true

                    ToolTip.visible: (show_tooltip) ? steadyStateMousearea.containsMouse : false
                    ToolTip.text: ToolTips.dosageDialog.steadyState

                    MouseArea {
                        id: steadyStateMousearea
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        width: parent.width - 30
                        hoverEnabled: true
                    }
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    Layout.preferredWidth: 100
                    text: atSteadyState.checked ? "Last dose:" : "From:"
                    tooltipText: atSteadyState.checked ? ToolTips.dosageDialog.date : ToolTips.dosageDialog.from
                }
                DatePicker {
                    id: appliedDateInput
                    onEditingFinished: {
                        self.validate();
                        updateEndDate();
                    }
                }
                EntityLabel {
                    text: "at:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 50
                }
                TimePicker {
                    id: appliedTimeInput
                    onEditingFinished: {
                        self.validate();
                        updateEndDate();
                    }
                }
                EntityLabel {
                }
            }

            RowLayout {
                spacing: 2
                visible: atSteadyState.checked ? false : true

                EntityLabel {
                    Layout.preferredWidth: 100
                    text: "To:"
                    tooltipText: ToolTips.dosageDialog.to
                }
/*
                ColumnLayout {
                    ExclusiveGroup { id: endDateGroup }
                    RadioButton {
                        id: noEndDateRadio
                        text: "No end"
                        checked: true
                        exclusiveGroup: endDateGroup
                    }
                    RowLayout {
                        RadioButton {
                            exclusiveGroup: endDateGroup
                        }
*/
                        DatePicker {
                            id: stoppedDateInput
                            onEditingFinished: { self.validate() }
                            //isEnabled: noEndDateRadio.checked === false
                        }
                        EntityLabel {
                            text: "at:"
                            horizontalAlignment: Text.AlignRight
                            Layout.preferredWidth: 50
                        }
                        TimePicker {
                            id: stoppedTimeInput
                            onEditingFinished: { self.validate() }
                            //isEnabled: noEndDateRadio.checked === false
                        }
                        EntityLabel {
                        }
/*
                    }
                }
*/
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
                    text: "Ok"
                    objectName: "okDosage"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        if (self.apply()) {
                            self.exit(true);
                        }
                    }
                }
                Button {
                    id: applyBtn
                    objectName: "applyDosage"
                    text: "Apply"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        self.apply()
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
