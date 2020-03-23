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
    property alias currentItem: patientsTable.currentItem

    //The patients filters
    ProxyModelFilter {
        id: extidFilter
        role: PatientListModel.ExternalIdRole
    }

    ProxyModelFilter {
        id: staynbrFilter
        role: PatientListModel.StayNumberRole
    }

    ProxyModelFilter {
        id: statokFilter
        role: PatientListModel.StatOkRole
    }

    ProxyModelFilter {
        id: fnameFilter
        role: PersonSubmodel.FirstNameRole
    }

    ProxyModelFilter {
        id: lnameFilter
        role: PersonSubmodel.LastNameRole
    }

    ProxyModelFilter {
        id: genderFilter
        role: PersonSubmodel.GenderRole

    }

    ProxyModelFilter {
        id: fromDateFilter
        role: PersonSubmodel.BirthDateRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.GreaterOrEqual
    }

    ProxyModelFilter {
        id: toDateFilter
        role: PersonSubmodel.BirthDateRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.LessOrEqual
    }

    //The patients delegates
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

    //The patients table
    Table {
        id: patientsTable
        Component.onCompleted: console.log("table wh: " + width + " " + height)
        anchors.fill: parent

        model: patientListModel
        delegate: rowDelegate
        filters: [extidFilter, staynbrFilter, statokFilter, fnameFilter, lnameFilter, genderFilter, fromDateFilter, toDateFilter]

        TableHeader {
//            Layout.fillWidth: true

            headerData: ListModel {
                ListElement {
                    columnName: "External Id";
                    columnRole: PatientListModel.ExternalIdRole
                }
                ListElement {
                    columnName: "Stay Number";
                    columnRole: PatientListModel.StayNumberRole
                }
                ListElement {
                    columnName: "Stat Ok";
                    columnRole: PatientListModel.StatOkRole
                }
                ListElement {
                    columnName: "First Name";
                    columnRole: PatientListModel.FirstNameRole
                }
                ListElement {
                    columnName: "Last Name";
                    columnRole: PatientListModel.LastNameRole
                }
                ListElement {
                    columnName: "Gender";
                    columnRole: PatientListModel.GenderRole
                }
                ListElement {
                    columnName: "Birth Date";
                    columnRole: PatientListModel.BirthDateRole
                }
            }

            onAdd: newPatientDialog.show()
            onSort: patientListModel.sort(role, order)
            onFilter: {
                switch (role) {
                case PatientListModel.ExternalIdRole:  extidFilterView.visible  = !extidFilterView.visible;  break;
                case PatientListModel.StayNumberRole: staynbrFilterView.visible = !staynbrFilterView.visible; break;
                case PatientListModel.StatOkRole:  statokFilterView.visible  = !statokFilterView.visible;  break;
                case PersonSubmodel.BirthDateRole: dateFilterView.visible  = !dateFilterView.visible;  break;
                case PersonSubmodel.FirstNameRole: fnameFilterView.visible  = !fnameFilterView.visible;  break;
                case PersonSubmodel.LastNameRole: lnameFilterView.visible  = !lnameFilterView.visible;  break;
                case PersonSubmodel.GenderRole: genderFilterView.visible  = !genderFilterView.visible;  break;
                }
            }
        }

        TableFilter {
            id: extidFilterView
            Layout.fillWidth: true

            EzText {
                id: extidFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "External Id:"
            }

            TextField {
                id: extidFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "type"
                onTextChanged: extidFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }
            onClear: extidFilterInput.text = "";
        }

        TableFilter {
            id: staynbrFilterView
            Layout.fillWidth: true

            EzText {
                id: staynbrFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "External Id:"
            }

            TextField {
                id: staynbrFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "type"
                onTextChanged: staynbrFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: staynbrFilterInput.text = "";
        }

        TableFilter {
            id: statokFilterView
            Layout.fillWidth: true

            EzText {
                id: statokFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "External Id:"
            }

            TextField {
                id: statokFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "type"
                onTextChanged: statokFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }
            onClear: statokFilterInput.text = "";
        }

        TableFilter {
            id: fnameFilterView
            Layout.fillWidth: true

            EzText {
                id: fnameFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "First Name:"
            }

            TextField {
                id: fnameFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "type"
                onTextChanged: fnameFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: fnameFilterInput.text = "";
        }

        TableFilter {
            id: lnameFilterView
            Layout.fillWidth: true

            EzText {
                id: lnameFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "First Name:"
            }

            TextField {
                id: lnameFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "type"
                onTextChanged: lnameFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: lnameFilterInput.text = "";
        }

        TableFilter {
            id: genderFilterView
            Layout.fillWidth: true

            EzText {
                id: genderFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "First Name:"
            }

            TextField {
                id: genderFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "type"
                onTextChanged: genderFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: genderFilterInput.text = "";
        }

        TableFilter {
            id: dateFilterView
            Layout.fillWidth: true

            EzText {
                id: dateFilterLabel
                Layout.preferredWidth: Math.max(typeFilterLabel.contentWidth, valueFilterLabel.contentWidth, dateFilterLabel.contentWidth)
                text: "From:"
            }

            DatePicker {
                id: dateFilterFromDateInput
                date: new Date(1900, 0, 1);
                onDateChanged: fromDateFilter.value = new Date(date.getFullYear(), date.getMonth(), date.getDate(), dateFilterFromTimeInput.hours, dateFilterFromTimeInput.minutes)
            }

            TimePicker {
                id: dateFilterFromTimeInput
                Layout.fillWidth: true
                onDateChanged: fromDateFilter.value = new Date(dateFilterFromDateInput.date.getFullYear(), dateFilterFromDateInput.date.getMonth(), dateFilterFromDateInput.date.getDate(), hours, minutes)
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

    //The add patient dialog

//    TableDialog {
//        id: newPatientDialog
//        title: "Add patient"

//        width: 300
//        height: 160

//        onAccepted: {
//            extidFilterView.clear();
//            staynbrFilterView.clear();
//            statokFilterView.clear();
//            fnameFilterView.clear();
//            lnameFilterView.clear();
//            genderFilterView.clear();
//            dateFilterView.clear();

//            patientListModel.insertRows(patientListModel.rowCount(), 1);
//            patientsTable.currentIndex = patientsTable.count - 1;

//            close();
//        }

//        RowLayout {
//            Text {
//                Layout.preferredWidth: 40
//                text: "External Id:"
//                horizontalAlignment: Text.AlignRight
//                color: TableStyle.dialog.foregroundColor
//            }
//            ComboBox {
//                Layout.fillWidth: true
//                model: ["Age", "GIST", "Weight"]
//                style: EzComboBoxStyle {}
//            }
//        }

//        RowLayout {
//            Text {
//                Layout.preferredWidth: 40
//                text: "Stay Number:"
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

    //The patients row delegate
    Component {
        id: rowDelegate

        TableRow {
            id: row
//            Component.onCompleted: console.log("row wh: " + width + " " + height)
            ListView.onIsCurrentItemChanged: {
                if (ListView.isCurrentItem)
                    tab_patient.patientid = model.id
                    flowModel.setData(flowModel.index(1,1),model.id, FlowModel.PatientIdRole)
                }
//            width: ListView.view.width

            property variant modelData: model

            onEdit: state = "edit"
            onRemove: patientListModel.removeRows(index, 1)

            TableCell {
                id: extidCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    text: model.externalId
                    color: TableStyle.cell.foregroundcolor
                }
            }

            TableCell {
                id: staynbrCell
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
                    text: model.stayNumber;
                    color: TableStyle.cell.foregroundcolor
                }

                edit: RowLayout {
                    anchors.fill: parent
                    Loader {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        property var modelData: model

                        sourceComponent: TextField {
                            text: model.stayNumber
                            style: EzTextFieldStyle {}
                        }
                    }
                }
            }

            TableCell {
                id: statokCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    text: model.statOk
                    color: TableStyle.cell.foregroundcolor
                }
            }

            TableCell {
                id: fnameCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    text: model.firstName
                    color: TableStyle.cell.foregroundcolor
                }
            }

            TableCell {
                id: lnameCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    text: model.lastName
                    color: TableStyle.cell.foregroundcolor
                }
            }

            TableCell {
                id: genderCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    text: model.gender
                    color: TableStyle.cell.foregroundcolor
                }
            }

            TableCell {
                id: dateCell
//                Layout.fillWidth: true
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: Qt.formatDateTime(model.birthDate)
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
                        onEditingFinished: model.birthDate = new Date(date.getFullYear(), date.getMonth(), date.getDate(), timeInput.hours, timeInput.minutes)
                    }
                    TimePicker {
                        id: timeInput
                        Binding { target: timeInput; property: "hours";   value: model.birthDate.getHours() }
                        Binding { target: timeInput; property: "minutes"; value: model.birthDate.getMinutes() }
                        onEditingFinished: model.birthDate = new Date(dateInput.date.getFullYear(), dateInput.date.getMonth(), dateInput.date.getDate(), hours, minutes)
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
