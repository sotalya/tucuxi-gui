import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0
import guiutils.qml.controls.table 1.0

import ezechiel 1.0

Rectangle {
    id: root
    color: "transparent"
    property alias currentItem: drugvariatesTable.currentItem

    //The drugvariates filters
    ProxyModelFilter {
        id: drugIdFilter
        role: DrugVariateListModel.DrugIdRole
        value: tab_drug.drugid
//        onValueChanged: console.log("drugfilterval: " + value)
    }

    ProxyModelFilter {
        id: typeFilter
        role: DrugVariateListModel.NameRole
    }

    ProxyModelFilter {
        id: valueFilter
        role: DrugVariateListModel.ValueRole
    }

    //The drugvariates delegates
    Component {
        id: valueDoubleDelegate

        TextField {
            anchors.fill: parent
            placeholderText : "value"
            text: modelData.value
            onEditingFinished: modelData.value = text
            font.family: TableStyle.fontfamily
            font.pixelSize: TableStyle.textpxsize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            style: EzTextFieldStyle {}
        }
    }

    Component {
        id: valueBoolDelegate

        ComboBox {
            id: valueBoolDelegateControl
            anchors.fill: parent
            model: [ "No", "Yes" ]
            onActivated: modelData.value = index
            Binding { target: valueBoolDelegateControl; property: "currentIndex"; value: modelData.value }
            style: EzComboBoxStyle {}
        }
    }

    //The drugvariates table
    Table {
        id: drugvariatesTable
        Component.onCompleted: console.log("table wh: " + width + " " + height)
        anchors.fill: parent

        model: drugVariateListModel
        delegate: rowDelegate
        filters: [drugIdFilter, typeFilter, valueFilter]

        TableHeader {
            id: thdr_drugvariate
//            Layout.fillWidth: true

            headerData: ListModel {
                ListElement {
                    columnName: "Type";
                    columnWidth: 150;
                    columnRole: DrugVariateListModel.NameRole
                }
                ListElement {
                    columnName: "Default Value";
                    columnWidth: 150;
                    columnRole: DrugVariateListModel.ValueRole
                }
            }

            onAdd: newDrugVariateDialog.show()
            onSort: drugVariateListModel.sort(role, order)
            onFilter: {
                switch (role) {
                case DrugVariateListModel.NameRole:  typeFilterView.visible  = !typeFilterView.visible;  break;
                case DrugVariateListModel.ValueRole: valueFilterView.visible = !valueFilterView.visible; break;
                }
            }
        }

        TableFilter {
            id: typeFilterView
            Layout.fillWidth: true

            Text {
                id: typeFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "Type:"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: TableStyle.filter.foregroundColor
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
            }

            TextField {
                id: typeFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "type"
                onTextChanged: typeFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: typeFilterInput.text = "";
        }

        TableFilter {
            id: valueFilterView
            Layout.fillWidth: true

            Text {
                id: valueFilterLabel
                Layout.preferredWidth: Math.max(typeFilterLabel.contentWidth, valueFilterLabel.contentWidth)
                Layout.fillHeight: true
                text: "Value:"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: TableStyle.filter.foregroundColor
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
            }

            TextField {
                id: valueFilterInput
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "value"
                onTextChanged: valueFilter.setRegExp(text)
                style: EzTextFieldStyle {}
            }

            onClear: valueFilterInput.text = "";
        }
    }

    //The add drugvariate dialog
    TableDialog {
        id: newDrugVariateDialog
        title: "Add drugvariate"

        width: 300
        height: 160

        onAccepted: {
            typeFilterView.clear();
            valueFilterView.clear();

            drugVariateListModel.insertRows(drugVariateListModel.rowCount(), 1);
            drugvariatesTable.currentIndex = drugvariatesTable.count - 1;

//            drugvariatesTable.currentItem.modelData.name = "Age";
//            drugvariatesTable.currentItem.modelData.description = "The age of the patient";
//            drugvariatesTable.currentItem.modelData.value = 97;
//            drugvariatesTable.currentItem.modelData.unit = "years";
//            drugvariatesTable.currentItem.modelData.type = "int";
//            drugvariatesTable.currentItem.modelData.date = Date.fromLocaleString(Qt.locale(), "01.01.15 12:00", Qt.DefaultLocaleShortDate);
            //ToDo: set the drugvariates patient (need to update the model)

            close();
        }

        RowLayout {
            Text {
                Layout.preferredWidth: 40
                text: "Type:"
                horizontalAlignment: Text.AlignRight
                color: TableStyle.dialog.foregroundColor
            }
            TextField {
                Layout.fillWidth: true
                placeholderText: "type"
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                style: EzTextFieldStyle {}
            }
        }

        RowLayout {
            Text {
                Layout.preferredWidth: 40
                text: "Value:"
                horizontalAlignment: Text.AlignRight
                color: TableStyle.dialog.foregroundColor
            }
            TextField {
                Layout.fillWidth: true
                placeholderText: "value"
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                style: EzTextFieldStyle {}
            }
            Text {
                Layout.preferredWidth: contentWidth
                text: "kg"
                color: TableStyle.dialog.foregroundColor
            }
        }
    }

    //The drugvariates row delegate
    Component {
        id: rowDelegate

        TableRow {
            id: row
//            Component.onCompleted: console.log("row wh: " + width + " " + height)
            ListView.onIsCurrentItemChanged: {
                if (ListView.isCurrentItem)
                    tab_covariate.variatename = model.name
                    console.log("variatename: " + tab_covariate.variatename)
                }
//            width: ListView.view.width

            property variant modelData: model

            onEdit: state = "edit"
            onRemove: drugVariateListModel.removeRows(index, 1)

            TableCell {
                id: typeCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    text: model.name
                    color: TableStyle.cell.foregroundcolor
                }
            }

            TableCell {
                id: valueCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    onTextChanged: {
                        if (contentWidth > TableStyle.cell.colwidth) {
                            TableStyle.cell.colwidth = contentWidth
                            console.log("contentwidth: " + contentWidth)
                        }
                    }
                    text: {
                        var unit = model.unit;
                        switch(model.type) {
                        case "bool": return model.value ? "Yes" : "No";
                        default:     return unit ? model.value + " " + unit : model.value ;
                        }
                    }
                    color: TableStyle.cell.foregroundcolor
                }

                edit: RowLayout {
                    anchors.fill: parent
                    Loader {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        property var modelData: model

                        sourceComponent: {
                            switch(model.type) {
                            case "bool": return valueBoolDelegate;
                            default:     return valueDoubleDelegate;
                            }
                        }
                    }

                    Text {
                        anchors.fill: parent
                        Layout.preferredWidth: contentWidth
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.family: TableStyle.fontfamily
                        minimumPixelSize:TableStyle.textminpxsize
                        font.pixelSize: TableStyle.textpxsize
                        text: model.unit
                        visible: model.unit ? true : false
                    }
                }
            }

            states: [
                State {
                    name: "edit"

                    PropertyChanges { target: typeCell; editMode: true }
                    PropertyChanges { target: valueCell; editMode: true }

                    PropertyChanges { target: row; editText: "Done" }
                    PropertyChanges { target: row; onEdit: row.state = "" }
                }
            ]

            ListView.onCurrentItemChanged: {
                if (state === "edit")
                    state = ""
            }

            ListView.onRemove: SequentialAnimation {
                PropertyAction { target: row; property: "ListView.delayRemove"; value: true }
                NumberAnimation { target: row; property: "scale"; to: 0; duration: 250; easing.type: Easing.InOutQuad }
                PropertyAction { target: row; property: "ListView.delayRemove"; value: false }
            }

            ListView.onAdd: SequentialAnimation {
                NumberAnimation { target: row; property: "scale"; from: 0; to: 1; duration: 250; easing.type: Easing.InOutQuad }
            }
        }
    }
}
