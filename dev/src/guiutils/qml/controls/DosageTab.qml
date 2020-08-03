import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {
    id: dosagePanel
    property int index:  -1
    property int status: Status.invalid;
    property bool enabled: false;

    property var routes: interpretation.drugResponseAnalysis.drugModel? interpretation.drugResponseAnalysis.drugModel.intakes : 0

    property var routeobjs: routes? routes.objlist : 0

    property var routesName : []

    onRouteobjsChanged: {
        routesName = [];
        if (!routeobjs) {return;}
        for (var i = 0; i < routeobjs.length; ++i) {
            //                    console.log("route at (" + i + "): " + routeobjs[i].label);
            routesName[i] = routeobjs[i].label;
        }
        //routeInput.model = routesName;
    }

    signal currentIndexChanged(int index)
    signal reset()

    onIndexChanged:  {
        currentIndexChanged(index)
    }

    anchors.fill: parent
    color: Style.table.background.table

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5


        EntityTitle {
            label.text: "Dosages History"
            id: dosagesListExpandHeader
            tooltipText: ToolTips.dosageTab.dosageHistoryTitle
        }

        //DosageList
        EntityList {
            id: dosageList

            visible : dosagesListExpandHeader.expanded

            columnlayout.children: [
                //DosageList header
                RowLayout {
                    spacing: 2

                    EntityListHeaderItem {
                        Layout.preferredWidth: 115;
                        label.text: "From"
                        tooltipText: ToolTips.dosageTab.from
                    }
                    EntityListHeaderItem {
                        Layout.fillWidth: true;
                        Layout.preferredWidth: 85;
                        label.text: "To"
                        tooltipText: ToolTips.dosageTab.to
                    }
                    EntityListHeaderItem {
                        label.text: "Dose"
                        tooltipText: ToolTips.dosageTab.dose
                    }
                    EntityListHeaderItem {
                        label.text: "Interval"
                        tooltipText: ToolTips.dosageTab.interval
                    }
                    EntityListHeaderItem {
                        label.text: "Infusion"
                        tooltipText: ToolTips.dosageTab.infusion
                    }

                    EntityListHeaderImage {
                        objectName: "addDosage"
                        image.source: dosageTabController.canHaveMoreDosages ? "qrc:/icons/buttons/add.png" : ""
                        mousearea.onClicked: {
                            dosageTabController.addDosage();
                            dosageListView.currentIndex = dosageListView.count-1;
                            var model = interpretation.drugResponseAnalysis.treatment.dosages.objlist[dosageListView.currentIndex]
                            var drugModel = interpretation.drugResponseAnalysis.drugModel
                            dosageDialog.exec(model, routesName, true, false, drugModel)
                        }

                        tooltipText: ToolTips.dosageTab.add
                    }
                },

                EntityHeaderEnd {
                    visible : true
                    objectName: "dosageTab"
                },

                //DosageList view
                EntityListView {
                    id: dosageListView

                    property var dosages: interpretation.drugResponseAnalysis.treatment.dosages;
                    model: dosages.objlist

                    delegate: EntityListDelegate {
                        id: dosageListDelegate
                        property var selected_model: model
                        mousearea.onClicked: {
                            dosageListDelegate.ListView.view.currentIndex = index
                            currentIndexChanged(index);
                        }
                        rowlayout.children: [
                            EntityListDelegateItem {
                                Layout.preferredWidth:115
                                color: dosageListDelegate.mousearea.hovered ? dosageList.rowBackgroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowBackgroundSelected : dosageList.rowBackground)
                                label.text: modelData.isAtSteadyState ? "steady state" : Qt.formatDateTime(modelData.applied)
                                label.color: dosageListDelegate.mousearea.hovered ? dosageList.rowForegroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowForegroundSelected : dosageList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.fillWidth: true
                                color: dosageListDelegate.mousearea.hovered ? dosageList.rowBackgroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowBackgroundSelected : dosageList.rowBackground)
                                label.text: modelData.isAtSteadyState ? "steady state" : Qt.formatDateTime(modelData.endtime)
                                label.color: dosageListDelegate.mousearea.hovered ? dosageList.rowForegroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowForegroundSelected : dosageList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth:85
                                color: dosageListDelegate.mousearea.hovered ? dosageList.rowBackgroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowBackgroundSelected : dosageList.rowBackground)
                                label.text: modelData.quantity.dbvalue + " " + modelData.quantity.unitstring
                                label.color: dosageListDelegate.mousearea.hovered ? dosageList.rowForegroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowForegroundSelected : dosageList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth:85
                                color: dosageListDelegate.mousearea.hovered ? dosageList.rowBackgroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowBackgroundSelected : dosageList.rowBackground)
                                label.text: modelData.dbinterval + " hours"
                                label.color: dosageListDelegate.mousearea.hovered ? dosageList.rowForegroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowForegroundSelected : dosageList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth:85
                                color: dosageListDelegate.mousearea.hovered ? dosageList.rowBackgroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowBackgroundSelected : dosageList.rowBackground)
                                label.text: modelData.route.label === "INFUSION" ? modelData.dbtinf + " min" : "-"
                                label.color: dosageListDelegate.mousearea.hovered ? dosageList.rowForegroundHover : (dosageListDelegate.ListView.isCurrentItem ? dosageList.rowForegroundSelected : dosageList.rowForeground)
                            },
                            EntityListImage {
                                image.source: "qrc:/icons/buttons/edit.png"
                                mousearea.onClicked: {
                                    dosageListView.currentIndex = index;
                                    dosageDialog.exec(modelData, routesName, false, index == 0)
                                }
                                tooltipText: ToolTips.dosageTab.edit
                            },
                            EntityListImage {
                                image.source: "qrc:/icons/buttons/remove.png"
                                mousearea.onClicked: {
                                    dosageTabController.removeDosage(index);
                                }
                                tooltipText: ToolTips.dosageTab.remove
                            }
                        ]

                    }

                    onCurrentIndexChanged: index = currentIndex
                    Component.onCompleted: reset.connect(clearSelection)

                    function clearSelection() {
                        currentIndex = -1;
                    }
                },

                UncastedListView {

                    Layout.preferredHeight: 300
                    Layout.minimumHeight:   150

                    values : dosageListView.currentItem?
                                 (dosageListView.currentItem.selected_model ?
                                      (dosageListView.currentItem.selected_model.modelData.uncastedValues.size() > 0 ?
                                           dosageListView.currentItem.selected_model.modelData.uncastedValues : 0
                                       ) : 0
                                  ) : 0
                }
            ]
        }

/*
        EntityBlockEnd {
            visible : (dosageListView.count > 0) && dosagesListExpandHeader.expanded
        }

        EntityTitle {
            label.text: "Dosage Information"
            id: dosageExpandHeader
            visible : dosageListView.count > 0
        }

        //Dosage view
        EntityView {
            id: dosage
            Layout.preferredHeight: 250
            property var model: dosageListView.currentItem ?
                                    (dosageListView.currentItem.selected_model ?
                                         dosageListView.currentItem.selected_model.modelData : 0)
                                  : 0

            visible: (dosageListView.count > 0) && dosageExpandHeader.expanded;

            columnlayout.children: [
                RowLayout {
                    Layout.preferredHeight: dosage.rowHeight
                    Layout.minimumHeight:   dosage.rowHeight
                    spacing: 2

                    EntityLabel { text: "Dose:" }
                    EntitySpinBox {
                        suffix: " " + (dosage.model ? dosage.model.quantity.unitstring : "")
                        value: dosage.model? dosage.model.quantity.dbvalue : -1
                        onEditingFinished: {
                            dosageTabController.setDbValue(index, value);
                        }
                    }
                    EntityLabel { text: "Interval:" }
                    EntitySpinBox {
                        suffix: " hours"
                        value: dosage.model? dosage.model.dbinterval : -1
                        onEditingFinished: {
                            dosageTabController.setDbInterval(index, value);
                        }
                    }
                },
                RowLayout {
                    Layout.preferredHeight: dosage.rowHeight
                    Layout.minimumHeight:   dosage.rowHeight
                    spacing: 2

                    EntityLabel { text: "Route:" }
                    EntityTextField {
                        text: dosage.model ? dosage.model.route.label : ""
                        readOnly: true
                    }
                    EntityComboBox {
                        id: routeInput
                        model: [] // Empty at the beginning. It is filled when the drug model is defined
                        visible: false

                        // *******************************************
                        // TODO : To check if this function is called at the right time.
                        // It works now but maybe not when multiple intakes are available.
                        // *******************************************
                        currentIndex: dosage.model ? dosageTabController.getRelativeRouteValue(index) : 0;
                        onCurrentIndexChanged: {
                            dosageTabController.setRouteValue(index, currentIndex);
                        }
                    }
                    EntityLabel { text: "Infusion:";
                        visible: dosage.model ? (dosage.model.route.label === "INFUSION") : false
                        Layout.preferredWidth: 100
                    }
                    EntitySpinBox {
                        id: infusionInput
                        visible: dosage.model ? (dosage.model.route.label === "INFUSION") : false
                        suffix: " minutes"
                        value: dosage.model? dosage.model.dbtinf : -1
                        onEditingFinished: {
                            dosageTabController.setDbTinf(index, value);
                        }
                    }
                },
                RowLayout {
                    Layout.preferredHeight: dosage.rowHeight
                    Layout.minimumHeight:   dosage.rowHeight
                    spacing: 2

                    EntityLabel { text: "From:" }
                    DatePicker {
                        id: appliedDateInput
                        Layout.preferredWidth: 150
                        Binding { target: appliedDateInput; property: "date"; value: dosage.model? dosage.model.applied : new Date()}
                        onEditingFinished: {
                            dosageTabController.setAppliedTime(index, new Date(date.getFullYear(), date.getMonth(), date.getDate(), appliedTimeInput.hours, appliedTimeInput.minutes));
                        }
                    }
                    TimePicker {
                        id: appliedTimeInput
                        Layout.fillWidth: true
                        Binding { target: appliedTimeInput; property: "hours";   value: dosage.model? dosage.model.applied.getHours() : new Date() }
                        Binding { target: appliedTimeInput; property: "minutes"; value: dosage.model? dosage.model.applied.getMinutes() : new Date() }
                        onEditingFinished: {
                            dosageTabController.setAppliedTime(index, new Date(appliedDateInput.date.getFullYear(), appliedDateInput.date.getMonth(), appliedDateInput.date.getDate(), hours, minutes));
                        }
                    }
                },
                RowLayout {
                    Layout.preferredHeight: dosage.rowHeight
                    Layout.minimumHeight:   dosage.rowHeight
                    spacing: 2

                    EntityLabel { text: "To:" }
                    DatePicker {
                        id: stoppedDateInput
                        Layout.preferredWidth: 150
                        Binding { target: stoppedDateInput; property: "date"; value: dosage.model? dosage.model.endtime : new Date()}
                        onEditingFinished: {
                            dosageTabController.setEndTime(index, new Date(date.getFullYear(), date.getMonth(), date.getDate(), stoppedTimeInput.hours, stoppedTimeInput.minutes));
                        }
                    }
                    TimePicker {
                        id: stoppedTimeInput
                        Layout.fillWidth: true
                        Binding { target: stoppedTimeInput; property: "hours";   value: dosage.model? dosage.model.endtime.getHours() : new Date() }
                        Binding { target: stoppedTimeInput; property: "minutes"; value: dosage.model? dosage.model.endtime.getMinutes() : new Date()}
                        onEditingFinished: {
                            dosageTabController.setEndTime(index, new Date(stoppedDateInput.date.getFullYear(), stoppedDateInput.date.getMonth(), stoppedDateInput.date.getDate(), hours, minutes));
                        }
                    }
                },

                UncastedListView {

                    Layout.preferredHeight: 150
                    Layout.minimumHeight:   150

                    values : dosageListView.currentItem?
                                 (dosageListView.currentItem.selected_model ?
                                      (dosageListView.currentItem.selected_model.modelData.uncastedValues.size() > 0 ?
                                           dosageListView.currentItem.selected_model.modelData.uncastedValues : 0
                                       ) : 0
                                  ) : 0
                },


                RowLayout {} //Spacer
            ]
        }

        EntityBlockEnd {
            visible : (dosageListView.count > 0) && dosageExpandHeader.expanded
        }
*/
    }

    DosageDialog {
        id: dosageDialog
        objectName: "dosageDialog"

        function exec(model, routesName, bCreatingNewItem, allowSteadyState, drugModel)
        {
            if (model) {
                var enableAtSteadyState = allowSteadyState || (bCreatingNewItem && (dosageTabController.getNbDosages() === 1));
                var disableAtSteadyState = !enableAtSteadyState; //((!allowSteadyState) && bCreatingNewItem && dosageTabController.getNbDosages() > 1);
                this.init(
                    model.quantity,
                    model.dbinterval,
                    model.route,
                    model.dbtinf,
                    ((!disableAtSteadyState && model.isAtSteadyState) ? model.steadyStateLastDoseDate : model.applied),
                    model.endtime,
                    model.hasEndDate,
                    model.isAtSteadyState,
                    routesName,
                    disableAtSteadyState,
                    interpretation.drugResponseAnalysis.drugModel.standardTreatment,
                    drugModel);
                this.open(bCreatingNewItem);
            }
        }

        function applyChanges()
        {
            var appliedDate = this.getAppliedDate()
            var endDate = this.getEndDate()
            if (dosageTabController.checkModifiedDates(dosageListView.currentIndex, appliedDate, endDate)) {
                dosageTabController.setDbValue(dosageListView.currentIndex, this.getQuantity());
                dosageTabController.setDbInterval(dosageListView.currentIndex, this.getInterval());
                dosageTabController.setDbTinf(dosageListView.currentIndex, this.getInfusion());
                //dosageTabController.setRouteValue(dosageListView.currentIndex, this.getRoute());
                dosageTabController.setAppliedTime(dosageListView.currentIndex, appliedDate);
                dosageTabController.setEndTime(dosageListView.currentIndex, endDate);
                dosageTabController.setHasEndDate(dosageListView.currentIndex, !this.isAtSteadyState() && this.hasEndDate());
                dosageTabController.setAtSteadyState(dosageListView.currentIndex, this.isAtSteadyState(), this.getAppliedDate());
                dosageTabController.forceRefresh()
                this.showOverlappingMessage(false);
                return true;
            }
            this.showOverlappingMessage(true);
            return false;
        }

        function onUpdated(bApplied, bCreatingNewItem)
        {
            if (bCreatingNewItem && !bApplied) {
                dosageTabController.removeDosage(dosageListView.currentIndex);
            }
        }
    }
}
