import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    id: covariateDialog
    title: "Covariate parameters"
    width: 500
    height: 230

    property var dataType

    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            if (covariateDialog.apply()) {
                covariateDialog.exit(true);
            }
        }
    }

    function init(name, type, value, date, description)
    {
        dataType = type

        // Hide all value controls
        valueBoolControl.visible = false
        valueDoubleControl.visible = false
        valueGenderControl.visible = false

        valueDoubleControl.doValidation = function() { return !valueDoubleControl.visible || (valueDoubleControl.visible && getValue() >= 0) }
        valueGenderControl.doValidation = function() { return !valueGenderControl.visible || (valueGenderControl.visible && getValue() < 2) }

        // Set basic info
        nameText.text = name
        dateInput.date = date
        timeInput.date = date
        descriptionText.text = description === "" ? "N/A" : description

        // Handle different types of values
        if (name === "Sex" || name === "sex" || name === "Gender" || name === "gender") {
            valueGenderControl.visible = true
            valueGenderControl.currentIndex = value.dbvalue === 0.5 ? 2 : value.dbvalue;
        }
        else if (type === "bool") {
            valueBoolControl.visible = true;
            valueBoolControl.currentIndex = value.dbvalue === 0.5 ? 2 : value.dbvalue;
        }
        else {
            valueDoubleControl.visible = true
            valueDoubleControl.decimals = 2
            if (type === "int") {
                valueDoubleControl.decimals = 0
            }
            valueDoubleControl.setRealValue(value.dbvalue)
            valueDoubleControl.minimumValue = 0
            valueDoubleControl.maximumValue = 1000000 // Number.POSITIVE_INFINITY
            valueDoubleControl.suffix = " " + value.unitstring
        }

        validate()
    }    

    function getValue() {
        var name = nameText.text
        if (name === "Sex" || name === "sex" || name === "Gender" || name === "gender") {
            return valueGenderControl.currentIndex === 2 ? 0.5 : valueGenderControl.currentIndex
        }
        if (dataType === "bool") {
            return valueBoolControl.currentIndex === 2 ? 0.5 : valueBoolControl.currentIndex
        }
        return valueDoubleControl.getRealValue();
    }

    function getDate() {
        return new Date(dateInput.date.getFullYear(),
                        dateInput.date.getMonth(),
                        dateInput.date.getDate(),
                        timeInput.date.getHours(),
                        timeInput.date.getMinutes())
    }

    function validate()
    {
        // Fix problem with TimePicker values not used...
        // ... force a change of focus to ensure TimePicker values are actually used
        dateInput.focus = true

        var bOk = valueDoubleControl.validate()
        bOk = valueGenderControl.validate() && bOk
        return bOk;
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
            height: parent.parent.width - 50

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Type:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.covariateDialog.type
                }
                EntityLabel {
                    id: nameText
                    Layout.preferredWidth: 200
                    tooltipText: ToolTips.covariateDialog.type
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Value:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.covariateDialog.value
                }
                EntitySpinBox {
                    id: valueDoubleControl
                    Layout.preferredWidth: 200
                    onEditingFinished: { covariateDialog.validate() }
                }
                EntityComboBox {
                    id: valueBoolControl
                    Layout.preferredWidth: 200
                    model: [ "False", "True", "Undefined" ]
                    onCurrentIndexChanged: { covariateDialog.validate() }
                }
                EntityComboBox {
                    id: valueGenderControl
                    Layout.preferredWidth: 200
                    model: [ "Female", "Male", "Unknown" ]
                    onCurrentIndexChanged: { covariateDialog.validate() }
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Date:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.covariateDialog.date
                }
                DatePicker {
                    id: dateInput
                }
                EntityLabel {
                    text: "at:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 50
                }
                TimePicker {
                    id: timeInput
                }
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    text: "Description:"
                    Layout.preferredWidth: 100
                }
                EntityLabel {
                    id: descriptionText
                    Layout.preferredWidth: 200
                }
            }

            RowLayout {
                Button {
                    id: acceptBtn
                    text: "Ok"
                    Layout.preferredWidth: 145
                    onClicked: function() {
                        if (covariateDialog.apply()) {
                            covariateDialog.exit(true);
                        }
                    }
                }
                Button {
                    id: cancelBtn
                    text: "Cancel"
                    Layout.preferredWidth: 145
                    onClicked: function() {
                        covariateDialog.exit(false);
                    }
                }
            }
        }
    }
}