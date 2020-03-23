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
    property alias currentItem: dosagesTable.currentItem

    //The dosages filters
    ProxyModelFilter {
        id: patientIdFilter
        role: DosageListModel.PatientIdRole
        value: tab_patient.patientid
    }

    ProxyModelFilter {
        id: drugIdFilter
        role: DosageListModel.DrugIdRole
        value: tab_drug.drugid
    }

    ProxyModelFilter {
        id: takenFilter
        role: DosageListModel.TakenRole
        value: true
    }

    ProxyModelFilter {
        id: predictionIdFilter
        role: DosageListModel.PredictionIdRole
        value: flowModel.data(flowModel.index(0,0), FlowModel.PredictionIdRole)
//        onValueChanged: console.log("flowmodel predictionid: " + value)
    }

    ProxyModelFilter {
        id: routeFilter
        role: DosageListModel.RouteRole
    }

    ProxyModelFilter {
        id: valueFilter
        role: DosageListModel.ValueRole
    }

    ProxyModelFilter {
        id: fromAppliedFilter
        role: DosageListModel.AppliedRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.GreaterOrEqual
    }

    ProxyModelFilter {
        id: toAppliedFilter
        role: DosageListModel.AppliedRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.LessOrEqual
    }

    ProxyModelFilter {
        id: fromEndtimeFilter
        role: DosageListModel.EndtimeRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.GreaterOrEqual
    }

    ProxyModelFilter {
        id: toEndtimeFilter
        role: DosageListModel.EndtimeRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.LessOrEqual
    }
    //The dosages delegates
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

    //The dosages table
    Table {
        id: dosagesTable
        Component.onCompleted: console.log("table wh: " + width + " " + height)
        anchors.fill: parent

        model: dosageListModel
        delegate: rowDelegate
        filters: [valueFilter, patientIdFilter, drugIdFilter, takenFilter, fromAppliedFilter, toAppliedFilter, fromEndtimeFilter, toEndtimeFilter]

        TableHeader {
            id: thdr_dosage
//            Layout.fillWidth: true

            headerData: ListModel {
                ListElement {
                    columnName: "Value";
                    columnWidth: 150;
                    columnRole: DosageListModel.ValueRole
                }
                ListElement {
                    columnName: "Start Date";
                    columnWidth: 150;
                    columnRole: DosageListModel.AppliedRole
                }
                ListElement {
                    columnName: "End Date";
                    columnWidth: 150;
                    columnRole: DosageListModel.EndtimeRole
                }
                ListElement {
                    columnName: "Route";
                    columnWidth: 150;
                    columnRole: DosageListModel.RouteRole
                }
            }

            onAdd: {
                dosageListModel.insertRows(dosageListModel.rowCount(), 1);
//                dosageListModel.setData(dosageListModel.index(dosageListModel.rowCount(),0),predictionIdFilter.value, DosageListModel.PredictionIdRole)
                dosagesTable.currentIndex = dosagesTable.count - 1;
                flowModel.sourceModel.refresh()
            }
                //newDosageDialog.show()
            onSort: dosageListModel.sort(role, order)
            onFilter: {
                switch (role) {
                case DosageListModel.ValueRole: valueFilterView.visible = !valueFilterView.visible; break;
                case DosageListModel.AppliedRole:  appliedFilterView.visible  = !appliedFilterView.visible;  break;
                case DosageListModel.EndtimeRole:  endtimeFilterView.visible  = !endtimeFilterView.visible;  break;
                case DosageListModel.RouteRole:  routeFilterView.visible  = !routeFilterView.visible;  break;
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
            id: appliedFilterView
            Layout.fillWidth: true

            Text {
                id: appliedFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                verticalAlignment: Text.AlignVCenter
                color: TableStyle.filter.foregroundColor
                text: "From:"
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
            }

            DatePicker {
                id: appliedFilterFromDateInput
                date: new Date(1900, 0, 1);
                onDateChanged: fromAppliedFilter.value = new Date(date.getFullYear(), date.getMonth(), date.getDate(), appliedFilterFromTimeInput.hours, appliedFilterFromTimeInput.minutes)
            }

            TimePicker {
                id: appliedFilterFromTimeInput
                Layout.fillWidth: true
                onDateChanged: fromAppliedFilter.value = new Date(appliedFilterFromDateInput.date.getFullYear(), appliedFilterFromDateInput.date.getMonth(), appliedFilterFromDateInput.date.getDate(), hours, minutes)
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
                id: appliedFilterToDateInput
                date: new Date(2100, 0, 1);
                onDateChanged: toAppliedFilter.value = new Date(date.getFullYear(), date.getMonth(), date.getDate(), appliedFilterToTimeInput.hours, appliedFilterToTimeInput.minutes)
            }

            TimePicker {
                id: appliedFilterToTimeInput
                Layout.fillWidth: true
                onDateChanged: toAppliedFilter.value = new Date(appliedFilterToDateInput.date.getFullYear(), appliedFilterToDateInput.date.getMonth(), appliedFilterToDateInput.date.getDate(), hours, minutes)
            }

            onClear: {
                appliedFilterFromDateInput.date = new Date(1900, 0, 1);
                appliedFilterToDateInput.date   = new Date(2100, 0, 1);
                appliedFilterFromTimeInput.hours = 0;
                appliedFilterToTimeInput.hours   = 0;
                appliedFilterFromTimeInput.minutes = 0
                appliedFilterToTimeInput.minutes   = 0
            }
        }

        TableFilter {
            id: endtimeFilterView
            Layout.fillWidth: true

            Text {
                id: endtimeFilterLabel
                Layout.preferredWidth: Math.max(routeFilterLabel.contentWidth, valueFilterLabel.contentWidth, endtimeFilterLabel.contentWidth)
                verticalAlignment: Text.AlignVCenter
                color: TableStyle.filter.foregroundColor
                text: "From:"
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
            }

            DatePicker {
                id: endtimeFilterFromDateInput
                date: new Date(1900, 0, 1);
                onDateChanged: fromEndtimeFilter.value = new Date(date.getFullYear(), date.getMonth(), date.getDate(), endtimeFilterFromTimeInput.hours, endtimeFilterFromTimeInput.minutes)
            }

            TimePicker {
                id: endtimeFilterFromTimeInput
                Layout.fillWidth: true
                onDateChanged: fromEndtimeFilter.value = new Date(endtimeFilterFromDateInput.date.getFullYear(), endtimeFilterFromDateInput.date.getMonth(), endtimeFilterFromDateInput.date.getDate(), hours, minutes)
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
                id: endtimeFilterToDateInput
                date: new Date(2100, 0, 1);
                onDateChanged: toEndtimeFilter.value = new Date(date.getFullYear(), date.getMonth(), date.getDate(), endtimeFilterToTimeInput.hours, endtimeFilterToTimeInput.minutes)
            }

            TimePicker {
                id: endtimeFilterToTimeInput
                Layout.fillWidth: true
                onDateChanged: toEndtimeFilter.value = new Date(endtimeFilterToDateInput.date.getFullYear(), endtimeFilterToDateInput.date.getMonth(), endtimeFilterToDateInput.date.getDate(), endtimeFilterFromTimeInput.hours, endtimeFilterToTimeInput.minutes)
            }

            onClear: {
                endtimeFilterFromDateInput.date = new Date(1900, 0, 1);
                endtimeFilterToDateInput.date   = new Date(2100, 0, 1);
                endtimeFilterFromTimeInput.hours = 0;
                endtimeFilterToTimeInput.hours   = 0;
                endtimeFilterFromTimeInput.minutes = 0
                endtimeFilterToTimeInput.minutes   = 0
            }
        }

        TableFilter {
            id: routeFilterView
            Layout.fillWidth: true

            Text {
                id: routeFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "Route:"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: TableStyle.filter.foregroundColor
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
            }

            TextField {
                id: routeFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "route"
                onTextChanged: routeFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: routeFilterInput.text = "";
        }
    }

    //The add dosage dialog

//    TableDialog {
//        id: newDosageDialog
//        title: "Add dosage"

//        width: 300
//        height: 160

//        onAccepted: {
//            valueFilterView.clear();
//            appliedFilterView.clear();
//            endtimeFilterView.clear();
//            routeFilterView.clear();

//            dosageListModel.insertRows(dosageListModel.rowCount(), 1);
//            dosagesTable.currentIndex = dosagesTable.count - 1;

//            //ToDo: set the dosages patient (need to update the model)

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
//                text: "Start Date:"
//                horizontalAlignment: Text.AlignRight
//                color: TableStyle.dialog.foregroundColor
//            }
//            TextField {
//                Layout.fillWidth: true
//                placeholderText: "start date"
//                font.family: TableStyle.fontfamily
//                font.pixelSize: TableStyle.textpxsize
//                style: EzTextFieldStyle {}
//            }
//        }

//        RowLayout {
//            Text {
//                Layout.preferredWidth: 40
//                text: "End Date:"
//                horizontalAlignment: Text.AlignRight
//                color: TableStyle.dialog.foregroundColor
//            }
//            TextField {
//                Layout.fillWidth: true
//                placeholderText: "end date"
//                font.family: TableStyle.fontfamily
//                font.pixelSize: TableStyle.textpxsize
//                style: EzTextFieldStyle {}
//            }
//        }

//        RowLayout {
//            Text {
//                Layout.preferredWidth: 40
//                text: "Route:"
//                horizontalAlignment: Text.AlignRight
//                color: TableStyle.dialog.foregroundColor
//            }
//            ComboBox {
//                Layout.fillWidth: true
////                model: ["Age", "GIST", "Weight"]
//                model: drugVariateListModel.names
//                style: EzComboBoxStyle {}
//            }
//        }
//    }

    //The dosages row delegate
    Component {
        id: rowDelegate

        TableRow {
            id: row
            Component.onCompleted: console.log("row wh: " + width + " " + height)
//            width: ListView.view.width

            property variant modelData: model

            onEdit: state = "edit"
            onRemove: {
                dosageListModel.removeRows(index, 1)
                flowModel.sourceModel.refresh()
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
                id: appliedCell
//                Layout.fillWidth: true
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: Qt.formatDateTime(model.applied)
                    color: TableStyle.cell.foregroundcolor
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    anchors.fill: parent
                }

                edit: RowLayout {
                    anchors.fill: parent

                    DatePicker {
                        id: adateInput
                        Binding { target: adateInput; property: "date"; value: model.applied }
                        onEditingFinished: model.applied = new Date(date.getFullYear(), date.getMonth(), date.getDate(), timeInput.hours, timeInput.minutes)
                    }
                    TimePicker {
                        id: atimeInput
                        Binding { target: atimeInput; property: "hours";   value: model.applied.getHours() }
                        Binding { target: atimeInput; property: "minutes"; value: model.applied.getMinutes() }
                        onEditingFinished: model.applied = new Date(adateInput.date.getFullYear(), adateInput.date.getMonth(), adateInput.date.getDate(), hours, minutes)
                    }
                }
            }

            TableCell {
                id: endtimeCell
//                Layout.fillWidth: true
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: Qt.formatDateTime(model.endtime)
                    color: TableStyle.cell.foregroundcolor
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    anchors.fill: parent
                }

                edit: RowLayout {
                    anchors.fill: parent

                    DatePicker {
                        id: edateInput
                        Binding { target: edateInput; property: "date"; value: model.endtime}
                        onEditingFinished: model.endtime = new Date(date.getFullYear(), date.getMonth(), date.getDate(), etimeInput.hours, etimeInput.minutes)
                    }
                    TimePicker {
                        id: etimeInput
                        Binding { target: etimeInput; property: "hours";   value: model.endtime.getHours() }
                        Binding { target: etimeInput; property: "minutes"; value: model.endtime.getMinutes() }
                        onEditingFinished: model.endtime = new Date(edateInput.date.getFullYear(), edateInput.date.getMonth(), edateInput.date.getDate(), hours, minutes)
                    }
                }
            }

            TableCell {
                id: routeCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    text: model.route
                    color: TableStyle.cell.foregroundcolor
                }
            }

            states: [
                State {
                    name: "edit"

                    PropertyChanges { target: valueCell; editMode: true }
                    PropertyChanges { target: appliedCell; editMode: true }
                    PropertyChanges { target: endtimeCell; editMode: true }
                    PropertyChanges { target: routeCell; editMode: true }

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
