import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    title: "Known sentences"
    width: 600
    height: 300

    property var self
    property var sectionRef
    property var objlist
    property var paletteChoice: true


    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            self.exit(false);
        }
    }

    function initAddSentence(_sectionRef, _objlist){
        sectionRef = _sectionRef
        objlist = _objlist
        paletteChoice = false
        self = this
    }


    function init(_sectionRef, _objlist)
    {
        sectionRef = _sectionRef
        objlist = _objlist
        self = this
        paletteChoice = true
    }

    GridLayout {
        anchors.fill: parent
        rows: 3
        columns: 3

        Rectangle {
            Layout.row: 0
            Layout.column: 0
            Layout.fillWidth:  true
            Layout.fillHeight:  true
        }
        Rectangle {
            Layout.row: 2
            Layout.column: 2
            Layout.fillWidth:  true
            Layout.fillHeight:  true
        }

        ColumnLayout {
            Layout.row: 1
            Layout.column:1
            width: parent.parent.width - 100
            height: parent.parent.height - 50
            spacing: 2

            RowLayout{
                Layout.fillWidth: true
                Layout.fillHeight: true


                    EntityList {
                        id: globalSentences
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columnlayout.spacing: 2


                        columnlayout.children: [


                            Label{
                                text: "Global"
                                width: parent.width
                            },

                            EntityHeaderEnd {
                                visible: true
                            },

                            EntityListView {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                id: globalSentencesList
                                objectName: "parametersListView"
                                model: sentencePalettes.objat(objlist).globalSentences

                                delegate: EntityListDelegate {
                                    id: globalSentencesdelegate
                                    property var globalListIndex: index
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    rowlayout.children: [

                                        EntityListDelegateItem {
                                            Layout.fillWidth: true
                                            Layout.fillHeight: true
                                            color: paletteChoice ? (globalSentencesdelegate.mousearea.hovered ? globalSentences.rowBackgroundHover : globalSentences.rowBackground) : "white"
                                            label.color: paletteChoice ? (globalSentencesdelegate.mousearea.hovered ? globalSentences.rowForegroundHover : globalSentences.rowForeground) : "black"
                                            label.text: modelData

                                            MouseArea{
                                                anchors.fill: parent
                                                onClicked: {
                                                    if (paletteChoice){
                                                        sectionRef.text = modelData
                                                    }
                                                }
                                            }
                                        },
//                                        Button{
//                                            spacing: 2
//                                            visible: btnVisible
//                                            enabled: btnVisible
//                                            text: "Add"
//                                            Layout.preferredHeight: parent.height
//                                            Layout.preferredWidth: 50
//                                            onClicked: {
//                                                sectionRef.text = modelData
//                                            }
//                                        },
                                        Button{
                                            id: globalDeleteBtn
                                            spacing: 2
                                            visible: paletteChoice
                                            enabled: paletteChoice
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 32
                                            background: Rectangle{
                                                color: "white"
                                            }
                                            onClicked: {
                                                sentencePalettes.objat(objlist).removeSentenceFromGlobal(globalListIndex)
                                                globalSentencesList.model = sentencePalettes.objat(objlist).globalSentences
                                            }
                                            Image {
                                                anchors.verticalCenter: globalDeleteBtn.verticalCenter
                                                anchors.right: globalDeleteBtn.right
                                                anchors.left: globalDeleteBtn.left
                                                width:  32
                                                height: 32
                                                source: "qrc:/icons/buttons/remove.png";
                                                antialiasing: true
                                                mipmap:       true
                                                fillMode: Image.PreserveAspectFit
                                            }
                                        }

                                    ]
                                }
                            },
                            Button{
                                anchors.left: parent.horizontalCenter
                                visible: !paletteChoice
                                enabled: !paletteChoice
                                id :addgloballistbtn
                                Layout.preferredWidth: 45
                                background: Rectangle{
                                    color: "white"
                                }
                                onClicked: {
                                    if (sectionRef.text !== ""){
                                        sentencePalettes.objat(objlist).addSentenceToGlobal(sectionRef.text)
                                        globalSentencesList.model = sentencePalettes.objat(objlist).globalSentences
                                    }
                                }
                                Image {
                                    anchors.verticalCenter: addgloballistbtn.verticalCenter
                                    anchors.right: addgloballistbtn.right
                                    anchors.left: addgloballistbtn.left
                                    width:  45
                                    height: 45
                                    source: "qrc:/icons/buttons/add.png";
                                    antialiasing: true
                                    mipmap:       true
                                    fillMode: Image.PreserveAspectFit
                                }
                            }
                        ]
                    }

                    EntityList {
                        id: specificSentence
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columnlayout.spacing: 2

                        columnlayout.children: [

                            Label{
                                text: interpretationController.currentActiveSubstance.name.value() + " specific"
                                width: parent.width
                            },

                            EntityHeaderEnd {
                                visible: true
                            },

                            EntityListView {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                id: specificSentenceList
                                objectName: "parametersListView"
                                model: sentencePalettes.objat(objlist).drugSpecificSentences

                                delegate: EntityListDelegate {
                                    id: specificSentenceListDelegate
                                    property var specificListIndex: index
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    rowlayout.children: [

                                        EntityListDelegateItem{
                                            Layout.fillWidth: true
                                            Layout.fillHeight: true
                                            label.text: modelData
                                            color: paletteChoice ? (specificSentenceListDelegate.mousearea.hovered ? specificSentence.rowBackgroundHover : specificSentence.rowBackground) : "white"
                                            label.color: paletteChoice ? (specificSentenceListDelegate.mousearea.hovered ? specificSentence.rowForegroundHover : specificSentence.rowForeground) : "black"

                                            MouseArea{
                                                anchors.fill: parent
                                                onClicked: {
                                                    if (paletteChoice){
                                                        sectionRef.text = modelData
                                                    }
                                                }
                                            }
                                        },
//                                                Button{
//                                                    spacing: 2
//                                                    text: "Add"
//                                                    visible: btnVisible
//                                                    enabled: btnVisible
//                                                    Layout.preferredHeight: parent.height
//                                                    Layout.preferredWidth: 50
//                                                    onClicked: {
//                                                        sectionRef.text = modelData
//                                                    }
//                                                },
                                        Button{
                                            id: specificDeleteBtn
                                            spacing: 2
                                            visible: paletteChoice
                                            enabled: paletteChoice
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 32
                                            background: Rectangle{
                                                color: "white"
                                            }
                                            onClicked: {
                                                sentencePalettes.objat(objlist).removeSentenceFromSpecific(specificListIndex)
                                                specificSentenceList.model = sentencePalettes.objat(objlist).drugSpecificSentences
                                            }
                                            Image {
                                                anchors.verticalCenter: specificDeleteBtn.verticalCenter
                                                anchors.right: specificDeleteBtn.right
                                                anchors.left: specificDeleteBtn.left
                                                width:  32
                                                height: 32
                                                source: "qrc:/icons/buttons/remove.png";
                                                antialiasing: true
                                                mipmap:       true
                                                fillMode: Image.PreserveAspectFit
                                            }
                                        }
                                    ]
                                }
                            },
                            Button{
                                id: addspecificlistbtn
                                anchors.left: parent.horizontalCenter
                                visible: !paletteChoice
                                enabled: !paletteChoice
                                Layout.preferredWidth: 45
                                background: Rectangle{
                                    color: "white"
                                }
                                onClicked: {
                                    if (sectionRef.text !== ""){
                                        sentencePalettes.objat(objlist).addSentenceToSpecific(sectionRef.text)
                                        specificSentenceList.model = sentencePalettes.objat(objlist).drugSpecificSentences
                                    }
                                }
                                Image {
                                    anchors.verticalCenter: addspecificlistbtn.verticalCenter
                                    anchors.right: addspecificlistbtn.right
                                    anchors.left: addspecificlistbtn.left
                                    width:  45
                                    height: 45
                                    source: "qrc:/icons/buttons/add.png";
                                    antialiasing: true
                                    mipmap:       true
                                    fillMode: Image.PreserveAspectFit
                                }
                            }
                        ]
                    }

               }



            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                id: cancelBtn
                text: "Close"
                Layout.preferredWidth: 125
                onClicked: function() {
                    self.exit(false);
                }
            }
        }
        }

}
