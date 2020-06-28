import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.0

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    id: targetDialog
    title: "Target parameters"
    width: 450
    height: 450

    property string baseUnit : "ug/l"
    property string currentUnit : "ug/l"
    property string timeUnit : "h"


    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            if (targetDialog.apply()) {
                targetDialog.exit(true);
            }
        }
    }


    function calculateUnit(type, baseUnit) {
        return targetTabController.calculateUnit(type, baseUnit);
    }

    function init(type, cmin, cbest, cmax, tmin, tbest, tmax)
    {
        typeInput.currentIndex = type

        currentUnit = cmin.unitstring

        cMinInput.decimals = 2;
        cMinInput.setRealValue(cmin.dbvalue)
        cMinInput.suffix = " " + currentUnit
        cBestInput.decimals = 2;
        cBestInput.setRealValue(cbest.dbvalue)
        cBestInput.suffix = " " + currentUnit
        cMaxInput.decimals = 2;
        cMaxInput.setRealValue(cmax.dbvalue)
        cMaxInput.suffix = " " + currentUnit

        timeUnit = tmin.unitstring

        tMinInput.decimals = 2;
        tMinInput.setRealValue(tmin.dbvalue)
        tMinInput.suffix= " " + timeUnit
        tBestInput.decimals = 2;
        tBestInput.setRealValue(tbest.dbvalue)
        tBestInput.suffix= " " + timeUnit
        tMaxInput.decimals = 2;
        tMaxInput.setRealValue(tmax.dbvalue)
        tMaxInput.suffix= " " + timeUnit

        cMinInput.doValidation = function () { return cMinInput.getRealValue() >= 0 && cMinInput.getRealValue() <= cBestInput.getRealValue() }
        cBestInput.doValidation = function () { return cBestInput.getRealValue() >= 0 }
        cMaxInput.doValidation = function () { return cMaxInput.getRealValue() >= 0 && cMaxInput.getRealValue() >= cBestInput.getRealValue() }
        tMinInput.doValidation = function () { return tMinInput.getRealValue() >= 0 && tMinInput.getRealValue() <= tBestInput.getRealValue() }
        tBestInput.doValidation = function () { return tBestInput.getRealValue() >= 0 }
        tMaxInput.doValidation = function () { return tMaxInput.getRealValue() >= 0 && tMaxInput.getRealValue() >= tBestInput.getRealValue() }

        validate()
    }

    function getType()  { return typeInput.currentIndex }
    function getCMin()  { return cMinInput.getRealValue() }
    function getCBest() { return cBestInput.getRealValue() }
    function getCMax()  { return cMaxInput.getRealValue() }
    function getTMin()  { return tMinInput.getRealValue() }
    function getTBest() { return tBestInput.getRealValue() }
    function getTMax()  { return tMaxInput.getRealValue() }
    function getUnit()  { return currentUnit}

    function validate()
    {
        var bOk = cMinInput.validate()
        bOk = cBestInput.validate() && bOk
        bOk = cMaxInput.validate() && bOk
        bOk = tMinInput.validate() && bOk
        bOk = tBestInput.validate() && bOk
        bOk = tMaxInput.validate() && bOk
        return bOk
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
                    id: typeLabel
                    text: "TargetType:"
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.targetDialog.targetType
                }
                EntityComboBox {
                    id: typeInput
                    Layout.preferredWidth: 200
                    model: targetTabController.getTargetTypes()

                    onCurrentIndexChanged: {
                        // Modify the units
                        currentUnit = calculateUnit(typeInput.currentIndex, baseUnit)

                        cMinInput.suffix = " " + currentUnit
                        cBestInput.suffix = " " + currentUnit
                        cMaxInput.suffix = " " + currentUnit

//                        tMinInput.suffix= " " + tmin.unitstring
//                        tMinInput.suffix= " " + "h"
//                        tBestInput.suffix= " " + tbest.unitstring
//                        tMaxInput.suffix= " " + tmax.unitstring
                    }

                }
            }

            EntityLabel {
                text: "Target values:"
                Layout.preferredWidth: 100
                tooltipText: ToolTips.targetDialog.concentrations
            }

            RowLayout {
                spacing: 2
                EntityLabel {
                    id: cMinLabel
                    text: "Min:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.targetDialog.cMin
                }
                EntitySpinBox {
                    id: cMinInput
                    Layout.preferredWidth: 200
                    onEditingFinished: { targetDialog.validate() }
                }
            }
            RowLayout {
                spacing: 2
                EntityLabel {
                    id: cBestLabel
                    text: "Best:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.targetDialog.cBest
                }

                EntitySpinBox {
                    id: cBestInput
                    Layout.preferredWidth: 200
                    onEditingFinished: { targetDialog.validate() }
                }
            }
            RowLayout {
                spacing: 2
                EntityLabel {
                    id: cMaxLabel
                    text: "Max:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.targetDialog.cMax
                }
                EntitySpinBox {
                    id: cMaxInput
                    Layout.preferredWidth: 200
                    onEditingFinished: { targetDialog.validate() }
                }

            }
            EntityLabel {
                text: "Times:"
                Layout.preferredWidth: 100
                visible: typeInput.currentText === "Peak";
                tooltipText: ToolTips.targetDialog.times
            }

            RowLayout {
                spacing: 2
                visible: typeInput.currentText === "Peak";
                EntityLabel {
                    id: tMinLabel
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                    text: "Min:"
                    tooltipText: ToolTips.targetDialog.tMin
                }
                EntitySpinBox {
                    id: tMinInput
                    Layout.preferredWidth: 200
                    enabled: typeInput.currentIndex === 1
                    onEditingFinished: { targetDialog.validate() }                    
                }
            }
            RowLayout {
                spacing: 2
                visible: typeInput.currentText === "Peak";
                EntityLabel {
                    id: tBestLabel
                    text: "Best:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.targetDialog.tBest
                }
                EntitySpinBox {
                    id: tBestInput
                    Layout.preferredWidth: 200
                    enabled: typeInput.currentIndex === 1
                    onEditingFinished: { targetDialog.validate() }
                }
            }
            RowLayout {
                spacing: 2
                visible: typeInput.currentText === "Peak";
                EntityLabel {
                    id: tMaxLabel                    
                    text: "Max:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.targetDialog.tMax
                }
                EntitySpinBox {
                    id: tMaxInput
                    Layout.preferredWidth: 200
                    enabled: typeInput.currentIndex === 1
                    onEditingFinished: { targetDialog.validate() }
                }
            }

            RowLayout {
                Button {                    
                    id: acceptBtn
                    text: "Ok"
                    Layout.preferredWidth: 150
                    onClicked: function() {
                        if (targetDialog.apply()) {
                            targetDialog.exit(true);
                        }
                    }
                }
                Button {
                    id: cancelBtn
                    text: "Cancel"
                    Layout.preferredWidth: 150
                    onClicked: function() {
                        targetDialog.exit(false);
                    }
                }
            }
        }
    }
}
