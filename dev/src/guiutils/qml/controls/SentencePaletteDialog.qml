import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    title: sectionName ? sectionName : ""
    width: 600
    height: 300
    id:root

    property var self
    property var sectionText
    property var sectionNb
    property var paletteChoice: true
    property var currentGlobalModel
    property var currentSpecificModel
    property var currentDrugId

    property var sectionData
    property var globalSectionData
    property var specificSectionData
    property var sectionName
    property var oldSectionText

    property var inputText


    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            if (inputText) {
                inputText.text = sectionText
            }
            self.exit(false);
        }
    }

    function init(_sectionText, _sectionNb, _inputText, _sectionName, _paletteBtn)
    {
        sectionName = _sectionName
        inputText = _inputText
        sectionText = _sectionText
        sectionNb = _sectionNb
        paletteChoice = _paletteBtn
        self = this
        currentDrugId =  interpretationController.currentActiveSubstance ? interpretationController.currentActiveSubstance.substanceId : ""
        update()
        oldSectionText = _sectionText
    }

    function update()
    {
        sectionData = sentencesPalettes.sectionsList.objat(sectionNb)
        globalSectionData = sentencesPalettes.sectionsList.objat(sectionNb).globalSentences
        specificSectionData = sentencesPalettes.sectionsList.objat(sectionNb).getSpecificSentencesList(currentDrugId)
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


                            RowLayout{
                                width: parent.width
                                Label{
                                    text: "Global"

                                }
                                Rectangle{
                                    Layout.fillWidth: true
                                }

                                CheckBox{
                                    id:globalCB
                                    checked: true
                                    text: "Overall"
                                    visible: false
                                    enabled: false
                                }

                            },

                            EntityHeaderEnd {
                                visible: true
                            },

                            EntityListView {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                id: globalSentencesList
                                objectName: "parametersListView"

                                model: globalSectionData

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
                                                        if (inputText) {
                                                            inputText.text = modelData
                                                        }
                                                    }
                                                }
                                            }
                                        },

                                        Button{
                                            id: globalDeleteBtn
                                            objectName: "globalDeleteButton_" + index
                                            spacing: 2
                                            visible: paletteChoice
                                            enabled: paletteChoice
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 32
                                            background: Rectangle{
                                                color: "white"
                                            }
                                            onClicked: {
                                                sectionData.removeSentenceFromGlobal(globalListIndex)
                                                root.update()
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
                                Layout.alignment: Qt.AlignHCenter
                                visible: !paletteChoice
                                enabled: !paletteChoice
                                id :addgloballistbtn
                                objectName: "addGlobalListButton"
                                Layout.preferredWidth: 45
                                background: Rectangle{
                                    color: "white"
                                }
                                onClicked: {
                                    if (sectionText !== ""){
                                        sectionData.addSentenceToGlobal(sectionText)
                                        root.update()
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
                                text: (interpretationController.currentActiveSubstance ? interpretationController.currentActiveSubstance.name.value() : "") + " specific"
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

                                model: specificSectionData

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
                                                        if (inputText) {
                                                            inputText.text = modelData
                                                        }
                                                    }
                                                }
                                            }
                                        },

                                        Button{
                                            id: specificDeleteBtn
                                            objectName: "specificDeleteButton_" + index
                                            spacing: 2
                                            visible: paletteChoice
                                            enabled: paletteChoice
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 32
                                            background: Rectangle{
                                                color: "white"
                                            }
                                            onClicked: {
                                                var sec = sentencesPalettes.getSection(sectionNb)
                                                sec.removeSentenceFromDrugSentencesList(currentDrugId, specificListIndex)
                                                root.update()
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
                                objectName: "addSpecificListButton"
                                Layout.alignment: Qt.AlignHCenter
                                visible: !paletteChoice
                                enabled: !paletteChoice
                                Layout.preferredWidth: 45
                                background: Rectangle{
                                    color: "white"
                                }
                                onClicked: {
                                    if (sectionText !== ""){
                                        var sec = sentencesPalettes.getSection(sectionNb)
                                        sec.addSentenceToDrugSentencesList(currentDrugId, sectionText)
                                        root.update()
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

            RowLayout{
                Layout.alignment: Qt.AlignHCenter
                Button {
                    id: closeBtn
                    objectName: "validationCloseButton"
                    text: "Close"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        sentencesPalettes.exportToXml();
                        self.exit(false);
                    }
                }

                Button {
                    id: cancelBtn
                    text: "Cancel"
                    visible: paletteChoice
                    enabled: paletteChoice
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        if (inputText) {
                            inputText.text = oldSectionText
                        }
                        self.exit(false);
                    }
                }
            }
        }
        }
}
