import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

import ezechiel 1.0

VirtualTab {
    id: tabRoot

    ratio: 3

    property var covariateSelected: (index != -1) ? undoRedoController.covariates.objat(index) : undefined

    leftPanel: Rectangle {
        id: covariateSelectContainer
        anchors.fill: parent
        anchors.margins: 5
        color: Style.table.background.table

        property var modelSelected: undoRedoController.covariates.objlist

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
                        text: "Covariates"
                        color: headerForeground
                        font.family: headerFontFamily
                        font.pixelSize: headerFontSize
                        font.bold: headerFontBold
                        verticalAlignment: headerVAlign
                        horizontalAlignment: headerHAlign
                    }
                }

                Rectangle {
                    Layout.preferredHeight: headerHeight
                    Layout.preferredWidth: headerHeight
                    border.width : cellFrameWidth
                    radius: cellFrameRadius
                    color: headerBackground

                    Image {
                        anchors.fill: parent
                        source:  tabRoot.index !== -1 ? "qrc:/icons/buttons/remove.png" : "qrc:/icons/buttons/remove_disabled.png"
                        fillMode: Image.PreserveAspectFit
                        antialiasing: true
                        mipmap: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            tabRoot.entityChanged("covariate",tabRoot.index)
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
                                tabRoot.entityChanged("covariate",-1)
                            }
                        }
                    }
                }
            }

            Component {
                id:covariateDisplayDelegate
                Rectangle {
                    id: covariateDisplayDelegateContainer
                    width: ListView.view.width
                    height: rowHeight
                    color: covariateDisplayDelegateMouseArea.hovered ? rowBackgroundHover : (ListView.view.currentIndex === index ? rowBackgroundSelected : rowBackground)

                    property bool editionTime: false

                    MouseArea {
                        id:covariateDisplayDelegateMouseArea
                        anchors.fill: parent

                        hoverEnabled: true

                        property bool hovered: false
                        onEntered: hovered = true
                        onExited: hovered = false


                        onClicked: {
                            if(covariateDisplayDelegateContainer.ListView.view.currentIndex == index)
                                editionTime = true
                            else
                                currentIndexChanged("covariateDisplayView",index)
                        }

                    }

                    Text {
                        id:covariateDisplayText
                        text: modelData.name
                        visible: !editionTime
                    }

                    TextField {
                        id:covariateDisplayInput
                        text: covariateDisplayText.text
                        visible: editionTime
                        onVisibleChanged: if(editionTime) forceActiveFocus();
                        onEditingFinished: {
                            tabRoot.textChanged("name-"+index,text)
                            editionTime = false
                        }
                    }

                }
            }

            ListView {
                id: displayListView
                objectName: "covariateDisplayView"
                Layout.fillHeight: true
                Layout.fillWidth: true

                model: modelSelected
                delegate: covariateDisplayDelegate

                onCurrentIndexChanged: {
                    tabRoot.index = currentIndex
                }

                Component.onCompleted: tabRoot.reset.connect(clearSelection)
                function clearSelection() {
                    currentIndex = -1;
                }
            }
        }
    }

    middlePanel: Rectangle {
        anchors.fill: parent
        color: Style.table.background.table
        visible: tabRoot.index != -1


        CommentsView {
            anchors.top:parent.top
            anchors.bottom:parent.verticalCenter
            anchors.right:parent.right
            anchors.left:parent.left
            commentModel:covariateSelected.visualNameTranslation
            headerText: "Names"
            preName: "trans-name"
        }

        CommentsView {
            anchors.top:parent.verticalCenter
            anchors.bottom:parent.bottom
            anchors.right:parent.right
            anchors.left:parent.left
            commentModel:covariateSelected.descriptionTranslation
            headerText: "Description"
            preName: "trans-desc"
        }
    }

    rightPanel: Rectangle {
        anchors.fill: parent
        color: Style.table.background.table
        visible: tabRoot.index != -1

        //Values
        ColumnLayout {
            id: covariateValues
            anchors.top:parent.top
            anchors.bottom: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right

            Component.onCompleted: {
                displayEditableRow.createObject(this,{"role":"quantity","name":"Default Value"})
                displayEditableRow.createObject(this,{"role":"unitString","name":"Unit"})
            }

            Component {
                id: displayEditableRow
                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    property string name
                    property string role
                    property var model: covariateSelected

                    property string txt: (model !== undefined ? (role == "quantity" ? model[role].dbvalue : model[role]) : "")

                    RowLayout {
                        id: displayEditableRowLayout
                        width: parent.width
                        height: parent.height

                        Text {
                            Layout.preferredWidth: 100
                            id:displayText
                            text: qsTr(name)
                        }

                        TextField {
                            id: displayField
                            Layout.fillWidth: true
                            placeholderText: qsTr(name)

                            text: txt
                            onTextChanged: {
                                if(index != -1)
                                tabRoot.textChanged(role+"-"+index,text)
                            }
                            font.family: inputFontFamily
                            font.pixelSize: inputFontSize
                            verticalAlignment: inputVAlign
                            horizontalAlignment: inputHAlign
                        }
                    }
                }


            }

        }

        CommentsView {
            anchors.top:parent.verticalCenter
            anchors.bottom:parent.bottom
            anchors.right:parent.right
            anchors.left:parent.left
            commentModel:covariateSelected.comments
            preName: "trans-comment"
        }

    }
}
