import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

import ezechiel 1.0



VirtualTab {
    id: tabRoot
    anchors.fill: parent
    ratio: 3

    ///in this tab the main index (A.K.A. tabRoot.index) represent the middlePanel index (binds the selected target to its comments)

    property string targetSelected: ""


    leftPanel: Rectangle {
        id: targetSelectPanel

        anchors.fill: parent
        color: Style.table.background.table



        Component {
            id: targetSelectDelegateComponent
            Item {
                id: targetSelectDelegate
                height: ListView.view.height/ListView.view.count
                width: ListView.view.width

                ColumnLayout {
                    anchors.fill: parent
                    Rectangle {
                        width: parent.width
                        height: parent.height/2
                        color: (targetSelectDelegate.ListView.isCurrentItem) ? Style.flow.background.button_selected : ( targetSelectDelegateMouseArea.hovered? Style.flow.background.button_hover : Style.flow.background.button_disabled)
                        radius: 20
                        MouseArea {
                            id: targetSelectDelegateMouseArea
                            anchors.fill: parent

                            hoverEnabled: true
                            property bool hovered: false
                            onEntered: hovered = true
                            onExited: {hovered = false; }

                            onClicked: tabRoot.currentIndexChanged("targetSelectListView-"+type,index)
                        }

                        Text {
                            anchors.fill: parent
                            text:model["type"]
                            color: headerForeground
                             font.family: headerFontFamily
                            font.pixelSize: headerFontSize*2
                            font.bold: headerFontBold
                            verticalAlignment: headerVAlign
                            horizontalAlignment: headerHAlign
                        }
                    }

                    RowLayout {
                        width:parent.width
                        Text {
                            text:"concentration unit:"
                        }
                        TextField {
                            text:model.unitConcentration
                        }
                        Text {
                            text:"Time unit:"
                        }
                        TextField {
                            text: model.unitTime
                        }
                    }


                }
            }
        }


        ListView {
            id: targetSelectListView
            objectName: "targetSelectListView"
            anchors.fill: parent
            interactive: false
            currentIndex: -1
            delegate: targetSelectDelegateComponent
            model: ListModel { //TODO link to proper model
                id: targetSelectModel
                ListElement {
                    type: "Peak"
                    unitConcentration: "mg/l"
                    unitTime: "h"
                }
                ListElement {
                    type: "Residual"
                    unitConcentration: "mg/l"
                    unitTime: "h"
                }
                ListElement {
                    type: "Mean"
                    unitConcentration: "mg/l"
                    unitTime: "h"
                }
            }

            function refreshModel() {
                //TODO set units for concentration and time once there is single place where to find them. Somewhere inside the TargetList would be nice
//                targetSelectModel.setProperty(0,"unitConcentration", undoRedoController.currentDrug.)
            }

            onCurrentIndexChanged: {
                if(currentIndex == 0) {
                    tabRoot.targetSelected = "Peak";
                } else if (currentIndex == 1) {
                    tabRoot.targetSelected = "Residual";
                } else if (currentIndex == 2) {
                    tabRoot.targetSelected = "Mean";
                } else
                    tabRoot.targetSelected = "";

                reset()
            }
        }
    }

    middlePanel: Rectangle {
        id: displayPanel
        objectName: "targetDisplayPanel"
        anchors.fill: parent
        anchors.margins: 5
        visible: targetSelected != ""

        property int leftIndex: -1
        property int rightIndex: -1
        property int index: -1




        property var listViewLeft:displayListViewTemplate.createObject(targetDisplayListViewsLayout,{"objectName":"targetDisplayListView_left"})
        property var listViewRight: displayListViewTemplate.createObject(targetDisplayListViewsLayout,{"position":"right","objectName":"targetDisplayListView_right"})

        function resetIndexes(){
            leftIndex = -1
            rightIndex = -1
            index = -1
        }

        Component.onCompleted: tabRoot.reset.connect(resetIndexes)

        onIndexChanged: {
            listViewLeft.currentIndex = index
            listViewRight.currentIndex = index
            tabRoot.index = index
        }

        ColumnLayout {
            id: rootDisplayLayout
            anchors.fill: parent
            spacing: 5

            //main Header
            RowLayout {

                Component.onCompleted: {
                    headerText.createObject(this,{"textLocation":"targetSelected"})
                    headerButton.createObject(this,{"imageSrc":"qrc:/icons/buttons/remove.png","imageDisabledSrc":"qrc:/icons/buttons/remove_disabled.png"})
                    headerButton.createObject(this,{"imageSrc":"qrc:/icons/buttons/add.png"})

                }
            }

            //sub Headers
            RowLayout {

                Component.onCompleted: {

                    //TODO indexes for buttons
                    headerText.createObject(this,{"textBox":"concentrations"})
//                    headerButton.createObject(this,{"position":"left","imageSrc":"qrc:/icons/buttons/remove.png","imageDisabledSrc":"qrc:/icons/buttons/remove_disabled.png"})
//                    headerButton.createObject(this,{"position":"left","imageSrc":"qrc:/icons/buttons/add.png"})

                    headerText.createObject(this,{"textBox":"times"})
//                    headerButton.createObject(this,{"position":"right","imageSrc":"qrc:/icons/buttons/remove.png","imageDisabledSrc":"qrc:/icons/buttons/remove_disabled.png"})
//                    headerButton.createObject(this,{"position":"right","imageSrc":"qrc:/icons/buttons/add.png"})
                }

            }
            //cells Headers
            RowLayout {
                Component.onCompleted: {
                    headerText.createObject(this,{"textBox":"min"})
                    headerText.createObject(this,{"textBox":"max"})
                    headerText.createObject(this,{"textBox":"best"})
                    headerText.createObject(this,{"textBox":"min"})
                    headerText.createObject(this,{"textBox":"max"})
                    headerText.createObject(this,{"textBox":"best"})
                }
            }

            //ListViews
            RowLayout {
                id:targetDisplayListViewsLayout

                Component.onCompleted: {
//                    displayListViewTemplate.createObject(this,{"objectName":"targetDisplayListView_left"})
//                    displayListViewTemplate.createObject(this,{"position":"right","objectName":"targetDisplayListView_right"})
                }
            }

            Component {
                id: headerText
                Rectangle {
                    Layout.preferredHeight: headerHeight
                    Layout.fillWidth: true
                    color: headerBackground
                    property string textBox
                    property string textLocation
                    Text {
                        anchors.fill: parent
                        text: (textBox !== "") ? textBox : tabRoot[textLocation]
                        color: headerForeground
                        font.family: headerFontFamily
                        font.pixelSize: headerFontSize
                        font.bold: headerFontBold
                        verticalAlignment: headerVAlign
                        horizontalAlignment: headerHAlign
                    }
                }
            }

            Component {
                id: headerButton
                Rectangle {
                    Layout.preferredHeight: headerHeight
                    Layout.preferredWidth: headerHeight
                    color: headerBackground
                    property string position: ""
                    property string entityName: tabRoot.targetSelected
                    property int onClickIndex: (imageDisabledSrc === "" ? -1 : (position == "" ? displayPanel.index : displayPanel[position+"Index"]))
                    property string imageSrc: ""
                    property string imageDisabledSrc: ""
                    Image {
                        id: headerButtonImage
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        antialiasing: true
                        mipmap: true
                        source: imageDisabledSrc === "" ? imageSrc : (onClickIndex !== -1 ? imageSrc : imageDisabledSrc)
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(imageDisabledSrc !== "" && tabRoot.index === -1) {
                                    return;
                                }

                                tabRoot.entityChanged(entityName+"-"+position,onClickIndex)
                                //TODO take into account currentIndex&Co refresh
                            }
                        }
                    }
                }
            }

            Component {
                id: displayListViewTemplate
                ListView {
                    property string position: "left"

                    property string valueType: (position == "left") ? "c" : (position == "right" ? "t" : "")

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    highlightFollowsCurrentItem: true

                    onCurrentIndexChanged: displayPanel[position+"Index"] = currentIndex


                    model:undoRedoController.targets.objlist

                    delegate: Rectangle {
                        id: displayListViewDelegate
                        height: (modelData.name(modelData.type) === tabRoot.targetSelected) ? rowHeight : 0
                        width: ListView.view.width


                        visible: modelData.name(modelData.type) === tabRoot.targetSelected

                        onVisibleChanged: if(visible) console.log("type: "+modelData.name(modelData.type)+"; index: "+index)

                        color: (ListView.isCurrentItem) ? rowBackgroundSelected : rowBackground

                        //cells
                        RowLayout {
                            anchors.fill: parent
                            Component.onCompleted: {
                                listViewEditableCell.createObject(this,{"role":valueType+"min"})
                                listViewEditableCell.createObject(this,{"role":valueType+"max"})
                                listViewEditableCell.createObject(this,{"role":valueType+"best"})
                            }

                        }


                        Component {
                            id: listViewEditableCell
                            Rectangle {

                                property string role
                                property string position: parent.parent.ListView.view.position
                                property bool editionTime: false

                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: listViewEditableCellMouseArea.hovered ? rowBackgroundHover : "transparent"
                                Text {
                                    id: listViewCellText
                                    anchors.fill: parent

                                    color: listViewEditableCellMouseArea.hovered ? rowForegroundHover: (displayListViewDelegate.ListView.isCurrentItem ? rowForegroundSelected : rowForeground)
                                    font.family: rowFontFamily
                                    font.pixelSize: rowFontSize
                                    wrapMode: Text.WordWrap
                                    visible: !editionTime
                                    verticalAlignment:   rowVAlign
                                    horizontalAlignment: headerHAlign
                                    text:modelData[role].dbvalue
                                }

                                TextField {
                                    id: listViewCellEditorField
                                    anchors.fill: parent
                                    text: listViewCellText.text
                                    font.pixelSize: rowFontSize
                                    visible: editionTime
                                    onVisibleChanged: if(visible) forceActiveFocus()
                                    onEditingFinished: {
                                        tabRoot.textChanged(role+"-"+index+"-"+tabRoot.targetSelected,text)
//                                        listViewCellText.text = text
                                        editionTime = false
                                    }
                                }

                                MouseArea {
                                    id: listViewEditableCellMouseArea
                                    anchors.fill: parent

                                    property bool hovered: false
                                    hoverEnabled: true

                                    onEntered: hovered = true
                                    onExited: hovered = false

                                    onClicked: {
                                        if(displayListViewDelegate.ListView.isCurrentItem)
                                            editionTime = true
                                        else {
                                            tabRoot.currentIndexChanged("targetDisplayPanel",index)
//                                            tabRoot.currentIndexChanged("targetDisplayListView_"+position,index)
//                                        displayListViewDelegate.ListView.view.currentIndex = index
                                        }
                                    }
                                }
                            }
                        }


                    }//end of delegate
                }//end of listView

            }//end of listViewTemplate Component


        }
    }

    rightPanel: CommentsView {
        objectName: "targetCommentsView"
        commentModel: undoRedoController.targetComments
    }

}
