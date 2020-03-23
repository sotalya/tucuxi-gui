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
    property int patientid: 0

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
            id: patientstab
            leftPanel: form
            rightPanel: window.chart
        }
    }

    //left side
    property Component form: Item {
        id: item_pform
        anchors.fill: parent
        Component.onCompleted: console.log("patient form loaded")
        Rectangle {
            id: rect_pformmain
            anchors.fill: parent
            color: "transparent"
            anchors.margins: 25

                ColumnLayout {
                    id: col_pformmain
                    anchors.fill: parent
//                    move: Transition {
//                            NumberAnimation { properties: "y"; duration: 250 }
//                        }

                    ColumnLayout {
                        id: grid_pform
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        visible: true
                        opacity: 1
                        spacing: 5
                        property int sumtotal: 4
                        property int suminvalid: 0//txtfld_fname.valid + txtfld_lname.valid + txtfld_bday.valid + txtfld_weight.valid
                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
//                            console.log("opacity: " + opacity + "visible: " + visible)
                        }

                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "First Name"
                            textfield.text: tbl_patients.currentItem.modelData.firstName
                            textfield.onTextChanged: tbl_patients.currentItem.modelData.firstName = textfield.text
                        }

                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Last Name"
                            textfield.text: tbl_patients.currentItem.modelData.lastName
                            textfield.onTextChanged: tbl_patients.currentItem.modelData.lastName = textfield.text
                        }
                        EzDateSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Birthday"
                            Layout.preferredHeight: TableStyle.cell.height * 1.2
                            Layout.preferredWidth: TableStyle.cell.colwidth
                            Layout.minimumHeight: TableStyle.cell.height * 1.2
                            Layout.minimumWidth: TableStyle.cell.colwidth
                            date: tbl_patients.currentItem.modelData.birthDate
                            cal.onSelectedDateChanged: tbl_patients.currentItem.modelData.birthDate = cal.selectedDate
                        }
                    }

                    ColumnLayout {
                        id: grid_extended
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        visible: true//false
                        spacing: 5
//                        Layout.leftMargin: 20
//                        Layout.rightMargin: 20
                        property int sumtotal: 7
                        property int suminvalid: txtfld_extid.valid + txtfld_staynbr.valid + txtfld_country.valid + txtfld_city.valid + txtfld_addr.valid + txtfld_postcode.valid + txtfld_state.valid
                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
//                            console.log("opacity: " + opacity + "visible: " + visible)
                        }
//                       visible: state == "extendedform"
                        //a row
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "ExternalId"
                            textfield.text: tbl_patients.currentItem.modelData.externalId
                            textfield.onTextChanged: tbl_patients.currentItem.modelData.externalId = textfield.text
                        }
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Stay Number"
                            textfield.text: tbl_patients.currentItem.modelData.stayNumber
                            textfield.onTextChanged: tbl_patients.currentItem.modelData.stayNumber = textfield.text
                        }
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Country"
                            textfield.text: tbl_patients.currentItem.modelData.country
                            textfield.onTextChanged: tbl_patients.currentItem.modelData.country = textfield.text
                        }
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "City"
                            textfield.text: tbl_patients.currentItem.modelData.city
                            textfield.onTextChanged: tbl_patients.currentItem.modelData.city = textfield.text
                        }
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "State"
                            textfield.text: tbl_patients.currentItem.modelData.state
                            textfield.onTextChanged: tbl_patients.currentItem.modelData.state = textfield.text
                        }
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Address"
                            textfield.text: tbl_patients.currentItem.modelData.address
                            textfield.onTextChanged: tbl_patients.currentItem.modelData.address = textfield.text
                        }
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Postcode"
                            textfield.text: tbl_patients.currentItem.modelData.postcode
                            textfield.onTextChanged: tbl_patients.currentItem.modelData.postcode = textfield.text
                        }
                    }

                    PatientTable {
                        id: tbl_patients
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.margins: 10
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
