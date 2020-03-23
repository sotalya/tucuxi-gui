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
    property int status: Status.valid

    signal currentIndexChanged(int index)
    signal currentStatusChanged(int status)

    signal settingsValidated()
    signal dosageValidated()

    signal reset()

    onIndexChanged:  currentIndexChanged(index)
    onStatusChanged: currentStatusChanged(status)

    leftPanel: Component {
        id: adjustmentPanel

        Rectangle {
            anchors.fill: parent
            color: Style.table.background.table

            property int status: {
                if (adjustmentSettingsView.status === Status.invalid || adjustmentListView.status === Status.invalid || adjustmentView.status === Status.invalid)
                    return Status.invalid
                if (adjustmentSettingsView.status === Status.valid   || adjustmentListView.status === Status.valid   || adjustmentView.status === Status.valid)
                    return Status.valid

                return Status.complete
            }

            onStatusChanged: root.status = status

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5

                //AdjustmentSettings
                Rectangle {
                    id: adjustmentSettings
                    Layout.fillWidth: true
                    Layout.preferredHeight: 35
                    Layout.minimumHeight:   35

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

                    //AdjustmentSettings view
                    ListView {
                        id: adjustmentSettingsView
                        anchors.fill: parent
                        anchors.margins: 0
                        highlightFollowsCurrentItem: true
                        snapMode: ListView.SnapToItem
                        clip: true
                        interactive: false

                        model: adjustmentSettingsModel
                        delegate: adjustmentSettingsDelegate

                        property int status: Status.complete
                    }

                    //AdjustmentSettings delegate
                    Component {
                        id: adjustmentSettingsDelegate

                        Item {
                            width:  ListView.view.width
                            height: ListView.view.height

                            Rectangle {
                                id: adjustmentSettingsFrame
                                anchors.fill: parent
                                anchors.margins: 0
                                color:  adjustmentSettings.frameBackground
                                radius: adjustmentSettings.frameRadius
                                border.width: adjustmentSettings.frameWidth
                                border.color: adjustmentSettings.frameColor

                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.margins: adjustmentSettings.framePadding
                                    spacing: 5

                                    RowLayout {
                                        Layout.preferredHeight: adjustmentSettings.rowHeight
                                        Layout.minimumHeight:   adjustmentSettings.rowHeight
                                        spacing: 2

                                        Text {
                                            id: atSettingLabel
                                            Layout.preferredWidth: 75
                                            text: "Adjust at:"
                                            elide: Text.ElideRight
                                            color: adjustmentSettings.labelForeground
                                            font.family:    adjustmentSettings.labelFontFamily
                                            font.pixelSize: adjustmentSettings.labelFontSize
                                            font.bold: adjustmentSettings.labelFontBold
                                            verticalAlignment:   adjustmentSettings.labelVAlign
                                            horizontalAlignment: adjustmentSettings.labelHAlign
                                        }
//                                        DatePicker {
//                                            id: atDateSettingInput
//                                            Layout.preferredWidth: 150
//                                            Binding { target: atDateSettingInput; property: "date"; value: model.date }
//                                            onEditingFinished: model.date = new Date(date.getFullYear(), date.getMonth(), date.getDate(), atMinutesSettingInput.hours, atMinutesSettingInput.minutes)
//                                        }
//                                        TimePicker {
//                                            id: atMinutesSettingInput
//                                            Layout.fillWidth: true
//                                            Binding { target: atMinutesSettingInput; property: "hours";   value: model.date.getHours()   }
//                                            Binding { target: atMinutesSettingInput; property: "minutes"; value: model.date.getMinutes() }
//                                            onEditingFinished: model.date = new Date(atDateSettingInput.date.getFullYear(), atDateSettingInput.date.getMonth(), atDateSettingInput.date.getDate(), hours, minutes)
//                                        }
                                        Rectangle {
                                            Layout.preferredWidth:  adjustmentSettings.rowHeight
                                            Layout.preferredHeight: adjustmentSettings.rowHeight
                                            color: adjustmentSettings.frameBackground

                                            Image {
                                                anchors.fill: parent
                                                anchors.margins: -5
                                                source: "qrc:/icons/buttons/check_disabled.png"
                                                fillMode: Image.PreserveAspectFit
                                                antialiasing: true
                                                mipmap: true

                                                MouseArea {
                                                    anchors.fill: parent
                                                    onClicked: {
                                                        model.valid = true;
                                                        adjustmentList.show();
                                                        root.settingsValidated();
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    RowLayout {} //Spacer
                                }
                            }
                        }
                    }
                }


                //Adjustment
                Rectangle {
                    id: adjustment
                    Layout.fillWidth: true
                    Layout.preferredHeight: 85
                    Layout.minimumHeight:   85

                    property int rowHeight: 25

                    property string frameBackground: Style.form.background.frame
                    property string frameColor:      Style.form.border.color
                    property int frameWidth:   Style.form.border.width
                    property int frameRadius:  Style.form.border.radius
                    property int framePadding: 5

                    property string labelForeground: Style.form.foreground.label
                    property string labelFontFamily: Style.form.font.label
                    property string labelFontSize:   Style.form.size.label
                    property bool labelFontBold: true
                    property int labelVAlign: Text.AlignVCenter
                    property int labelHAlign: Text.AlignRight

                    property string inputForeground: Style.form.foreground.input
                    property string inputFontFamily: Style.form.font.input
                    property string inputFontSize:   Style.form.size.input
                    property int inputVAlign: Text.AlignVCenter
                    property int inputHAlign: Text.AlignLeft

                    //Adjustment view
                    ListView {
                        id: adjustmentView
                        anchors.fill: parent
                        anchors.margins: 0
                        highlightFollowsCurrentItem: true
                        snapMode: ListView.SnapToItem
                        clip: true
                        interactive: false

                        model: adjustmentModel
                        delegate: adjustmentDelegate

                        property int delegateStatus: Status.valid
                        property int status: count === 0 ? Status.valid : delegateStatus
                    }

                    //Adjustment delegate
                    Component {
                        id: adjustmentDelegate

                        Item {
                            id: root
                            width:  ListView.view.width
                            height: ListView.view.height

                            property int status: model.valid ? Status.complete : Status.valid

                            onStatusChanged: ListView.view.delegateStatus = status

                            Rectangle {
                                id: adjustmentFrame
                                anchors.fill: parent
                                anchors.margins: 0
                                color:  model.valid ? "lightgreen" : adjustment.frameBackground
                                radius: adjustment.frameRadius
                                border.width: adjustment.frameWidth
                                border.color: adjustment.frameColor

                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.margins: adjustment.framePadding
                                    spacing: 0

                                    RowLayout {
                                        Layout.preferredHeight: adjustment.rowHeight
                                        Layout.minimumHeight:   adjustment.rowHeight
                                        spacing: 4

                                        Text {
                                            id: doseLabel
                                            Layout.preferredWidth: 75
                                            text: "Dose:"
                                            elide: Text.ElideRight
                                            color: adjustment.labelForeground
                                            font.family:    adjustment.labelFontFamily
                                            font.pixelSize: adjustment.labelFontSize
                                            font.bold: adjustment.labelFontBold
                                            verticalAlignment:   adjustment.labelVAlign
                                            horizontalAlignment: adjustment.labelHAlign
                                        }
                                        Text {
                                            id: doseInput
                                            Layout.preferredWidth: 100
                                            text: model.value + " " + model.unit
                                            elide: Text.ElideRight
                                            color: adjustment.inputForeground
                                            font.family:    adjustment.inputFontFamily
                                            font.pixelSize: adjustment.inputFontSize
                                            verticalAlignment:   adjustment.inputVAlign
                                            horizontalAlignment: adjustment.inputHAlign
                                        }

                                        Text {
                                            id: intervalLabel
                                            Layout.preferredWidth: 100
                                            text: "Interval:"
                                            elide: Text.ElideRight
                                            color: adjustment.labelForeground
                                            font.family:    adjustment.labelFontFamily
                                            font.pixelSize: adjustment.labelFontSize
                                            font.bold: adjustment.labelFontBold
                                            verticalAlignment:   adjustment.labelVAlign
                                            horizontalAlignment: adjustment.labelHAlign
                                        }
                                        Text {
                                            id: intervalInput
                                            Layout.fillWidth:  true
                                            text: model.interval + " hours"
                                            elide: Text.ElideRight
                                            color: adjustment.inputForeground
                                            font.family:    adjustment.inputFontFamily
                                            font.pixelSize: adjustment.inputFontSize
                                            verticalAlignment:   adjustment.inputVAlign
                                            horizontalAlignment: adjustment.inputHAlign
                                        }
                                    }

                                    RowLayout {
                                        Layout.preferredHeight: adjustment.rowHeight
                                        Layout.minimumHeight:   adjustment.rowHeight
                                        spacing: 4

                                        Text {
                                            id: appliedLabel
                                            Layout.preferredWidth: 75
                                            text: "From:"
                                            elide: Text.ElideRight
                                            color: adjustment.labelForeground
                                            font.family:    adjustment.labelFontFamily
                                            font.pixelSize: adjustment.labelFontSize
                                            font.bold: adjustment.labelFontBold
                                            verticalAlignment:   adjustment.labelVAlign
                                            horizontalAlignment: adjustment.labelHAlign
                                        }
                                        Text {
                                            id: appliedInput
                                            Layout.preferredWidth: 100
                                            text: Qt.formatDateTime(model.applied)
                                            elide: Text.ElideRight
                                            color: adjustment.inputForeground
                                            font.family:    adjustment.inputFontFamily
                                            font.pixelSize: adjustment.inputFontSize
                                            verticalAlignment:   adjustment.inputVAlign
                                            horizontalAlignment: adjustment.inputHAlign
                                        }

                                        Text {
                                            id: stoppedLabel
                                            Layout.preferredWidth: 100
                                            text: "To:"
                                            elide: Text.ElideRight
                                            color: adjustment.labelForeground
                                            font.family:    adjustment.labelFontFamily
                                            font.pixelSize: adjustment.labelFontSize
                                            font.bold: adjustment.labelFontBold
                                            verticalAlignment:   adjustment.labelVAlign
                                            horizontalAlignment: adjustment.labelHAlign
                                        }
                                        Text {
                                            id: stoppedInput
                                            Layout.fillWidth:  true
                                            text: Qt.formatDateTime(model.endtime)
                                            elide: Text.ElideRight
                                            color: adjustment.inputForeground
                                            font.family:    adjustment.inputFontFamily
                                            font.pixelSize: adjustment.inputFontSize
                                            verticalAlignment:   adjustment.inputVAlign
                                            horizontalAlignment: adjustment.inputHAlign
                                        }
                                    }

                                    RowLayout {
                                        Layout.preferredHeight: adjustment.rowHeight
                                        Layout.minimumHeight:   adjustment.rowHeight
                                        spacing: 4

                                        Text {
                                            id: routeLabel
                                            Layout.preferredWidth: 75
                                            text: "Route:"
                                            elide: Text.ElideRight
                                            color: adjustment.labelForeground
                                            font.family:    adjustment.labelFontFamily
                                            font.pixelSize: adjustment.labelFontSize
                                            font.bold: adjustment.labelFontBold
                                            verticalAlignment:   adjustment.labelVAlign
                                            horizontalAlignment: adjustment.labelHAlign
                                        }
                                        Text {
                                            id: routeInput
                                            Layout.preferredWidth: 100
                                            text: {
                                                switch (model.route) {
                                                case 0:  return "Bolus";
                                                case 1:  return "Infusion";
                                                case 2:  return "Extravascular";
                                                default: return "Unknown"
                                                }
                                            }
                                            elide: Text.ElideRight
                                            color: adjustment.inputForeground
                                            font.family:    adjustment.inputFontFamily
                                            font.pixelSize: adjustment.inputFontSize
                                            verticalAlignment:   adjustment.inputVAlign
                                            horizontalAlignment: adjustment.inputHAlign
                                        }

                                        Text {
                                            id: infusionLabel
                                            Layout.preferredWidth: 100
                                            visible: routeInput.currentIndex === 1
                                            text: "Infusion:"
                                            elide: Text.ElideRight
                                            color: adjustment.labelForeground
                                            font.family:    adjustment.labelFontFamily
                                            font.pixelSize: adjustment.labelFontSize
                                            font.bold: adjustment.labelFontBold
                                            verticalAlignment:   adjustment.labelVAlign
                                            horizontalAlignment: adjustment.labelHAlign
                                        }
                                        Text {
                                            id: infusionInput
                                            Layout.fillWidth:  true
                                            visible: routeInput.currentIndex === 1
                                            text: model.tinf + " minutes"
                                            elide: Text.ElideRight
                                            color: adjustment.inputForeground
                                            font.family:    adjustment.inputFontFamily
                                            font.pixelSize: adjustment.inputFontSize
                                            verticalAlignment:   adjustment.inputVAlign
                                            horizontalAlignment: adjustment.inputHAlign
                                        }
                                    }

                                    RowLayout {} //Spacer
                                }
                            }
                        }
                    }
                }

                //AdjustmentList
                Rectangle {
                    id: adjustmentList
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

                    property int adjustmentDoseColumnWidth:       85
                    property int adjustmentIntervalColumnWidth:   85
                    property int adjustmentAppliedColumnWidth:    85
                    property int adjustmentStoppedColumnWidth:    95

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

                    Component.onCompleted: root.reset.connect(hide)

                    function hide() {
                        adjustmentListHeaders.visible = false;
                        adjustmentListView.visible    = false;
                    }

                    function show() {
                        adjustmentListHeaders.visible = true;
                        adjustmentListView.visible    = true;
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 0

                        //AdjustmentList header
                        RowLayout {
                            id: adjustmentListHeaders
                            spacing: 2
                            visible: false

                            Rectangle {
                                Layout.preferredWidth:  adjustmentList.adjustmentDoseColumnWidth
                                Layout.preferredHeight: adjustmentList.headerHeight
                                border.width: adjustmentList.cellFrameWidth
                                radius: adjustmentList.cellFrameRadius
                                color: adjustmentList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Dose"
                                    elide: Text.ElideRight
                                    color: adjustmentList.headerForeground
                                    font.family: adjustmentList.headerFontFamily
                                    font.pixelSize: adjustmentList.headerFontSize
                                    font.bold: adjustmentList.headerFontBold
                                    font.underline: adjustmentList.headerFontUnderline
                                    verticalAlignment:   adjustmentList.headerVAlign
                                    horizontalAlignment: adjustmentList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  adjustmentList.adjustmentIntervalColumnWidth
                                Layout.preferredHeight: adjustmentList.headerHeight
                                border.width: adjustmentList.cellFrameWidth
                                radius: adjustmentList.cellFrameRadius
                                color: adjustmentList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Interval"
                                    elide: Text.ElideRight
                                    color: adjustmentList.headerForeground
                                    font.family: adjustmentList.headerFontFamily
                                    font.pixelSize: adjustmentList.headerFontSize
                                    font.bold: adjustmentList.headerFontBold
                                    font.underline: adjustmentList.headerFontUnderline
                                    verticalAlignment:   adjustmentList.headerVAlign
                                    horizontalAlignment: adjustmentList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  adjustmentList.adjustmentAppliedColumnWidth
                                Layout.preferredHeight: adjustmentList.headerHeight
                                border.width: adjustmentList.cellFrameWidth
                                radius: adjustmentList.cellFrameRadius
                                color: adjustmentList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Applied"
                                    elide: Text.ElideRight
                                    color: adjustmentList.headerForeground
                                    font.family: adjustmentList.headerFontFamily
                                    font.pixelSize: adjustmentList.headerFontSize
                                    font.bold: adjustmentList.headerFontBold
                                    font.underline: adjustmentList.headerFontUnderline
                                    verticalAlignment:   adjustmentList.headerVAlign
                                    horizontalAlignment: adjustmentList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: adjustmentList.headerHeight
                                border.width: adjustmentList.cellFrameWidth
                                radius: adjustmentList.cellFrameRadius
                                color: adjustmentList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Stopped"
                                    elide: Text.ElideRight
                                    color: adjustmentList.headerForeground
                                    font.family: adjustmentList.headerFontFamily
                                    font.pixelSize: adjustmentList.headerFontSize
                                    font.bold: adjustmentList.headerFontBold
                                    font.underline: adjustmentList.headerFontUnderline
                                    verticalAlignment:   adjustmentList.headerVAlign
                                    horizontalAlignment: adjustmentList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  adjustmentList.headerHeight
                                Layout.preferredHeight: adjustmentList.headerHeight
                                border.width: adjustmentList.cellFrameWidth
                                radius: adjustmentList.cellFrameRadius
                                color:  adjustmentList.headerBackground

                                Image {
                                    anchors.fill: parent
                                    source: adjustmentListView.currentIndex === -1 ? "qrc:/icons/buttons/check_disabled.png" : "qrc:/icons/buttons/check.png"
                                    fillMode: Image.PreserveAspectFit
                                    antialiasing: true
                                    mipmap: true

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            if (adjustmentListView.currentIndex === -1)
                                                return;

                                            adjustmentList.hide();
                                            root.dosageValidated();
                                        }
                                    }
                                }
                            }
                        }

                        //AdjustmentList view
                        ListView {
                            id: adjustmentListView
                            Layout.fillWidth:  true
                            Layout.fillHeight: true
                            visible: false
                            highlightFollowsCurrentItem: true
                            snapMode: ListView.SnapToItem
                            spacing: 2
                            clip: true

                            model: adjustmentListModel
                            delegate: adjustmentListDelegate

                            property int status: Status.complete

                            onCurrentIndexChanged: root.index = currentIndex
                            onCountChanged: currentIndex = count > 0 ? 0 : -1
                            Component.onCompleted: {
                                root.reset.connect(clearSelection);
                                root.settingsValidated.connect(clearSelection);
                            }

                            function clearSelection() {
                                currentIndex = -1;
                            }
                        }

                        //AdjustmentList delegate
                        Component {
                            id: adjustmentListDelegate

                            Item {
                                id: root
                                width:  ListView.view.width
                                height: adjustmentList.rowHeight

                                MouseArea {
                                    id: adjustmentListDelegateMouseArea
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
                                        Layout.preferredWidth:  adjustmentList.adjustmentDoseColumnWidth
                                        Layout.fillHeight: true
                                        border.width: adjustmentList.cellFrameWidth
                                        radius: adjustmentList.cellFrameRadius
                                        color: adjustmentListDelegateMouseArea.hovered ? adjustmentList.rowBackgroundHover : (root.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: adjustmentList.cellMargin
                                            text: model.value + " " + model.unit
                                            elide: Text.ElideRight
                                            color: adjustmentListDelegateMouseArea.hovered ? adjustmentList.rowForegroundHover : (root.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                                            font.family:    adjustmentList.rowFontFamily
                                            font.pixelSize: adjustmentList.rowFontSize
                                            verticalAlignment:   adjustmentList.rowVAlign
                                            horizontalAlignment: adjustmentList.rowHAlign
                                        }
                                    }
                                    Rectangle {
                                        Layout.preferredWidth:  adjustmentList.adjustmentIntervalColumnWidth
                                        Layout.fillHeight: true
                                        border.width: adjustmentList.cellFrameWidth
                                        radius: adjustmentList.cellFrameRadius
                                        color: adjustmentListDelegateMouseArea.hovered ? adjustmentList.rowBackgroundHover : (root.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: adjustmentList.cellMargin
                                            text: model.interval + " hours"
                                            elide: Text.ElideRight
                                            color: adjustmentListDelegateMouseArea.hovered ? adjustmentList.rowForegroundHover : (root.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                                            font.family:    adjustmentList.rowFontFamily
                                            font.pixelSize: adjustmentList.rowFontSize
                                            verticalAlignment:   adjustmentList.rowVAlign
                                            horizontalAlignment: adjustmentList.rowHAlign
                                        }
                                    }
                                    Rectangle {
                                        Layout.preferredWidth:  adjustmentList.adjustmentAppliedColumnWidth
                                        Layout.fillHeight: true
                                        border.width: adjustmentList.cellFrameWidth
                                        radius: adjustmentList.cellFrameRadius
                                        color: adjustmentListDelegateMouseArea.hovered ? adjustmentList.rowBackgroundHover : (root.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: adjustmentList.cellMargin
                                            text: Qt.formatDateTime(model.applied)
                                            elide: Text.ElideRight
                                            color: adjustmentListDelegateMouseArea.hovered ? adjustmentList.rowForegroundHover : (root.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                                            font.family:    adjustmentList.rowFontFamily
                                            font.pixelSize: adjustmentList.rowFontSize
                                            verticalAlignment:   adjustmentList.rowVAlign
                                            horizontalAlignment: adjustmentList.rowHAlign
                                        }
                                    }
                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        border.width: adjustmentList.cellFrameWidth
                                        radius: adjustmentList.cellFrameRadius
                                        color: adjustmentListDelegateMouseArea.hovered ? adjustmentList.rowBackgroundHover : (root.ListView.isCurrentItem ? adjustmentList.rowBackgroundSelected : adjustmentList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: adjustmentList.cellMargin
                                            text: Qt.formatDateTime(model.endtime)
                                            elide: Text.ElideRight
                                            color: adjustmentListDelegateMouseArea.hovered ? adjustmentList.rowForegroundHover : (root.ListView.isCurrentItem ? adjustmentList.rowForegroundSelected : adjustmentList.rowForeground)
                                            font.family:    adjustmentList.rowFontFamily
                                            font.pixelSize: adjustmentList.rowFontSize
                                            verticalAlignment:   adjustmentList.rowVAlign
                                            horizontalAlignment: adjustmentList.rowHAlign
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            RowLayout {}
        }
    }
}
