import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles
import QtQuick.Window

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

DialogBase {
    id: root
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
            if (root.apply()) {
                root.exit(true);
            }
        }
    }


    function calculateUnit(type, baseUnit) {
        return targetTabController.calculateUnit(type, baseUnit);
    }

    function init(type, cmin, cbest, cmax, tmin, tbest, tmax, mic)
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

        micInput.decimals = 2;
        micInput.setRealValue(mic.dbvalue)
        micInput.suffix = " " + mic.unitstring

        // left cMin in red (check not dynamic), to be replaced (see below)
//        cMinInput.doValidation = function () { return cMinInput.getRealValue() >= 0 && cMinInput.getRealValue() <= cBestInput.getRealValue() }
//        cBestInput.doValidation = function () { return cBestInput.getRealValue() >= 0 }
//        cMaxInput.doValidation = function () { return cMaxInput.getRealValue() >= 0 && cMaxInput.getRealValue() >= cBestInput.getRealValue() }

        cMinInput.doValidation = function () { /*console.log("c1");*/ return cMinInput.getRealValue() >= 0 }
        cBestInput.doValidation = function () { /*console.log("c2");*/ return cBestInput.getRealValue() >= cMinInput.getRealValue() }
        cMaxInput.doValidation = function () { /*console.log("c3");*/ return cMaxInput.getRealValue() >= cBestInput.getRealValue() && cMaxInput.getRealValue() >= cMinInput.getRealValue() }

        tMinInput.doValidation = function () { /*console.log("t1");*/ return tMinInput.getRealValue() >= 0 }
        tBestInput.doValidation = function () { /*console.log("t2");*/ return tBestInput.getRealValue() >= tMinInput.getRealValue()}
        tMaxInput.doValidation = function () { /*console.log("t3");*/ return tMaxInput.getRealValue() >= tBestInput.getRealValue() && tMaxInput.getRealValue() >= tMinInput.getRealValue() }

        micInput.doValidation = function () { return micInput.getRealValue() >= 0 }

        validate()
    }

    function getType()  { return typeInput.currentIndex }
    function getCMin()  { return cMinInput.getRealValue() }
    function getCBest() { return cBestInput.getRealValue() }
    function getCMax()  { return cMaxInput.getRealValue() }
    function getTMin()  { return tMinInput.getRealValue() }
    function getTBest() { return tBestInput.getRealValue() }
    function getTMax()  { return tMaxInput.getRealValue() }
    function getMic()   { return micInput.getRealValue() }
    function getUnit()  { return currentUnit}

    function validate()
    {
        var bOk = cMinInput.validate()
        bOk = cBestInput.validate() && bOk
        bOk = cMaxInput.validate() && bOk
        bOk = tMinInput.validate() && bOk
        bOk = tBestInput.validate() && bOk
        bOk = tMaxInput.validate() && bOk
        bOk = micInput.validate() && bOk
        return bOk
    }

    // This function is used for automated tests, called from the C++ of GuiTests
    function setActiveType(typeIndex) {
        typeInput.currentIndex = typeIndex;
        //switchActiveType(typeIndex);

    }

    function checkValues() {
//        console.log("check *all* spinbox values")
        cMinInput.validate()
        cBestInput.validate()
        cMaxInput.validate()
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
            width: parent.width - 100
            height: parent.height - 50

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
                    objectName: "typeInput"
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
                    objectName: "cMinInput"
                    Layout.preferredWidth: 200
                    onTextChangedSignal: { checkValues() }
                    onEditingFinished: { root.validate() }
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
                    objectName: "cBestInput"
                    Layout.preferredWidth: 200
                    onTextChangedSignal: { checkValues() }
                    onEditingFinished: { root.validate() }
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
                    objectName: "cMaxInput"
                    Layout.preferredWidth: 200
                    onTextChangedSignal: { checkValues() }
                    onEditingFinished: { root.validate() }
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
                    objectName: "tMinInput"
                    Layout.preferredWidth: 200
                    enabled: typeInput.currentText === "Peak";
                    onTextChangedSignal: { checkValues() }
                    onEditingFinished: { root.validate() }
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
                    objectName: "tBestInput"
                    Layout.preferredWidth: 200
                    enabled: typeInput.currentText === "Peak";
                    onTextChangedSignal: { checkValues() }
                    onEditingFinished: { root.validate() }
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
                    objectName: "tMaxInput"
                    Layout.preferredWidth: 200
                    enabled: typeInput.currentText === "Peak";
                    onTextChangedSignal: { checkValues() }
                    onEditingFinished: { root.validate() }
                }
            }

            RowLayout {
                spacing: 2
                visible: typeInput.currentIndex > 5;
                EntityLabel {
                    id: micLabel
                    text: "MIC:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 100
                    tooltipText: ToolTips.targetDialog.mic
                }
                EntitySpinBox {
                    id: micInput
                    objectName: "micInput"
                    Layout.preferredWidth: 200
                    onEditingFinished: { root.validate() }
                }
            }

            RowLayout {
                Button {                    
                    id: acceptBtn
                    text: "Ok"
                    objectName: "okTarget"
                    Layout.preferredWidth: 150
                    onClicked: function() {
                        if (root.apply()) {
                            root.exit(true);
                        }
                    }
                }
                Button {
                    id: cancelBtn
                    text: "Cancel"
                    objectName: "cancelTarget"
                    Layout.preferredWidth: 150
                    onClicked: function() {
                        root.exit(false);
                    }
                }
            }
        }
    }
}
