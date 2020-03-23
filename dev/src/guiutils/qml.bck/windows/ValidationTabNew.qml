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
            id: validationstab
            leftPanel: form
            rightPanel: window.chart
        }
    }

    //left side
    property Component form: Item {
        id: item_pform
        z:10
        anchors.fill: parent
        Component.onCompleted: console.log("validation form loaded")
        Rectangle {
            id: rect_pformmain
            anchors.fill: parent
            color: "transparent"
            Rectangle {
                width: Math.max(grid_pform.width, tbl_validations.width)
                height: grid_pform.visible * grid_pform.height + tbl_validations.visible * tbl_validations.height + grid_extended.visible * grid_extended.height + rect_ring1.height + rect_ring2.height + rect_ring3.height
                x: (parent.width - width)/2
                y: (parent.height - height)/2
                color: "transparent"
//                color: "blue"
                Behavior on height { PropertyAnimation {easing.type: Easing.InOutQuad} }
                onHeightChanged: console.log("column height: " + height)

//                states: [
//                    State {
//                        name: "valid"
//                        PropertyChanges { target: tbl_validations; visible: true }
//                    },
//                    State {
//                        name: "invalid"
//                        PropertyChanges { target: tbl_validations; visible: false}
//                    }
//                ]
                Column {
                    move: Transition {
                            NumberAnimation { properties: "x,y"; duration: 250 }
                        }
                    Rectangle {
                        id: rect_ring1
                        scale: 0.5 * flow.flowscale
                        width: 128
                        height: 128
                        color: "transparent"
                        x: (grid_pform.width - width)/2
                        AnimatedSprite {
                            id: anim_ring1
                            frameRate: 50
                            width: 128
                            height: 128
                            source: "qrc:/sprites/blueringrotate.png"
                            frameWidth: 128
                            frameHeight: 128
                            frameCount: 40
                            running: false
                            loops: 1
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    anim_ring1.running = true
                                    anim_ring1.reverse = grid_pform.visible
                                    grid_pform.opacity = !grid_pform.opacity
                                }
                            }
                        }
                        Image {
                            anchors.left: anim_ring1.right
                            source: grid_pform.suminvalid == grid_pform.sumtotal? "qrc:/images/itemvalid.png" : "qrc:/images/iteminvalid.png"
                            width: 64
                            height: 64
                        }
                    }

                   //only show grid or forms, not both, managed by states
                    Grid {
                        id: grid_pform
                        visible: false
                        width: columns*2/3 * cellw + (columns - 1) * spacing
                        height: rows * cellh + (rows - 1) * spacing
                        spacing: 5
                        columns: 6
                        rows: 2
                        property int cellw: TableStyle.cell.colwidth
                        property int cellh: TableStyle.cell.height
                        property int sumtotal: 4
                        property int suminvalid: txtfld_fname.valid + txtfld_lname.valid + txtfld_bday.valid + txtfld_weight.valid
                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
//                            console.log("opacity: " + opacity + "visible: " + visible)
                        }
                        //a row
                        Rectangle {
                            color: "transparent"
                               width: grid_pform.cellw; height: grid_pform.cellh
                           EzText {
                               id: txt_fname
                               text: "First Name"
                               horizontalAlignment: Text.AlignRight
                               anchors.fill: parent
                           }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_fname.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                       TextField {
                           id: txtfld_fname
                           property bool valid: text != ""
                           width: grid_pform.cellw; height: grid_pform.cellh
                           style: EzTextFieldStyle {}
                        }
                        //a row
                        Rectangle {
                            color: "transparent"
                               width: grid_pform.cellw; height: grid_pform.cellh
                           EzText {
                               id: txt_lname
                               anchors.fill: parent
                               text: "Last Name"
                               horizontalAlignment: Text.AlignRight
                           }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_lname.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                       TextField {
                           id: txtfld_lname
                           property bool valid: text != ""
                           width: grid_pform.cellw; height: grid_pform.cellh
                           style: EzTextFieldStyle {}
                       }
                        //a row
                        Rectangle {
                            color: "transparent"
                               width: grid_pform.cellw; height: grid_pform.cellh
                           EzText {
                               id: txt_bday
                               anchors.fill: parent
                               text: "Birth Date"
                               horizontalAlignment: Text.AlignRight
                           }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_bday.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                           TextField {
                           id: txtfld_bday
                           property bool valid: text != ""
                           width: grid_pform.cellw; height: grid_pform.cellh
                           style: EzTextFieldStyle {}
                           }
                        //a row
                        Rectangle {
                            color: "transparent"
                               width: grid_pform.cellw; height: grid_pform.cellh
                            EzText {
                            id: txt_weight
                            anchors.fill: parent
                            text: "Weight"
                            horizontalAlignment: Text.AlignRight
                            }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_weight.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                        TextField {
                            id: txtfld_weight
                           property bool valid: text != ""
                           width: grid_pform.cellw; height: grid_pform.cellh
                            text: "78"
                            style: EzTextFieldStyle {}
                        }
                    }

                    Rectangle {
                        id: rect_ring2
                        scale: 0.5 * flow.flowscale
                        width: 128
                        height: 128
                        color: "transparent"
                        x: (grid_pform.width - width)/2
                        AnimatedSprite {
                            id: anim_ring2
                            frameRate: 50
                            width: 128
                            height: 128
                            source: "qrc:/sprites/blueringrotate.png"
                            anchors.fill: parent
                            frameWidth: 128
                            frameHeight: 128
                            frameCount: 40
                            running: false
                            loops: 1
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    anim_ring2.running = true
                                    anim_ring2.reverse = grid_extended.visible
                                    grid_extended.opacity = !grid_extended.opacity
                                }
                            }
                        }
                        Image {
                            anchors.left: anim_ring2.right
                            source: grid_extended.suminvalid == grid_extended.sumtotal? "qrc:/images/itemvalid.png" : "qrc:/images/iteminvalid.png"
                            width: 64
                            height: 64
                        }
                    }

                    Grid {
                        id: grid_extended
                        opacity: 0
                        visible: false
                        width: columns*2/3 * cellw + (columns - 1) * spacing
                        height: rows * cellh + (rows - 1) * spacing
                        spacing: 5
                        columns: 6
                        rows: 4
                        property int cellw: TableStyle.cell.colwidth
                        property int cellh: TableStyle.cell.height
                        property int sumtotal: 7
                        property int suminvalid: txtfld_extid.valid + txtfld_staynbr.valid + txtfld_country.valid + txtfld_city.valid + txtfld_addr.valid + txtfld_postcode.valid + txtfld_state.valid



                        Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                        onOpacityChanged: {
                            opacity == 0 ? visible = false : visible = true
//                            console.log("opacity: " + opacity + "visible: " + visible)
                        }
//                       visible: state == "extendedform"
                        //a row
                        Rectangle {
                                color: "transparent"
                                   width: grid_pform.cellw; height: grid_pform.cellh
                                EzText {
                                    id: txt_extid
                                    anchors.fill: parent
                                    text: "IPP"
                                    horizontalAlignment: Text.AlignRight
                                }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_extid.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                        TextField {
                            id: txtfld_extid
                           property bool valid: text != ""
                            width: grid_pform.cellw; height: grid_pform.cellh
                            text: ""
                            style: EzTextFieldStyle {}
                        }
                        //a row
                        Rectangle {
                                color: "transparent"
                                   width: grid_pform.cellw; height: grid_pform.cellh
                                EzText {
                                    id: txt_staynbr
                                    anchors.fill: parent
                                    text: "Stay Number"
                                    horizontalAlignment: Text.AlignRight
                                }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_staynbr.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                        TextField {
                            id: txtfld_staynbr
                           property bool valid: text != ""
                            width: grid_pform.cellw; height: grid_pform.cellh
                            text: ""
                            style: EzTextFieldStyle {}
                        }
                        //a row
                        Rectangle {
                                color: "transparent"
                                   width: grid_pform.cellw; height: grid_pform.cellh
                                EzText {
                                    id: txt_country
                                    anchors.fill: parent
                                    text: "Country"
                                    horizontalAlignment: Text.AlignRight
                                }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_country.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                        TextField {
                            id: txtfld_country
                           property bool valid: text != ""
                            width: grid_pform.cellw; height: grid_pform.cellh
                            text: ""
                            style: EzTextFieldStyle {}
                        }
                        //a row
                        Rectangle {
                                color: "transparent"
                                   width: grid_pform.cellw; height: grid_pform.cellh
                                EzText {
                                    id: txt_city
                                    anchors.fill: parent
                                    text: "Country"
                                    horizontalAlignment: Text.AlignRight
                                }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_city.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                        TextField {
                            id: txtfld_city
                           property bool valid: text != ""
                            width: grid_pform.cellw; height: grid_pform.cellh
                            text: ""
                            style: EzTextFieldStyle {}
                        }
                        //a row
                        Rectangle {
                                color: "transparent"
                                   width: grid_pform.cellw; height: grid_pform.cellh
                                EzText {
                                    id: txt_state
                                    anchors.fill: parent
                                    text: "State"
                                    horizontalAlignment: Text.AlignRight
                                }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_state.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                        TextField {
                            id: txtfld_state
                           property bool valid: text != ""
                            width: grid_pform.cellw; height: grid_pform.cellh
                            text: ""
                            style: EzTextFieldStyle {}
                        }
                        //a row
                        Rectangle {
                                color: "transparent"
                                   width: grid_pform.cellw; height: grid_pform.cellh
                                EzText {
                                    id: txt_addr
                                    anchors.fill: parent
                                    text: "Address"
                                    horizontalAlignment: Text.AlignRight
                                }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_addr.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                        TextField {
                            id: txtfld_addr
                           property bool valid: text != ""
                            width: grid_pform.cellw; height: grid_pform.cellh
                            text: ""
                            style: EzTextFieldStyle {}
                        }
                        //a row
                        Rectangle {
                                color: "transparent"
                                   width: grid_pform.cellw; height: grid_pform.cellh
                                EzText {
                                    id: txt_postcode
                                    anchors.fill: parent
                                    text: "Postcode"
                                    horizontalAlignment: Text.AlignRight
                                }
                        }
                        Image {
                           width: grid_pform.cellh; height: grid_pform.cellh
                            source: txtfld_postcode.text == ""? "qrc:/images/redpencil.png" : "qrc:/images/pencil.png"
                        }
                        TextField {
                            id: txtfld_postcode
                           property bool valid: text != ""
                            width: grid_pform.cellw; height: grid_pform.cellh
                            text: ""
                            style: EzTextFieldStyle {}
                        }
                    }


                    Rectangle {
                        id: rect_ring3
                        scale: 0.5 * flow.flowscale
                        width: 128
                        height: 128
                        color: "transparent"
                        x: (grid_pform.width - width)/2
                        AnimatedSprite {
                            id: anim_ring3
                            frameRate: 50
                            width: 128
                            height: 128
                            source: "qrc:/sprites/blueringrotate.png"
                            anchors.fill: parent
                            frameWidth: 128
                            frameHeight: 128
                            frameCount: 40
                            running: false
                            loops: 1
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    anim_ring3.running = true
                                    anim_ring3.reverse = tbl_validations.visible
                                    tbl_validations.opacity = !tbl_validations.opacity
                                }
                            }
                        }
                        Image {
                            anchors.left: anim_ring3.right
                            source: grid_extended.suminvalid == grid_extended.sumtotal? "qrc:/images/itemvalid.png" : "qrc:/images/iteminvalid.png"
                            width: 64
                            height: 64
                        }
                    }

                    Rectangle {
                        width: 400
                        height: 300
                        color: "transparent"
                        CovariateTable {
                            id: tbl_validations
                            x: (parent.width - width)/2
                            anchors.fill: parent
                            visible: false
                            Behavior on opacity { PropertyAnimation {duration: 1000; easing.type: Easing.InOutQuad} }
                            onOpacityChanged: {
                                opacity == 0 ? visible = false : visible = true
                //                            console.log("opacity: " + opacity + "visible: " + visible)
                            }
                        }

                    }
                }
            }

        }
    }
}
