import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
//import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

VirtualTab {
    id:tabRoot
    ratio: 3



    property string conversionSelected: ""

    leftPanel: Rectangle {
        id: conversionSelectContainer
        anchors.fill: parent
        anchors.margins: 5
        color: Style.table.background.table


        ColumnLayout {
            anchors.fill: parent

            ListView {
                id: conversionSelectListView
               objectName: "conversionSelectListView"

               anchors.fill: parent
               interactive: false
               currentIndex: -1

               model: ListModel {
                   id: conversionSelectModel
                   ListElement {
                       name: "ADME"
                   }
                   ListElement {
                       name: "Halflife"
                   }
                   ListElement {
                       name: "Conversion"
                   }
                   ListElement {
                       name: "Error model"
                   }
               }

               delegate: Item {
                   id: conversionSelectDelegate
                   height: ListView.view.height/ListView.view.count
                   width: ListView.view.width

                   ColumnLayout {
                       anchors.fill: parent
                       Rectangle {
                           width: parent.width
                           height: parent.height/2
                           color: (conversionSelectDelegate.ListView.isCurrentItem) ? Style.flow.background.button_selected : ( conversionSelectDelegateMouseArea.hovered? Style.flow.background.button_hover : Style.flow.background.button_disabled)
                           radius: 20
                           MouseArea {
                               id: conversionSelectDelegateMouseArea
                               anchors.fill: parent

                               hoverEnabled: true
                               property bool hovered: false
                               onEntered: hovered = true
                               onExited: {hovered = false; }
                               //TODO onClicked
                               onClicked: {
//                                   conversionSelectDelegate.ListView.view.currentIndex = index
                                   tabRoot.currentIndexChanged("conversionSelectListView-"+name,index)
                               }
                           }

                           Text {
                               anchors.fill: parent
                               text:model["name"]
                               color: headerForeground
                               font.family: headerFontFamily
                               font.pixelSize: headerFontSize*2
                               font.bold: headerFontBold
                               verticalAlignment: headerVAlign
                               horizontalAlignment: headerHAlign
                           }
                       }




                   }


               }

               onCurrentIndexChanged: selectionUpdate()

               function selectionUpdate(){
                   if(currentIndex == 0) {
                       tabRoot.conversionSelected = "ADME";
                   } else if (currentIndex == 1) {
                       tabRoot.conversionSelected = "Halflife";
                   } else if (currentIndex == 2) {
                       tabRoot.conversionSelected = "Conversion";
                   } else if (currentIndex == 3) {
                       tabRoot.conversionSelected = "Error_Model";
                   } else
                       tabRoot.conversionSelected = "";
                   reset()
               }



               onVisibleChanged: {
                      currentIndex = -1
                   selectionUpdate()
               }

            }


        }

    }

    middlePanel:Rectangle {
        id: conversionDisplayContainer
        objectName: "conversionDisplayPanel"
        anchors.fill: parent
        anchors.margins: 5



        property var modelSelected: layoutUpdate()
        function layoutUpdate(){
            if(conversionSelected == "ADME") {
                admeLayout.visible = true
                halflifeLayout.visible = false
                conversionLayout.visible = false
                errorModelLayout.visible = false
            } else if (conversionSelected == "Halflife") {
                admeLayout.visible = false
                halflifeLayout.visible = true
                conversionLayout.visible = false
                errorModelLayout.visible = false
            } else if (conversionSelected == "Conversion") {
                admeLayout.visible = false
                halflifeLayout.visible = false
                conversionLayout.visible = true
                errorModelLayout.visible = false

            } else if (conversionSelected == "Error_Model") {
                admeLayout.visible = false
                halflifeLayout.visible = false
                conversionLayout.visible = false
                errorModelLayout.visible = true
            } else {
                admeLayout.visible = false
                halflifeLayout.visible = false
                conversionLayout.visible = false
                errorModelLayout.visible = false
            }
        }

        property var admeModel: undoRedoController.adme
        property var admeArray: {"Default Intake":"defaultIntake","Distribution":"distribution","Elimination":"elimination"}

        property var halfLifeModel: undoRedoController.halflife
        property var halflifeArray :{"value":"value","multiplier":"multiplier","unit":"unitString"}

        property var errorModelModel: undoRedoController.errorModel
        property var errorModelArray: {"additive":"additive","proportional":"proportional"}


        property var admeLayout: displayLayoutTemplate
        .createObject(this,{"model":admeModel,"modelArray":admeArray})

        property var halflifeLayout: displayLayoutTemplate
        .createObject(this,{"model":halfLifeModel,"modelArray":halflifeArray})

        property var errorModelLayout: displayLayoutTemplate
        .createObject(this,{"model":errorModelModel,"modelArray":errorModelArray})

        Rectangle {
            id:conversionLayout
            anchors.fill:conversionDisplayContainer
            visible: false
            Text {text: qsTr("sorry, conversions not supported yet");}
        }

        Component {
            id:displayLayoutTemplate
            ColumnLayout {
                anchors.fill: parent

                property var model
                property var modelArray

                visible: false

                Component.onCompleted: {
                    for (var prop in modelArray) {
                        displayEditableRow.createObject(this,{"name":prop,"role":modelArray[prop],"model":model});
//                        console.log("prop: "+prop+"; "+modelArray[prop]+" = "+model[modelArray[prop]])
                    }
                }

                Component {
                    id: displayEditableRow
                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        property string name
                        property string role
                        property var model

                        property string txt: (role == "defaultIntake" ) ? model.routeToString(model[role])+"(not working)" : model[role]

                        RowLayout {
                            id: displayEditableRowLayout
                            width: parent.width
                            height: parent.height

                            Text {
                                id:displayText
                                text: qsTr(name)
                            }

                            TextField {
                                id: displayField
                                Layout.fillWidth: true
                                placeholderText: qsTr(txt)

                                text: txt
                                onTextChanged: {
                                    if(role != "defaultIntake") //FIXME erase me once route been taken care of
                                    tabRoot.textChanged(conversionSelected+"-"+role,text)
                                }
                                font.family: inputFontFamily
                                font.pixelSize: inputFontSize
                                verticalAlignment: inputVAlign
                                horizontalAlignment: inputHAlign
                            }
                        }
                    }
                }


                //header
                RowLayout {
                    //Title
                    Rectangle {
                        height: headerHeight
                        Layout.fillWidth: true
                        color: headerBackground
                        Text {
                            anchors.fill: parent
                            text: conversionSelected == "Error_Model" ? "Error Model" : conversionSelected
                            color: headerForeground
                            font.family: headerFontFamily
                            font.pixelSize: headerFontSize
                            font.bold: headerFontBold
                            verticalAlignment: headerVAlign
                            horizontalAlignment: headerHAlign
                        }
                    }
                }

            }
        }
    }

    rightPanel: CommentsView {
        objectName: "covariateCommentsView"
        //TODO commentModel: using smth like .objat(index).comments
        commentModel: {
            if(conversionSelected == "ADME") {
                return undoRedoController.admeComments
            } else if(conversionSelected == "Halflife") {
                return undoRedoController.halflifeComments
            } else if (conversionSelected == "Error_Model") {
                return undoRedoController.errorModelComments;
            } else {
                return "";
            }
        }
        rootIndex:0//FIXME ERASEME
    }
}
