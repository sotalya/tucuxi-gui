import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

DialogBase {
    title: sectionName ? sectionName : ""
    width: 600
    height: 300
    id:root

    property var self
    property var sectionText
    property var sectionNb
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

    // For testing
    function getGlobalSentencesCount(){
        return globalSentencesList.count
    }
    // For testing
    function getSpecificSentencesCount(){
        return specificSentenceList.count
    }

    function init(_sectionText, _sectionNb, _inputText, _sectionName)
    {
        sectionName = _sectionName
        inputText = _inputText
        sectionText = _sectionText
        sectionNb = _sectionNb
        self = this
        currentDrugId =  interpretationController.currentActiveSubstance ? interpretationController.currentActiveSubstance.substanceId : ""
        update()
        oldSectionText = _sectionText
    }

    function update()
    {
        sectionData = sentencesPalettes.sectionsList.objat(sectionNb)
        globalSectionData = sentencesPalettes.sectionsList.objat(sectionNb).getGlobalSentencesTextsList()
        specificSectionData = sentencesPalettes.sectionsList.objat(sectionNb).getSpecificSentencesList(currentDrugId)
    }

    function openshortcutDialog(_key, _modifier, _text, _sectionNb, _drugId, _isEdit, _index){
        if(_sectionNb === 0){
            expectednessSentenceShortcutDialog.init(_key, _modifier, _text, _sectionNb, _drugId, _isEdit, _index)
            expectednessSentenceShortcutDialog.open(true)
        }
        else if(_sectionNb === 1){
            suitabilitySentenceShortcutDialog.init(_key, _modifier, _text, _sectionNb, _drugId, _isEdit, _index)
            suitabilitySentenceShortcutDialog.open(true)
        }
        else if(_sectionNb === 2){
            predictionSentenceShortcutDialog.init(_key, _modifier, _text, _sectionNb, _drugId, _isEdit, _index)
            predictionSentenceShortcutDialog.open(true)
        }
        else if(_sectionNb === 3){
            remonitoringSentenceShortcutDialog.init(_key, _modifier, _text, _sectionNb, _drugId, _isEdit, _index)
            remonitoringSentenceShortcutDialog.open(true)
        }
        else if(_sectionNb === 4){
            warningSentenceShortcutDialog.init(_key, _modifier, _text, _sectionNb, _drugId, _isEdit, _index)
            warningSentenceShortcutDialog.open(true)
        }
        root.update()
    }

    GridLayout {
        anchors.fill: parent
        rows: 3
        columns: 3

        Rectangle {
            Layout.row: 0
            Layout.column: 0
            width: 50
            height: 25
        }
        Rectangle {
            Layout.row: 2
            Layout.column: 2
            width: 50
            height: 25
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
                                id: globalSentencesList
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                objectName: "parametersListView"

                                model: globalSectionData

                                delegate: EntityListDelegate {
                                    id: globalSentencesdelegate
                                    property var globalListIndex: index
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    rowlayout.children: [

                                        EntityListDelegateItem {
                                            id: globalText
                                            objectName: "globalText_" + index
                                            Layout.fillWidth: true
                                            Layout.fillHeight: true
                                            color: globalSentencesdelegate.mousearea.hovered ? globalSentences.rowBackgroundHover : globalSentences.rowBackground
                                            label.color: globalSentencesdelegate.mousearea.hovered ? globalSentences.rowForegroundHover : globalSentences.rowForeground
                                            label.text: modelData

                                            MouseArea{
                                                anchors.fill: parent
                                                onClicked: {
                                                        if (inputText) {
                                                            inputText.text = modelData
                                                        }
                                                }
                                            }
                                        },

                                        Button{
                                            id: globalEditBtn
                                            objectName: "globalEditButton_" + index
                                            spacing: 2
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 32
                                            background: Rectangle{
                                                color: "white"
                                            }
                                            onClicked: {
                                                var key = sectionData.globalSentences[index].key
                                                var modifier = sectionData.globalSentences[index].modifier

                                                root.openshortcutDialog(key, modifier, globalText.label.text, sectionNb, "", true, globalSentencesList.currentIndex)
                                            }
                                            Image {
                                                anchors.verticalCenter: globalEditBtn.verticalCenter
                                                anchors.right: globalEditBtn.right
                                                anchors.left: globalEditBtn.left
                                                width:  32
                                                height: 32
                                                source: "qrc:/icons/buttons/edit.png";
                                                antialiasing: true
                                                mipmap:       true
                                                fillMode: Image.PreserveAspectFit
                                            }
                                        },

                                        Button{
                                            id: globalDeleteBtn
                                            objectName: "globalDeleteButton_" + index
                                            spacing: 2
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
                                id :addgloballistbtn
                                objectName: "addGlobalListButton"
                                Layout.preferredWidth: 45
                                background: Rectangle{
                                    color: "white"
                                }
                                onClicked: {
                                    root.openshortcutDialog(0, 0, sectionText, sectionNb, "", false, globalSentencesList.currentIndex)
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
                                id: specificSentenceList
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                objectName: "parametersListView"

                                model: specificSectionData

                                delegate: EntityListDelegate {
                                    id: specificSentenceListDelegate
                                    property var specificListIndex: index
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    rowlayout.children: [

                                        EntityListDelegateItem{
                                            id: specificText
                                            objectName: "specificText_" + index
                                            Layout.fillWidth: true
                                            Layout.fillHeight: true
                                            label.text: modelData
                                            color: specificSentenceListDelegate.mousearea.hovered ? specificSentence.rowBackgroundHover : specificSentence.rowBackground
                                            label.color: specificSentenceListDelegate.mousearea.hovered ? specificSentence.rowForegroundHover : specificSentence.rowForeground

                                            MouseArea{
                                                anchors.fill: parent
                                                onClicked: {

                                                    if (inputText) {
                                                        inputText.text = modelData
                                                    }
                                                }
                                            }
                                        },

                                        Button{
                                            id: specificEditBtn
                                            objectName: "specificEditButton_" + index
                                            spacing: 2
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 32
                                            background: Rectangle{
                                                color: "white"
                                            }
                                            onClicked: {
                                                var key = sectionData.getSentenceFromDrugId(currentDrugId)[index].key
                                                var modifier = sectionData.getSentenceFromDrugId(currentDrugId)[index].modifier

                                                root.openshortcutDialog(key, modifier, specificText.label.text, sectionNb, currentDrugId, true, specificSentenceList.currentIndex)
                                            }
                                            Image {
                                                anchors.verticalCenter: specificEditBtn.verticalCenter
                                                anchors.right: specificEditBtn.right
                                                anchors.left: specificEditBtn.left
                                                width:  32
                                                height: 32
                                                source: "qrc:/icons/buttons/edit.png";
                                                antialiasing: true
                                                mipmap:       true
                                                fillMode: Image.PreserveAspectFit
                                            }
                                        },

                                        Button{
                                            id: specificDeleteBtn
                                            objectName: "specificDeleteButton_" + index
                                            spacing: 2
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
                                Layout.preferredWidth: 45
                                background: Rectangle{
                                    color: "white"
                                }
                                onClicked: {
                                    root.openshortcutDialog(0, 0, sectionText, sectionNb, currentDrugId, false, specificSentenceList.currentIndex)
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

            Item {
                // spacer
                Layout.preferredHeight: 10
                Layout.fillWidth: true
            }

            RowLayout{
                Layout.alignment: Qt.AlignHCenter
                spacing: 50
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

    SentenceShortcutDialog{
        id: expectednessSentenceShortcutDialog
        objectName: "expectednessSentenceShortcutDialog"

        onExited: {
            root.update()
        }
    }

    SentenceShortcutDialog{
        id: suitabilitySentenceShortcutDialog
        objectName: "suitabilitySentenceShortcutDialog"

        onExited: {
            root.update()
        }
    }

    SentenceShortcutDialog{
        id: predictionSentenceShortcutDialog
        objectName: "predictionSentenceShortcutDialog"

        onExited: {
            root.update()
        }
    }

    SentenceShortcutDialog{
        id: remonitoringSentenceShortcutDialog
        objectName: "remonitoringSentenceShortcutDialog"

        onExited: {
            root.update()
        }
    }

    SentenceShortcutDialog{
        id: warningSentenceShortcutDialog
        objectName: "warningSentenceShortcutDialog"

        onExited: {
            root.update()
        }
    }
}
