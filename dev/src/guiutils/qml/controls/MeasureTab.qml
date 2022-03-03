import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {
    id: measurePanel
    property int index:  -1;
    property int status: Status.invalid;
    property bool enabled: false;

    signal currentIndexChanged(int index)
    signal reset()

    onIndexChanged:  currentIndexChanged(index)

    anchors.fill: parent
    color: Style.table.background.table

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        EntityTitle {
            label.text: "Measures List"
            id: measuresListExpandHeader
            tooltipText: ToolTips.measureTab.listTitle
        }

        //MeasureList
        EntityList {
            id: measureList

            visible : measuresListExpandHeader.expanded

            columnlayout.children: [
                //MeasureList header
                EntityListHeader {
                    EntityListHeaderItem {
                        Layout.preferredWidth: 50
                        label.text: "ID"
                        tooltipText : ToolTips.measureTab.id
                    }
                    EntityListHeaderItem {
                        Layout.preferredWidth: 80
                        label.text: "Value"
                        tooltipText : ToolTips.measureTab.value
                    }
                    EntityListHeaderItem {
                        Layout.preferredWidth: 115
                        label.text: "Sample date"
                        tooltipText : ToolTips.measureTab.sampleDate
                    }
                    EntityListHeaderItem {
                        Layout.fillWidth: true
                        label.text: "Arrival date"
                        tooltipText : ToolTips.measureTab.arrivalDate
                    }
                    EntityListHeaderImage {
                        image.source: "qrc:/icons/buttons/add.png"
                        tooltipText : ToolTips.measureTab.add
                        objectName: "addMeasure"
                        mousearea.onClicked: {
                            measureTabController.addMeasure();
                            measureListView.currentIndex = measureListView.count-1;
                            var model = measureTabController.measures.objlist[measureListView.currentIndex]
                            measureDialog.exec(model, true)
                        }
                    }
                },

                EntityHeaderEnd {
                    visible: true
                },

                //MeasureList view
                EntityListView {
                    id: measureListView
                    Layout.fillWidth:  true
                    property var measures: measureTabController.measures;
                    onMeasuresChanged: {
                        //                console.log("# measures: " + measures.size());
                    }
                    model: measures ? measures.objlist : 0 //measureListModel
                    onModelChanged: {
                        //                console.log("# measures: " + measures.size());
                        for (var i = 0; i < measures.size(); ++i) {
                            //                    console.log("measure " + i + ": " + measures.at(i).concentration.dbvalue);
                            //                    console.log("measure " + i + ": " + measures.at(i).concentration.unitstring);
                            //                    console.log("measure " + i + ": " + measures.at(i).sampleID);
                            //                    console.log("measure " + i + ": " + measures.at(i).moment);
                            //                    console.log("measure " + i + ": " + measures.at(i).arrivalDate);
                        }
                    }
                    //MeasureList delegate
                    delegate: EntityListDelegate {
                        id: root
                        property var selected_model: model
                        mousearea.onClicked: {
                            root.ListView.view.currentIndex = index;
                            currentIndexChanged(index);
                        }
                        rowlayout.children: [

                            EntityListDelegateItem {
                                Layout.preferredWidth: 50
                                color: root.mousearea.hovered ? measureList.rowBackgroundHover : (root.ListView.isCurrentItem ? measureList.rowBackgroundSelected : measureList.rowBackground)
                                label.text: modelData.sampleID
                                label.color: root.mousearea.hovered ? measureList.rowForegroundHover : (root.ListView.isCurrentItem ? measureList.rowForegroundSelected : measureList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth: 80
                                color: root.mousearea.hovered ? measureList.rowBackgroundHover : (root.ListView.isCurrentItem ? measureList.rowBackgroundSelected : measureList.rowBackground)
                                label.text: modelData.concentration.dbvalue + " " + modelData.concentration.unitstring
                                label.color: root.mousearea.hovered ? measureList.rowForegroundHover : (root.ListView.isCurrentItem ? measureList.rowForegroundSelected : measureList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth: 115
                                color: root.mousearea.hovered ? measureList.rowBackgroundHover : (root.ListView.isCurrentItem ? measureList.rowBackgroundSelected : measureList.rowBackground)
                                label.text: Qt.formatDateTime(modelData.moment)
                                label.color: root.mousearea.hovered ? measureList.rowForegroundHover : (root.ListView.isCurrentItem ? measureList.rowForegroundSelected : measureList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.fillWidth: true
                                color: root.mousearea.hovered ? measureList.rowBackgroundHover : (root.ListView.isCurrentItem ? measureList.rowBackgroundSelected : measureList.rowBackground)
                                label.text: Qt.formatDateTime(modelData.arrivalDate)
                                label.color: root.mousearea.hovered ? measureList.rowForegroundHover : (root.ListView.isCurrentItem ? measureList.rowForegroundSelected : measureList.rowForeground)
                            },
                            EntityListImage {
                                objectName: "editMeasure_" + index;                       // JRT 23.02.2022
                                image.source: "qrc:/icons/buttons/edit.png"
                                mousearea.onClicked: {
                                    measureListView.currentIndex = index;
                                    measureDialog.exec(modelData, false)
                                }
                                tooltipText : ToolTips.measureTab.modify
                            },
                            EntityListImage {
                                objectName: "removeMeasure_" + index;
                                image.source: "qrc:/icons/buttons/remove.png"
                                mousearea.onClicked: {
                                    measureTabController.removeMeasure(index);
                                }
                                tooltipText : ToolTips.measureTab.remove
                            }
                        ]
                    }

                    onCurrentIndexChanged: {
                        index = currentIndex;
                    }

                    Component.onCompleted: reset.connect(clearSelection)

                    function clearSelection() {
                        currentIndex = -1;
                    }
                },

                UncastedListView {

                    Layout.preferredHeight: 150
                    Layout.minimumHeight:   150

                    controller : measureTabController

                    values : measureListView.currentItem ?
                             ( measureListView.currentItem.selected_model ?
                                ( measureListView.currentItem.selected_model.modelData.uncastedValues.size() > 0 ?
                                    measureListView.currentItem.selected_model.modelData.uncastedValues : 0
                                ) : 0
                             ) : 0
                }
            ]
        }

/*
        EntityBlockEnd {
            visible : measuresListExpandHeader.expanded
        }

        EntityTitle {
            label.text: "Measure details"
            id: measureExpandHeader
            visible: measureListView.count > 0
        }

        //Measure
        EntityView {
            id: measure
            visible: (measureListView.count > 0) && measureExpandHeader.expanded;
            Layout.preferredHeight: 200 // 100
            Layout.minimumHeight:   200 // 100
            property var model: (measureListView.currentItem? (measureListView.currentItem.selected_model? measureListView.currentItem.selected_model.modelData : 0) : 0)
            columnlayout.children: [
                RowLayout {
                    Layout.preferredHeight: measure.rowHeight
                    Layout.minimumHeight:   measure.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "Sample ID:"
                    }
                    EntityTextField {
                        placeholderText: "sample id"
                        text: measure.model? measure.model.sampleID : ""
                        onEditingFinished: {
                            measureTabController.setSampleId(index, text)
                        }
                    }
                    EntityLabel {
                        text: "Value:"
                        Layout.preferredWidth: 100
                    }
                    EntitySpinBox {
                        id: valueInput
                        Layout.fillWidth:  true
                        suffix: measure.model? " " + measure.model.concentration.unitstring : ""
                        value: measure.model? measure.model.concentration.dbvalue : -1
                        onEditingFinished: {
                            measureTabController.setDbValue(index, value);
                        }
                    }
                },

                RowLayout {
                    Layout.preferredHeight: measure.rowHeight
                    Layout.minimumHeight:   measure.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "Measured:"
                    }
                    DatePicker {
                        id: sampleDateInput
                        Layout.preferredWidth: 150
                        Binding { target: sampleDateInput; property: "date"; value: measure.model ? measure.model.moment : new Date()}
                        onEditingFinished: {
                            measureTabController.setMoment(index, new Date(date.getFullYear(), date.getMonth(), date.getDate(), sampleTimeInput.hours, sampleTimeInput.minutes))
                        }
                    }
                    TimePicker {
                        id: sampleTimeInput
                        Layout.fillWidth: true
                        Binding { target: sampleTimeInput; property: "hours";   value: measure.model? measure.model.moment.getHours() : new Date() }
                        Binding { target: sampleTimeInput; property: "minutes"; value: measure.model? measure.model.moment.getMinutes() : new Date() }
                        onEditingFinished: {
                            measureTabController.setMoment(index, new Date(sampleDateInput.date.getFullYear(), sampleDateInput.date.getMonth(), sampleDateInput.date.getDate(), hours, minutes))
                        }
                    }
                },

                RowLayout {
                    Layout.preferredHeight: measure.rowHeight
                    Layout.minimumHeight:   measure.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "Received:"
                    }
                    DatePicker {
                        id: arrivalDateInput
                        Layout.preferredWidth: 150
                        Binding { target: arrivalDateInput; property: "date"; value: measure.model? measure.model.arrivalDate : new Date()}
                        onEditingFinished: {
                            measureTabController.setArrivalDate(index, new Date(date.getFullYear(), date.getMonth(), date.getDate(), arrivalTimeInput.hours, arrivalTimeInput.minutes))
                        }
                    }
                    TimePicker {
                        id: arrivalTimeInput
                        Layout.fillWidth: true
                        Binding { target: arrivalTimeInput; property: "hours";   value: measure.model? measure.model.arrivalDate.getHours() : new Date()}
                        Binding { target: arrivalTimeInput; property: "minutes"; value: measure.model? measure.model.arrivalDate.getMinutes() : new Date()}
                        onEditingFinished: {
                            measureTabController.setArrivalDate(index, new Date(arrivalDateInput.date.getFullYear(), arrivalDateInput.date.getMonth(), arrivalDateInput.date.getDate(), hours, minutes))
                        }
                    }
                },


                UncastedListView {

                    Layout.preferredHeight: 150
                    Layout.minimumHeight:   150

                    controller : measureTabController

                    values : measureListView.currentItem ?
                             ( measureListView.currentItem.selected_model ?
                                ( measureListView.currentItem.selected_model.modelData.uncastedValues.size() > 0 ?
                                    measureListView.currentItem.selected_model.modelData.uncastedValues : 0
                                ) : 0
                             ) : 0
                },

                RowLayout {} //Spacer
            ]

        }

        EntityBlockEnd {
            visible : measureExpandHeader.visible && measureExpandHeader.expanded
        }
*/
    }

    MeasureDialog {
        id: measureDialog
        objectName: "measureDialog"
        function exec(model, bCreatingNewItem)
        {
            if (model) {
                this.init(
                    model.sampleID,
                    model.concentration,
                    model.moment,
                    model.arrivalDate)
                this.open(bCreatingNewItem);
            }
        }

        function applyChanges()
        {
            var sampleDate = this.getSampleDate()
            var arrivalDate = this.getArrivalDate()
            if (!interpretationController.isFlowRequest()) {
              arrivalDate = sampleDate
            }
            measureTabController.setSampleId(measureListView.currentIndex, this.getSampleId())
            measureTabController.setDbValue(measureListView.currentIndex, this.getValue())
            measureTabController.setMoment(measureListView.currentIndex, sampleDate)
            measureTabController.setArrivalDate(measureListView.currentIndex, arrivalDate)
            measureTabController.forceRefresh()
            return true
        }

        function onUpdated(bApplied, bCreatingNewItem)
        {
            if (bCreatingNewItem && !bApplied) {
                measureTabController.removeMeasure(measureListView.currentIndex)
            }
        }
    }
}
