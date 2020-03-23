import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

import ezechiel 1.0


VirtualTab {
    id:tabRoot
    anchors.fill: parent
    ratio: 3


    property string dosageSelected: ""

    onIndexChanged: {
        if(index == 0) {
            dosageSelected = "Doses";
        } else if (index == 1) {
            dosageSelected = "Intervals";
        } else if (index = 2) {
            dosageSelected = "Infusions";
        } else {
            dosageSelected = "";
        }
    }


    leftPanel: Component {
        id: dosageSelectPanel

        Rectangle{
            id: dosageSelectContainer
            anchors.fill: parent
            color: Style.table.background.table


            ListView{
                id: dosageSelectListView
                objectName: "dosageSelectListView"
                anchors.fill: parent
                interactive: false
                currentIndex: -1

                model: ListModel {
                    id: dosageSelectModel
                    ListElement {
                        name: "Doses"
                        defaultValue: 0
                        defaultUnit: ""
                        any: false
                    }
                    ListElement {
                        name: "Intervals"
                        defaultValue:0
                        defaultUnit: ""
                        any:false

                    }
                    ListElement {
                        name: "Infusions"
                        defaultValue: 0
                        defaultUnit: ""
                        any:false

                    }
                }
                //TODO proper refresh of model above
                function refreshModel(){
                    dosageSelectModel.setProperty(0,"defaultValue",undoRedoController.currentDrug.doses.quantity.dbvalue)
                    dosageSelectModel.setProperty(0,"defaultUnit",undoRedoController.currentDrug.doses.quantity.unitstring)
                    dosageSelectModel.setProperty(0,"any",undoRedoController.currentDrug.doses.anyDose)

//                    //TODO once intervals are here
                    dosageSelectModel.setProperty(1,"defaultValue",undoRedoController.currentDrug.intervals.quantity.dbvalue)
//                    dosageSelectModel.setProperty(1,"defaultValue",undoRedoController.drugs.objat(0).intervals.quantity.dbvalue)
                    dosageSelectModel.setProperty(1,"defaultUnit",undoRedoController.currentDrug.intervals.quantity.unitstring)
                    dosageSelectModel.setProperty(1,"any",undoRedoController.currentDrug.intervals.any)

//                    //TODO once infusions are around
                    dosageSelectModel.setProperty(2,"defaultValue",undoRedoController.currentDrug.infusions.quantity.dbvalue)
                    dosageSelectModel.setProperty(2,"defaultUnit",undoRedoController.currentDrug.infusions.quantity.unitstring)
                    dosageSelectModel.setProperty(2,"any",undoRedoController.currentDrug.infusions.any)
                }

                Component.onCompleted: refreshModel()

                onCurrentIndexChanged: tabRoot.index = currentIndex

                onVisibleChanged: dosageSelectListView.refreshModel()


                delegate:  Item {
                    id: dosageSelectDelegate
                    height: ListView.view.height/3
                    width: ListView.view.width


                    ColumnLayout {
                        anchors.fill: parent

                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height:dosageSelectDelegate.height/2

                            MouseArea {

                                id: dosageSelectDelegateMouseArea
                                anchors.fill: parent
                                hoverEnabled: true

                                property bool hovered: false
                                onEntered: hovered = true
                                onExited: {hovered = false; }


                                onClicked: {//TODO signal stuff
                                    tabRoot.currentIndexChanged("dosageSelectListView",index)
                                    //                                    dosageSelectDelegate.ListView.view.currentIndex = index
//                                    dosageSelected = name
                                }

                            }
                            color: (dosageSelectDelegate.ListView.isCurrentItem) ? Style.flow.background.button_selected : ( dosageSelectDelegateMouseArea.hovered? Style.flow.background.button_hover : Style.flow.background.button_disabled)
                            radius: 20
                            Text {
                                id: dosesText
                                anchors.fill: parent
                                text: name
                                color: headerForeground
                                font.family: headerFontFamily
                                font.pixelSize: headerFontSize*2
                                font.bold: headerFontBold
                                verticalAlignment: headerVAlign
                                horizontalAlignment: headerHAlign
                            }
                        }
                        //xml attributes
                        RowLayout {
                            //                        anchors.fill: parent
                            //Default
                            Text {
                                id: defaultText
                                text: qsTr("Default:")
                                color: labelForeground
                                font.family:    labelFontFamily
                                font.pixelSize: labelFontSize
                                font.bold:      labelFontBold
                                verticalAlignment:   labelVAlign
                                horizontalAlignment: labelHAlign

                            }

                            TextField {
                                id: defaultInput
                                text: defaultValue//"400"//TODO
                                width: 20
                                font.family:    inputFontFamily
                                font.pixelSize: inputFontSize
                                verticalAlignment:   inputVAlign
                                horizontalAlignment: inputHAlign
                            }

                            Text {
                                id: unitText
                                text: qsTr("Unit:")
                                color: labelForeground
                                font.family:    labelFontFamily
                                font.pixelSize: labelFontSize
                                font.bold:      labelFontBold
                                verticalAlignment:   labelVAlign
                                horizontalAlignment: labelHAlign

                            }

                            TextField {
                                id: unitInput
                                text: defaultUnit//"mg"//TODO
                                font.family:    inputFontFamily
                                font.pixelSize: inputFontSize
                                verticalAlignment:   inputVAlign
                                horizontalAlignment: inputHAlign
                            }

                            Text {
                                id: anyText
                                text: qsTr("any? ")
                                color: labelForeground
                                font.family:    labelFontFamily
                                font.pixelSize: labelFontSize
                                font.bold:      labelFontBold
                                verticalAlignment:   labelVAlign
                                horizontalAlignment: labelHAlign

                            }

                            CheckBox{
                                id: anyInput
                                checked:model.any// objMod.anyDose//TODO
                            }


                        }

                    }
                }//end delegate
            }
        }

    }

    middlePanel: Rectangle {
        anchors.fill: parent
        anchors.margins: 5
        visible:  dosageSelected != ""

        property var modelSelected: (dosageSelected == "Doses") ? undoRedoController.doses.objlist: (dosageSelected == "Intervals" ? undoRedoController.intervals.objlist : (dosageSelected == "Infusions" ? undoRedoController.infusions.objlist: ""))
        ColumnLayout {
            id: rootDisplayLayout
            anchors.fill: parent
            spacing: 5

            //header
            RowLayout {
                Rectangle {
                    height: headerHeight
                    Layout.fillWidth: true
                    color: headerBackground
                    Text {
                        anchors.fill: parent
                        text: dosageSelected
                        color: headerForeground
                        font.family: headerFontFamily
                        font.pixelSize: headerFontSize
                        font.bold: headerFontBold
                        verticalAlignment: headerVAlign
                        horizontalAlignment: headerHAlign
                    }
                }

                Rectangle {
                    Layout.preferredWidth:  headerHeight
                    Layout.preferredHeight: headerHeight
                    border.width: cellFrameWidth
                    radius: cellFrameRadius
                    color:  headerBackground

                    Image {
                        anchors.fill: parent
                        source:  displayListView.currentIndex !== -1 ? "qrc:/icons/buttons/remove.png" : "qrc:/icons/buttons/remove_disabled.png"
                        fillMode: Image.PreserveAspectFit
                        antialiasing: true
                        mipmap: true

                        MouseArea {
                            anchors.fill: parent
                            enabled: displayListView.currentIndex !== -1
                            onClicked: {//TODO signal stuff
                                var currentIndex =  displayListView.currentIndex;
                                tabRoot.entityChanged(dosageSelected,currentIndex)
                            }
                        }
                    }
                }
                Rectangle {
                    Layout.preferredWidth:  headerHeight
                    Layout.preferredHeight: headerHeight
                    border.width: cellFrameWidth
                    radius: cellFrameRadius
                    color:  headerBackground

                    Image {
                        anchors.fill: parent
                        source: "qrc:/icons/buttons/add.png"
                        fillMode: Image.PreserveAspectFit
                        antialiasing: true
                        mipmap: true

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                tabRoot.entityChanged(dosageSelected,-1)
                            }
                        }
                    }
                }
            }

            Component {
                id:dosageDisplayDelegate
                Rectangle {
                    id: dosageDisplayDelegateContainer
                    width: ListView.view.width
                    height: rowHeight
                    color: dosageDisplayDelegateMouseArea.hovered ? rowBackgroundHover : (ListView.view.currentIndex === index ? rowBackgroundSelected : rowBackground)


                    property bool editionTime: false

                    MouseArea {
                        id:dosageDisplayDelegateMouseArea
                        anchors.fill: parent

                        hoverEnabled: true

                        property bool hovered: false
                        onEntered: hovered = true
                        onExited: hovered = false


                        onClicked: {
                            if(dosageDisplayDelegateContainer.ListView.view.currentIndex == index)
                                editionTime = true
                            else
                                //TODO send signal
                                currentIndexChanged("dosageDisplayView",index)
//                                dosageDisplayDelegateContainer.ListView.view.currentIndex = index;
                        }

                    }


                    Text {
                        id:dosageDisplayText
                        text: modelData.quantity.dbvalue
                        visible: !editionTime
                    }

                    TextField {
                        id:dosageDisplayInput
                        text: dosageDisplayText.text
                        visible: editionTime
                        onVisibleChanged: if(editionTime) forceActiveFocus();
                        onEditingFinished: {
                            tabRoot.textChanged(dosageSelected+"-"+index, text)
                            editionTime = false
                        }
                    }

                }
            }

            ListView {
                id: displayListView
                objectName: "dosageDisplayView"
                Layout.fillHeight: true
                Layout.fillWidth: true

                model: modelSelected
                delegate: dosageDisplayDelegate


            }

        }

    }

    rightPanel: CommentsView {
        commentModel:undoRedoController.dosesComments
        objectName: "doseCommentsView"
        rootIndex:0
    }

}
