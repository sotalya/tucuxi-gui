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
    property int drugid: 0
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
            id: drugstab
            leftPanel: form
            rightPanel: window.chart
        }
    }

    //left side
    property Component form: Item {
        id: item_pform
        anchors.fill: parent
        Component.onCompleted: console.log("drug form loaded")
        Rectangle {
            id: rect_pformmain
            anchors.fill: parent
            color: "transparent"
            anchors.margins: 25
                ColumnLayout {
                    id: col_pformmain
                    anchors.fill: parent

                    DrugTable {
                        id: tbl_drugs
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        anchors.margins: 20
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        opacity: 1
                        visible: true
                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
                        }
                    }

                    ColumnLayout {
                        id: grid_dform
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        anchors.centerIn: item_pform
                        visible: true
                        opacity: 1
                        spacing: 5
                        property int sumtotal: 4
                        property int suminvalid: txtfld_drug.valid + txtfld_pop.valid + txtfld_study.valid + txtfld_atc.valid
                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
//                            console.log("opacity: " + opacity + "visible: " + visible)
                        }
                        //a row
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Drug"
                            textfield.text: tbl_drugs.currentItem.modelData.name
                            textfield.onTextChanged: tbl_drugs.currentItem.modelData.name = textfield.text
                        }
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Atc"
                            textfield.text: tbl_drugs.currentItem.modelData.atc
                            textfield.onTextChanged: tbl_drugs.currentItem.modelData.atc = textfield.text
                        }
                        EzEditTextSet {
                            objectName: "domainsView" //Fake, to make it compile
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Population"
                            textfield.text: tbl_drugs.currentItem.modelData.population
                            textfield.onTextChanged: tbl_drugs.currentItem.modelData.population = textfield.text
                        }
                        EzEditTextSet {
                            objectName: "studiesView" //Fake, to make it compile
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Study Name"
                            textfield.text: tbl_drugs.currentItem.modelData.studyname
                            textfield.onTextChanged: tbl_drugs.currentItem.modelData.studyname = textfield.text
                        }
                    }

                    ColumnLayout {
                        id: grid_dextended
                        Layout.fillWidth: true
                        opacity: 1
                        visible: true
                        spacing: 5
                        property int sumtotal: 4
                        property int suminvalid: txtfld_comed.valid + txtfld_effness.valid + txtfld_tox.valid + txtfld_brand.valid



                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
//                            console.log("opacity: " + opacity + "visible: " + visible)
                        }
//                       visible: state == "extendedform"
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Comedication"
//                            textfield.text: tbl_drugs.currentItem.modelData.studyname
//                            textfield.onTextChanged: tbl_drugs.currentItem.modelData.studyname = textfield.text
                        }
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Effectiveness"
//                            textfield.text: tbl_drugs.currentItem.modelData.studyname
//                            textfield.onTextChanged: tbl_drugs.currentItem.modelData.studyname = textfield.text
                        }
                        EzEditTextSet {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            labeltext: "Brand"
//                            textfield.text: tbl_drugs.currentItem.modelData.indication
//                            textfield.onTextChanged: tbl_drugs.currentItem.modelData.indication = textfield.text
                        }
                    }
                }
            }
    }
}
