import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.0

import QtGraphicalEffects 1.0

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

import ezechiel 1.0

Item {
    id: root
    width:  800
    height: 600

//    property alias chart: impl.chart
    property int index: 0


    signal changedTab(int index);
    signal currentIndexChanged(string view,int index)


    onCurrentIndexChanged: changedTab(index)

    function reset() {
        index = 0;
    }

    Item {
        id: impl
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            FlowMenu {
                id: menu
                Layout.fillWidth: true
                Layout.preferredHeight: 128

                informationPanel: FlowInformationPanel {
                    drugName:         drugTab.drugName

                 }

                FlowMenuButton {
                    id: drugButton
                    objectName: "drugButton"
                    legend: "Drugs"
                    icon:          "qrc:/icons/flow/drugs.png"
                    icon_disabled: "qrc:/icons/flow/drugs_disabled.png"
                    icon_hovered:  "qrc:/icons/flow/drugs_hover.png"
                    icon_selected: "qrc:/icons/flow/drugs.png"
                    status:    drugTab.status
                    isEnabled: drugTab.enabled
                    index: 0
                    currentIndex: root.index
                    onPressed:root.currentIndexChanged("flowView",index)
                }

                FlowMenuButton {
                    id: editorButton
                    objectName: "editorButton"
                    legend: "Editors"
                    icon:          "qrc:/icons/flow/patient.png"
                    icon_disabled: "qrc:/icons/flow/patient_disabled.png"
                    icon_hovered:  "qrc:/icons/flow/patient_hover.png"
                    icon_selected: "qrc:/icons/flow/patient.png"
                    status:    editorTab.status
                    isEnabled: editorTab.enabled
                    index: 1
                    currentIndex: root.index
                    onPressed:root.currentIndexChanged("flowView",index)
                }

                FlowMenuButton {
                    id: dosageButton
                    legend: "Dosages"
                    icon:          "qrc:/icons/flow/dosages.png"
                    icon_disabled: "qrc:/icons/flow/dosages_disabled.png"
                    icon_hovered:  "qrc:/icons/flow/dosages_hover.png"
                    icon_selected: "qrc:/icons/flow/dosages.png"
                    status:    dosageTab.status
                    isEnabled: dosageTab.enabled
                    index: 2
                    currentIndex: root.index
                    onPressed:root.currentIndexChanged("flowView",index)
                }
                FlowMenuButton {
                    id: targetButton
                    legend: "Targets"
                    icon:          "qrc:/icons/flow/targets.png"
                    icon_disabled: "qrc:/icons/flow/targets_disabled.png"
                    icon_hovered:  "qrc:/icons/flow/targets_hover.png"
                    icon_selected: "qrc:/icons/flow/targets.png"
                    status:    targetTab.status
                    isEnabled: targetTab.enabled
                    index: 3
                    currentIndex: root.index
                    onPressed: root.currentIndexChanged("flowView",index)
                }
                FlowMenuButton {
                    id: conversionButton
                    legend: "Conversions"
                    icon:          "qrc:/icons/flow/measures.png"
                    icon_disabled: "qrc:/icons/flow/measures_disabled.png"
                    icon_hovered:  "qrc:/icons/flow/measures_hover.png"
                    icon_selected: "qrc:/icons/flow/measures.png"
                    status:    conversionTab.status
                    isEnabled: conversionTab.enabled
                    index: 4
                    currentIndex: root.index
                    onPressed:root.currentIndexChanged("flowView",index)
                }

                FlowMenuButton {
                    id: parameterButton
                    legend: "Parameters"
                    icon:          "qrc:/icons/flow/adjustments.png"
                    icon_disabled: "qrc:/icons/flow/adjustments_disabled.png"
                    icon_hovered:  "qrc:/icons/flow/adjustments_hover.png"
                    icon_selected: "qrc:/icons/flow/adjustments.png"
                    status:    parameterTab.status
                    isEnabled: parameterTab.enabled
                    index: 5
                    currentIndex: root.index
                    onPressed:  root.currentIndexChanged("flowView",index)
                }
                FlowMenuButton {
                    id: covariateButton
                    legend: "Covariates"
                    icon:          "qrc:/icons/flow/covariates.png"
                    icon_disabled: "qrc:/icons/flow/covariates_disabled.png"
                    icon_hovered:  "qrc:/icons/flow/covariates_hover.png"
                    icon_selected: "qrc:/icons/flow/covariates.png"
                    status:    covariateTab.status
                    isEnabled: covariateTab.enabled
                    index: 6
                    currentIndex: root.index
                    onPressed: root.currentIndexChanged("flowView",index)
                    isLast:true
                }

            }

            Item {
                id: content
                Layout.fillWidth:  true;
                Layout.fillHeight: true;

                Layout.preferredWidth:  mainWindow.contentPreferredWidth
                Layout.preferredHeight: mainWindow.contentPreferredHeight

                Layout.minimumWidth:  mainWindow.contentMinimumWidth
                Layout.minimumHeight: mainWindow.contentMinimumHeight - menu.height




                DrugTab {
                    id: drugTab
                    objectName:   "drugsView"
                    anchors.fill: parent
                    visible: drugButton.isCurrent
                }

                EditorTab {
                    id: editorTab
                    objectName:   "editorView"
                    anchors.fill: parent

                    visible: editorButton.isCurrent
                    enabled: drugTab.status !== Status.invalid
                }

                DosageTab {
                    id: dosageTab
                    objectName:   "doseView"
                    anchors.fill: parent
                    visible: dosageButton.isCurrent
                    enabled: drugTab.status !== Status.invalid
                }
                TargetTab {
                    id: targetTab
                    objectName:   "targetsView"
                    anchors.fill: parent
                    visible: targetButton.isCurrent
                    enabled: drugTab.status !== Status.invalid
                }

                ConversionTab {
                    id: conversionTab
                    objectName: "conversionView"
                    anchors.fill: parent
                    visible: conversionButton.isCurrent
                    enabled: drugTab.status !== Status.invalid
                }

                ParameterTab {
                    id: parameterTab
                    objectName: "parameterView"
                    anchors.fill: parent
                    visible: parameterButton.isCurrent
                    enabled: drugTab.status !== Status.invalid
                }

                CovariateTab {
                    id: covariateTab
                    objectName:   "covariateView"
                    anchors.fill: parent
                    visible: covariateButton.isCurrent
                    enabled: drugTab.status !== Status.invalid
                }
            }
          //  DemoBanner {
          //      Layout.fillWidth: true
          //      Layout.preferredHeight: 128
          //  }

        }
    }
}
