import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
//import QtQuick.Controls.Styles
import QtQuick.Window

import Qt5Compat.GraphicalEffects

//import QtWebEngine

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel


ColumnLayout {
    id: root
    objectName: "flowView"
    width:  800
    height: 600
    focus: true

    property alias chart: chart
    property alias drugtab: drugTab
    property alias druginfo: druginfo
    property int currentIndex: 0
    signal changedTab(int index);
    signal validateTab(int step);
    signal toggleValidation(int step);

    // For testing
    function getCurrentIndex() {
        return currentIndex;
    }

    function reset() {
        currentIndex = 0;
    }

    //    WebEngineView {
    //            objectName: "channelView"
    //            url: "qrc:/controls/index.html"
    //        }

    function setStatus(status)
    {
        var tabs = [patientTab, drugTab, dosageTab, covariateTab, measureTab, targetTab, adjustmentTab, validationTab, reportTab];
        for (var i=0; i<tabs.length; i++)
        {
            tabs[i].status = status.getStatus(i);
            tabs[i].enabled = status.getEnable(i);
        }
    }

    function handleOnPress(index)
    {
        if (flow.currentIndex === index) {
            toggleValidation(flow.currentIndex);
        }
        else {
            root.currentIndex = index
            root.changedTab(currentIndex)
            root.focus = true;
        }
    }

    Keys.onPressed: function(event) {
        if(event.modifiers === Qt.NoModifier || event.modifiers === Qt.KeypadModifier){
            if (event.key === Qt.Key_Right) {
                interpretationController.goToNextEvent();
            }

            if (event.key === Qt.Key_Left) {
                interpretationController.goToPreviousEvent();
            }

            if (event.key === Qt.Key_0) {
                validateTab(flow.currentIndex);
                event.accepted = true;
                return;
            }
            if (event.key === Qt.Key_1) {
                if (patientTab.enabled) {
                    flow.currentIndex = 0;
                    event.accepted = true;
                }
                return;
            }
            if (event.key === Qt.Key_2) {
                if (drugTab.enabled) {
                    flow.currentIndex = 1;
                    event.accepted = true;
                }
                return;
            }
            if (event.key === Qt.Key_3) {
                if (dosageTab.enabled) {
                    flow.currentIndex = 2;
                    event.accepted = true;
                }
                return;
            }
            if (event.key === Qt.Key_4) {
                if (covariateTab.enabled) {
                    flow.currentIndex = 3;
                    event.accepted = true;
                }
                return;
            }
            if (event.key === Qt.Key_5) {
                if (measureTab.enabled) {
                    flow.currentIndex = 4;
                    event.accepted = true;
                }
                return;
            }
            if (event.key === Qt.Key_6) {
                if (targetTab.enabled) {
                    flow.currentIndex = 5;
                    event.accepted = true;
                }
                return;
            }
            if (event.key === Qt.Key_7) {
                if (adjustmentTab.enabled) {
                    flow.currentIndex = 6;
                    event.accepted = true;
                }
                return;
            }
            if (event.key === Qt.Key_8) {
                if (validationTab.enabled) {
                    flow.currentIndex = 7;
                    event.accepted = true;
                }
                return;
            }
            if (event.key === Qt.Key_9) {
                if (reportTab.enabled) {
                    flow.currentIndex = 8;
                    event.accepted = true;
                }
                return;
            }
        }
    }

    FlowMenu {
        id: menu
        Layout.fillWidth: true
        Layout.preferredHeight: 128


        FlowInformationPanel {
            id : infoPanel
            Layout.preferredWidth: parent.width - 9 * menu.buttonWidth
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        property int buttonWidth : (parent.width - infoPanel.totalWidth)/9

        FlowMenuButton {
            id: patientButton
            objectName: "patientButton"
            // These 200 should depend on informationPanel instead
            Layout.preferredWidth: menu.buttonWidth
            legend: (width > 100) ? "Patient" : "Pat."
            icon:          "qrc:/icons/flow/patient.png"
            icon_disabled: "qrc:/icons/flow/patient_disabled.png"
            icon_hovered:  "qrc:/icons/flow/patient_hover.png"
            icon_selected: "qrc:/icons/flow/patient.png"
            status:    patientTab.status
            isEnabled: patientTab.enabled
            index: 0
            currentIndex: root.currentIndex
            onPressed:    {
                handleOnPress(index);
            }
            onDoublePressed: {
                validateTab(index);
            }

            tooltipText : ToolTips.flowButtons.patient
        }
        FlowMenuButton {
            id: drugButton
            objectName : "drugButton"
            Layout.preferredWidth: menu.buttonWidth
            legend: "Drugs"
            icon:          "qrc:/icons/flow/drugs.png"
            icon_disabled: "qrc:/icons/flow/drugs_disabled.png"
            icon_hovered:  "qrc:/icons/flow/drugs_hover.png"
            icon_selected: "qrc:/icons/flow/drugs.png"
            status:    drugTab.status
            isEnabled: drugTab.enabled
            index: 1
            currentIndex: root.currentIndex
            onPressed:    {
                handleOnPress(index);
            }
            onDoublePressed: {
                validateTab(index);
            }

            tooltipText : ToolTips.flowButtons.drug
        }
        FlowMenuButton {
            id: dosageButton
            objectName : "dosageButton"
            Layout.preferredWidth: menu.buttonWidth
            legend: (width > 100) ? "Dosages" : "Dos."
            icon:          "qrc:/icons/flow/dosages.png"
            icon_disabled: "qrc:/icons/flow/dosages_disabled.png"
            icon_hovered:  "qrc:/icons/flow/dosages_hover.png"
            icon_selected: "qrc:/icons/flow/dosages.png"
            status:    dosageTab.status
            isEnabled: dosageTab.enabled
            index: 2
            currentIndex: root.currentIndex
            onPressed:    {
                handleOnPress(index);
            }
            onDoublePressed: {
                validateTab(index);
            }

            tooltipText : ToolTips.flowButtons.dosage
        }
        FlowMenuButton {
            id: covariateButton
            objectName: "covariateButton"
            Layout.preferredWidth: menu.buttonWidth
            legend: (width > 100) ? "Covariates" : "Cov."
            icon:          "qrc:/icons/flow/covariates.png"
            icon_disabled: "qrc:/icons/flow/covariates_disabled.png"
            icon_hovered:  "qrc:/icons/flow/covariates_hover.png"
            icon_selected: "qrc:/icons/flow/covariates.png"
            status:    covariateTab.status
            isEnabled: covariateTab.enabled
            index: 3
            currentIndex: root.currentIndex
            onPressed:    {
                handleOnPress(index);
            }
            onDoublePressed: {
                validateTab(index);
            }
            tooltipText : ToolTips.flowButtons.covariates
        }
        FlowMenuButton {
            id: measureButton
            objectName: "measureButton"
            Layout.preferredWidth: menu.buttonWidth
            legend: (width > 100) ? "Measures" : "Meas."
            icon:          "qrc:/icons/flow/measures.png"
            icon_disabled: "qrc:/icons/flow/measures_disabled.png"
            icon_hovered:  "qrc:/icons/flow/measures_hover.png"
            icon_selected: "qrc:/icons/flow/measures.png"
            status:    measureTab.status
            isEnabled: measureTab.enabled
            index: 4
            currentIndex: root.currentIndex
            onPressed:    {
                handleOnPress(index);
            }
            onDoublePressed: {
                validateTab(index);
            }

            tooltipText : ToolTips.flowButtons.measures
        }
        FlowMenuButton {
            id: targetButton
            objectName: "targetButton"
            Layout.preferredWidth: menu.buttonWidth
            legend: (width > 100) ? "Targets" : "Tar."
            icon:          "qrc:/icons/flow/targets.png"
            icon_disabled: "qrc:/icons/flow/targets_disabled.png"
            icon_hovered:  "qrc:/icons/flow/targets_hover.png"
            icon_selected: "qrc:/icons/flow/targets.png"
            status:    targetTab.status
            isEnabled: targetTab.enabled
            index: 5
            currentIndex: root.currentIndex
            onPressed:    {
                handleOnPress(index);
            }
            onDoublePressed: {
                validateTab(index);
            }

            tooltipText : ToolTips.flowButtons.targets
        }
        FlowMenuButton {
            id: adjustmentButton
            objectName: "adjustmentButton"
            isLast : appGlobals.cdssOnly
            Layout.preferredWidth: menu.buttonWidth
            legend: (width > 100) ? "Adjustments" : "Adj."
            icon:          "qrc:/icons/flow/adjustments.png"
            icon_disabled: "qrc:/icons/flow/adjustments_disabled.png"
            icon_hovered:  "qrc:/icons/flow/adjustments_hover.png"
            icon_selected: "qrc:/icons/flow/adjustments.png"
            status:    adjustmentTab.status
            isEnabled: adjustmentTab.enabled
            index: 6
            currentIndex: root.currentIndex
            onPressed:    {
                handleOnPress(index);
            }
            onDoublePressed: {
                validateTab(index);
            }

            tooltipText : ToolTips.flowButtons.adjustment
        }
        FlowMenuButton {
            id: validationButton
            objectName: "validationButton"
            Layout.preferredWidth: menu.buttonWidth
            legend: (width > 100) ? "Validation" : "Val."
            icon:          "qrc:/icons/flow/validation.png"
            icon_disabled: "qrc:/icons/flow/validation_disabled.png"
            icon_hovered:  "qrc:/icons/flow/validation_hover.png"
            icon_selected: "qrc:/icons/flow/validation.png"
            status:    validationTab.status
            isEnabled: validationTab.enabled
            index: 7
            currentIndex: root.currentIndex
            onPressed:    {
                handleOnPress(index);
            }
            onDoublePressed: {
                validateTab(index);
            }
            visible : !appGlobals.cdssOnly
            tooltipText : ToolTips.flowButtons.validation
        }
        FlowMenuButton {
            id: reportButton
            objectName: "reportButton"
            Layout.preferredWidth: menu.buttonWidth
            isLast : !appGlobals.cdssOnly // This is the last button, so don't draw the arrow
            legend: (width > 100) ? "Reports" : "Rep."
            icon:          "qrc:/icons/toolbar/file.png"
            icon_disabled: "qrc:/icons/toolbar/file_disabled.png"
            icon_hovered:  "qrc:/icons/toolbar/file_hover.png"
            icon_selected: "qrc:/icons/toolbar/file.png"
            status:    reportTab.status
            isEnabled: reportTab.enabled
            index: 8
            currentIndex: root.currentIndex
            onPressed:    {
                handleOnPress(index);
            }
            onDoublePressed: {
                validateTab(index);
            }
            visible : !appGlobals.cdssOnly
            tooltipText : ToolTips.flowButtons.report
        }
    }

    RowLayout {
        id: cc
        //anchors.fill: parent
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumHeight: 500
        Layout.minimumWidth: 800

        property real ratio: 0.6

        Item  {
            id: content
//            Layout.minimumWidth: parent.width * (1 - parent.ratio)
           // Layout.preferredWidth:  320
            Layout.fillWidth: true
            Layout.minimumWidth: 320
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.fillHeight: true

            PatientTab {
                id: patientTab
                objectName:   "patientsView"
                anchors.fill: parent
                visible: patientButton.isCurrent
            }

            DrugTab {
                id: drugTab
                objectName:   "drugsView"
                anchors.fill: parent
                visible: drugButton.isCurrent
                //enabled: patientTab.status !== Status.invalid
            }
            DosageTab {
                id: dosageTab
                objectName:   "dosagesView"
                anchors.fill: parent
                visible: dosageButton.isCurrent
                //enabled: patientTab.status !== Status.invalid && drugTab.status !== Status.invalid
            }
            CovariateTab {
                id: covariateTab
                objectName:   "covariatesView"
                anchors.fill: parent
                visible: covariateButton.isCurrent
                //enabled: patientTab.status !== Status.invalid && drugTab.status !== Status.invalid
            }
            MeasureTab {
                id: measureTab
                objectName:   "measuresView"
                anchors.fill: parent
                visible: measureButton.isCurrent
                //enabled: patientTab.status !== Status.invalid && drugTab.status !== Status.invalid
            }
            TargetTab {
                id: targetTab
                objectName:   "targetsView"
                anchors.fill: parent
                visible: targetButton.isCurrent
                //enabled: patientTab.status !== Status.invalid && drugTab.status !== Status.invalid
            }
            AdjustmentTab {
                id: adjustmentTab
                objectName:   "adjustmentsView"
                anchors.fill: parent
                visible: adjustmentButton.isCurrent
                //enabled: dosageTab.status !== Status.invalid && targetTab.status === Status.complete
            }
            ValidationTab {
                id: validationTab
                objectName:   "validationView"
                anchors.fill: parent
                visible: validationButton.isCurrent
                //enabled: patientTab.status    !== Status.invalid &&
                //         drugTab.status       !== Status.invalid &&
                //         dosageTab.status     !== Status.invalid &&
                //         covariateTab.status  !== Status.invalid &&
                //         measureTab.status    !== Status.invalid &&
                //         targetTab.status     !== Status.invalid &&
                //         adjustmentTab.status !== Status.invalid
            }
            ReportTab {
                id:reportTab
                objectName:   "reportView"
                anchors.fill: parent
                visible: reportButton.isCurrent
                //enabled: dosageTab.status !== Status.invalid && measureTab.status === Status.complete
            }
        }
        Item {
            Layout.fillWidth: true
         //   Layout.preferredWidth:  480
            Layout.minimumWidth: 480
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.fillHeight: true
          //  anchors.right: parent.right
          //  anchors.top: parent.top
          //  anchors.bottom: parent.bottom

            Chart {
                id: chart
                anchors.fill: parent
                visible: !patientButton.isCurrent && !drugButton.isCurrent && !appGlobals.cdssOnly
            }

            WelcomeInfo {
                anchors.fill: parent
                visible: patientButton.isCurrent && (interpretationController.rawRequest === "")
            }

            RawRequest {
                anchors.fill: parent
                visible: patientButton.isCurrent && (interpretationController.rawRequest !== "")
            }

            DrugInfo {
                id: druginfo
                anchors.fill: parent
                visible: drugButton.isCurrent
            }
        }
    }
/*
    DemoBanner {
        Layout.fillWidth: true
        Layout.minimumHeight: 100
        Layout.preferredHeight: 128
    }
*/
}
