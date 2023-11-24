import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQml
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

DialogBase {
    id: root
    title: "Dosage definition"
    width: 600
    height: 400

    property var self

    property var stdTreatment

    property var availableRoutes

    property bool multipleRoutes : false

    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            if (self.apply()) {
                self.exit(true);
            }
        }
    }

    function init(quantity, interval, route, tinf, appliedDate, endDate, hasEndDate, isAtSteadyState, routes, routeNames, disableAtSteadyState,
                  standardTreatment, drugmodel)
    {
        self = this

        stdTreatment = standardTreatment;

        availableRoutes = routes;

        // doses
//        doseSpinBox.decimals = 3;
        doseSpinBox.setRealValue(quantity.dbvalue);
        doseSpinBox.suffix = " " + quantity.unitstring;
        doseSpinBox.valuesList = drugmodel.doses.dosesList
        //doseSpinBox.from = drugmodel.doses.dosesList[0] * Math.pow(10, doseSpinBox.decimals);
        //doseSpinBox.to = drugmodel.doses.dosesList[(drugmodel.doses.dosesList.length - 1)] * Math.pow(10, doseSpinBox.decimals);
        doseSpinBox.doValidation = function() { return getQuantity() > 0 }

        // intervals
//        intervalSpinBox.decimals = 2;
        intervalSpinBox.setRealValue(interval);
        intervalSpinBox.valuesList = drugmodel.intervals.intervalsList
        //intervalSpinBox.from = drugmodel.intervals.intervalsList[0] * Math.pow(10, doseSpinBox.decimals);
        //intervalSpinBox.to = drugmodel.intervals.intervalsList[(drugmodel.intervals.intervalsList.length - 1)] * Math.pow(10, doseSpinBox.decimals);
        intervalSpinBox.doValidation = function() { return getInterval () >= 1 }

        multipleRoutes = (routes.objlist.length > 1);

        var rNames = [];
        for (var i = 0; i < routes.objlist.length; ++i) {
            //                    console.log("route at (" + i + "): " + routeobjs[i].label);
            rNames[i] = routes.objlist[i].description;
            if (routes.objlist[i].description === route.description) {
                routeComboBox.currentIndex = i;
            }
        }
        routeComboBox.model = rNames;
//        routeComboBox.currentIndex = route.value;
        routeText.text = route.description

        // infusions
        infusionRow.visible = route.label === "INFUSION"
//        infusionSpinBox.decimals = 2;   // 2
        infusionSpinBox.setRealValue(tinf);
        infusionSpinBox.valuesList = drugmodel.infusions.infusionsList
//        infusionSpinBox.from = drugmodel.infusions.infusionsList[0] * Math.pow(10, doseSpinBox.decimals);
//        infusionSpinBox.to = drugmodel.infusions.infusionsList[(drugmodel.infusions.infusionsList.length - 1)] * Math.pow(10, doseSpinBox.decimals);
        infusionSpinBox.doValidation = function() { return getInfusion() >= 1 }
        // TODO We have an issue with these units. To be refactored at some stage
        //infusionSpinBox.suffix = drugmodel.infusions.quantity.unitstring
        infusionSpinBox.suffix = "minutes"

        var validateDates = function() {
            return getAppliedDate() <= getEndDate()
        }

        appliedTimeInput.setDate(appliedDate)
        appliedTimeInput.doValidation = validateDates

        appliedDateInput.setDate(appliedDate)
        appliedDateInput.doValidation = validateDates

        stoppedTimeInput.setDate(endDate)
        stoppedTimeInput.doValidation = validateDates

        stoppedDateInput.setDate(endDate)
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
    function getRoute()       { return routeComboBox.currentIndex }
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
            stoppedDateInput.setDate(newEndDate);
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
        if (routeText.text === "Intravenous drip")
            bOk = infusionSpinBox.validate() && bOk     // if infusion time exists : make infusion value check active
        bOk = appliedDateInput.validate() && bOk
        bOk = stoppedDateInput.validate() && bOk
        bOk = appliedTimeInput.validate() && bOk
        bOk = stoppedTimeInput.validate() && bOk
        return bOk
    }


    function showOverlappingMessage(bShow) {
        overlappingLabel.visible = bShow
    }

    function refresh()
    {
        var route = root.availableRoutes.objlist[routeComboBox.currentIndex];
        infusionRow.visible = route.label === "INFUSION";
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

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Dose:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.dosageDialog.dose
                }
                EntitySpinBoxList {
                    id: doseSpinBox
                    objectName: "doseSpinBox"                   // JRT 17.02.2022
                    Layout.preferredWidth: 250
                    horizontalAlignment: Text.AlignLeft
                    decimals: 2
                    onTextChangedSignal: { validate() }
                    onEditingFinished: { doValidation() }
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Interval:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.dosageDialog.interval
                }
                EntitySpinBoxList {
                    id: intervalSpinBox
                    objectName: "intervalSpinBox"                 // JRT 17.02.2022
                    Layout.preferredWidth: 250
                    horizontalAlignment: Text.AlignLeft
                    decimals: 0
                    suffix: " hours"
                    onTextChangedSignal: { validate() }
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

                EntityComboBox {
                    id: routeComboBox
                    visible : root.multipleRoutes
                    Layout.preferredWidth: 300
                    model: [] // Empty at the beginning. It is filled when the drug model is defined
                    onCurrentIndexChanged: {
                        root.refresh()
                    }
                }

                EntityTextField {
                    id: routeText
                    objectName: "routeText"
                    visible : !root.multipleRoutes
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
                EntitySpinBoxList {
                    id: infusionSpinBox
                    objectName: "infusionSpinBox"
                    //suffix: " minutes"
                    horizontalAlignment: Text.AlignLeft
                    Layout.preferredWidth: 250
                    decimals: 0
                    onTextChangedSignal: { validate() }
                    onEditingFinished: { self.validate() }
                }
            }

            RowLayout {
                CheckBox {
                    id: atSteadyState
                    objectName: "atSteadyState"
                    text: "At steady state?"
                    checked: true

                    // For testing
                    function setSteadyState (state) {
                        atSteadyState.checked = state
                    }
                    function getSteadyState () {
                        return atSteadyState.checked
                    }

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
                    objectName: "LastDoseOrFromDateInput"
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
                    objectName: "LastDoseOrFromTimeInput"
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
                            objectName: "stoppedDateInput"
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
                            objectName: "stoppedTimeInput"
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
                        if (root.apply()) {
                            root.exit(true);
                        }
                    }
                }
                Button {
                    id: applyBtn
                    objectName: "applyDosage"
                    text: "Apply"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        root.apply()
                    }
                }
                Button {
                    id: cancelBtn
                    objectName: "cancelDosage"
                    text: "Cancel"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        root.exit(false);
                    }
                }
            }
        }
    }
}
