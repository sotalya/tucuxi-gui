import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    id: measureDialog
    title: "Measure parameters"
    width: 500
    height: 280


    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            if (measureDialog.apply()) {
                measureDialog.exit(true);
            }
        }
    }

    function init(sampleId, value, sampleDate, arrivalDate)
    {
        sampleIdField.text = sampleId
        valueInput.decimals = 2
        valueInput.setRealValue(value.dbvalue)
        valueInput.suffix = " " + value.unitstring

        var validateDates = function() {
            if (interpretationController.isFlowRequest()) {
                return getArrivalDate() >= getSampleDate();
            }
            return true;
        }

        sampleDateInput.date = sampleDate
        sampleDateInput.doValidation = validateDates

        sampleTimeInput.setDate(sampleDate)
        sampleTimeInput.doValidation = validateDates

        arrivalDateInput.date = arrivalDate
        arrivalDateInput.doValidation = validateDates

        arrivalTimeInput.setDate(arrivalDate)
        arrivalTimeInput.doValidation = validateDates

        receivedRow.visible = interpretationController.isFlowRequest()

        validate()
    }

    function getSampleId()    { return sampleIdField.text }
    function getValue()       { return valueInput.getRealValue() }

    function getSampleDate() {
        return new Date(sampleDateInput.date.getFullYear(),
                        sampleDateInput.date.getMonth(),
                        sampleDateInput.date.getDate(),
                        sampleTimeInput.date.getHours(),
                        sampleTimeInput.date.getMinutes())
    }

    function getArrivalDate() {
        return new Date(arrivalDateInput.date.getFullYear(),
                        arrivalDateInput.date.getMonth(),
                        arrivalDateInput.date.getDate(),
                        arrivalTimeInput.date.getHours(),
                        arrivalTimeInput.date.getMinutes())
    }

    function validate()
    {
        // Fix problem with TimePicker values not used...
        // ... force a change of focus to ensure TimePicker values are actually used
        sampleDateInput.focus = true

        var bOk = valueInput.validate()
        bOk = sampleDateInput.validate() && bOk
        bOk = sampleTimeInput.validate() && bOk
        bOk = arrivalDateInput.validate() && bOk
        bOk = arrivalTimeInput.validate() && bOk
        return bOk
    }

    GridLayout {
        anchors.fill: parent
        rows: 4
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
            height: parent.parent.width - 50

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Sample ID:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.measureDialog.sampleId
                }
                EntityTextField {
                    id: sampleIdField
                    objectName: "sampleIdField"
                    placeholderText: "sample id"
                    Layout.preferredWidth: 200
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Value:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.measureDialog.value
                }
                EntitySpinBox {
                    id: valueInput
                    objectName: "measureValueInput"
                    Layout.preferredWidth: 200
                    onEditingFinished: { measureDialog.validate() }
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Measured:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.measureDialog.date
                }
                DatePicker {
                    id: sampleDateInput
                    objectName : "sampleDateInput"
                    onEditingFinished: { measureDialog.validate() }
                }
                EntityLabel {
                    text: "at:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 50
                }
                TimePicker {
                    id: sampleTimeInput
                    objectName : "sampleTimeInput"
                    onEditingFinished: { measureDialog.validate() }
                }
            }

            RowLayout {
                spacing: 2
                id: receivedRow
                EntityLabel {
                    text: "Received:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.measureDialog.arrivalDate
                }
                DatePicker {
                    id: arrivalDateInput
                    onEditingFinished: { measureDialog.validate() }
                }
                EntityLabel {
                    text: "at:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 50
                }
                TimePicker {
                    id: arrivalTimeInput
                    onEditingFinished: { measureDialog.validate() }
                }
            }

            RowLayout {
                Button {
                    id: acceptBtn
                    text: "Ok"
                    objectName: "okMeasure"
                    Layout.preferredWidth: 145
                    onClicked: function() {
                        if (measureDialog.apply()) {
                            measureDialog.exit(true);
                        }
                    }
                }
                Button {
                    id: cancelBtn
                    text: "Cancel"
                    objectName: "cancelMeasure"
                    Layout.preferredWidth: 145
                    onClicked: function() {
                        measureDialog.exit(false);
                    }
                }
            }
        }
    }
}
