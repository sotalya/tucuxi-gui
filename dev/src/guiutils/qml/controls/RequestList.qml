import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import guiutils.qml.controls

import ezechiel 1.0

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
            Layout.minimumHeight: 35
            Layout.maximumHeight: 35

            border.color: "DarkGray"
            radius: 2

            ColumnLayout {
                anchors.fill: filtersContainer
                anchors.margins: 2

                RowLayout {
                    Layout.minimumHeight: 30
                    Layout.maximumHeight: 30
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5

                    Text {
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        horizontalAlignment: Text.AlignRight
                        text: "Drug:"
                    }

                    TextField {
                        Layout.fillWidth: true
                        placeholderText: "drug"
                        onTextChanged: drugFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
                        horizontalAlignment: Text.AlignRight
                        text: "Sample satus:"
                    }

                    ComboBox {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
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
                        text: "refresh..."
                        color: "blue"
                        font.underline: true

                        MouseArea {
                            anchors.fill: parent
                            onClicked: requestListController.queryList()
                        }
                    }

                    Text {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        horizontalAlignment: Text.Center
                        text: "more filters..."
                        color: "blue"
                        font.underline: true

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (parent.text == "more filters...") {
                                    parent.text = "less filters..."
                                    filtersContainer.Layout.minimumHeight = 105
                                    filtersContainer.Layout.maximumHeight = 105
                                } else {
                                    parent.text = "more filters..."
                                    filtersContainer.Layout.minimumHeight = 35
                                    filtersContainer.Layout.maximumHeight = 35
                                }
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.minimumHeight: 30
                    Layout.maximumHeight: 30
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5

                    Text {
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        horizontalAlignment: Text.AlignRight
                        text: "ID:"
                    }

                    TextField {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        placeholderText : "request id"
                        onTextChanged: requestIdFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        horizontalAlignment: Text.AlignRight
                        text: "First Name:"
                    }

                    TextField {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        placeholderText : "first name"
                        onTextChanged: firstNameFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        horizontalAlignment: Text.AlignRight
                        text: "Last Name:"
                    }

                    TextField {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        placeholderText : "last name"
                        onTextChanged: lastNameFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 75
                        Layout.maximumWidth: 75
                        horizontalAlignment: Text.AlignRight
                        text: "Institute:"
                    }

                    TextField {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
                        placeholderText : "institute"
                        onTextChanged: instituteFilter.setRegExp(text)
                    }

                    Text {
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        horizontalAlignment: Text.AlignRight
                        text: "Status:"
                    }

                    TextField {
                        Layout.fillWidth: true
                        placeholderText : "status"
                        onTextChanged: statusFilter.setRegExp(text)
                    }
                }

                RowLayout {
                    Layout.minimumHeight: 30
                    Layout.maximumHeight: 30
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5

                    Text {
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        horizontalAlignment: Text.AlignRight
                        text: "From:"
                    }

                    TextField {
                        id: fromDate
                        Layout.fillWidth: true
                        placeholderText : "minimum date"
                        onTextChanged: fromDateFilter.value = text === "" ? Date() : fromDatePicker.item.date

                        Loader {
                            id: fromDatePicker
                            source: "DatePicker.qml"
                        }

                        Connections {
                            target: fromDatePicker.item
                            // function onEditingFinished()
                            onEditingFinished: {
                                fromDate.text = Qt.formatDateTime(date, Qt.SystemLocaleShortDate)
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: fromDatePicker.item.show()
                        }
                    }

                    Text {
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        horizontalAlignment: Text.AlignRight
                        text: "To:"
                    }

                    TextField {
                        id: toDate
                        Layout.fillWidth: true
                        placeholderText : "maximum date"
                        onTextChanged: toDateFilter.value = text === "" ? Date() : toDatePicker.item.date

                        Loader {
                            id: toDatePicker
                            source: "DatePicker.qml"
                        }

                        Connections {
                            target: toDatePicker.item
                            // function onEditingFinished()
                            onEditingFinished: {
                                toDate.text = Qt.formatDateTime(date, Qt.SystemLocaleShortDate)
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: toDatePicker.item.show()
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

            TableView {
                id: requestView
                objectName: "requestView"
                anchors.fill: viewContainer
                anchors.margins: 2

                frameVisible: false
                alternatingRowColors: false
                sortIndicatorVisible: true

                model: requestListModel

                //                onDoubleClicked: {        // easier to single click from Gui tests
                clicked: {
                    var requestId = requestListModel.data(requestListModel.index(row, 0), PartialRequestListModel.RequestIdRole);
                    var patientId = requestListModel.data(requestListModel.index(row, 0), PartialRequestListModel.PatientIdRole);
                    var drugId    = requestListModel.data(requestListModel.index(row, 0), PartialRequestListModel.DrugIdRole);
                    root.requestSelected(requestId, patientId, drugId);

                    console.log(requestListModel.index(row, 0))
                }

                onSortIndicatorColumnChanged: sort()
                onSortIndicatorOrderChanged: sort()

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

                TableViewColumn {
                    title: "ID"
                    width: 50
                    delegate: Component {
                        Text {
                            text: model.requestId
                            color: model.color
                            elide: styleData.elideMode
                        }
                    }
                }
                TableViewColumn {
                    title: "Patient"
                    width: 150
                    delegate: Component {
                        Text {
                            text: model.patientFirstName + " " + model.patientLastName
                            color: model.color
                            elide: styleData.elideMode
                        }
                    }
                }
                TableViewColumn {
                    title: "Institute"
                    width: 100
                    delegate: Component {
                        Text {
                            text: model.institute
                            color: model.color
                            elide: styleData.elideMode
                        }
                    }
                }
                TableViewColumn {
                    title: "Drug"
                    width: 100
                    delegate: Component {
                        Text {
                            text: model.drug
                            color: model.color
                            elide: styleData.elideMode
                        }
                    }
                }
                TableViewColumn {
                    title: "Last sample value"
                    width: 100
                    delegate: Component {
                        Text {
                            text: (model.sampleValue === 0.0) ? "" : model.sampleValue + " " + model.sampleUnit
                            color: model.color
                            elide: styleData.elideMode
                        }
                    }
                }
                TableViewColumn {
                    title: "Sample date"
                    width: 150
                    delegate: Component {
                        Text {
                            text: (model.sampleValue === 0.0) ? "" : model.sampleDate
                            color: model.color
                            elide: styleData.elideMode
                        }
                    }
                }
                TableViewColumn {
                    title: "Status"
                    width: 50
                    delegate: Component {
                        Text {
                            text: model.requestCpcl
                            color: model.color
                            elide: styleData.elideMode
                        }
                    }
                }
            }
        }
    }
}
