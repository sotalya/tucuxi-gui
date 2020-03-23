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
    property alias currentItem: drugsTable.currentItem

    //The drugs filters
    ProxyModelFilter {
        id: nameFilter
        role: DrugListModel.NameRole
    }

    ProxyModelFilter {
        id: atcFilter
        role: DrugListModel.AtcRole
    }

    ProxyModelFilter {
        id: descFilter
        role: DrugListModel.DescriptionRole
    }

    ProxyModelFilter {
        id: studyFilter
        role: DrugListModel.StudynameRole
    }

    ProxyModelFilter {
        id: domainnameFilter
        role: DrugListModel.DomainnameRole
    }

    ProxyModelFilter {
        id: indicationFilter
        role: DrugListModel.IndicationRole
    }

    ProxyModelFilter {
        id: populationFilter
        role: DrugListModel.PopulationRole
    }

    //The drugs delegates
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

    //The drugs table
    Table {
        id: drugsTable
        anchors.fill: parent
        Component.onCompleted: console.log("table wh: " + width + " " + height)

        model: drugListModel
        delegate: rowDelegate
        filters: [nameFilter, atcFilter, descFilter, studyFilter, domainnameFilter, indicationFilter, populationFilter]

        TableHeader {
//            Layout.fillWidth: true

            headerData: ListModel {
                ListElement {
                    columnName: "Name";
                    columnRole: DrugListModel.NameRole
                }
                ListElement {
                    columnName: "ATC";
                    columnRole: DrugListModel.AtcRole
                }
//                ListElement {
//                    columnName: "Description";
//                    columnRole: DrugListModel.DescriptionRole
//                }
//                ListElement {
//                    columnName: "Study Name";
//                    columnRole: DrugListModel.StudynameRole
//                }
//                ListElement {
//                    columnName: "Domain Name";
//                    columnRole: DrugListModel.DomainnameRole
//                }
//                ListElement {
//                    columnName: "Indication";
//                    columnRole: DrugListModel.IndicationRole
//                }
//                ListElement {
//                    columnName: "Population";
//                    columnRole: DrugListModel.PopulationRole
//                }
            }

            onAdd: newDrugDialog.show()
            onSort: drugListModel.sort(role, order)
            onFilter: {
                switch (role) {
                case DrugListModel.NameRole:  nameFilterView.visible  = !nameFilterView.visible;  break;
                case DrugListModel.AtcRole: atcFilterView.visible = !atcFilterView.visible; break;
                case DrugListModel.DescriptionRole:  descFilterView.visible  = !descFilterView.visible;  break;
                case DrugListModel.StudynameRole: studyFilterView.visible  = !studyFilterView.visible;  break;
                case DrugListModel.DomainnameRole: domainFilterView.visible  = !domainFilterView.visible;  break;
                case DrugListModel.IndicationRole: indicationFilterView.visible  = !indicationFilterView.visible;  break;
                case DrugListModel.PopulationRole: populationFilterView.visible  = !populationFilterView.visible;  break;
                }
            }
        }

        TableFilter {
            id: nameFilterView
            Layout.fillWidth: true

            EzText {
                id: nameFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "Name:"
            }

            TextField {
                id: nameFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "name"
                onTextChanged: nameFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }
            onClear: nameFilterInput.text = "";
        }

        TableFilter {
            id: atcFilterView
            Layout.fillWidth: true

            EzText {
                id: atcFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "ATC:"
            }

            TextField {
                id: atcFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "atc"
                onTextChanged: atcFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: atcFilterInput.text = "";
        }

        TableFilter {
            id: descFilterView
            Layout.fillWidth: true

            EzText {
                id: descFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "Description:"
            }

            TextField {
                id: descFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "description"
                onTextChanged: descFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }
            onClear: descFilterInput.text = "";
        }

        TableFilter {
            id: studyFilterView
            Layout.fillWidth: true

            EzText {
                id: studyFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "Study:"
            }

            TextField {
                id: studyFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "study"
                onTextChanged: studyFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: studyFilterInput.text = "";
        }

        TableFilter {
            id: domainFilterView
            Layout.fillWidth: true

            EzText {
                id: domainFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "Domain:"
            }

            TextField {
                id: domainFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "domain"
                onTextChanged: domainnameFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: domainFilterInput.text = "";
        }

        TableFilter {
            id: indicationFilterView
            Layout.fillWidth: true

            EzText {
                id: indicationFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "Indication:"
            }

            TextField {
                id: indicationFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "indication"
                onTextChanged: indicationFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: indicationFilterInput.text = "";
        }

        TableFilter {
            id: populationFilterView
            Layout.fillWidth: true

            EzText {
                id: populationFilterLabel
                Layout.preferredWidth: TableStyle.cell.colwidth
                Layout.fillHeight: true
                text: "Population:"
            }

            TextField {
                id: populationFilterInput
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: "population"
                onTextChanged: populationFilter.setRegExp(text)
                font.family: TableStyle.fontfamily
                font.pixelSize: TableStyle.textpxsize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: EzTextFieldStyle {}
            }

            onClear: populationFilterInput.text = "";
        }
    }

    //The add drug dialog

//    TableDialog {
//        id: newDrugDialog
//        title: "Add drug"

//        width: 300
//        height: 160

//        onAccepted: {
//            nameFilterView.clear();
//            atcFilterView.clear();
//            descFilterView.clear();
//            studyFilterView.clear();
//            domainFilterView.clear();
//            indicationFilterView.clear();
//            dateFilterView.clear();

//            drugListModel.insertRows(drugListModel.rowCount(), 1);
//            drugsTable.currentIndex = drugsTable.count - 1;

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

    //The drugs row delegate
    Component {
        id: rowDelegate

        TableRow {
            id: row
//            Component.onCompleted: console.log("row wh: " + width + " " + height)
            ListView.onIsCurrentItemChanged: {
                if (ListView.isCurrentItem)
                    tab_drug.drugid = model.id
                    flowModel.setData(flowModel.index(1,1),model.id, FlowModel.DrugIdRole)
                }
//            width: ListView.view.width

            property variant modelData: model

            onEdit: state = "edit"
            onRemove: drugListModel.removeRows(index, 1)

            TableCell {
                id: nameCell
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
                id: atcCell
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
                    text: model.atc
                    color: TableStyle.cell.foregroundcolor
                }

                edit: RowLayout {
                    anchors.fill: parent
                    Loader {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        property var modelData: model

                        sourceComponent: TextField {
                            text: model.atc
                            style: EzTextFieldStyle {}
                        }
                    }
                }
            }

//            TableCell {
//                id: descCell
////                Layout.preferredWidth: 150
//                isCurrent: row.ListView.isCurrentItem

//                display: Text {
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.family: TableStyle.fontfamily
//                    minimumPixelSize:TableStyle.textminpxsize
//                    font.pixelSize: TableStyle.textpxsize
//                    text: model.description
//                    color: TableStyle.cell.foregroundcolor
//                }
//            }

//            TableCell {
//                id: studyCell
////                Layout.preferredWidth: 150
//                isCurrent: row.ListView.isCurrentItem

//                display: Text {
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.family: TableStyle.fontfamily
//                    minimumPixelSize:TableStyle.textminpxsize
//                    font.pixelSize: TableStyle.textpxsize
//                    text: model.studyname
//                    color: TableStyle.cell.foregroundcolor
//                }
//            }

//            TableCell {
//                id: domainCell
////                Layout.preferredWidth: 150
//                isCurrent: row.ListView.isCurrentItem

//                display: Text {
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.family: TableStyle.fontfamily
//                    minimumPixelSize:TableStyle.textminpxsize
//                    font.pixelSize: TableStyle.textpxsize
//                    text: model.domainname
//                    color: TableStyle.cell.foregroundcolor
//                }
//            }

//            TableCell {
//                id: indicationCell
////                Layout.preferredWidth: 150
//                isCurrent: row.ListView.isCurrentItem

//                display: Text {
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.family: TableStyle.fontfamily
//                    minimumPixelSize:TableStyle.textminpxsize
//                    font.pixelSize: TableStyle.textpxsize
//                    text: model.indication
//                    color: TableStyle.cell.foregroundcolor
//                }
//            }

//            TableCell {
//                id: poulationCell
////                Layout.preferredWidth: 150
//                isCurrent: row.ListView.isCurrentItem

//                display: Text {
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.family: TableStyle.fontfamily
//                    minimumPixelSize:TableStyle.textminpxsize
//                    font.pixelSize: TableStyle.textpxsize
//                    text: model.population
//                    color: TableStyle.cell.foregroundcolor
//                }
//            }

            states: [
                State {
                    name: "edit"

                    PropertyChanges { target: nameCell; editMode: true }
                    PropertyChanges { target: atcCell; editMode: true }
                    PropertyChanges { target: descCell; editMode: true }
                    PropertyChanges { target: studyCell; editMode: true }
                    PropertyChanges { target: domainCell; editMode: true }
                    PropertyChanges { target: indicationCell; editMode: true }
                    PropertyChanges { target: populationCell; editMode: true }

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
