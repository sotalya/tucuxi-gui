import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

import Qt.labs.platform

Rectangle {
    id: adjustmentPanel
    property int status: Status.invalid;
    property bool enabled: false;

    property bool withLoadingDoseYN: true;

    signal imUpdated();
    signal currentIndexChanged(int index)
    signal settingsValidated(date adate)
    signal dosageValidated()
    signal reset()

    property var routes: interpretation.drugResponseAnalysis.drugModel? interpretation.drugResponseAnalysis.drugModel.intakes : 0


    property var routeobjs: routes? routes.objlist : 0


    property var routesName : []

    onRouteobjsChanged: {
        if (!routeobjs) {return;}
        for (var i = 0; i < routeobjs.length; ++i) {
            //                    console.log("route at (" + i + "): " + routeobjs[i].label);
            routesName[i] = routeobjs[i].label;
        }
    }

    function extAdjDate(yearAdj, monthAdj, dayAdj, hourAdj, minuteAdj) {
//        adjustmentTabController.adjustmentDate = (new Date("2027","08","09","00","00"));
        adjustmentTabController.adjustmentDate = (new Date(yearAdj, monthAdj, dayAdj, hourAdj, minuteAdj))
    }

    function extLoadingDose(input) {            // not able to edit Loading dose or Rest time. To be continued...

//        withLoadingDose.checked = true
//        withRestPeriod.checked = true
//        adjustmentTabController.adjustmentWithLoadingDose = true
//        adjustmentTabController.adjustmentWithRestPeriod = withLoadingDoseYN
    }

    anchors.fill: parent
    color: Style.table.background.table

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        //AdjustmentSettings
        Rectangle {
            id: adjustmentSettings
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            Layout.minimumHeight:   50

            property int rowHeight: 25

            property string frameBackground: Style.form.background.frame
            property string frameColor:      Style.form.border.color
            property int frameWidth:   Style.form.border.width
            property int frameRadius:  Style.form.border.radius
            property int framePadding: 5

            property string labelForeground: Style.form.foreground.label
            property string labelFontFamily: Style.form.font.label
            property string labelFontSize:   Style.form.size.label
            property bool labelFontBold: false
            property int labelVAlign: Text.AlignVCenter
            property int labelHAlign: Text.AlignRight

            property string inputFontFamily: Style.form.font.input
            property string inputFontSize:   Style.form.size.input
            property int inputVAlign: Text.AlignVCenter
            property int inputHAlign: Text.AlignLeft

            //AdjustmentSettings view
            ListView {
                id: asv
                anchors.fill: parent
                anchors.margins: 0
                highlightFollowsCurrentItem: true
                snapMode: ListView.SnapToItem
                clip: true
                interactive: false

                model: 1//adjustmentSettingsModel
                //            property var date: interpretationController.adjustmentDate
                delegate: adjustmentSettingsDelegate
            }

            //AdjustmentSettings delegate
            Component {
                id: adjustmentSettingsDelegate

                Item {
                    width:  ListView.view.width
                    height: ListView.view.height


                    Rectangle {
                        id: adjustmentSettingsFrame
                        anchors.fill: parent
                        anchors.margins: 0
                        color:  adjustmentSettings.frameBackground
                        radius: adjustmentSettings.frameRadius
                        border.width: adjustmentSettings.frameWidth
                        border.color: adjustmentSettings.frameColor

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: adjustmentSettings.framePadding
                            spacing: 5

                            RowLayout {
                                Layout.preferredHeight: adjustmentSettings.rowHeight / 2
                                Layout.minimumHeight:   adjustmentSettings.rowHeight / 2
                                spacing: 2


                                EntityLabel {
                                    text: "Adjust at:"
                                    Layout.preferredWidth: 75
                                    tooltipText : ToolTips.adjustmentTab.adjustmentTime
                                }
                                /*
                                Text {
                                    id: atSettingLabel
                                    Layout.preferredWidth: 75
                                    text: "Adjust at:"
                                    elide: Text.ElideRight
                                    color: adjustmentSettings.labelForeground
                                    font.family:    adjustmentSettings.labelFontFamily
                                    font.pixelSize: adjustmentSettings.labelFontSize
                                    font.bold: adjustmentSettings.labelFontBold
                                    verticalAlignment:   adjustmentSettings.labelVAlign
                                    horizontalAlignment: adjustmentSettings.labelHAlign

                                }
                                */
                                DatePicker {
                                    id: atDateSettingInput
                                    objectName: "atDateSettingInput"
                                    Layout.preferredWidth: 150
                                    date: adjustmentTabController.adjustmentDate
                                    onEditingFinished: {
                                        adjustmentTabController.adjustmentDate = (new Date(atDateSettingInput.date.getFullYear(), atDateSettingInput.date.getMonth(), atDateSettingInput.date.getDate(), atMinutesSettingInput.date.getHours(), atMinutesSettingInput.date.getMinutes()));
                                    }
                                }
                                TimePicker {
                                    id: atMinutesSettingInput
                                    objectName: "atMinutesSettingInput"
                                    Layout.fillWidth: true
                                    date: adjustmentTabController.adjustmentDate
                                    onEditingFinished: {
                                        adjustmentTabController.adjustmentDate = (new Date(atDateSettingInput.date.getFullYear(), atDateSettingInput.date.getMonth(), atDateSettingInput.date.getDate(), atMinutesSettingInput.date.getHours(), atMinutesSettingInput.date.getMinutes()));
                                    }
                                }
/*
                                Rectangle {
                                    Layout.preferredWidth:  adjustmentSettings.rowHeight
                                    Layout.preferredHeight: adjustmentSettings.rowHeight
                                    color: adjustmentSettings.frameBackground

                                    Image {
                                        anchors.fill: parent
                                        anchors.margins: -5
                                        source: "qrc:/icons/buttons/check_disabled.png"
                                        fillMode: Image.PreserveAspectFit
                                        antialiasing: true
                                        mipmap: true

                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: {
                                                adjustmentTabController.validateDate();
                                                //                            adjustmentList.show();
                                                //                            manualAdjustmentList.show();
                                                //                            settingsValidated(asv.date);
                                            }
                                        }
                                    }
                                }
*/
                            }

                            RowLayout {} //Spacer

                            Button {
                                id: launchCdssButton
                                Layout.alignment: Qt.AlignTop
                                text: "Launch Report Generation"
                                implicitWidth: 200
                                onClicked: interpretationController.launchCdss()
                                visible: appGlobals.cdssOnly
                            }

                            RowLayout {
                                Layout.preferredHeight: adjustmentSettings.rowHeight / 2
                                Layout.minimumHeight:   adjustmentSettings.rowHeight / 2
                                spacing: 2
                                visible: !appGlobals.cdssOnly

                                CheckBox {
                                    id: withLoadingDose
                                    objectName: "withLoadingDose"

                                    text: "Loading dose?"
                                    checked: adjustmentTabController.adjustmentWithLoadingDose
                                    onCheckStateChanged: adjustmentTabController.adjustmentWithLoadingDose = checked

                                    ToolTip.visible: (show_tooltip) ? withLoadingDoseMousearea.containsMouse : false
                                    ToolTip.text: ToolTips.adjustmentTab.withLoadingDose

                                    MouseArea {
                                        id: withLoadingDoseMousearea
                                        objectName: "withLoadingDoseMousearea"
                                        anchors.top: parent.top
                                        anchors.bottom: parent.bottom
                                        anchors.right: parent.right
                                        width: parent.width - 30
                                        hoverEnabled: true

                                    }
                                }


                                CheckBox {
                                    id: withRestPeriod
                                    objectName: "withRestPeriod"
                                    text: "Rest period?"
                                    checked: adjustmentTabController.adjustmentWithRestPeriod
                                    onCheckStateChanged: adjustmentTabController.adjustmentWithRestPeriod = checked

                                    ToolTip.visible: (show_tooltip) ? withRestPeriodMousearea.containsMouse : false
                                    ToolTip.text: ToolTips.adjustmentTab.withRestPeriod

                                    MouseArea {
                                        id: withRestPeriodMousearea
                                        anchors.top: parent.top
                                        anchors.bottom: parent.bottom
                                        anchors.right: parent.right
                                        width: parent.width - 30
                                        hoverEnabled: true
                                    }
                                }


                            }
                        }
                    }
                }
            }

        }


        /*******************************************************************************
        Suggested adjustments list
        *******************************************************************************/

        EntityTitle {
            label.text: "Suggested Adjustments"
            id: suggestedExpandHeader
            expanded: true
            tooltipText : ToolTips.adjustmentTab.suggestedListTitle
            visible: !appGlobals.cdssOnly
        }

        //AdjustmentList
        EntityList {
            id: adjustmentList

            visible : suggestedExpandHeader.expanded && !appGlobals.cdssOnly

            Layout.preferredHeight: adjustmentList.headerHeight + 10 + (adjustmentList.rowHeight + adjustmentList.spacing) * adjustmentListView.count

            columnlayout.children: [

                EntityListHeader {
                    id: adjustmentListHeaders
                    EntityListHeaderItem { Layout.preferredWidth: 115; label.text: "From"; tooltipText : ToolTips.adjustmentTab.from }
                    EntityListHeaderItem { Layout.preferredWidth: 115; Layout.fillWidth: true; label.text: "To"; tooltipText : ToolTips.adjustmentTab.to }
                    EntityListHeaderItem { Layout.preferredWidth: 85; label.text: "Dose"; tooltipText : ToolTips.adjustmentTab.dose }
                    EntityListHeaderItem { Layout.preferredWidth: 85; label.text: "Interval"; tooltipText : ToolTips.adjustmentTab.interval }
                    EntityListHeaderItem { Layout.preferredWidth: 85; label.text: "Infusion"; tooltipText : ToolTips.adjustmentTab.infusion }
                    /*
                    EntityListHeaderItem { Layout.preferredWidth: 85; label.text: "Value" }
                    EntityListHeaderItem { Layout.preferredWidth: 85; label.text: "Score" }
                    */
                    EntityListHeaderItem { Layout.preferredWidth: 30; label.text: "Info"; tooltipText : ToolTips.adjustmentTab.info }
                    EntityListHeaderItem { Layout.preferredWidth: 30 }
                },

                EntityHeaderEnd {
                    visible : true
                },

                EntityListView {
                    id: adjustmentListView
                    objectName : "adjustmentListView"
                    //Layout.minimumHeight: adjustmentList.rowHeight * 5
                    property var adjustments: interpretation.analysis.chartData.revPred.adjustments;
                    function nbProposedAdjustments() {
                        return adjustments ? adjustments.size() : 0;
                    }

                    onAdjustmentsChanged: {
                        //                console.log("# adjustments: " + adjustments.size());
                    }
                    model: adjustments ? adjustments.objlist : 0;
                    onModelChanged: {
                        //                console.log("# adjustments: " + adjustments.size());
                    }

                    delegate: EntityListDelegate {
                        id: adjustmentListDelegate
                        property var dosage: modelData.dosageHistory.last();
                        property var targetEvaluation : modelData.targetEvaluationResults;
                        mousearea.onClicked: {
                            adjustmentListView.currentIndex = index
                            currentIndexChanged(index)
                        }
                        rowlayout.children: [
                            EntityListDelegateItem {
                                Layout.preferredWidth: 115
                                color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowBackgroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)
                                label.text: Qt.formatDateTime(dosage.applied, "dd/MM/yyyy hh:mm")
                                label.color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowForegroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)

                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth: 115
                                Layout.fillWidth: true
                                color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowBackgroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)
                                label.text: Qt.formatDateTime(dosage.endtime, "dd/MM/yyyy hh:mm")
                                label.color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowForegroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth:85
                                color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowBackgroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)
                                label.text: dosage.quantity.dbvalue + " " + dosage.quantity.unitstring
                                label.color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowForegroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth:85
                                color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowBackgroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)
                                label.text: dosage.dbinterval + " hours"
                                label.color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowForegroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth:85
                                color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowBackgroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)
                                label.text: dosage.route.hasInfusion ? dosage.dbtinf + " min" : "-"
                                label.color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowForegroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                            },/*
                            EntityListDelegateItem {
                                Layout.preferredWidth:85
                                color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowBackgroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)
                                label.text: targetEvaluation.value
                                // label.text: Number.toLocaleString(targetEvaluation.value, 'f', 2)
                                label.color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowForegroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth:85
                                color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowBackgroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)
                                label.text: targetEvaluation.score
                               // label.text: Number.toLocaleString(targetEvaluation.score, 'f', 2)
                                label.color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowForegroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                            },*/
                            EntityListDelegateItem {
                                Layout.preferredWidth:30
                                color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowBackgroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)
                                label.text: "i"
                                label.color: adjustmentListDelegate.mousearea.hovered ? adjustmentList.rowForegroundHover : (adjustmentListDelegate.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)

                                MouseArea {
                                    id: mousearea
                                    anchors.fill: parent
                                    onClicked: {
                                        var message = "The target details are the following:\n\n";
                                        for(var i = 0; i < targetEvaluation.size(); i++) {
                                            // One option is to let the C++ generate the text,
                                            // However it seems better to let QML handle this.
                                            // Not very nice though, we have to find a way to present it better
                                            //message += targetEvaluation.at(i).toHtml();
                                            //message += "\n";
                                            message += "Target type : " + targetEvaluation.at(i).targetType.label + "\n";
                                            message += "Value : " + (targetEvaluation.at(i).value).toFixed(2) + " " + targetEvaluation.at(i).unit + "\n";
                                            message += "Score : " + (targetEvaluation.at(i).score).toFixed(2) + "\n";

                                        }

                                        messageDialog.text = message;
                                        messageDialog.open();
                                    }
                                }
                            },
                            EntityListHeaderImage {
                                objectName: "selectAdjustment_" + index
                                Layout.preferredWidth:30
                                image.source: "qrc:/icons/buttons/check.png"
                                mousearea.onClicked: {
                                    adjustmentTabController.selectAdjustment(index);
                                }
                                tooltipText: ToolTips.adjustmentTab.select
                            }
                        ]

                    }

                    MessageDialog {
                         id: messageDialog
                         title: "Target details"
                         text: ""
                     }

                    onCountChanged: currentIndex = count > 0 ? 0 : -1
                    Component.onCompleted: {
                        reset.connect(clearSelection);
                        settingsValidated.connect(clearSelection);
                    }

                    function clearSelection() {
                        currentIndex = -1;
                    }
                }
            ]
        }


        EntityBlockEnd {
            visible : suggestedExpandHeader.expanded && !appGlobals.cdssOnly
        }

        /*******************************************************************************
        Adjustments list
        *******************************************************************************/
        EntityTitle {
            label.text: "Adjustment"
            id: manualExpandHeader
            expanded: true
            tooltipText: ToolTips.adjustmentTab.adjustmentListTitle
            visible: !appGlobals.cdssOnly
        }

        EntityList {
            id: manualAdjustmentList

            visible: manualExpandHeader.expanded && !appGlobals.cdssOnly

            columnlayout.children: [

                //DosageList header
                RowLayout {
                    id: manualAdjustmentListHeaders
                    spacing: 2
                    EntityListHeaderItem { Layout.preferredWidth: 80; label.text: "Type"; tooltipText: ToolTips.adjustmentTab.type }
                    EntityListHeaderItem { Layout.preferredWidth: 115; label.text: "From"; tooltipText: ToolTips.adjustmentTab.from }
                    EntityListHeaderItem { Layout.preferredWidth: 115; Layout.fillWidth: true; label.text: "To"; tooltipText: ToolTips.adjustmentTab.to }
                    EntityListHeaderItem { Layout.preferredWidth: 60; label.text: "Dose"; tooltipText: ToolTips.adjustmentTab.dose }
                    EntityListHeaderItem { Layout.preferredWidth: 80; label.text: "Interval"; tooltipText: ToolTips.adjustmentTab.interval }
                    EntityListHeaderItem { Layout.preferredWidth: 80; label.text: "Duration"; tooltipText: ToolTips.adjustmentTab.infusion }
                    EntityListHeaderImage { }
                    EntityListHeaderImage {
                        objectName: "addAdjustment"
                        image.source: !adjustmentTabController.isManualAdjustmentDefined ? "qrc:/icons/buttons/add.png" : ""
                        mousearea.onClicked: {
                            adjustmentTabController.addAdjustment();
                            manualAdjustmentListView.currentIndex = manualAdjustmentListView.count-1;
                            }
                        tooltipText: ToolTips.adjustmentTab.add
                    }
                },

                EntityHeaderEnd {
                    visible : true
                },

                EntityListView {
                    id: manualAdjustmentListView
                    property var routeobjs: routes? routes.objlist : 0

                    property var dosages: adjustmentTabController.adjustments;
                    onDosagesChanged: {
                        //                console.log("# adj dosages: " + dosages.size());
                    }
                    model: dosages ? dosages.objlist : 0;
                    onModelChanged: {
                        //                console.log("# adj dosages: " + dosages.size());
                    }
                    delegate: EntityListDelegate {
                        id: manualAdjustmentListDelegate
                        mousearea.onClicked: {
                            manualAdjustmentListView.currentIndex = index
                        }
                        rowlayout.children: [
                            EntityListDelegateItem {
                                Layout.preferredWidth: 80
                                color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowBackgroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowBackgroundSelected : manualAdjustmentList.rowBackground)
                                label.text: modelData.type
                                label.color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowForegroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowForegroundSelected : manualAdjustmentList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth: 115
                                color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowBackgroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowBackgroundSelected : manualAdjustmentList.rowBackground)
                                label.text: Qt.formatDateTime(modelData.applied, "dd/MM/yyyy hh:mm")
                                label.color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowForegroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowForegroundSelected : manualAdjustmentList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth: 115;
                                Layout.fillWidth: true
                                color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowBackgroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowBackgroundSelected : manualAdjustmentList.rowBackground)
                                label.text: Qt.formatDateTime(modelData.endtime, "dd/MM/yyyy hh:mm")
                                label.color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowForegroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowForegroundSelected : manualAdjustmentList.rowForeground)
                            },
                            EntityListDelegateItem {
                                color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowBackgroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowBackgroundSelected : manualAdjustmentList.rowBackground)
                                label.text: modelData.quantity.dbvalue + " " + modelData.quantity.unitstring
                                Layout.preferredWidth: 60
                                label.color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowForegroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowForegroundSelected : manualAdjustmentList.rowForeground)
                            },
                            EntityListDelegateItem {
                                color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowBackgroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowBackgroundSelected : manualAdjustmentList.rowBackground)
                                label.text: modelData.dbinterval + " hours"
                                Layout.preferredWidth: 80
                                label.color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowForegroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowForegroundSelected : manualAdjustmentList.rowForeground)
                            },
                            EntityListDelegateItem {
                                color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowBackgroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowBackgroundSelected : manualAdjustmentList.rowBackground)
                                label.text: modelData.route.label === "INFUSION" ? modelData.dbtinf + " min" : "-"
                                Layout.preferredWidth: 80
                                label.color: manualAdjustmentListDelegate.mousearea.hovered ? manualAdjustmentList.rowForegroundHover : (manualAdjustmentListDelegate.ListView.isCurrentItem ? manualAdjustmentList.rowForegroundSelected : manualAdjustmentList.rowForeground)
                            },
                            EntityListImage {
                                objectName: "editAdjustment_" + index;
                                image.source: "qrc:/icons/buttons/edit.png"
                                mousearea.onClicked: {
                                    manualAdjustmentListView.currentIndex = index
                                    var model = modelData
                                    if (model) {
                                        adjustmentDialog.init(model.quantity,
                                            model.dbinterval,
                                            model.route,
                                            model.dbtinf,
                                            model.applied,
                                            model.endtime,
                                            routesName);
                                        adjustmentDialog.open(false);
                                    }
                                }
                                tooltipText: ToolTips.adjustmentTab.edit
                            },
                            EntityListImage {
                                objectName: "removeAdjustment_" + index;
                                image.source: "qrc:/icons/buttons/remove.png"
                                mousearea.onClicked: {
                                    adjustmentTabController.removeAdjustment(index);
                                }
                                tooltipText: ToolTips.adjustmentTab.remove
                            }
                        ]
                    }
                }
            ]
        }
    }


    AdjustmentDialog {
        id: adjustmentDialog

         function applyChanges()
         {
            var index = manualAdjustmentListView.currentIndex
            adjustmentTabController.setDbValue(index, this.getQuantity())
            adjustmentTabController.setDbInterval(index, this.getInterval())
            adjustmentTabController.setDbTinf(index, this.getInfusion())
            //adjustmentTabController.setRouteValue(index, this.getRoute())
            adjustmentTabController.setAppliedTime(index, this.getAppliedDate())
            adjustmentTabController.setEndTime(index, this.getEndDate())
            adjustmentTabController.forceRefresh()
            return true;
        }
    }
}


