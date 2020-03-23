import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import ezechiel 1.0

Item {
    id: root
    width: 800
    height: 600

    MouseArea {
        anchors.fill: parent
        onClicked: parent.forceActiveFocus()
    }

    ListView {
        id: measuresView
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        width: parent.width / 2 - 20
        spacing: 5
        clip: true

        model: measureListModel
        delegate: measureDelegate
        header: measureHeader
        footer: measureFooter
    }

    Component {
        id: measureHeader

        Item {
            width: ListView.view.width
            height: 85

            Rectangle {
                id: headerContainer
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.left: parent.left
                anchors.right: parent.right
                width: parent.width
                height: parent.height - 5
                radius: 10

                color: "LightSteelBlue"
                border.color: Qt.darker(color)
            }

            ProxyModelFilter {
                id: sampleIdFilter
                role: MeasureListModel.SampleIdRole
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

            ColumnLayout {
                anchors.fill: headerContainer
                anchors.margins: 5

                RowLayout {
                    Text {
                        Layout.minimumWidth: contentWidth
                        Layout.maximumWidth: contentWidth
                        text: "Sample ID:"
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText : "filter"
                        onTextChanged: sampleIdFilter.setRegExp(text)
                    }
                }
                RowLayout {
                    Text {
                        Layout.minimumWidth: contentWidth
                        Layout.maximumWidth: contentWidth
                        text: "From:"
                    }
                    TextField {
                        id: fromDate
                        Layout.fillWidth: true
                        placeholderText : "Minimum date"
                        onTextChanged: fromDateFilter.value = text === "" ? Date() : fromDatePicker.item.date

                        Loader {
                            id: fromDatePicker
                            source: "DatePicker.qml"
                        }

                        Connections {
                            target: fromDatePicker.item
                            onDateChosen: fromDate.text = Qt.formatDateTime(date, Qt.SystemLocaleShortDate)
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: fromDatePicker.item.show()
                        }
                    }
                    Text {
                        Layout.minimumWidth: contentWidth
                        Layout.maximumWidth: contentWidth
                        text: "To:"
                    }
                    TextField {
                        id: toDate
                        Layout.fillWidth: true
                        placeholderText : "Maximum date"
                        onTextChanged: toDateFilter.value = text === "" ? Date() : toDatePicker.item.date

                        Loader {
                            id: toDatePicker
                            source: "DatePicker.qml"
                        }

                        Connections {
                            target: toDatePicker.item
                            onDateChosen: toDate.text = Qt.formatDateTime(date, Qt.SystemLocaleShortDate)
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: toDatePicker.item.show()
                        }
                    }
                }
            }

            Component.onCompleted: {
                measureListModel.setFilter(sampleIdFilter);
                measureListModel.setFilter(fromDateFilter);
                measureListModel.setFilter(toDateFilter);
            }
        }
    }

    Component {
        id: measureFooter

        Item {
            width: ListView.view.width
            height: 85

            Rectangle {
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.right: parent.right
                width: parent.width
                height: parent.height - 5
                radius: 10

                color: "LightSteelBlue"
                border.color: Qt.darker(color)

                Text {
                    anchors.centerIn: parent
                    font.bold: true
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                    text: "New measure..."
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: measureListModel.insertRows(measureListModel.rowCount(), 1)
                }
            }
        }
    }

    Component {
        id: measureDelegate

        Item {
            width: ListView.view.width
            height: 85

            MouseArea {
                anchors.fill: parent
                onClicked: parent.forceActiveFocus()
            }

            Rectangle {
                id: delegateContainer
                anchors.fill: parent
                radius: 10
                color: "LightSteelBlue"
                border.color: Qt.darker(color)
            }

            GridLayout {
                anchors.fill: delegateContainer
                anchors.margins: 5
                columns: 3

                Text {
                    Layout.minimumWidth: contentWidth
                    Layout.preferredWidth: 50
                    Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                    Layout.rowSpan: 2
                    text: model.sampleId
                    font.bold: true
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                }
                RowLayout {
                    Text {
                        Layout.minimumWidth: contentWidth
                        Layout.preferredWidth: 75
                        Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                        text: "Value (in " + model.unit + "):"
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText : "value"
                        text: model.value
                        onEditingFinished: model.value = text
                    }
                }
                ColumnLayout {
                    Layout.rowSpan: 2

                    Rectangle {
                        Layout.minimumWidth: 18
                        Layout.maximumWidth: Layout.minimumWidth
                        Layout.minimumHeight: Layout.minimumWidth
                        Layout.maximumHeight: Layout.minimumWidth
                        radius: Layout.minimumWidth / 2
                        antialiasing: true
                        color: "white"
                        border.color: "black"
                        Image {
                            id: checkedImage
                            anchors.fill: parent
                            anchors.margins: 2
                            source: "qrc:/icons/checked.png"
                            fillMode: Image.PreserveAspectCrop
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: checkedImage.visible = !checkedImage.visible
                        }
                    }
                    Rectangle {
                        Layout.minimumWidth: 18
                        Layout.maximumWidth: Layout.minimumWidth
                        Layout.minimumHeight: Layout.minimumWidth
                        Layout.maximumHeight: Layout.minimumWidth
                        radius: Layout.minimumWidth / 2
                        antialiasing: true
                        color: "white"
                        border.color: "black"
                        Image {
                            anchors.fill: parent
                            anchors.margins: 2
                            source: "qrc:/icons/remove.png"
                            fillMode: Image.PreserveAspectCrop
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: measureListModel.removeRows(index, 1)
                        }
                    }
                }
                RowLayout {
                    Text {
                        Layout.minimumWidth: contentWidth
                        Layout.preferredWidth: contentWidth
                        Layout.maximumWidth: contentWidth
                        text: "Date:"
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText : "date"
                        text: Qt.formatDateTime(model.sampleDate)
                        onEditingFinished: model.sampleDate = Date.fromLocaleString(Qt.locale(), text, Qt.DefaultLocaleShortDate)
                    }
                    Text {
                        Layout.minimumWidth: contentWidth
                        Layout.preferredWidth: contentWidth
                        Layout.maximumWidth: contentWidth
                        text: "Arrival:"
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText : "date"
                        text: Qt.formatDateTime(model.arrivalDate)
                        onEditingFinished: model.arrivalDate = Date.fromLocaleString(Qt.locale(), text, Qt.DefaultLocaleShortDate)
                    }
                }
            }
        }
    }
}
