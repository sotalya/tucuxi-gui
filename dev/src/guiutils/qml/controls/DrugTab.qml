import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {
    id: drugPanel
    property int index:  -1
    property string drugName: ""
    property int status: Status.invalid;
    property bool enabled: false;
    property alias drugListView: drugListView;

    signal switchActiveSubstance(int index)
    signal switchDrugModel(int index)
    signal reset()

    onReset: init();

    function clearDrugModelSelection() {
        domainListView.clearSelection();
        studyListView.clearSelection();
    }

    function init() {
        //        root.druginfo.text = interpretationController.getDrugHtmlDescription()
    }

    function setCurrentActiveSubstance(substanceIndex) {
        drugListView.currentIndex = substanceIndex;
    }

    // This function is used for automated tests, called from the C++ of GuiTests
    function setExtCurrentActiveSubstance(substanceIndex) {
        drugListView.currentIndex = substanceIndex;
        switchActiveSubstance(substanceIndex);

    }

    function setCurrentDrugModel(drugModelIndex) {
        domainListView.currentIndex = drugModelIndex;
        // Maybe not totally correct, but Ok considering the current implementation
        studyListView.currentIndex = drugModelIndex;
    }

    // This function is used for automated tests, called from the C++ of GuiTests
    function setExtCurrentDrugModel(drugModelIndex) {
        domainListView.currentIndex = drugModelIndex;
        // Maybe not totally correct, but Ok considering the current implementation
        studyListView.currentIndex = drugModelIndex;
        switchDrugModel(drugModelIndex);
    }

    anchors.fill: parent
    color: Style.table.background.table

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        EntityTitle {
            label.text: "Active substance"
            id: drugListExpandHeader
            tooltipText: ToolTips.drugTab.activeSubstanceTitle
        }

        //DrugList
        EntityList {
            id: drugList
            Layout.fillWidth:  true

            visible : drugListExpandHeader.expanded

            columnlayout.children: [

                //DrugList view
                EntityListView {
                    id: drugListView
                    objectName: "drugListView"
                    Layout.fillWidth:  true
                    property var drugs: interpretationController.activeSubstances;
                    onDrugsChanged: {
                    //    console.log("# drugs: " + interpretationController.drugs.size());
                    //    console.log("# drugs: " + interpretationController.activeSubstances.size());
                    }

                    model: drugs.objlist//drugListModel
                    onModelChanged: {
                        //                console.log("# drugs: " + interpretationController.drugs.size());
                        //                for (var i = 0; i < model.length; ++i) {
                        //                    console.log("drugname: " + model[i].name.value());
                        //                }
                    }
                    delegate: EntityListDelegate {
                        id: drugListDelegate
                        property var selected_model: model

                        objectName: "drugList_" + modelData.name.value() // index

                        mousearea.onClicked: {
                            drugListDelegate.ListView.view.currentIndex = index
                            switchActiveSubstance(index);
                        }
                        rowlayout.children: [
                            EntityListDelegateItem {
                                Layout.fillWidth: true
                                label.text: modelData.name.value()
                                color: drugListDelegate.mousearea.hovered ? drugList.rowBackgroundHover : (drugListDelegate.ListView.isCurrentItem ? drugList.rowBackgroundSelected : drugList.rowBackground)
                                label.color: drugListDelegate.mousearea.hovered ? drugList.rowForegroundHover : (drugListDelegate.ListView.isCurrentItem ? drugList.rowForegroundSelected : drugList.rowForeground)
                            }
                        ]
                    }
                    property string currentDrugName: currentIndex === -1 ? "" : drugs.at(currentIndex).name.value()//drugListModel.data(drugListModel.index(currentIndex, 0), DrugListModel.NameRole);
                    onCurrentDrugNameChanged: drugName = currentDrugName

                    onCurrentIndexChanged: {
                        index = currentIndex;
                        if (currentIndex !== -1) {
                            domainListView.currentIndex = -1;
                            studyListView.currentIndex  = -1;
                        }
                    }

                    Component.onCompleted: reset.connect(clearSelection)

                    function clearSelection() {
                        currentIndex = -1;
                    }

                }
            ]
        }

        EntityTitle {
            label.text: "Domain"
            id: domainListExpandHeader
            tooltipText: ToolTips.drugTab.domainTitle
        }

        //DomainList
        EntityList {
            id: domainList
            objectName: "domainsView"
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.preferredHeight: headerHeight + 3 + (rowHeight + domainListView.spacing) * 3
            Layout.minimumHeight:   headerHeight + 3 + (rowHeight + domainListView.spacing) * 3

            visible : domainListExpandHeader.expanded

            property var model: interpretationController.drugModels; //ListView.currentItem ? drugListView.currentItem.selected_model.modelData : 0;
            signal currentIndexChanged(int index)

            columnlayout.children: [
                //DomainList view
                EntityListView {
                    id: domainListView
                    Layout.fillWidth:  true
                    property var drugModels : interpretationController.drugModels;

                    model: drugModels.objlist

                    delegate: EntityListDelegate {
                        id: domainListDelegate

                        mousearea.onClicked: {
                            domainListDelegate.ListView.view.currentIndex = index
                            switchDrugModel(index);
                        }

                        rowlayout.children: [
                            EntityListDelegateItem {
                                Layout.fillWidth: true
                                label.text: modelData.domainName
                                color: domainListDelegate.mousearea.hovered ? domainList.rowBackgroundHover : (domainListDelegate.ListView.isCurrentItem ? domainList.rowBackgroundSelected : domainList.rowBackground)
                                label.color: domainListDelegate.mousearea.hovered ? domainList.rowForegroundHover : (domainListDelegate.ListView.isCurrentItem ? domainList.rowForegroundSelected : domainList.rowForeground)
                            }
                        ]
                    }

                    onCurrentIndexChanged: {
                        //domainList.currentIndexChanged(currentIndex)
                        studyListView.currentIndex  = currentIndex;
                    }

                    Component.onCompleted: reset.connect(clearSelection)

                    function clearSelection() {
                        currentIndex = -1;
                    }
                }
            ]
            //DomainList header
        }

        EntityTitle {
            label.text: "Study"
            id: studyListExpandHeader
            tooltipText: ToolTips.drugTab.studyTitle
        }

        //StudyList
        EntityList {
            id: studyList
            objectName: "studysView"
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.preferredHeight: headerHeight + 3 + (rowHeight + studyListView.spacing) * 3
            Layout.minimumHeight:   headerHeight + 3 + (rowHeight + studyListView.spacing) * 3

            visible : studyListExpandHeader.expanded

            property var model:  interpretationController.drugModels; // drugListView.currentItem? drugListView.currentItem.selected_model.modelData : 0;
            signal currentIndexChanged(int index)

            columnlayout.children: [
                //StudyList view
                EntityListView {
                    id: studyListView
                    Layout.fillWidth:  true

                    property var drugModels : interpretationController.drugModels;

                    model: drugModels.objlist

                    delegate: EntityListDelegate {
                        id: studyListDelegate

                        mousearea.onClicked: {
                            studyListDelegate.ListView.view.currentIndex = index
                            switchDrugModel(index);
                        }

                        rowlayout.children: EntityListDelegateItem {
                            Layout.fillWidth: true
                            label.text: modelData.studyName
                            color: studyListDelegate.mousearea.hovered ? studyList.rowBackgroundHover : (studyListDelegate.ListView.isCurrentItem ? studyList.rowBackgroundSelected : studyList.rowBackground)
                            label.color: studyListDelegate.mousearea.hovered ? studyList.rowForegroundHover : (studyListDelegate.ListView.isCurrentItem ? studyList.rowForegroundSelected : studyList.rowForeground)
                        }
                    }

                    onCurrentIndexChanged: {
                        //studyList.currentIndexChanged(currentIndex);
                        domainListView.currentIndex = currentIndex;
                    }

                    Component.onCompleted: reset.connect(clearSelection)

                    function clearSelection() {
                        currentIndex = -1;
                    }
                }
            ]
        }
    }
}
