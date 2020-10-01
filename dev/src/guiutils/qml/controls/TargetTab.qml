import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {
    id: targetPanel
    property int index:  -1
    property int status: Status.invalid
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
            label.text: "Targets List"
            id: targetsListExpandHeader
            tooltipText: ToolTips.targetTab.listTitle
        }

        //TargetList
        EntityList {
            id: targetList

            visible : targetsListExpandHeader.expanded

            columnlayout.children: [

                EntityListHeader {
                    EntityListHeaderItem {
                        label.text: "Type"
                        Layout.preferredWidth: 80
                        tooltipText: ToolTips.targetTab.type
                    }
                    EntityListHeaderItem {
                        label.text: "Min"
                        Layout.preferredWidth: 90
                        tooltipText: ToolTips.targetTab.cMin
                    }
                    EntityListHeaderItem {
                        label.text: "Best"
                        Layout.preferredWidth: 90
                        tooltipText: ToolTips.targetTab.cBest
                    }
                    EntityListHeaderItem {
                        Layout.fillWidth: true
                        label.text: "Max"
                        tooltipText: ToolTips.targetTab.cMax
                    }
                    EntityListHeaderImage {
                        image.source: "qrc:/icons/buttons/add.png"
                        mousearea.onClicked: {
                            targetTabController.addTarget();
                            targetListView.currentIndex = targetListView.count-1;
                            var model = targetTabController.targets.objlist[targetListView.currentIndex]
                            targetDialog.exec(model, true);
                        }
                        tooltipText: ToolTips.targetTab.add
                    }
                },

                EntityHeaderEnd {
                    visible: true
                },

                //TargetList view
                EntityListView {
                    id: targetListView
                    property var targets: targetTabController.targets;
                    onTargetsChanged: {
                        //                console.log("# targets: " + targets.size());
                    }

                    model: targets.objlist
                    onModelChanged: {
                        //                console.log("# targets: " + targets.size());
                        //                for (var i = 0; i < targets.size(); ++i) {
                        //                    console.log("target " + i + ": " + targets.at(i).cmax.dbvalue);
                        //                    console.log("target " + i + ": " + targets.at(i).cmax.unitstring);
                        //                    console.log("target " + i + ": " + targets.at(i).cmin.dbvalue);
                        //                    console.log("target " + i + ": " + targets.at(i).cmin.unitstring);
                        //                    console.log("target " + i + ": " + targets.at(i).cbest.dbvalue);
                        //                    console.log("target " + i + ": " + targets.at(i).cbest.unitstring);
                        //                    console.log("target " + i + ": " + targets.at(i).tmax.dbvalue);
                        //                    console.log("target " + i + ": " + targets.at(i).tmax.unitstring);
                        //                    console.log("target " + i + ": " + targets.at(i).tmin.dbvalue);
                        //                    console.log("target " + i + ": " + targets.at(i).tmin.unitstring);
                        //                    console.log("target " + i + ": " + targets.at(i).tbest.dbvalue);
                        //                    console.log("target " + i + ": " + targets.at(i).tbest.unitstring);
                        //                }
                    }

                    delegate: EntityListDelegate {
                        id: root
                        property var selected_model: model? model : 0
                        mousearea.onClicked: {
                            root.ListView.view.currentIndex = index
                            currentIndexChanged(index);
                        }
                        rowlayout.children: [
                            EntityListDelegateItem {
                                Layout.preferredWidth: 80
                                color: root.mousearea.hovered ? targetList.rowBackgroundHover : (root.ListView.isCurrentItem ? targetList.rowBackgroundSelected : targetList.rowBackground)
                                label.text: modelData.type.label
                                label.color: root.mousearea.hovered ? targetList.rowForegroundHover : (root.ListView.isCurrentItem ? targetList.rowForegroundSelected : targetList.rowForeground)

                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth: 90
                                color: root.mousearea.hovered ? targetList.rowBackgroundHover : (root.ListView.isCurrentItem ? targetList.rowBackgroundSelected : targetList.rowBackground)
                                label.text: modelData.cmin.dbvalue + " " + modelData.cmin.unitstring
                                label.color: root.mousearea.hovered ? targetList.rowForegroundHover : (root.ListView.isCurrentItem ? targetList.rowForegroundSelected : targetList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth: 90
                                color: root.mousearea.hovered ? targetList.rowBackgroundHover : (root.ListView.isCurrentItem ? targetList.rowBackgroundSelected : targetList.rowBackground)
                                label.text: modelData.cbest.dbvalue + " " + modelData.cbest.unitstring
                                label.color: root.mousearea.hovered ? targetList.rowForegroundHover : (root.ListView.isCurrentItem ? targetList.rowForegroundSelected : targetList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.fillWidth: true
                                color: root.mousearea.hovered ? targetList.rowBackgroundHover : (root.ListView.isCurrentItem ? targetList.rowBackgroundSelected : targetList.rowBackground)
                                label.text: modelData.cmax.dbvalue + " " + modelData.cmax.unitstring
                                label.color: root.mousearea.hovered ? targetList.rowForegroundHover : (root.ListView.isCurrentItem ? targetList.rowForegroundSelected : targetList.rowForeground)
                            },
                            EntityListImage {
                                image.source: "qrc:/icons/buttons/edit.png"
                                mousearea.onClicked: {
                                    targetListView.currentIndex = index;
                                    targetDialog.exec(modelData, false);
                                }
                                tooltipText: ToolTips.targetTab.edit
                            },
                            EntityListImage {
                                image.source: "qrc:/icons/buttons/remove.png"
                                mousearea.onClicked: {
                                    targetTabController.removeTarget(index);
                                }
                                tooltipText: ToolTips.targetTab.remove
                            }
                        ]

                    }

                    onCurrentIndexChanged: index = currentIndex
                    Component.onCompleted: reset.connect(clearSelection)

                    function clearSelection() {
                        currentIndex = -1;
                    }
                }

            ]

        }
/*
        EntityBlockEnd {
            visible : targetsListExpandHeader.expanded
        }

        EntityTitle {
            label.text: "Target Details"
            id: targetExpandHeader
            visible : (targetListView.count > 0)
        }

        //Target
        EntityView {
            id: target

            visible : (targetListView.count > 0) && targetExpandHeader.expanded

            Layout.fillWidth: true
            Layout.preferredHeight: 100
            Layout.minimumHeight:   100
            property var model: targetListView.currentItem? (targetListView.currentItem.selected_model? targetListView.currentItem.selected_model.modelData : 0) : 0
            onModelChanged: {
                if (!target.model) {return;}
                typeInput.currentIndex = target.model.type.value;
            }

            columnlayout.children: [

                RowLayout {
                    Layout.preferredHeight: target.rowHeight
                    Layout.minimumHeight:   target.rowHeight
                    spacing: 2

                    EntityLabel {
                        id: typeLabel
                        text: "TargetType:"
                    }
                    EntityComboBox {
                        id: typeInput
                        Layout.fillWidth: true
                        model: ["Residual", "Peak", "Mean", "AUC"]
                        currentIndex: target.model ? target.model.type.value : 0;
                        onCurrentIndexChanged: {
                            targetTabController.setTargetType(index, currentIndex);
                        }
                    }
                },

                RowLayout {
                    Layout.preferredHeight: target.rowHeight
                    Layout.minimumHeight:   target.rowHeight
                    spacing: 2

                    EntityLabel {
                        id: cMinLabel
                        text: "CMin:"
                    }
                    EntitySpinBox {
                        id: cMinInput
                        Layout.fillWidth:  true
                        suffix: target.model? " " + (target.model.type.value === 3 ? "h*" : "") + target.model.cmin.unitstring : ""
                        value: target.model? target.model.cmin.dbvalue : -1
                        onEditingFinished: {
                            targetTabController.setCminDbValue(index, value)
                        }
                    }

                    EntityLabel {
                        id: cBestLabel
                        text: "CBest:"
                    }

                    EntitySpinBox {
                        id: cBestInput
                        Layout.fillWidth:  true
                        suffix: target.model? " " + (target.model.type.value === 3 ? "h*" : "") + target.model.cbest.unitstring : ""
                        value: target.model? target.model.cbest.dbvalue : -1
                        onEditingFinished: {
                            targetTabController.setCbestDbValue(index, value)
                        }
                    }

                    EntityLabel {
                        id: cMaxLabel
                        Layout.preferredWidth: 45
                        text: "CMax:"
                    }

                    EntitySpinBox {
                        id: cMaxInput
                        Layout.fillWidth:  true
                        suffix: target.model? " " + (target.model.type.value === 3 ? "h*" : "") + target.model.cmax.unitstring : ""
                        value: target.model? target.model.cmax.dbvalue : -1
                        onEditingFinished: {
                            targetTabController.setCmaxDbValue(index, value)
                        }
                    }

                },

                RowLayout {
                    Layout.preferredHeight: target.rowHeight
                    Layout.minimumHeight:   target.rowHeight
                    spacing: 2

                    EntityLabel {
                        id: tMinLabel
                        Layout.preferredWidth: 75
                        text: "TMin:"
                    }

                    EntitySpinBox {
                        id: tMinInput
                        Layout.fillWidth:  true
                        enabled: typeInput.currentIndex === 1
                        suffix: target.model? " " + target.model.tmin.unitstring : ""
                        value: target.model? target.model.tmin.dbvalue : -1
                        onEditingFinished: {
                            targetTabController.setTminDbValue(index, value);
                        }
                    }

                    EntityLabel {
                        id: tBestLabel
                        Layout.preferredWidth: 45
                        text: "TBest:"
                    }

                    EntitySpinBox {
                        id: tBestInput
                        Layout.fillWidth:  true
                        enabled: typeInput.currentIndex === 1
                        suffix: target.model? " " + target.model.tbest.unitstring : ""
                        value: target.model? target.model.tbest.dbvalue : -1
                        onEditingFinished: {
                            targetTabController.setTbestDbValue(index, value);
                        }
                    }


                    EntityLabel {
                        id: tMaxLabel
                        Layout.preferredWidth: 45
                        text: "TMax:"
                    }

                    EntitySpinBox {
                        id: tMaxInput
                        Layout.fillWidth:  true
                        suffix: target.model? " " + target.model.tmax.unitstring : ""
                        enabled: typeInput.currentIndex === 1
                        value: target.model? target.model.tmax.dbvalue : -1
                        onEditingFinished: {
                            targetTabController.setTmaxDbValue(index, value);
                        }
                    }
                },

                RowLayout {} //Spacer
            ]
        }


        EntityBlockEnd {
            visible : (targetListView.count > 0) && targetExpandHeader.expanded
        }        
*/
    }

    TargetDialog {
        id: targetDialog
        function exec(model, bCreatingNewItem)
        {
            if (model) {
                this.init(
                    model.type.value,
                    model.cmin,
                    model.cbest,
                    model.cmax,
                    model.tmin,
                    model.tbest,
                    model.tmax,
                    model.mic)
                this.open(bCreatingNewItem);
            }
        }

        function applyChanges()
        {
            targetTabController.setTargetType(targetListView.currentIndex, this.getType())
            targetTabController.setCminDbValue(targetListView.currentIndex, this.getCMin())
            targetTabController.setCbestDbValue(targetListView.currentIndex, this.getCBest())
            targetTabController.setCmaxDbValue(targetListView.currentIndex, this.getCMax())
            targetTabController.setTminDbValue(targetListView.currentIndex, this.getTMin())
            targetTabController.setTbestDbValue(targetListView.currentIndex, this.getTBest())
            targetTabController.setTmaxDbValue(targetListView.currentIndex, this.getTMax())
            targetTabController.setMicDbValue(targetListView.currentIndex, this.getMic())
            targetTabController.setUnit(targetListView.currentIndex, this.getUnit())
            targetTabController.forceRefresh()
            return true
        }

        function onUpdated(bApplied, bCreatingNewItem)
        {
            if (bCreatingNewItem && !bApplied) {
                targetTabController.removeTarget(targetListView.currentIndex)
            }
        }
    }
}
