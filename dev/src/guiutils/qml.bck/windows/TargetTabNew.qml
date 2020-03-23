import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0
import guiutils.qml.controls.table 1.0
import guiutils.qml.controls.flow 1.0

import ezechiel 1.0

Item {
    id: root
    width: flow.width
    height: flow.height - rect_topbar.height
    anchors.bottom: flow.bottom
    property int suminvalid: 0//number of invalid editable components in tab
    property int sumtotal: 0//total number of editable components

    states: [
                State {
                    name: "valid"
                },
                State {
                    name: "invalid"
                }
    ]
    Item {
        anchors.fill: parent
//        color: "grey"
        Rectangle {
            anchors.fill: parent
            color: "#dee8f4"//greece white
        }
//        Image {
//            anchors.fill: parent
//            fillMode: Image.Tile
//            horizontalAlignment: Image.AlignRight
//            verticalAlignment: Image.AlignTop
//            source: "qrc:/images/paper.jpg"
//        }
        FlowTab {
            id: targetstab
            leftPanel: form
            rightPanel: window.chart
        }
    }

    //left side
    property Component form: Item {
        id: item_pform
        anchors.fill: parent
        Component.onCompleted: console.log("target form loaded")
        Rectangle {
            id: rect_pformmain
            anchors.fill: parent
            color: "transparent"
            anchors.margins: 5

                ColumnLayout {
                    id: col_pformmain
                    anchors.fill: parent

                    ColumnLayout {
                        id: grid_tform
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        visible: true
                        opacity: 1
                        spacing: 5
                        property int sumtotal: 5
                        property int suminvalid: txtfld_ttype.valid + txtfld_min.valid + txtfld_best.valid + txtfld_max.valid
                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
//                            console.log("opacity: " + opacity + "visible: " + visible)
                        }
                        EzComboSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Type"
                            box.model: ["Residual", "Peak", "Mean", "Unknown"]
                            box.currentIndex: tbl_targets.currentItem.modelData.type
                            box.onCurrentIndexChanged: tbl_targets.currentItem.modelData.type = box.currentIndex
                        }
                        EzNumericalComboSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            labeltext: "cmin"
                            box.suffix: " mg/L"
                            box.value: tbl_targets.currentItem.modelData.cmin
                            box.onValueChanged: tbl_targets.currentItem.modelData.cmin = box.value
                        }
                        EzNumericalComboSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            labeltext: "cmax"
                            box.suffix: " mg/L"
                            box.value: tbl_targets.currentItem.modelData.cmax
                            box.onValueChanged: tbl_targets.currentItem.modelData.cmax = box.value
                        }
                        EzNumericalComboSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            labeltext: "cbest"
                            box.suffix: " mg/L"
                            box.value: tbl_targets.currentItem.modelData.cbest
                            box.onValueChanged: tbl_targets.currentItem.modelData.cbest = box.value
                        }
                        EzEditDateTimeSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Begin Time"
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            date: tbl_targets.currentItem.modelData.tmin
                            cal.onSelectedDateChanged: tbl_targets.currentItem.modelData.tmin = cal.selectedDate
                            hoursbox.value: tbl_targets.currentItem.modelData.tmin.getHours()
                            hoursbox.onValueChanged: tbl_targets.currentItem.modelData.tmin.setHours(hoursbox.value)
                            minsbox.value: tbl_targets.currentItem.modelData.tmin.getMinutes()
                            minsbox.onValueChanged: tbl_targets.currentItem.modelData.tmin.setMinutes(minsbox.value)
                        }
                        EzEditDateTimeSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "End Time"
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            date: tbl_targets.currentItem.modelData.tmax
                            hoursbox.value: tbl_targets.currentItem.modelData.tmax.getHours()
                            hoursbox.onValueChanged: tbl_targets.currentItem.modelData.tmax.setHours(hoursbox.value)
                            minsbox.value: tbl_targets.currentItem.modelData.tmax.getMinutes()
                            minsbox.onValueChanged: tbl_targets.currentItem.modelData.tmax.setMinutes(minsbox.value)
                        }
                        EzEditDateTimeSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Best Time"
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            date: tbl_targets.currentItem.modelData.tbest
                            hoursbox.value: tbl_targets.currentItem.modelData.tbest.getHours()
                            hoursbox.onValueChanged: tbl_targets.currentItem.modelData.tbest.setHours(hoursbox.value)
                            minsbox.value: tbl_targets.currentItem.modelData.tbest.getMinutes()
                            minsbox.onValueChanged: tbl_targets.currentItem.modelData.tbest.setMinutes(minsbox.value)
                        }
                    }

                    TargetTable {
                        id: tbl_targets
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.margins: 20
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        visible: true
                        opacity: 1
                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
                        }
                    }

                }
        }
    }
}
