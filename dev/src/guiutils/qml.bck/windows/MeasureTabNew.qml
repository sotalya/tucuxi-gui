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
    Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
    onOpacityChanged: {
        opacity == 0 ? visible = false : visible = true
    }

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
            id: measurestab
            leftPanel: form
            rightPanel: window.chart
        }
    }

    //left side
    property Component form: Item {
        id: item_pform
        anchors.fill: parent
        Component.onCompleted: console.log("measure form loaded")
        Rectangle {
            id: rect_pformmain
            anchors.fill: parent
            color: "transparent"
            anchors.margins: 25
                ColumnLayout {
                    id: col_pformmain
                    anchors.fill: parent

                    ColumnLayout {
                        id: grid_dform
                        z: 1
                        visible: true
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        opacity: 1
                        spacing: 5
                        property int sumtotal: 2
                        property int suminvalid: txtfld_value.valid + txtfld_date.valid
                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
//                            console.log("opacity: " + opacity + "visible: " + visible)
                        }
                        EzEditDateTimeSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Time"
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            date: tbl_measures.currentItem.modelData.sampleDate
                            cal.onSelectedDateChanged: tbl_measures.currentItem.modelData.sampleDate = cal.selectedDate
                            hoursbox.value: tbl_measures.currentItem.modelData.sampleDate.getHours()
                            hoursbox.onValueChanged: tbl_measures.currentItem.modelData.sampleDate.setHours(hoursbox.value)
                            minsbox.value: tbl_measures.currentItem.modelData.sampleDate.getMinutes()
                            minsbox.onValueChanged: tbl_measures.currentItem.modelData.sampleDate.setMinutes(minsbox.value)
                        }
                        EzNumericalComboSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            labeltext: "Value"
                            box.suffix: " mg/L"
                            box.value: tbl_measures.currentItem.modelData.value
                            box.onValueChanged: tbl_measures.currentItem.modelData.value = box.value
                        }


//                        EzEditTextSet {
//                            Layout.fillHeight: true
//                            Layout.fillWidth: true
//                            labeltext: "Sample Date"
//                            textfield.text: tbl_measures.currentItem.modelData.sampleDate
//                            textfield.onTextChanged: tbl_measures.currentItem.modelData.sampleDate = textfield.text
//                        }
                    }

                    MeasureTable {
                        id: tbl_measures
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
