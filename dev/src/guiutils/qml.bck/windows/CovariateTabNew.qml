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
    property string variatename: ""
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

//        color: "grey"
//        Image {
//            anchors.fill: parent
//            fillMode: Image.Tile
//            horizontalAlignment: Image.AlignRight
//            verticalAlignment: Image.AlignTop
//            source: "qrc:/images/paper.jpg"
//        }
        FlowTab {
            id: covariatestab
            leftPanel: form
            rightPanel: window.chart
        }
    }

    //left side
    property Component form: Item {
        id: item_pform
        anchors.fill: parent
        Component.onCompleted: console.log("covariate form loaded")
        Rectangle {
            id: rect_pformmain
            anchors.fill: parent
            color: "transparent"
            anchors.margins: 25
                ColumnLayout {
                    id: col_pformmain
                    anchors.fill: parent
                    z: 1

                    DrugVariateTable {
                        id: tbl_drugvariates
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

                    ColumnLayout {
                        id: grid_cform
                        z:1
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        visible: true
                        opacity: 1
                        spacing: 5
                        property int sumtotal: 4
                        property int suminvalid: txtfld_type.valid + txtfld_value.valid + txtfld_date.valid
                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
//                            console.log("opacity: " + opacity + "visible: " + visible)
                        }
//                        EzEditTextSet {
//                            Layout.fillHeight: true
//                            Layout.fillWidth: true
//                            labeltext: "Type"
//                            textfield.text: tbl_patientvariates.currentItem.modelData.name
//                            textfield.onTextChanged: tbl_patientvariates.currentItem.modelData.name = textfield.text
//                        }
                        EzEditDateTimeSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Time"
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            date: tbl_patientvariates.currentItem.modelData.date
                            cal.onSelectedDateChanged: tbl_patientvariates.currentItem.modelData.date = cal.selectedDate
                            hoursbox.value: tbl_patientvariates.currentItem.modelData.date.getHours()
                            hoursbox.onValueChanged: tbl_patientvariates.currentItem.modelData.date.setHours(hoursbox.value)
                            minsbox.value: tbl_patientvariates.currentItem.modelData.date.getMinutes()
                            minsbox.onValueChanged: tbl_patientvariates.currentItem.modelData.date.setMinutes(minsbox.value)
                        }
                        EzNumericalComboSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Value"
                            box.suffix: tbl_patientvariates.currentItem.modelData.unit
                            box.value: tbl_patientvariates.currentItem.modelData.value
                            box.onValueChanged: tbl_patientvariates.currentItem.modelData.value = box.value
                        }
                    }

                    CovariateTable {
                        id: tbl_patientvariates
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
