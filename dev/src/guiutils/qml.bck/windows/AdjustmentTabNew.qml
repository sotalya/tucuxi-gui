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
            id: adjustmentstab
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
            anchors.margins: 25

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
                        EzEditDateTimeSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Start"
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            date: tbl_adjustments.currentItem.modelData.applied
                            cal.onSelectedDateChanged: tbl_adjustments.currentItem.modelData.applied = cal.selectedDate
                            hoursbox.value: tbl_adjustments.currentItem.modelData.applied.getHours()
                            hoursbox.onValueChanged: tbl_adjustments.currentItem.modelData.applied.setHours(hoursbox.value)
                            minsbox.value: tbl_adjustments.currentItem.modelData.applied.getMinutes()
                            minsbox.onValueChanged: tbl_adjustments.currentItem.modelData.applied.setMinutes(minsbox.value)
                        }
                        EzNumericalComboSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Value"
                            box.suffix: " mg"
                            box.value: tbl_adjustments.currentItem.modelData.value
                            box.onValueChanged: tbl_adjustments.currentItem.modelData.value = box.value
                        }
                        EzEditDateTimeSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "End"
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            date: tbl_adjustments.currentItem.modelData.endtime
                            cal.onSelectedDateChanged: tbl_adjustments.currentItem.modelData.endtime = cal.selectedDate
                            hoursbox.value: tbl_adjustments.currentItem.modelData.endtime.getHours()
                            hoursbox.onValueChanged: tbl_adjustments.currentItem.modelData.endtime.setHours(hoursbox.value)
                            minsbox.value: tbl_adjustments.currentItem.modelData.endtime.getMinutes()
                            minsbox.onValueChanged: tbl_adjustments.currentItem.modelData.endtime.setMinutes(minsbox.value)
                        }
                        EzNumericalComboSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Infusion Time"
                            box.suffix: " h"
                            box.value: tbl_adjustments.currentItem.modelData.tinf
                            box.onValueChanged: tbl_adjustments.currentItem.modelData.tinf = box.value
                        }
                        EzComboSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Route"
                            box.model: ["Bolus", "Infusion", "Extra"]
                            box.currentIndex: tbl_adjustments.currentItem.modelData.route
                            box.onCurrentIndexChanged: tbl_adjustments.currentItem.modelData.route = box.currentIndex
                        }
                    }

                    AdjustmentTable {
                        id: tbl_adjustments
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
