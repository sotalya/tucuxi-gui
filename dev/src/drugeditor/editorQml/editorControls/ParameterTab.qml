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


    property var parameterSelected: index != -1 ? undoRedoController.parameters.objat(tabRoot.index) : undefined


    leftPanel: Rectangle {
        id: parameterSelectContainer
        anchors.fill: parent
        anchors.margins: 5
        color: Style.table.background.table

        property var modelSelected: undoRedoController.parameters.objlist


        ColumnLayout {
            id : rootDisplayLayout
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
                        text: "Parameters"
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
                            tabRoot.entityChanged("parameter",tabRoot.index)
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
                                tabRoot.entityChanged("parameter",-1)
                            }
                        }
                    }
                }
            }

            Component {
                id:parameterSelectDelegate
                Rectangle {
                    id:parameterSelectDelegateContainer
                    width: ListView.view.width
                    height: rowHeight
                    color: parameterSelectMouseArea.hovered ? rowBackgroundHover : (ListView.view.currentIndex === index ? rowBackgroundSelected : rowBackground)

                    property bool editionTime: false


                    MouseArea {
                        id:parameterSelectMouseArea
                        anchors.fill: parent

                        hoverEnabled: true

                        property bool hovered: false
                        onEntered: hovered = true
                        onExited: hovered = false

                        onClicked: {
                            if(parameterSelectDelegateContainer.ListView.view.currentIndex == index)
                                editionTime = true
                            else
                                currentIndexChanged("parameterSelectView",index)
                        }

                    }

                    Text {
                        id:parameterSelectText
                        text: modelData.name
                        visible: !editionTime
                    }

                    TextField {
                        id: parameterSelectInput
                        text: parameterSelectText.text
                        visible: editionTime
                        onVisibleChanged: if(editionTime) forceActiveFocus();
                        onEditingFinished: {
                            tabRoot.textChanged("name"+index,text)
                            editionTime = false
                        }
                    }
                }
            }
            //Ids
            ListView {
                id: parameterSelectListView
                objectName: "parameterSelectView"
                Layout.fillHeight: true
                Layout.fillWidth: true

                model: modelSelected
                delegate: parameterSelectDelegate

                onCurrentIndexChanged: {
                    tabRoot.index = currentIndex
                }

                Component.onCompleted: tabRoot.reset.connect(clearSelection)
                function clearSelection() {
                    currentIndex = -1;
                }
            }

            //Data
            Rectangle{
                Layout.fillHeight: true
                Layout.fillWidth: true

                ColumnLayout {
                    //value + unit
                    RowLayout {

                        Text {
                            id:valueText
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            text: qsTr("Value:")
                        }
                        TextField {
                            id:valueInput
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            placeholderText: "Value"
                            text:parameterSelected.quantity.dbvalue
                        }
                        Rectangle{}
                        Text {
                            id:unitText
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            text: qsTr("Unit:")
                        }
                        TextField {
                            id:unitInput
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            placeholderText: "Unit"
                            text:parameterSelected.quantity.unitstring
                        }
                    }
                    //min + max + step
                    RowLayout {
                        Text {
                            id:minText
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            text: qsTr("Min:")
                        }
                        TextField {
                            id:minInput
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            placeholderText: "Min"
                            text:parameterSelected.min
                        }
                        Text {
                            id:maxText
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            text: qsTr("Max:")
                        }
                        TextField {
                            id:maxInput
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            placeholderText: "Max"
                            text:parameterSelected.max
                        }
                        Text {
                            id:stepText
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            text: qsTr("Step:")
                        }
                        TextField {
                            id:stepInput
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            placeholderText: "Step"
                            text:parameterSelected.step
                        }
                    }

                    //Bsv header
                    RowLayout {

                        Rectangle {
                            height: headerHeight
                            Layout.fillWidth: true
                            color: headerBackground
                            Text {
                                anchors.fill: parent
                                text: "BSV"
                                color: headerForeground
                                font.family: headerFontFamily
                                font.pixelSize: headerFontSize
                                font.bold: headerFontBold
                                verticalAlignment: headerVAlign
                                horizontalAlignment: headerHAlign
                            }
                        }
                    }
                    //BSV
                    RowLayout {
                        Text {
                            id:additiveText
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            text: qsTr("Additive:")
                        }
                        TextField {
                            id:additiveInput
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            placeholderText: "Additive"
                            text:parameterSelected.bsv.standard
                        }
                        Text {
                            id:proportionalText
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            text: qsTr("Proportional:")
                        }
                        TextField {
                            id:proportionalInput
                            font.family: labelFontFamily
                            font.pixelSize: labelFontSize
                            font.bold: labelFontBold
                            placeholderText: "Proportional"
                            text:parameterSelected.bsv.proportional
                        }
                    }
                }

            }


        }

    }


    rightPanel: CommentsView {
        objectName: "parameterCommentsView"
        commentModel: parameterSelected.comments

    }




}
