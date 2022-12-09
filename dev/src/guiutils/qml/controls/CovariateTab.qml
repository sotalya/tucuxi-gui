import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2


import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {

    function setCovariateType(i){
        drugVariateListView.currentIndex = i;
    } // Runs ok !

    id: covariatePanel
    property int index:  -1
    property int status: Status.invalid;
    property bool enabled: false;

    signal currentIndexChanged(int index)
    signal reset()

    onIndexChanged:  currentIndexChanged(index)

    anchors.fill: parent
    color: Style.table.background.table


    MessageDialog {
        id: messageNoAgeDialog
        title: "Covariates"
        text: "The age is automatically calculated based on the birthdate."
        standardButtons: StandardButton.Close
    }

    MessageDialog {
        id: messageNoSexDialog
        title: "Covariates"
        text: "The sex is based on patient's data."
        standardButtons: StandardButton.Close
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5


        /**********************************************************************
          Drug covariate list
        **********************************************************************/

        EntityTitle {
            label.text: "Drug covariates list"
            id: drugVariatesListExpandHeader
            tooltipText: ToolTips.covariateTab.drugVariateList
        }

        //DrugVariateList
        EntityList {
            id: drugVariateList

            visible: drugVariatesListExpandHeader.expanded

            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.preferredHeight: drugVariateList.headerHeight + 10 + (drugVariateList.rowHeight + drugVariateListView.spacing) * drugVariateListView.count

            columnlayout.children: [
                //DrugVariateList header
                EntityListHeader {
                    EntityListHeaderItem {
                        label.text: "Name"
                        Layout.preferredWidth: 200

                        tooltipText : ToolTips.covariateTab.name
                    }
                    EntityListHeaderItem {
                        Layout.fillWidth: true
                        label.text: "Default value"
                        Layout.preferredWidth: 200

                        tooltipText : ToolTips.covariateTab.defaultValue
                    }
                    EntityListHeaderItem {
                        Layout.fillWidth: true
                        label.text: "Current value"
                        Layout.preferredWidth: 200

                        tooltipText : ToolTips.covariateTab.currentValue
                    }
                },

                EntityHeaderEnd {
                    visible: true
                },


                //DrugVariateList view
                EntityListView {
                    id: drugVariateListView
                    objectName: "drugVariatesView"
                    signal currentIndexChanged(int index);
                    property var drugVariates: covariateTabController.drugVariateInfos;
                    onDrugVariatesChanged: {
                        currentIndex = -1;
                    }

                    model: drugVariates? drugVariates.objlist : 0
                    delegate: EntityListDelegate {
                        id: drugVariateListDelegate
                        mousearea.onClicked: {
                           covariateTabController.selectDrugVariate(index);
                           drugVariateListDelegate.ListView.view.currentIndex = index
                        }
                        rowlayout.children: [
                            EntityListDelegateItem {
                                id: modelDataId
                                Layout.preferredWidth: 200
                                color: drugVariateListDelegate.mousearea.hovered ? drugVariateList.rowBackgroundHover : (drugVariateListDelegate.ListView.isCurrentItem ? drugVariateList.rowBackgroundSelected : drugVariateList.rowBackground)
                                label.color: drugVariateListDelegate.mousearea.hovered ? drugVariateList.rowForegroundHover : drugVariateList.rowForeground
                                label.text: modelData.name
                            },
                            EntityListDelegateItem {
                                Layout.fillWidth: true
                                color: drugVariateListDelegate.mousearea.hovered ? drugVariateList.rowBackgroundHover : (drugVariateListDelegate.ListView.isCurrentItem ? drugVariateList.rowBackgroundSelected : drugVariateList.rowBackground)
                                label.color: drugVariateListDelegate.mousearea.hovered ? drugVariateList.rowForegroundHover : drugVariateList.rowForeground
                                label.text: {
                                    if (modelData.name === "Sex" || modelData.name === "sex" || modelData.name === "Gender" || modelData.name === "gender") {
                                        switch (modelData.defaultValue.dbvalue) {
                                        case 1:  return "Male";
                                        case 0:  return "Female";
                                        case 0.5:  return "Unknown";
                                        default: return "Undefined"
                                        }
                                    }
                                    if (modelData.typeToQString() === "bool") {
                                        switch (modelData.defaultValue.dbvalue) {
                                        case 0:  return "False";
                                        case 1:  return "True";
                                        default: return "Undefined"
                                        }
                                    }

                                    return  modelData.defaultValue.dbvalue + " " + modelData.defaultValue.unitstring;
                                }
                            },
                            EntityListDelegateItem {
                                Layout.fillWidth: true
                                color: drugVariateListDelegate.mousearea.hovered ? drugVariateList.rowBackgroundHover : (drugVariateListDelegate.ListView.isCurrentItem ? drugVariateList.rowBackgroundSelected : drugVariateList.rowBackground)
                                label.color: drugVariateListDelegate.mousearea.hovered ? drugVariateList.rowForegroundHover : drugVariateList.rowForeground
                                label.text: {
                                    if (modelData.name === "Sex" || modelData.name === "sex" || modelData.name === "Gender" || modelData.name === "gender") {
                                        switch (modelData.actualValue.dbvalue) {
                                        case 1:  return "Male";
                                        case 0:  return "Female";
                                        case 0.5:  return "Unknown";
                                        default: return "Undefined"
                                        }
                                    }
                                    if (modelData.typeToQString() === "bool") {
                                        switch (modelData.actualValue.dbvalue) {
                                        case 0:  return "False";
                                        case 1:  return "True";
                                        default: return "Undefined"
                                        }
                                    }
                                    return modelData.actualValue.dbvalue + " " + modelData.defaultValue.unitstring;
                                }
                            }
                        ]
                    }

                    onCurrentIndexChanged: {
                        patientVariateListView.clearSelection();
                        drugVariateListView.currentIndexChanged(currentIndex);

                        if (currentIndex !== -1 && patientVariateListView.count !== 0)
                            patientVariateListView.currentIndex = 0;
                    }
                    Component.onCompleted: reset.connect(clearSelection)

                    function clearSelection() {
                        //currentIndex = -1;
                    }
                }
            ]
        }

        EntityBlockEnd {
            visible : drugVariatesListExpandHeader.expanded
        }

        /**********************************************************************
          Patient covariate list
        **********************************************************************/

        EntityTitle {
            label.text: "Patient covariate history : %1".arg( (drugVariateListView.currentIndex != -1) ? drugVariateListView.drugVariates.objlist[drugVariateListView.currentIndex].name : "?")
            id: patientVariatesListExpandHeader
            visible : true
            tooltipText: ToolTips.covariateTab.patientVariateList
        }

        EntityList {
            id: patientVariateList

            visible: patientVariatesListExpandHeader.expanded

            Layout.fillWidth:  true
            Layout.fillHeight: true
            Layout.preferredHeight: patientVariateList.headerHeight + 40 + (patientVariateList.rowHeight + patientVariateListView.spacing) * patientVariateListView.count

            //Layout.preferredHeight: 100 + 50 * patientVariateListView.count;

            columnlayout.children: [

                EntityListHeader {
                    EntityListHeaderItem {
                        label.text: "Name"
                        Layout.preferredWidth: 200
                        tooltipText : ToolTips.covariateTab.name
                    }
                    EntityListHeaderItem {
                        Layout.preferredWidth: 150
                        label.text: "Value"
                        tooltipText : ToolTips.covariateTab.value
                    }
                    EntityListHeaderItem {
                        Layout.fillWidth: true
                        label.text: "Date"
                        tooltipText : ToolTips.covariateTab.date
                    }
                    EntityListHeaderImage {
                        id: addCovariateId
                        objectName: "addCovariate";

                        image.source: drugVariateListView.currentIndex == -1 ? "qrc:/icons/buttons/add_disabled.png" :
                                                                               (drugVariateListView.drugVariates.objlist[drugVariateListView.currentIndex].automatic ?
                                                                                    "qrc:/icons/buttons/add_disabled.png" : (drugVariateListView.drugVariates.objlist[drugVariateListView.currentIndex].name === "Sex" ? "qrc:/icons/buttons/add_disabled.png" : "qrc:/icons/buttons/add.png"))
                        tooltipText : ToolTips.covariateTab.add

                        mousearea.onClicked: {
                            if (drugVariateListView.currentIndex === -1)
                                return;
                            // Do not modify the age
                            if (drugVariateListView.drugVariates.objlist[drugVariateListView.currentIndex].automatic) {
                                messageNoAgeDialog.open();
//                                covariateTabController.removePatientVariate(patientVariateListView.currentIndex);
                            }
                            else if (drugVariateListView.drugVariates.objlist[drugVariateListView.currentIndex].name === "Sex") {
                                messageNoSexDialog.open();
                            }
                            else {
                                covariateTabController.addPatientVariate(drugVariateListView.currentIndex);
                                patientVariateListView.currentIndex = patientVariateListView.count-1;
                                var model = covariateTabController.fileredVariates.objlist[patientVariateListView.currentIndex];
                                covariateDialog.exec(model, true)
                            }
//                            console.log(drugVariateListView.drugVariates.objlist[drugVariateListView.currentIndex].name)
                        }

                    }
                },

                EntityHeaderEnd {
                    visible: patientVariateList.visible
                },

                //patientVariateList view
                EntityListView {
                    id: patientVariateListView
                    //property var patientVariates: covariateTabController.patientVariates;
                    property var patientVariates: covariateTabController.fileredVariates;


                    model: patientVariates.objlist//patientVariateListModel
                    delegate: EntityListDelegate {
                        id: patientVariateListDelegate
                        property var selected_model: model
                        mousearea.onClicked: patientVariateListDelegate.ListView.view.currentIndex = index
                        rowlayout.children: [
                            EntityListDelegateItem {
                                Layout.preferredWidth: 200
                                color: patientVariateListDelegate.mousearea.hovered ? patientVariateList.rowBackgroundHover : (patientVariateListDelegate.ListView.isCurrentItem ? patientVariateList.rowBackgroundSelected : patientVariateList.rowBackground)
                                label.color: patientVariateListDelegate.mousearea.hovered ? patientVariateList.rowForegroundHover : (patientVariateListDelegate.ListView.isCurrentItem ? patientVariateList.rowForegroundSelected : (/*hided ? patientVariateList.rowForegroundGray : */patientVariateList.rowForeground))
                                label.text: modelData.name
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth: 150
                                color: patientVariateListDelegate.mousearea.hovered ? patientVariateList.rowBackgroundHover : (patientVariateListDelegate.ListView.isCurrentItem ? patientVariateList.rowBackgroundSelected : patientVariateList.rowBackground)
                                label.color: patientVariateListDelegate.mousearea.hovered ? patientVariateList.rowForegroundHover : (patientVariateListDelegate.ListView.isCurrentItem ? patientVariateList.rowForegroundSelected : (/*hided ? patientVariateList.rowForegroundGray : */patientVariateList.rowForeground))
                                label.text: {
                                    if (modelData.name === "Sex" || modelData.name === "sex" || modelData.name === "Gender" || modelData.name === "gender") {
                                        switch (modelData.quantity.dbvalue) {
                                        case 1:  return "Male";
                                        case 0:  return "Female";
                                        case 0.5:  return "Unknown";
                                        default: return "Undefined"
                                        }
                                    }
                                    if (modelData.typeToQString() === "bool") {
                                        switch (modelData.quantity.dbvalue) {
                                        case 0:  return "False";
                                        case 1:  return "True";
                                        default: return "Undefined"
                                        }
                                    }
                                    return modelData.quantity.dbvalue + " " + modelData.quantity.unitstring;
                                }

                            },
                            EntityListDelegateItem {
                                Layout.fillWidth: true
                                color: patientVariateListDelegate.mousearea.hovered ? patientVariateList.rowBackgroundHover : (patientVariateListDelegate.ListView.isCurrentItem ? patientVariateList.rowBackgroundSelected : patientVariateList.rowBackground)
                                label.text: Qt.formatDateTime(modelData.date)
                                label.color: patientVariateListDelegate.mousearea.hovered ? patientVariateList.rowForegroundHover : (patientVariateListDelegate.ListView.isCurrentItem ? patientVariateList.rowForegroundSelected : (/* hided ? patientVariateList.rowForegroundGray : */patientVariateList.rowForeground))
                            },
                            EntityListImage {
                                objectName: "editCovariate_" + index;
                                image.source: "qrc:/icons/buttons/edit.png"
                                mousearea.onClicked: {
                                    // Do not modify the age
                                    if (modelData.covariateId === "age") {
                                        messageNoAgeDialog.open();
                                    }
                                    else if (modelData.covariateId === "sex") {
                                        messageNoSexDialog.open();
                                    }
                                    else {
                                        patientVariateListView.currentIndex = index
                                        covariateDialog.exec(modelData, false)
                                    }
                                }
                            },
                            EntityListImage {
                                objectName: "removeCovariate_" + index;
                                image.source: drugVariateListView.currentIndex == -1 ? "qrc:/icons/buttons/remove_disabled.png" :
                                                                                                                       (drugVariateListView.drugVariates.objlist[drugVariateListView.currentIndex].automatic ?
                                                                                                                            "qrc:/icons/buttons/remove_disabled.png" : (drugVariateListView.drugVariates.objlist[drugVariateListView.currentIndex].name === "Sex" ? "qrc:/icons/buttons/remove_disabled.png" : "qrc:/icons/buttons/remove.png"))
                                mousearea.onClicked: {
                                    // Do not modify the age
                                    if (modelData.covariateId === "age") {
                                        messageNoAgeDialog.open();
                                    }
                                    else if (modelData.covariateId === "sex") {

                                        messageNoSexDialog.open();
                                    }
                                    else {
                                        covariateTabController.removePatientVariate(index);
                                    }
                                }
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

                    Layout.preferredHeight: values ? values.size * 30 : 0
                    values : patientVariateListView.currentItem?
                                 (patientVariateListView.currentItem.selected_model ?
                                     (patientVariateListView.currentItem.selected_model.modelData.uncastedValues.size() > 0 ?
                                          patientVariateListView.currentItem.selected_model.modelData.uncastedValues : 0
                                      ) : 0
                                  ) : 0
                }
            ]
        }
/*
        EntityBlockEnd {
            visible : covariate.visible
        }

        // *******************************************************
        // Patient variate with all the input fields
        //

        EntityTitle {
            label.text: "Patient Variate"
            id: patientVariateExpandHeader
            visible :  (patientVariateListView.count > 0)
        }

        EntityView {
            id: covariate

            visible: patientVariateExpandHeader.expanded && (patientVariateListView.count > 0)

            Layout.fillWidth: true
            Layout.preferredHeight: 180
            Layout.minimumHeight:   180
            property var model: patientVariateListView.currentItem? (patientVariateListView.currentItem.selected_model ? patientVariateListView.currentItem.selected_model.modelData : 0) : 0

            columnlayout.children: [
                RowLayout {
                    Layout.preferredHeight: covariate.rowHeight
                    Layout.minimumHeight:   covariate.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "Type:"
                    }
                    EntityLabel {
                        text: covariate.model? covariate.model.name : ""
                        Layout.fillWidth:  true
                    }
                    EntityLabel {
                        text: "Value:"
                        Layout.preferredWidth: 50
                    }
                    Loader {
                        id: valueInput
                        Layout.fillWidth: true

                        //ToDo: update valid in the loader's delegates if necessary
                        //                    property var modelData: model
                        //                    property bool valid: Status.valid

                        sourceComponent: {
                            if (!covariate.model) {return;}
                            if (covariate.model.name === "Sex" || covariate.model.name === "sex" || covariate.model.name === "Gender" || covariate.model.name === "gender")
                                return valueGenderDelegate;

                            switch (covariate.model.typeToQString()) {
                            case "bool": return valueBoolDelegate;
                            default:     return valueDoubleDelegate;
                            }
                        }
                    }
                    Component {
                        id: valueDoubleDelegate

                        EntitySpinBox {
                            id: valueDoubleDelegateControl
                            suffix: covariate.model? (covariate.model.quantity.unitstring === "" ?  "" : " " + covariate.model.quantity.unitstring) : ""
                            onEditingFinished: {
                                covariateTabController.setDbValue(index, value);
                            }
                            font.family:    covariate.inputFontFamily
                            font.pixelSize: covariate.inputFontSize
                            horizontalAlignment: covariate.inputHAlign
                            decimals: covariate.model.type === "int" ? 0 : 2
                            minimumValue: 0
                            maximumValue: Number.POSITIVE_INFINITY


                            value: covariate.model? covariate.model.quantity.dbvalue : 0.0
                            //                            Binding { targdrugVariateListet: valueDoubleDelegateControl; property: "value"; value: covariate.model.quantity.dbvalue }
                        }
                    }
                    Component {
                        id: valueBoolDelegate


                        EntityComboBox {
                            id: valueBoolDelegateControl
                            model: [ "False", "True", "Undefined" ]
                            currentIndex : (covariate.model? -1 : covariate.model.quantity.dbvalue === 0.5 ? 2 : covariate.model.quantity.dbvalue)
                            onActivated: {
                                var v = 0.0;
                                switch (index) {
                                    case 0: v = 0; break;
                                    case 1: v = 1; break;
                                    case 2: v = 0.5; break;
                                }
                                covariateTabController.setDbValue(patientVariateListView.index, v);
                            }
                        }
                    }
                    Component {
                        id: valueGenderDelegate

                        ComboBox {
                            id: valueGenderDelegateControl
                            model: [ "Female", "Male", "Unknown" ]
                            currentIndex : (covariate.model? -1 : covariate.model.quantity.dbvalue === 0.5 ? 2 : covariate.model.quantity.dbvalue)
                            onActivated: {
                                var v = 0.0;
                                switch (index) {
                                    case 0: v = 0; break;
                                    case 1: v = 1; break;
                                    case 2: v = 0.5; break;
                                }
                                covariateTabController.setDbValue(patientVariateListView.index, v);
                            }
                        }
                    }
                },

                RowLayout {
                    Layout.preferredHeight: covariate.rowHeight
                    Layout.minimumHeight:   covariate.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "Date:"
                    }
                    DatePicker {
                        id: dateInput
                        Layout.preferredWidth: 150
                        Binding { target: dateInput; property: "date"; value: covariate.model? covariate.model.date : new Date()}
                        onEditingFinished: {
                            covariateTabController.setDate(index, new Date(date.getFullYear(), date.getMonth(), date.getDate(), timeInput.hours, timeInput.minutes));
                        }
                    }
                    TimePicker {
                        id: timeInput
                        Layout.fillWidth: true
                        Binding { target: timeInput; property: "hours";   value: covariate.model ? covariate.model.date.getHours() : new Date() }
                        Binding { target: timeInput; property: "minutes"; value: covariate.model ? covariate.model.date.getMinutes() : new Date() }
                        onEditingFinished: {
                            covariateTabController.setDate(index, new Date(dateInput.date.getFullYear(), dateInput.date.getMonth(), dateInput.date.getDate(), hours, minutes));
                        }
                    }
                },

                RowLayout {
                    Layout.preferredHeight: covariate.rowHeight
                    Layout.minimumHeight:   covariate.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "Description:"
                    }
                    EntityLabel {
                        text: covariate.model ? (covariate.model.description === "" ? "N/A" : covariate.model.description) : ""
                        Layout.fillWidth:  true
                    }
                },

                UncastedListView {

                    Layout.preferredHeight: values ? values.size * 30 : 0
                    values : patientVariateListView.currentItem?
                                 (patientVariateListView.currentItem.selected_model ?
                                     (patientVariateListView.currentItem.selected_model.modelData.uncastedValues.size() > 0 ?
                                          patientVariateListView.currentItem.selected_model.modelData.uncastedValues : 0
                                      ) : 0
                                  ) : 0
                },

                RowLayout {} //Spacer

            ]
        }

        EntityBlockEnd {
            visible : covariate.visible
        }
*/
        Rectangle {
            color: "white"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    CovariateDialog {
        id: covariateDialog
        objectName: "covariateDialog"
        function exec(model, bCreatingNewItem)
        {
            if (model) {
                this.init(
                    model.name,
                    model.typeToQString(),
                    model.quantity,
                    model.date,
                    model.description)
                this.open(bCreatingNewItem);
            }
        }

        function applyChanges()
        {
            covariateTabController.setDbValue(patientVariateListView.currentIndex, this.getValue())
            covariateTabController.setDate(patientVariateListView.currentIndex, this.getDate())
            covariateTabController.forceRefresh()
            return true
        }

        function onUpdated(bApplied, bCreatingNewItem)
        {
            if (bCreatingNewItem && !bApplied) {
                covariateTabController.removePatientVariate(patientVariateListView.currentIndex)
            }
        }
    }
}
