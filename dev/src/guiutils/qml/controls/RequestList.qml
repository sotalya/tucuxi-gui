import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels

import guiutils.qml.controls

import ezechiel

//TODO -> Fix the request table, as Qt changed the table component

Item {
    id: root
    width: 800
    height: 600

    signal requestSelected(string requestId, string patientId, string drugId);

    function enable() {
        visible = true;
    }

    Rectangle {
        id: container
        anchors.fill: parent
    }

    ColumnLayout {
        anchors.fill: container
        anchors.margins: 5
        spacing: 5

        Rectangle {
            id: filtersContainer
            Layout.fillWidth: true
            Layout.minimumHeight: 48
            Layout.maximumHeight: 48

            border.color: "DarkGray"
            radius: 2

            ColumnLayout {
                anchors.fill: filtersContainer
                anchors.margins: 2

                RowLayout {
                    id: filterTopRow
                    Layout.minimumHeight: 30
                    Layout.maximumHeight: 30
                    Layout.margins: 2
                    Layout.alignment: Qt.AlignTop

                    Text {
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: "Drug:"
                    }

                    TextField {
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        placeholderText: "drug"
                        onTextChanged: drugFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: "Sample satus:"
                    }

                    ComboBox {
                        id: abnormalSampleCombo
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 1005
                        textRole: "text"
                        valueRole: "value"
                        model: ListModel {
                            id: abnormalSampleModel
                            ListElement { text: "All";      value: "" }
                            ListElement { text: "Normal";   value: "false" }
                            ListElement { text: "Abnormal"; value: "true" }
                        }
                        onCurrentIndexChanged: abnormalSampleFilter.setRegExp(abnormalSampleModel.get(currentIndex).value)
                    }

                    Text {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        horizontalAlignment: Text.Center
                        verticalAlignment: Text.AlignVCenter
                        text: "refresh..."
                        color: "blue"
                        font.underline: true

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                requestListController.queryList()
                                requestView.model = requestListModel
                            }
                        }
                    }

                    Text {

                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        horizontalAlignment: Text.Center
                        verticalAlignment: Text.AlignVCenter
                        text: "more filters..."
                        color: "blue"
                        font.underline: true

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (parent.text == "more filters...") {
                                    parent.text = "less filters..."
                                    filtersContainer.Layout.minimumHeight = 145
                                    filtersContainer.Layout.maximumHeight = 145

                                    filterMidRow.opacity = 1
                                    filterBottomRow.opacity = 1
                                } else {
                                    parent.text = "more filters..."
                                    filtersContainer.Layout.minimumHeight = 48
                                    filtersContainer.Layout.maximumHeight = 48

                                    filterMidRow.opacity = 0
                                    filterBottomRow.opacity = 0
                                }
                            }
                        }
                    }
                }

                RowLayout {
                    id: filterMidRow
                    Layout.minimumHeight: 30
                    Layout.maximumHeight: 30
                    Layout.alignment: Qt.AlignTop

                    opacity: 0

                    Text {
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: "ID:"
                    }

                    TextField {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        verticalAlignment: Text.AlignVCenter
                        placeholderText : "request id"
                        onTextChanged: requestIdFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        text: "First Name:"
                    }

                    TextField {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        verticalAlignment: Text.AlignVCenter
                        placeholderText : "first name"
                        onTextChanged: firstNameFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        text: "Last Name:"
                    }

                    TextField {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        verticalAlignment: Text.AlignVCenter
                        placeholderText : "last name"
                        onTextChanged: lastNameFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: "Institute:"
                    }

                    TextField {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
                        verticalAlignment: Text.AlignVCenter
                        placeholderText : "institute"
                        onTextChanged: instituteFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: "Status:"
                    }

                    TextField {
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        placeholderText : "status"
                        onTextChanged: statusFilter.setRegExp(text)
                    }
                }

                RowLayout {
                    id: filterBottomRow
                    Layout.minimumHeight: 30
                    Layout.maximumHeight: 30
                    Layout.alignment: Qt.AlignTop

                    opacity: 0

                    EntityLabel {
                        text: "From:"
                    }
                    DatePicker {
                        id: fromDateInput
                        objectName: "fromDateInput"
                        Layout.fillWidth: true
                        onEditingFinished: {
                            fromDateFilter.value = new Date(date.getFullYear(), date.getMonth(), date.getDate())
                        }
                    }

                    EntityLabel {
                        text: "To:"
                    }
                    DatePicker {
                        id: toDateInput
                        objectName: "toDateInput"
                        Layout.fillWidth: true
                        onEditingFinished: {
                            toDateFilter.value = new Date(date.getFullYear(), date.getMonth(), date.getDate())
                        }
                    }
                }
            }

            ProxyModelFilter {
                id: requestIdFilter
                role: PartialRequestListModel.RequestIdRole
            }

            ProxyModelFilter {
                id: firstNameFilter
                role: PartialRequestListModel.FirstNameRole
            }

            ProxyModelFilter {
                id: lastNameFilter
                role: PartialRequestListModel.LastNameRole
            }

            ProxyModelFilter {
                id: instituteFilter
                role: PartialRequestListModel.InstituteRole
            }

            ProxyModelFilter {
                id: drugFilter
                role: PartialRequestListModel.DrugRole
            }

            ProxyModelFilter {
                id: statusFilter
                role: PartialRequestListModel.RequestCpclRole
            }

            ProxyModelFilter {
                id: fromDateFilter
                role: PartialRequestListModel.DateRole
                value: Qt.formatDateTime(Date())
                operation: ProxyModelFilter.GreaterOrEqual
            }

            ProxyModelFilter {
                id: toDateFilter
                role: PartialRequestListModel.DateRole
                value: Qt.formatDateTime(Date())
                operation: ProxyModelFilter.LessOrEqual
            }

            ProxyModelFilter {
                id: abnormalSampleFilter
                role: PartialRequestListModel.AlarmRole
            }

            Component.onCompleted: {
                requestListModel.setFilter(requestIdFilter);
                requestListModel.setFilter(firstNameFilter);
                requestListModel.setFilter(lastNameFilter);
                requestListModel.setFilter(instituteFilter);
                requestListModel.setFilter(drugFilter);
                requestListModel.setFilter(statusFilter);
                requestListModel.setFilter(fromDateFilter);
                requestListModel.setFilter(toDateFilter);
                requestListModel.setFilter(abnormalSampleFilter);
            }
        }

        Rectangle {
            id: viewContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: "DarkGray"
            radius: 2

            HorizontalHeaderView {
                id: horizontalHeader
                anchors.left: requestView.left
                anchors.top: viewContainer.top
                syncView: requestView
                clip: true
                model: PartialRequestListModel {}
            }

            TableView {
                id: requestView
                objectName: "requestView"
                anchors.top: horizontalHeader.bottom
                anchors.left: viewContainer.left
                anchors.right: viewContainer.right
                anchors.bottom: viewContainer.bottom
                anchors.margins: 2

                property int selectedRow: -1

                columnSpacing : 1
                rowSpacing: 1
                property real cellSize: (requestView.width / 14)
                property var columnWidths: [cellSize, cellSize*3, cellSize*2, cellSize*2, cellSize*2, cellSize*3, cellSize-7]

                model: PartialRequestListModel {}

                columnWidthProvider : function (column) { return columnWidths[column] }

                alternatingRows: false
                interactive: false

                ScrollBar.vertical: ScrollBar {}



                delegate: DelegateChooser{
                    DelegateChoice{
                        delegate: Rectangle {
                            implicitHeight: 25
                            required property int selectedRow
                            selectedRow: -1

                            color: {
                                console.log(requestView.selectedRow)
                                console.log(row)
                                row === requestView.selectedRow ? "#5BBCDD" : "white"
                            }

                            Text {
                                verticalAlignment: Text.AlignVCenter
                                text: {
                                    var col = model.column
                                    if(col === 0){
                                        return model.requestId
                                    }else if(col === 1){
                                        return model.patientFirstName + " " + model.patientLastName
                                    }else if(col === 2){
                                        return model.institute
                                    }else if(col === 3){
                                        return model.drug
                                    }else if(col === 4){
                                        return (model.sampleValue === 0.0) ? "" : model.sampleValue + " " + model.sampleUnit
                                    }else if(col === 5){
                                        return (model.sampleValue === 0.0) ? "" : model.sampleDate
                                    }else if(col === 6){
                                        return model.requestCpcl
                                    }
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked:
                                {
                                    // print values from all cells in a row
                                    console.log("req: " + model.requestId + " pat: " + model.patientId + " drug: " + model.drugId)
                                    root.requestSelected(model.requestId, model.patientId, model.drugId);
                                }
                                onEntered: requestView.selectedRow = row
                            }
                        }
                    }
                }

                //onSortIndicatorColumnChanged: sort()
                //onSortIndicatorOrderChanged: sort()

                function sort() {
                    switch (sortIndicatorColumn) {
                    case 0: requestListModel.sort(PartialRequestListModel.RequestIdRole, sortIndicatorOrder); break;
                    case 1: requestListModel.sort(PartialRequestListModel.FirstNameRole, sortIndicatorOrder); break;
                    case 2: requestListModel.sort(PartialRequestListModel.InstituteRole, sortIndicatorOrder); break;
                    case 3: requestListModel.sort(PartialRequestListModel.DrugRole, sortIndicatorOrder); break;
                    case 4: requestListModel.sort(PartialRequestListModel.ValueRole, sortIndicatorOrder); break;
                    case 5: requestListModel.sort(PartialRequestListModel.DateRole, sortIndicatorOrder); break;
                    case 6: requestListModel.sort(PartialRequestListModel.RequestCpclRole, sortIndicatorOrder); break;
                    }
                }
            }
        }
    }
}
