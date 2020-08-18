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
    height: 400

    property var self

    property var paletteRef

    property var objlistRef

    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            self.exit(false);
        }
    }

    function init(paletteReference, listRef)
    {
        paletteRef = paletteReference
        self = this
        objlistRef = listRef
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

            RowLayout{
                Layout.fillWidth: true
                Layout.fillHeight: true

                    EntityList {
                        id: globalSentences
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columnlayout.spacing: 2

                        columnlayout.children: [
                            EntityListView {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                id: globalSentencesList
                                objectName: "parametersListView"
                                model: sentencePalette.globalSentences

                                delegate: EntityListDelegate {
                                    id: globalSentencesdelegate
                                    property var globalListIndex: index
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    rowlayout.children: [

                                        EntityListDelegateItem {
                                            Layout.fillWidth: true
                                            Layout.fillHeight: true
                                            label.text: modelData
                                        },
                                        Button{
                                            spacing: 2
                                            text: "Add"
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 50
                                            onClicked: {
                                                paletteRef.text = modelData
                                            }
                                        },
                                        Button{
                                            spacing: 2
                                            text: "Del"
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 50
                                            onClicked: {
                                                sentencePalette.removeSentenceFromGlobal(globalListIndex)
                                                globalSentencesList.model = sentencePalette.globalSentences
                                            }
                                        }

                                    ]
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

                            EntityListView {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                id: specificSentenceList
                                objectName: "parametersListView"
                                model: sentencePalette.drugSpecificSentences

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
                                        },
                                        Button{
                                            spacing: 2
                                            text: "Add"
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 50
                                            onClicked: {
                                                paletteRef.text = modelData
                                            }
                                        },
                                        Button{
                                            spacing: 2
                                            text: "Del"
                                            Layout.preferredHeight: parent.height
                                            Layout.preferredWidth: 50
                                            onClicked: {
                                                sentencePalette.removeSentenceFromSpecific(specificListIndex)
                                                specificSentenceList.model = sentencePalette.drugSpecificSentences
                                            }
                                        }
                                    ]
                                }
                            }
                        ]
                    }
                }


            Button {
                anchors.centerIn: parent
                id: cancelBtn
                text: "Cancel"
                Layout.preferredWidth: 125
                onClicked: function() {
                    self.exit(false);
                }
            }
        }
        }

}
