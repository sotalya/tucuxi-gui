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
    property alias currentItem: covariatesTable.currentItem

    //The covariates filters
    ProxyModelFilter {
        id: patientIdFilter
        role: CovariateListModel.PatientIdRole
        value: tab_patient.patientid
//        onValueChanged: console.log("patientfilterval: " + value)
    }

    Connections {
        target: tab_covariate
        onVariatenameChanged: typeFilter.setFixedString(tab_covariate.variatename)
    }

    ProxyModelFilter {
        id: typeFilter
        role: CovariateListModel.NameRole
    }

    ProxyModelFilter {
        id: valueFilter
        role: CovariateListModel.ValueRole
    }

    ProxyModelFilter {
        id: fromDateFilter
        role: CovariateListModel.DateRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.GreaterOrEqual
    }

    ProxyModelFilter {
        id: toDateFilter
        role: CovariateListModel.DateRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.LessOrEqual
    }

    //The covariates delegates
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

    //The covariates table
    Table {
        id: covariatesTable
        Component.onCompleted: console.log("table wh: " + width + " " + height)
        anchors.fill: parent

        model: covariateListModel
        delegate: rowDelegate
        filters: [typeFilter, patientIdFilter, valueFilter, fromDateFilter, toDateFilter]

        TableHeader {
            id: thdr_covariate
//            Layout.fillWidth: true

            headerData: ListModel {
                ListElement {
                    columnName: "Type";
                    columnWidth: 150;
                    columnRole: CovariateListModel.NameRole
                }
                ListElement {
                    columnName: "Value";
                    columnWidth: 150;
                    columnRole: CovariateListModel.ValueRole
                }
                ListElement {
                    columnName: "Date";
                    columnWidth: 150;
                    columnRole: CovariateListModel.DateRole
                }
            }

            onAdd: {
                covariateListModel.sourceModel.insertRows(covariateListModel.sourceModel.rowCount(), 1);
                console.log("covariate added to table of count: " + covariateListModel.sourceModel.rowCount());
                covariateListModel.sourceModel.setData(covariateListModel.sourceModel.index(covariateListModel.sourceModel.rowCount()-1,0),patientIdFilter.value, CovariateListModel.PatientIdRole)
                console.log("covariate patientid set");
                covariateListModel.sourceModel.setData(covariateListModel.sourceModel.index(covariateListModel.sourceModel.rowCount()-1,0),tab_covariate.variatename, CovariateListModel.NameRole)
                console.log("covariate drugid set");
                patientIdFilter.roleChanged(CovariateListModel.PatientId);
                covariatesTable.currentIndex = covariatesTable.count - 1;
                flowModel.sourceModel.refresh()
            }
            onSort: covariateListModel.sort(role, order)
            onFilter: {
                switch (role) {
                case CovariateListModel.NameRole:  typeFilterView.visible  = !typeFilterView.visible;  break;
                case CovariateListModel.ValueRole: valueFilterView.visible = !valueFilterView.visible; break;
                case CovariateListModel.DateRole:  dateFilterView.visible  = !dateFilterView.visible;  break;
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
                Layout.preferredWidth: TableStyle.cell.colwidth
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

        TableFilter {
            id: dateFilterView
            Layout.fillWidth: true

            Text {
                id: dateFilterLabel
                Layout.preferredWidth: Math.max(typeFilterLabel.contentWidth, valueFilterLabel.contentWidth, dateFilterLabel.contentWidth)
                verticalAlignment: Text.AlignVCenter
                color: TableStyle.filter.foregroundColor
                text: "From:"
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
            }

            DatePicker {
                id: dateFilterFromDateInput
                date: new Date(1900, 0, 1);
                onDateChanged: fromDateFilter.value = new Date(date.getFullYear(), date.getMonth(), date.getDate(), dateFilterFromTimeInput.hours, dateFilterFromTimeInput.minutes)
            }

            TimePicker {
                id: dateFilterFromTimeInput
                Layout.fillWidth: true
                onDateChanged: fromDateFilter.value = new Date(dateFilterFromDateInput.date.getFullYear(), dateFilterFromDateInput.date.getMonth(), dateFilterFromDateInput.date.getDate(), dateFilterFromTimeInput.hours, dateFilterFromTimeInput.minutes)
            }

            Text {
                Layout.preferredWidth: contentWidth
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: TableStyle.filter.foregroundColor
                text: "To:"
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
            }

            DatePicker {
                id: dateFilterToDateInput
                date: new Date(2100, 0, 1);
                onDateChanged: toDateFilter.value = new Date(date.getFullYear(), date.getMonth(), date.getDate(), dateFilterToTimeInput.hours, dateFilterToTimeInput.minutes)
            }

            TimePicker {
                id: dateFilterToTimeInput
                Layout.fillWidth: true
                onDateChanged: toDateFilter.value = new Date(dateFilterToDateInput.date.getFullYear(), dateFilterToDateInput.date.getMonth(), dateFilterToDateInput.date.getDate(), dateFilterToTimeInput.hours, dateFilterToTimeInput.minutes)
            }

            onClear: {
                dateFilterFromDateInput.date = new Date(1900, 0, 1);
                dateFilterToDateInput.date   = new Date(2100, 0, 1);
                dateFilterFromTimeInput.hours = 0;
                dateFilterToTimeInput.hours   = 0;
                dateFilterFromTimeInput.minutes = 0
                dateFilterToTimeInput.minutes   = 0
            }
        }
    }

    //The add covariate dialog
    TableDialog {
        id: newCovariateDialog
        title: "Add covariate"

        width: 300
        height: 160

        onAccepted: {
            typeFilterView.clear();
            valueFilterView.clear();
            dateFilterView.clear();

            covariateListModel.insertRows(covariateListModel.rowCount(), 1);
            covariatesTable.currentIndex = covariatesTable.count - 1;

            //ToDo: set the covariates patient (need to update the model)

            close();
        }

        RowLayout {
            Text {
                Layout.preferredWidth: 40
                text: "Type:"
                horizontalAlignment: Text.AlignRight
                color: TableStyle.dialog.foregroundColor
            }
            ComboBox {
                Layout.fillWidth: true
//                model: ["Age", "GIST", "Weight"]
                model: drugVariateListModel.names
                style: EzComboBoxStyle {}
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

        RowLayout {
            Text {
                Layout.preferredWidth: 40
                text: "Date:"
                horizontalAlignment: Text.AlignRight
                color: TableStyle.dialog.foregroundColor
            }
            TextField {
                Layout.fillWidth: true
                placeholderText: "date"
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                style: EzTextFieldStyle {}
            }
        }
    }

    //The covariates row delegate
    Component {
        id: rowDelegate

        TableRow {
            id: row
            Component.onCompleted: console.log("row wh: " + width + " " + height)
//            width: ListView.view.width

            property variant modelData: model

            onEdit: state = "edit"
            onRemove: covariateListModel.removeRows(index, 1)

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

            TableCell {
                id: dateCell
//                Layout.fillWidth: true
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: Qt.formatDateTime(model.date)
                    color: TableStyle.cell.foregroundcolor
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    anchors.fill: parent
                }

                edit: RowLayout {
                    anchors.fill: parent

                    DatePicker {
                        id: dateInput
                        Binding { target: dateInput; property: "date"; value: model.date }
                        onEditingFinished: model.date = new Date(date.getFullYear(), date.getMonth(), date.getDate(), timeInput.hours, timeInput.minutes)
                    }
                    TimePicker {
                        id: timeInput
                        Binding { target: timeInput; property: "hours";   value: model.date.getHours() }
                        Binding { target: timeInput; property: "minutes"; value: model.date.getMinutes() }
                        onEditingFinished: model.date = new Date(dateInput.date.getFullYear(), dateInput.date.getMonth(), dateInput.date.getDate(), hours, minutes)
                    }
                }
            }

            states: [
                State {
                    name: "edit"

                    PropertyChanges { target: typeCell; editMode: true }
                    PropertyChanges { target: valueCell; editMode: true }
                    PropertyChanges { target: dateCell; editMode: true }

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
