import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

import guiutils.qml.controls 1.0
import guiutils.qml.controls.flow 1.0
import QtQuick.Window 2.2

import ezechiel 1.0

Rectangle {
    id: window
    width: 800
    height: 600

    property Component chart: Chart { objectName: "chartView" }
    property string ezfont: "sans-serif"
    Rectangle {
        id: flow
        anchors.fill: parent
//        color: "green"

        property date flowstarttime: new Date()
        Component.onCompleted: timer_session.start()
        property int activetabindex: 0
//        onActivetabindexChanged: console.log("atindex: " + activetabindex)

        property real textscalemin: 0.9
        PatientTabNew {id: tab_patient; objectName: "patientsView"; visible: flow.activetabindex == 0}
        DrugTabNew {id: tab_drug; objectName: "drugsView"; visible: flow.activetabindex == 1}
        DosageTabNew {id: tab_dosage; objectName: "dosagesView"; visible: flow.activetabindex == 2}
        CovariateTabNew {id: tab_covariate; objectName: "covariatesView"; visible: flow.activetabindex == 3}
        MeasureTabNew {id: tab_measure; objectName: "measuresView"; visible: flow.activetabindex == 4}
        TargetTabNew {id: tab_target; objectName: "targetsView"; visible: flow.activetabindex == 5}
        AdjustmentTabNew {id: tab_adjustment; objectName: "adjustmentsView"; visible: flow.activetabindex == 6}
        ValidationTabNew {id: tab_validation; objectName: "validationView"; visible: flow.activetabindex == 7}

        property int suminvalid: tab_patient.suminvalid + tab_drug.suminvalid + tab_dosage.suminvalid + tab_covariate.suminvalid + tab_measure.suminvalid
        property int sumtotal: tab_patient.sumtotal + tab_drug.sumtotal + tab_dosage.sumtotal + tab_covariate.sumtotal + tab_measure.sumtotal

        property real horiz_flowscale: flow.width/Screen.desktopAvailableWidth * 2
//        onHoriz_flowscaleChanged: console.log("hfscale: " + horiz_flowscale)
        property real vert_flowscale: flow.height/Screen.desktopAvailableHeight
//        onVert_flowscaleChanged: console.log("vfscale: " + vert_flowscale)
        property real flowscale: horiz_flowscale > vert_flowscale? vert_flowscale : horiz_flowscale
//        onFlowscaleChanged: console.log("fscale: " + flowscale)

        //PATHVIEW (a nicer tabview)
        Rectangle {
            id: rect_topbar
//            color: "red"
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width
            height: parent.height / 5
//            color: "#8a9ea4"//hospiatl turquoise
            color: "#00b6ff"//greece blue
//            RadialGradient {
//                    anchors.fill: parent
////                    horizontalRadius: parent.width
////                    verticalRadius: parent.height/2
//                    gradient: Gradient {
//                        GradientStop { position: 0.0; color: "#8a9ea4" }
//                        GradientStop { position: 0.999; color: "black" }
//                    }

//            }
            LinearGradient {
                   anchors.fill: parent
                   start: Qt.point(0, 0)
                   end: Qt.point(0, 300)
                   gradient: Gradient {
                       GradientStop { position: 0.0; color: "#00b6ff" }
                       GradientStop { position: 0.9; color: "#044ebb" }
                       GradientStop { position: 0.1; color: "#5cbdf4" }
                   }
               }

//            Image {
//                anchors.fill: parent
//                fillMode: Image.Tile
//                horizontalAlignment: Image.AlignLeft
//                verticalAlignment: Image.AlignTop
//                source: "qrc:/images/beigepaper.jpg"
//            }

            Rectangle {
               id: rect_pathview
               width: parent.width / 2; height: parent.height / 2
               x: parent.width / 4; y: parent.height / 4
               color: "transparent"
//               color: "blue"

                PathView {
                    id: pv_pathview
                    anchors.fill: parent
                    interactive: false
                    pathItemCount: 8
                    snapMode: PathView.SnapPosition
                    preferredHighlightBegin: 0.5; preferredHighlightEnd: 0.5
                    highlightRangeMode: ListView.StrictlyEnforceRange
                    model: ListModel {
                        ListElement {
                            name: "Patients"
                            icon: "qrc:/images/analysis.png"
                            help: "qrc:/images/help.png"
                        }
                        ListElement {
                            name: "Drugs"
                            icon: "qrc:/images/drugs.png"
                            help: "qrc:/images/help.png"
                        }
                        ListElement {
                            name: "Dosages"
                            icon: "qrc:/images/syringe.png"
                            help: "qrc:/images/help.png"
                        }
                        ListElement {
                            name: "Covariates"
                            icon: "qrc:/images/covariates.png"
                            help: "qrc:/images/help.png"
                        }
                        ListElement {
                            name: "Measures"
                            icon: "qrc:/images/measures.png"
                            help: "qrc:/images/help.png"
                        }
                        ListElement {
                            name: "Targets"
                            icon: "qrc:/images/target.png"
                            help: "qrc:/images/help.png"
                        }
                        ListElement {
                            name: "Adjustments"
                            icon: "qrc:/images/adjustment.png"
                            help: "qrc:/images/help.png"
                        }
                        ListElement {
                            name: "Validation"
                            icon: "qrc:/images/validation.png"
                            help: "qrc:/images/help.png"
                        }
                        ListElement {
                        }
                        ListElement {
                        }
                        ListElement {
                        }
                        ListElement {
                        }
                        ListElement {
                        }
                    }
                    delegate: delegate
                    path: Path {
                        startX: 0; startY: 0
                        PathAttribute { name: "iconScale"; value: 0.3 }
                        PathAttribute { name: "iconOpacity"; value: 0.3 }
                        PathAttribute { name: "iconZreal"; value: 0 }
                        PathLine { x: rect_pathview.width/2; y: rect_pathview.height/2; }
                        PathAttribute { name: "iconScale"; value: 1.0 }
                        PathAttribute { name: "iconOpacity"; value: 1.0 }
                        PathAttribute { name: "iconZreal"; value: 100 }
                        PathLine { x: rect_pathview.width; y: 0;}
                        PathAttribute { name: "iconScale"; value: 0.3 }
                        PathAttribute { name: "iconOpacity"; value: 0.3 }
                        PathAttribute { name: "iconZreal"; value: 0 }
                    }
                    onCurrentIndexChanged: flow.activetabindex = currentIndex
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.AllButtons
                        onClicked: {
//                            console.log("clicked, currnet pv index: " + pv_pathview.currentIndex)
                            switch(mouse.button) {
                                case Qt.RightButton:
                                    if (mouse.x > rect_pathview.width / 2 && pv_pathview.currentIndex > 0) {
                                        parent.decrementCurrentIndex()
                                    }
                                    else if (mouse.x < rect_pathview.width / 2 && pv_pathview.currentIndex < 6) {
                                        parent.incrementCurrentIndex()
                                    }
                                    break;
                                case Qt.LeftButton:
                                    if (mouse.x < rect_pathview.width / 2 && pv_pathview.currentIndex > 0) {
                                        parent.decrementCurrentIndex()
                                    }
                                    else if (mouse.x > rect_pathview.width / 2 && pv_pathview.currentIndex < 6) {
                                        parent.incrementCurrentIndex()
                                    }
                                    break;
                                case Qt.MiddleButton:
                                    if (pv_pathview.currentIndex < 6) {
                                        parent.incrementCurrentIndex()
                                    }
                                    break;
                            }
                        }
                        onWheel: {
                            if (wheel.angleDelta.y < 0 && pv_pathview.currentIndex > 0)
                                parent.decrementCurrentIndex();
                            else if (wheel.angleDelta.y > 0 && pv_pathview.currentIndex < 6) {
                                parent.incrementCurrentIndex();
                            }
                        }
                    }
                }
            }
        }

        Component {
            id: delegate
            Item {
                id: wrapper
                width: 100 * flow.flowscale * PathView.iconScale; height: 132 * flow.flowscale * PathView.iconScale
                opacity: PathView.iconOpacity
                z: PathView.iconZreal
                Column {
                    Image {
                        width: wrapper.width; height:wrapper.height - 32
                        source: icon
                    }
                    Row {
                        Text {
                            width: wrapper.width - 32; height: 32
                            text: name
                            font.family: "sans-serif"
                            fontSizeMode: Text.Fit
                            minimumPixelSize: 12
                            font.pixelSize: 20
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                        }
                        Image {
                            opacity: wrapper.PathView.isCurrentItem
                            Behavior on opacity { PropertyAnimation {easing.type: Easing.InOutQuad} }
                            width: 16; height: 16
                            source: help
                        }

                    }
                }
            }
        }

        //SESSION DETAILS
        Rectangle {
           id: rect_session
           anchors.left: flow.left
           anchors.top: flow.top
           width: flow.width / 5; height: rect_topbar.height
           y: 5
//           color: "pink"
           color: "transparent"
           transformOrigin: Item.TopLeft


           Grid {
               id: grid_session
               anchors.left: parent.left
               x: 5
               spacing: 2
               rows: 4
               columns: 3
               property real textrectwidth: rect_session.width * 0.5
               property real textrectheight: rect_session.height/6
               property real textnumwidth: rect_session.width * 0.05
               //a row
                Image {
                    width: 32 * flow.flowscale; height: 32 * flow.flowscale
                    source: "qrc:/images/hospital.png"
                }
               Rectangle {
                       color: "transparent"
                       width: grid_session.textrectwidth; height: grid_session.textrectheight
                       Text {
                       anchors.fill: parent
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       text: "Session Details"
                       font.family: ezfont
                       fontSizeMode: Text.Fit
                       minimumPixelSize: 12
                       font.pixelSize: 20
                       font.underline: true
                   }
               }
               Rectangle {
                   color: "transparent"
                   width: grid_session.textrectwidth; height: grid_session.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignLeft
                       verticalAlignment: Text.AlignVCenter
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
               //a row
                Image {
                    width: 32 * flow.flowscale; height: 32 * flow.flowscale
                    source: "qrc:/images/doctor.png"
                }
               Rectangle {
                   color: "transparent"
                   width: grid_session.textrectwidth; height: grid_session.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignRight
                       verticalAlignment: Text.AlignVCenter
                       text: "Logged in as: "
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
               Rectangle {
                   color: "transparent"
                   width: grid_session.textrectwidth; height: grid_session.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignLeft
                       verticalAlignment: Text.AlignVCenter
                       text: "Username"//need to get the user details in the gui
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
               //a row
                Image {
                    width: 32 * flow.flowscale; height: 32 * flow.flowscale
                    source: "qrc:/images/clock.png"
                }
               Rectangle {
                   color: "transparent"
                   width: grid_session.textrectwidth; height: grid_session.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignRight
                       verticalAlignment: Text.AlignVCenter
                       text: "Since logon: "
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
               Rectangle {
                   color: "transparent"
                   width: grid_session.textrectwidth; height: grid_session.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignLeft
                       verticalAlignment: Text.AlignVCenter
                       text: "0"//need to get the user details in the gui
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
               //a row
                Image {
                    width: 32 * flow.flowscale; height: 32 * flow.flowscale
                    source: "qrc:/images/clock.png"
                }
               Rectangle {
                   color: "transparent"
                   width: grid_session.textrectwidth; height: grid_session.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignRight
                       verticalAlignment: Text.AlignVCenter
                       text: "Editing time: "
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
               Rectangle {
                   color: "transparent"
                   width: grid_session.textrectwidth; height: grid_session.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignLeft
                       verticalAlignment: Text.AlignVCenter
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                        Timer {
                            id: timer_session
                            interval: 500; running: true; repeat: true
                            onTriggered: {
                                var msecs = new Date().getTime() - flow.flowstarttime.getTime()
                                var mins = Math.round(msecs / 60000 % 60)
                                var secs = Math.round(msecs / 1000 % 60)
                                var hrs = Math.round(msecs / 3600000 % 24)
                                parent.text =  hrs + " h " + mins + " m " + secs + " s "
                            }
                        }
                   }
               }
           }
        }

        //STATUS SPOT
        Rectangle {
           id: rect_statusspot
           anchors.right: flow.right
           anchors.top: flow.top
           width: flow.width / 5; height: rect_topbar.height
           y: 5
//           color: "pink"
           color: "transparent"
           transformOrigin: Item.TopRight

           Grid {
               id: grid_statusspot
               anchors.right: parent.right
               x: 5
               spacing: 2
               rows: 3
               columns: 3
               property real textrectwidth: rect_statusspot.width * 0.5
               property real textrectheight: rect_statusspot.height/6
               property real textnumwidth: rect_statusspot.width * 0.05
               //a row
                Image {
                    width: 32 * flow.flowscale; height: 32 * flow.flowscale
                    source: "qrc:/images/validation.png"
                }
               Rectangle {
                   id: rect_incomplete_items
                   color: "transparent"
                   width: grid_statusspot.textrectwidth; height: grid_statusspot.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       text: "Completion Status"
                       font.family: ezfont
                       fontSizeMode: Text.Fit
                       minimumPixelSize: 12
                       font.pixelSize: 20
                       font.underline: true
                   }
               }
               Rectangle {
                   color: "transparent"
                   width: grid_statusspot.textnumwidth; height: grid_statusspot.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignLeft
                       verticalAlignment: Text.AlignVCenter
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
               // a row
                Image {
                    width: 32 * flow.flowscale; height: 32 * flow.flowscale
                    source: "qrc:/images/redpencil.png"
                }
               Rectangle {
                   color: "transparent"
                   width: grid_statusspot.textrectwidth; height: grid_statusspot.textrectheight
                       Text {
                           anchors.fill: parent
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            text: "Incomplete: "
                            font.family: ezfont
                            fontSizeMode: Text.Fit
                            minimumPixelSize: 12
                            font.pixelSize: 20
                       }
               }
               Rectangle {
                   color: "transparent"
                   width: grid_statusspot.textnumwidth; height: grid_statusspot.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignLeft
                       verticalAlignment: Text.AlignVCenter
                       text: flow.suminvalid
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
               //a row
                Image {
                    width: 32 * flow.flowscale; height: 32 * flow.flowscale
                    source: "qrc:/images/pencil.png"
                }
               Rectangle {
                   color: "transparent"
                   width: grid_statusspot.textrectwidth; height: grid_statusspot.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignRight
                       verticalAlignment: Text.AlignVCenter
                       text: "Total Items: "
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
               Rectangle {
                   color: "transparent"
                   width: grid_statusspot.textnumwidth; height: grid_statusspot.textrectheight
                   Text {
                           anchors.fill: parent
                       horizontalAlignment: Text.AlignLeft
                       verticalAlignment: Text.AlignVCenter
                       text: flow.sumtotal
                        font.family: ezfont
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 12
                        font.pixelSize: 20
                   }
               }
           }
        }
    }
}
