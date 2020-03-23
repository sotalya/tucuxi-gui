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
    property alias currentItem: measuresTable.currentItem
    property alias drugfilter: drugIdFilter
    property alias patientfilter: patientIdFilter

    //The measures filters
    ProxyModelFilter {
        id: drugIdFilter
        role: MeasureListModel.DrugIdRole
        value: tab_drug.drugid
//        onValueChanged: console.log("drugfilterval: " + value)
    }

    ProxyModelFilter {
        id: patientIdFilter
        role: MeasureListModel.PatientIdRole
        value: tab_patient.patientid
//        onValueChanged: console.log("patientfilterval: " + value)
    }

    ProxyModelFilter {
        id: valueFilter
        role: MeasureListModel.ValueRole
    }

    ProxyModelFilter {
        id: fromDateFilter
        role: MeasureListModel.SampleDateRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.GreaterOrEqual
    }

    ProxyModelFilter {
        id: toDateFilter
        role: MeasureListModel.SampleDateRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.LessOrEqual
    }

    //The measures delegates
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

    //The measures table
    Table {
        id: measuresTable
        Component.onCompleted: console.log("table wh: " + width + " " + height)
        anchors.fill: parent

        model: measureListModel
        delegate: rowDelegate
        filters: [drugIdFilter, patientIdFilter, valueFilter, fromDateFilter, toDateFilter]

        TableHeader {
            id: thdr_measure
//            Layout.fillWidth: true

            headerData: ListModel {
                ListElement {
                    columnName: "Value";
                    columnWidth: 150;
                    columnRole: MeasureListModel.ValueRole
                }
                ListElement {
                    columnName: "Sample Date";
                    columnWidth: 150;
                    columnRole: MeasureListModel.SampleDateRole
                }
            }

            onAdd: {
                measureListModel.sourceModel.insertRows(measureListModel.sourceModel.rowCount(), 1);
                console.log("measure added to table of count: " + measureListModel.sourceModel.rowCount());
                measureListModel.sourceModel.setData(measureListModel.sourceModel.index(measureListModel.sourceModel.rowCount()-1,0),patientIdFilter.value, MeasureListModel.PatientIdRole)
                console.log("measure patientid set");
                measureListModel.sourceModel.setData(measureListModel.sourceModel.index(measureListModel.sourceModel.rowCount()-1,0),drugIdFilter.value, MeasureListModel.DrugIdRole)
                console.log("measure drugid set");
                patientIdFilter.roleChanged(MeasureListModel.PatientId);
                measuresTable.currentIndex = measuresTable.count - 1;
                flowModel.sourceModel.refresh()
            }

            onSort: measureListModel.sort(role, order)
            onFilter: {
                switch (role) {
                case MeasureListModel.ValueRole: valueFilterView.visible = !valueFilterView.visible; break;
                case MeasureListModel.SampleDateRole:  dateFilterView.visible  = !dateFilterView.visible;  break;
                }
            }
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
                Layout.preferredWidth: TableStyle.cell.colwidth
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
                onDateChanged: toDateFilter.value = new Date(dateFilterToDateInput.date.getFullYear(), dateFilterToDateInput.date.getMonth(), dateFilterToDateInput.date.getDate(), dateFilterFromTimeInput.hours, dateFilterFromTimeInput.minutes)
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

    //The add measure dialog

//    TableDialog {
//        id: newMeasureDialog
//        title: "Add measure"

//        width: 300
//        height: 160

//        onAccepted: {
//            typeFilterView.clear();
//            valueFilterView.clear();
//            dateFilterView.clear();

//            measureListModel.insertRows(measureListModel.rowCount(), 1);
//            measuresTable.currentIndex = measuresTable.count - 1;

//            //ToDo: set the measures patient (need to update the model)

//            close();
//        }

//        RowLayout {
//            Text {
//                Layout.preferredWidth: 40
//                text: "Value:"
//                horizontalAlignment: Text.AlignRight
//                color: TableStyle.dialog.foregroundColor
//            }
//            TextField {
//                Layout.fillWidth: true
//                placeholderText: "value"
//                font.family: TableStyle.fontfamily
//                font.pixelSize: TableStyle.textpxsize
//                style: EzTextFieldStyle {}
//            }
//            Text {
//                Layout.preferredWidth: contentWidth
//                text: "kg"
//                color: TableStyle.dialog.foregroundColor
//            }
//        }

//        RowLayout {
//            Text {
//                Layout.preferredWidth: 40
//                text: "Date:"
//                horizontalAlignment: Text.AlignRight
//                color: TableStyle.dialog.foregroundColor
//            }
//            TextField {
//                Layout.fillWidth: true
//                placeholderText: "date"
//                font.family: TableStyle.fontfamily
//                font.pixelSize: TableStyle.textpxsize
//                style: EzTextFieldStyle {}
//            }
//        }
//    }

    //The measures row delegate
    Component {
        id: rowDelegate

        TableRow {
            id: row
            Component.onCompleted: console.log("row wh: " + width + " " + height)
//            width: ListView.view.width

            property variant modelData: model

            onEdit: state = "edit"
            onRemove: {
                measureListModel.removeRows(index, 1)
                flowModel.sourceModel.refresh();
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
                    text: model.value + " " + model.unit

                    color: TableStyle.cell.foregroundcolor
                }

                edit: RowLayout {
                    anchors.fill: parent
                    Loader {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        property var modelData: model

                        sourceComponent: valueDoubleDelegate
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
                    text: Qt.formatDateTime(model.sampleDate)
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
                        Binding { target: dateInput; property: "date"; value: model.sampleDate }
                        onEditingFinished: model.sampleDate = new Date(date.getFullYear(), date.getMonth(), date.getDate(), timeInput.hours, timeInput.minutes)
                    }
                    TimePicker {
                        id: timeInput
                        Binding { target: timeInput; property: "hours";   value: model.sampleDate.getHours() }
                        Binding { target: timeInput; property: "minutes"; value: model.sampleDate.getMinutes() }
                        onEditingFinished: model.sampleDate = new Date(dateInput.date.getFullYear(), dateInput.date.getMonth(), dateInput.date.getDate(), hours, minutes)
                    }
                }
            }

            states: [
                State {
                    name: "edit"

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
