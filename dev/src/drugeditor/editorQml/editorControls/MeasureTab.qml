import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

import ezechiel 1.0

FlowTab {
    id: root

    property int index:  -1
    property int status: Status.invalid

    signal currentIndexChanged(int index)
    signal currentStatusChanged(int status)
    signal reset()

    onIndexChanged:  currentIndexChanged(index)
    onStatusChanged: currentStatusChanged(status)

    leftPanel: Component {
        id: measurePanel

        Rectangle {
            anchors.fill: parent
            color: Style.table.background.table

            property int status: {
                if (measureListView.status === Status.invalid || measureView.status === Status.invalid)
                    return Status.invalid
                if (measureListView.status === Status.valid   || measureView.status === Status.valid)
                    return Status.valid

                return Status.complete
            }

            onStatusChanged: root.status = status

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5

                //MeasureList
                Rectangle {
                    id: measureList
                    Layout.fillWidth:  true
                    Layout.fillHeight: true

                    property string headerBackground: Style.table.background.header
                    property string headerForeground: Style.table.foreground.header
                    property string headerFontFamily: Style.table.font.header
                    property string headerFontSize:   Style.table.size.header
                    property bool headerFontBold:      true
                    property bool headerFontUnderline: false
                    property int headerHeight: 25
                    property int headerVAlign: Text.AlignVCenter
                    property int headerHAlign: Text.AlignHCenter

                    property int measureExternalIdColumnWidth:  50
                    property int measureValueColumnWidth:       80
                    property int measureSampleDateColumnWidth:  110
                    property int measureArrivalDateColumnWidth: 110

                    property string rowBackground:         Style.table.background.row
                    property string rowBackgroundHover:    Style.table.background.row_hover
                    property string rowBackgroundSelected: Style.table.background.row_selected
                    property string rowForeground:         Style.table.foreground.row
                    property string rowForegroundHover:    Style.table.foreground.row_hover
                    property string rowForegroundSelected: Style.table.foreground.row_selected
                    property string rowFontFamily:  Style.table.font.row
                    property string rowFontSize:    Style.table.size.row
                    property int rowVAlign: Text.AlignVCenter
                    property int rowHAlign: Text.AlignLeft
                    property int rowHeight: 25

                    property int cellFrameRadius: 1
                    property int cellFrameWidth:  0
                    property int cellMargin:      5

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 0

                        //MeasureList header
                        RowLayout {
                            spacing: 2

                            Rectangle {
                                Layout.preferredWidth:  measureList.measureExternalIdColumnWidth
                                Layout.preferredHeight: measureList.headerHeight
                                border.width: measureList.cellFrameWidth
                                radius: measureList.cellFrameRadius
                                color: measureList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "ID"
                                    elide: Text.ElideRight
                                    color: measureList.headerForeground
                                    font.family: measureList.headerFontFamily
                                    font.pixelSize: measureList.headerFontSize
                                    font.bold: measureList.headerFontBold
                                    font.underline: measureList.headerFontUnderline
                                    verticalAlignment:   measureList.headerVAlign
                                    horizontalAlignment: measureList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  measureList.measureValueColumnWidth
                                Layout.preferredHeight: measureList.headerHeight
                                border.width: measureList.cellFrameWidth
                                radius: measureList.cellFrameRadius
                                color: measureList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Value"
                                    elide: Text.ElideRight
                                    color: measureList.headerForeground
                                    font.family: measureList.headerFontFamily
                                    font.pixelSize: measureList.headerFontSize
                                    font.bold: measureList.headerFontBold
                                    font.underline: measureList.headerFontUnderline
                                    verticalAlignment:   measureList.headerVAlign
                                    horizontalAlignment: measureList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  measureList.measureValueColumnWidth
                                Layout.preferredHeight: measureList.headerHeight
                                border.width: measureList.cellFrameWidth
                                radius: measureList.cellFrameRadius
                                color: measureList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Sample date"
                                    elide: Text.ElideRight
                                    color: measureList.headerForeground
                                    font.family: measureList.headerFontFamily
                                    font.pixelSize: measureList.headerFontSize
                                    font.bold: measureList.headerFontBold
                                    font.underline: measureList.headerFontUnderline
                                    verticalAlignment:   measureList.headerVAlign
                                    horizontalAlignment: measureList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: measureList.headerHeight
                                border.width: measureList.cellFrameWidth
                                radius: measureList.cellFrameRadius
                                color: measureList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Arrival date"
                                    elide: Text.ElideRight
                                    color: measureList.headerForeground
                                    font.family: measureList.headerFontFamily
                                    font.pixelSize: measureList.headerFontSize
                                    font.bold: measureList.headerFontBold
                                    font.underline: measureList.headerFontUnderline
                                    verticalAlignment:   measureList.headerVAlign
                                    horizontalAlignment: measureList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  measureList.headerHeight
                                Layout.preferredHeight: measureList.headerHeight
                                border.width: measureList.cellFrameWidth
                                radius: measureList.cellFrameRadius
                                color:  measureList.headerBackground

                                Image {
                                    anchors.fill: parent
                                    source: measureListView.currentIndex !== -1 ? "qrc:/icons/buttons/remove.png" : "qrc:/icons/buttons/remove_disabled.png"
                                    fillMode: Image.PreserveAspectFit
                                    antialiasing: true
                                    mipmap: true

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            var currentIndex = measureListView.currentIndex;
                                            measureListModel.removeRows(currentIndex, 1);

                                            //Update current index unless it's the last row
                                            if (measureListModel.rowCount() !== currentIndex) {
                                                console.assert(root.index === currentIndex, "Critical: indexes shouldn't differ");
                                                root.currentIndexChanged(currentIndex);
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  measureList.headerHeight
                                Layout.preferredHeight: measureList.headerHeight
                                border.width: measureList.cellFrameWidth
                                radius: measureList.cellFrameRadius
                                color:  measureList.headerBackground

                                Image {
                                    anchors.fill: parent
                                    source: "qrc:/icons/buttons/add.png"
                                    fillMode: Image.PreserveAspectFit
                                    antialiasing: true
                                    mipmap: true

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            measureListModel.appendRows(1);
                                            measureListView.currentIndex = measureListView.count - 1;
                                        }
                                    }
                                }
                            }
                        }

                        //MeasureList view
                        ListView {
                            id: measureListView
                            Layout.fillWidth:  true
                            Layout.fillHeight: true
                            highlightFollowsCurrentItem: true
                            snapMode: ListView.SnapToItem
                            spacing: 2
                            clip: true

                            model: measureListModel
                            delegate: measureListDelegate

                            property int status: currentIndex === -1 ? Status.valid : Status.complete

                            onCurrentIndexChanged: root.index = currentIndex
                            Component.onCompleted: root.reset.connect(clearSelection)

                            function clearSelection() {
                                currentIndex = -1;
                            }
                        }

                        //MeasureList delegate
                        Component {
                            id: measureListDelegate

                            Item {
                                id: root
                                width:  ListView.view.width
                                height: measureList.rowHeight

                                MouseArea {
                                    id: measureListDelegateMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    property bool hovered: false

                                    onClicked: root.ListView.view.currentIndex = index
                                    onEntered: hovered = true
                                    onExited:  hovered = false
                                }

                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 2

                                    Rectangle {
                                        Layout.preferredWidth:  measureList.measureExternalIdColumnWidth
                                        Layout.fillHeight: true
                                        border.width: measureList.cellFrameWidth
                                        radius: measureList.cellFrameRadius
                                        color: measureListDelegateMouseArea.hovered ? measureList.rowBackgroundHover : (root.ListView.isCurrentItem ? measureList.rowBackgroundSelected : measureList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: measureList.cellMargin
                                            text: model.sampleId
                                            elide: Text.ElideRight
                                            color: measureListDelegateMouseArea.hovered ? measureList.rowForegroundHover : (root.ListView.isCurrentItem ? measureList.rowForegroundSelected : measureList.rowForeground)
                                            font.family:    measureList.rowFontFamily
                                            font.pixelSize: measureList.rowFontSize
                                            verticalAlignment:   measureList.rowVAlign
                                            horizontalAlignment: measureList.rowHAlign
                                        }
                                    }
                                    Rectangle {
                                        Layout.preferredWidth:  measureList.measureValueColumnWidth
                                        Layout.fillHeight: true
                                        border.width: measureList.cellFrameWidth
                                        radius: measureList.cellFrameRadius
                                        color: measureListDelegateMouseArea.hovered ? measureList.rowBackgroundHover : (root.ListView.isCurrentItem ? measureList.rowBackgroundSelected : measureList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: measureList.cellMargin
                                            text: model.value + " " + model.unit
                                            elide: Text.ElideRight
                                            color: measureListDelegateMouseArea.hovered ? measureList.rowForegroundHover : (root.ListView.isCurrentItem ? measureList.rowForegroundSelected : measureList.rowForeground)
                                            font.family:    measureList.rowFontFamily
                                            font.pixelSize: measureList.rowFontSize
                                            verticalAlignment:   measureList.rowVAlign
                                            horizontalAlignment: measureList.rowHAlign
                                        }
                                    }
                                    Rectangle {
                                        Layout.preferredWidth:  measureList.measureValueColumnWidth
                                        Layout.fillHeight: true
                                        border.width: measureList.cellFrameWidth
                                        radius: measureList.cellFrameRadius
                                        color: measureListDelegateMouseArea.hovered ? measureList.rowBackgroundHover : (root.ListView.isCurrentItem ? measureList.rowBackgroundSelected : measureList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: measureList.cellMargin
                                            text: Qt.formatDateTime(model.sampleDate)
                                            elide: Text.ElideRight
                                            color: measureListDelegateMouseArea.hovered ? measureList.rowForegroundHover : (root.ListView.isCurrentItem ? measureList.rowForegroundSelected : measureList.rowForeground)
                                            font.family:    measureList.rowFontFamily
                                            font.pixelSize: measureList.rowFontSize
                                            verticalAlignment:   measureList.rowVAlign
                                            horizontalAlignment: measureList.rowHAlign
                                        }
                                    }
                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        border.width: measureList.cellFrameWidth
                                        radius: measureList.cellFrameRadius
                                        color: measureListDelegateMouseArea.hovered ? measureList.rowBackgroundHover : (root.ListView.isCurrentItem ? measureList.rowBackgroundSelected : measureList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: measureList.cellMargin
                                            text: Qt.formatDateTime(model.arrivalDate)
                                            elide: Text.ElideRight
                                            color: measureListDelegateMouseArea.hovered ? measureList.rowForegroundHover : (root.ListView.isCurrentItem ? measureList.rowForegroundSelected : measureList.rowForeground)
                                            font.family:    measureList.rowFontFamily
                                            font.pixelSize: measureList.rowFontSize
                                            verticalAlignment:   measureList.rowVAlign
                                            horizontalAlignment: measureList.rowHAlign
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                //Measure
                Rectangle {
                    id: measure
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    Layout.minimumHeight:   100

                    property int rowHeight: 25

                    property string frameBackground: Style.form.background.frame
                    property string frameColor:      Style.form.border.color
                    property int frameWidth:   Style.form.border.width
                    property int frameRadius:  Style.form.border.radius
                    property int framePadding: 5

                    property string labelForeground: Style.form.foreground.label
                    property string labelFontFamily: Style.form.font.label
                    property string labelFontSize:   Style.form.size.label
                    property bool labelFontBold: false
                    property int labelVAlign: Text.AlignVCenter
                    property int labelHAlign: Text.AlignRight

                    property string inputFontFamily: Style.form.font.input
                    property string inputFontSize:   Style.form.size.input
                    property int inputVAlign: Text.AlignVCenter
                    property int inputHAlign: Text.AlignLeft

                    //Measure view
                    ListView {
                        id: measureView
                        anchors.fill: parent
                        anchors.margins: 0
                        highlightFollowsCurrentItem: true
                        snapMode: ListView.SnapToItem
                        clip: true
                        interactive: false

                        model: measureModel
                        delegate: measureDelegate

                        property int delegateStatus
                        property int status: count === 0 ? Status.valid : delegateStatus
                    }

                    //Measure delegate
                    Component {
                        id: measureDelegate

                        Item {
                            id: root
                            width:  ListView.view.width
                            height: ListView.view.height

                            property int status: {
                                if (valueInput.value <= 0                   ||
                                    isNaN(sampleDateInput.date.getTime())   ||
                                    isNaN(arrivalDateInput.date.getTime()))
                                    return Status.invalid;
                                if (sampleIdInput.text === "" ||
                                    model.arrivalDate.getTime() < model.sampleDate.getTime())
                                    return Status.valid;

                                return Status.complete;
                            }

                            onStatusChanged: ListView.view.delegateStatus = status

                            Rectangle {
                                id: measureFrame
                                anchors.fill: parent
                                anchors.margins: 0
                                color:  measure.frameBackground
                                radius: measure.frameRadius
                                border.width: measure.frameWidth
                                border.color: measure.frameColor

                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.margins: measure.framePadding
                                    spacing: 5

                                    RowLayout {
                                        Layout.preferredHeight: measure.rowHeight
                                        Layout.minimumHeight:   measure.rowHeight
                                        spacing: 2

                                        Text {
                                            id: sampleIdLabel
                                            Layout.preferredWidth: 75
                                            text: "Sample ID:"
                                            elide: Text.ElideRight
                                            color: measure.labelForeground
                                            font.family:    measure.labelFontFamily
                                            font.pixelSize: measure.labelFontSize
                                            font.bold: measure.labelFontBold
                                            verticalAlignment:   measure.labelVAlign
                                            horizontalAlignment: measure.labelHAlign
                                        }
                                        TextField {
                                            id: sampleIdInput
                                            Layout.fillWidth:  true
                                            placeholderText: "sample id"
                                            text: model.sampleId
                                            onTextChanged: model.sampleId = text
                                            font.family:    measure.inputFontFamily
                                            font.pixelSize: measure.inputFontSize
                                            verticalAlignment:   measure.inputVAlign
                                            horizontalAlignment: measure.inputHAlign
                                        }

                                        Text {
                                            id: valueLabel
                                            Layout.preferredWidth: 100
                                            text: "Value:"
                                            elide: Text.ElideRight
                                            color: measure.labelForeground
                                            font.family:    measure.labelFontFamily
                                            font.pixelSize: measure.labelFontSize
                                            font.bold: measure.labelFontBold
                                            verticalAlignment:   measure.labelVAlign
                                            horizontalAlignment: measure.labelHAlign
                                        }
                                        SpinBox {
                                            id: valueInput
                                            Layout.fillWidth:  true
                                            suffix: " " + model.unit
                                            value: model.value
                                            onValueChanged: model.value = value
                                            font.family:    measure.inputFontFamily
                                            font.pixelSize: measure.inputFontSize
                                            horizontalAlignment: measure.inputHAlign
                                            decimals: 3
                                            minimumValue: 0
                                            maximumValue: Number.POSITIVE_INFINITY
                                        }
                                    }

                                    RowLayout {
                                        Layout.preferredHeight: measure.rowHeight
                                        Layout.minimumHeight:   measure.rowHeight
                                        spacing: 2

                                        Text {
                                            id: sampleDateLabel
                                            Layout.preferredWidth: 75
                                            text: "Measured:"
                                            elide: Text.ElideRight
                                            color: measure.labelForeground
                                            font.family:    measure.labelFontFamily
                                            font.pixelSize: measure.labelFontSize
                                            font.bold: measure.labelFontBold
                                            verticalAlignment:   measure.labelVAlign
                                            horizontalAlignment: measure.labelHAlign
                                        }
//                                        DatePicker {
//                                            id: sampleDateInput
//                                            Layout.preferredWidth: 150
//                                            Binding { target: sampleDateInput; property: "date"; value: model.sampleDate }
//                                            onEditingFinished: model.sampleDate = new Date(date.getFullYear(), date.getMonth(), date.getDate(), sampleTimeInput.hours, sampleTimeInput.minutes)
//                                        }
//                                        TimePicker {
//                                            id: sampleTimeInput
//                                            Layout.fillWidth: true
//                                            Binding { target: sampleTimeInput; property: "hours";   value: model.sampleDate.getHours()   }
//                                            Binding { target: sampleTimeInput; property: "minutes"; value: model.sampleDate.getMinutes() }
//                                            onEditingFinished: model.sampleDate = new Date(sampleDateInput.date.getFullYear(), sampleDateInput.date.getMonth(), sampleDateInput.date.getDate(), hours, minutes)
//                                        }
                                    }

                                    RowLayout {
                                        Layout.preferredHeight: measure.rowHeight
                                        Layout.minimumHeight:   measure.rowHeight
                                        spacing: 2

                                        Text {
                                            id: arrivalDateLabel
                                            Layout.preferredWidth: 75
                                            text: "Received:"
                                            elide: Text.ElideRight
                                            color: measure.labelForeground
                                            font.family:    measure.labelFontFamily
                                            font.pixelSize: measure.labelFontSize
                                            font.bold: measure.labelFontBold
                                            verticalAlignment:   measure.labelVAlign
                                            horizontalAlignment: measure.labelHAlign
                                        }
//                                        DatePicker {
//                                            id: arrivalDateInput
//                                            Layout.preferredWidth: 150
//                                            Binding { target: arrivalDateInput; property: "date"; value: model.arrivalDate }
//                                            onEditingFinished: model.arrivalDate = new Date(date.getFullYear(), date.getMonth(), date.getDate(), arrivalTimeInput.hours, arrivalTimeInput.minutes)
//                                        }
//                                        TimePicker {
//                                            id: arrivalTimeInput
//                                            Layout.fillWidth: true
//                                            Binding { target: arrivalTimeInput; property: "hours";   value: model.arrivalDate.getHours()   }
//                                            Binding { target: arrivalTimeInput; property: "minutes"; value: model.arrivalDate.getMinutes() }
//                                            onEditingFinished: model.arrivalDate = new Date(arrivalDateInput.date.getFullYear(), arrivalDateInput.date.getMonth(), arrivalDateInput.date.getDate(), hours, minutes)
//                                        }
                                    }

                                    RowLayout {} //Spacer
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
