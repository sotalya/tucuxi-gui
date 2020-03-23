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
    property alias currentItem: targetsTable.currentItem

    //The targets filters
    ProxyModelFilter {
        id: typeFilter
        role: TargetListModel.TypeRole
    }

    ProxyModelFilter {
        id: cminFilter
        role: TargetListModel.CminRole
    }

    ProxyModelFilter {
        id: cmaxFilter
        role: TargetListModel.CmaxRole
    }

    ProxyModelFilter {
        id: cbestFilter
        role: TargetListModel.CbestRole
    }

    ProxyModelFilter {
        id: drugIdFilter
        role: TargetListModel.DrugIdRole
        value: tab_drug.drugid
    }

    ProxyModelFilter {
        id: fromTminFilter
        role: TargetListModel.TminRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.GreaterOrEqual
    }

    ProxyModelFilter {
        id: toTminFilter
        role: TargetListModel.TminRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.LessOrEqual
    }

    ProxyModelFilter {
        id: fromTmaxFilter
        role: TargetListModel.TmaxRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.GreaterOrEqual
    }

    ProxyModelFilter {
        id: toTmaxFilter
        role: TargetListModel.TmaxRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.LessOrEqual
    }

    ProxyModelFilter {
        id: fromTbestFilter
        role: TargetListModel.TbestRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.GreaterOrEqual
    }

    ProxyModelFilter {
        id: toTbestFilter
        role: TargetListModel.TbestRole
        value: Qt.formatDateTime(Date())
        operation: ProxyModelFilter.LessOrEqual
    }

    //The targets delegates
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

    Table {
        id: targetsTable
        Component.onCompleted: console.log("table wh: " + width + " " + height)
        anchors.fill: parent

        model: targetListModel
        delegate: rowDelegate
        filters: [typeFIlter, cminFilter, cmaxFilter, cbestFilter, drugIdFilter, fromTminFilter, toTminFilter, fromTmaxFilter, toTmaxFilter, fromTbestFilter, toTbestFilter]

        TableHeader {
//            Layout.fillWidth: true

            headerData: ListModel {
                ListElement {
                    columnName: "Target Type";
                    columnRole: TargetListModel.TypeRole
                }
                ListElement {
                    columnName: "Cmin";
                    columnRole: TargetListModel.CminRole
                }
                ListElement {
                    columnName: "Cmax";
                    columnRole: TargetListModel.CmaxRole
                }
                ListElement {
                    columnName: "Cbest";
                    columnRole: TargetListModel.CbestRole
                }
                ListElement {
                    columnName: "Tmin";
                    columnRole: TargetListModel.TminRole
                }
                ListElement {
                    columnName: "Tmax";
                    columnRole: TargetListModel.TmaxRole
                }
                ListElement {
                    columnName: "Tbest";
                    columnRole: TargetListModel.TbestRole
                }
                ListElement {
                    columnName: "DrugId";
                    columnRole: TargetListModel.DrugIdRole
                }
            }

            onAdd: {
                targetListModel.sourceModel.insertRows(targetListModel.sourceModel.rowCount(), 1);
                console.log("target added to table of count: " + targetListModel.sourceModel.rowCount());
                targetListModel.sourceModel.setData(targetListModel.sourceModel.index(targetListModel.sourceModel.rowCount()-1,0),drugIdFilter.value, TargetListModel.DrugIdRole)
                console.log("target drugid set");
                targetsTable.currentIndex = targetsTable.count - 1;
                flowModel.sourceModel.refresh()
            }
            onSort: targetListModel.sort(role, order)
            onFilter: {
                switch (role) {
                }
            }
        }

        TableFilter {
            id: typeFilterView
            Layout.fillWidth: true

            EzText {
                id: extidFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "Target Type:"
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
    }

    //The add target dialog

//    TableDialog {
//        id: newTargetDialog
//        title: "Add target"

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

//            targetListModel.insertRows(targetListModel.rowCount(), 1);
//            targetsTable.currentIndex = targetsTable.count - 1;

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

    //The targets row delegate
    Component {
        id: rowDelegate

        TableRow {
            id: row
            Component.onCompleted: console.log("row wh: " + width + " " + height)
//            ListView.onIsCurrentItemChanged: {
//                if (ListView.isCurrentItem) {
//                    tab_target.targetid = model.id
//                    flowModel.setData(flowModel.index(1,1),model.id, FlowModel.TargetIdRole)
//                }
//            width: ListView.view.width

            property variant modelData: model

            onEdit: state = "edit"
            onRemove: targetListModel.removeRows(index, 1)

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
                    text: model.type
                    color: TableStyle.cell.foregroundcolor
                }
            }

            TableCell {
                id: cminCell
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
                    text: model.cmin;
                    color: TableStyle.cell.foregroundcolor
                }

                edit: RowLayout {
                    anchors.fill: parent
                    Loader {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        property var modelData: model

                        sourceComponent: TextField {
                            text: model.cmin
                            style: EzTextFieldStyle {}
                        }
                    }
                }
            }

            TableCell {
                id: cmaxCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    text: model.cmax
                    color: TableStyle.cell.foregroundcolor
                }
            }

            TableCell {
                id: cbestCell
//                Layout.preferredWidth: 150
                isCurrent: row.ListView.isCurrentItem

                display: Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: TableStyle.fontfamily
                    minimumPixelSize:TableStyle.textminpxsize
                    font.pixelSize: TableStyle.textpxsize
                    text: model.cbest
                    color: TableStyle.cell.foregroundcolor
                }
            }


//            TableCell {
//                id: tminCell
////                Layout.fillWidth: true
//                isCurrent: row.ListView.isCurrentItem

//                display: Text {
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    text: Qt.formatDateTime(model.birthDate)
//                    color: TableStyle.cell.foregroundcolor
//                    font.family: TableStyle.fontfamily
//                    minimumPixelSize:TableStyle.textminpxsize
//                    font.pixelSize: TableStyle.textpxsize
//                    anchors.fill: parent
//                }

//                edit: RowLayout {
//                    anchors.fill: parent

//                    DatePicker {
//                        id: dateInput
//                        Binding { target: dateInput; property: "date"; value: model.date }
//                        onEditingFinished: model.birthDate = new Date(date.getFullYear(), date.getMonth(), date.getDate(), timeInput.hours, timeInput.minutes)
//                    }
//                    TimePicker {
//                        id: timeInput
//                        Binding { target: timeInput; property: "hours";   value: model.birthDate.getHours() }
//                        Binding { target: timeInput; property: "minutes"; value: model.birthDate.getMinutes() }
//                        onEditingFinished: model.birthDate = new Date(dateInput.date.getFullYear(), dateInput.date.getMonth(), dateInput.date.getDate(), hours, minutes)
//                    }
//                }
//            }

            states: [
                State {
                    name: "edit"

//                    PropertyChanges { target: typeCell; editMode: true }
//                    PropertyChanges { target: valueCell; editMode: true }
//                    PropertyChanges { target: dateCell; editMode: true }

//                    PropertyChanges { target: row; editText: "Done" }
//                    PropertyChanges { target: row; onEdit: row.state = "" }
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
