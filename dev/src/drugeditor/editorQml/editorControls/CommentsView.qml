import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0







Item {

    id: commentListRoot

    //probably useful stuff

    property int index: -1
    property var commentModel //this is the model to use
    property int rootIndex: tabRoot.index //change this to your taste

    property string headerText: "Comment" //text to display in the header bar

    property string preName: "comment"
    property string postName: "" //do not use, comment might contain '-', not safe. Better to use post name with a '-' in it
    //Misc Config
    property int rowHeight: 20
    property int columnSpacing: 2

    //Header style
    property string headerBackground: Style.table.background.header
    property string headerForeground: Style.table.foreground.header
    property string headerFontFamily: Style.table.font.header
    property string headerFontSize:   Style.table.size.header
    property bool headerFontBold: true
    property int headerHeight: 25
    property int headerVAlign: Text.AlignVCenter
    property int headerHAlign: Text.AlignHCenter

    //Lang style
    property int langWidth: 75

    //comment style
    //    property int commentWidth:container.width - langWidth - columnSpacing
    property int commentWidth:500

    //Cell Style
    property string cellFontFamily: Style.table.font.row
    property string cellFontSize: Style.table.size.row
    property string cellBackground:         Style.table.background.row
    property string cellBackgroundHover:    Style.table.background.row_hover
    property string cellBackgroundSelected: Style.table.background.row_selected
    property string cellForeground:         Style.table.foreground.row
    property string cellForegroundHover:    Style.table.foreground.row_hover
    property string cellForegroundSelected: Style.table.foreground.row_selected
    property int cellVAlign: Text.AlignVCenter
    property int cellHAlign: Text.AlignLeft


    Component {
        id: headerText
        Rectangle {
            Layout.preferredHeight: headerHeight
            color: headerBackground
            property string boxText
            Text {
                anchors.fill: parent
                text: boxText
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
            property int onClickIndex
            property string imageSrc: ""
            property string imageDisabledSrc: ""
            Image {
                id: headerButtonImage
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                antialiasing: true
                mipmap: true
                source: imageDisabledSrc === "" ? imageSrc : (commentListRoot.index !== -1 ? imageSrc : imageDisabledSrc)
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(imageDisabledSrc !== "" && commentListRoot.index === -1) {
                          return;
                        }

                        tabRoot.entityChanged(preName +"-"+postName,onClickIndex)
                        //TODO take into account currentIndex&Co refresh
                    }
                }
            }
        }
    }


    //Good stuff
    Rectangle{
        id: mainContainer

        anchors.fill: parent
        anchors.margins: 5

        opacity: (rootIndex !== -1)

        color: Style.table.background.table

        property var headerAddButton
        property var headerRemoveButton

        ColumnLayout{
            id: rootLayout
            anchors.fill: parent
            spacing: 5

            //header
            RowLayout{
                id:header
                spacing: columnSpacing



                Component.onCompleted: {

                    headerText.createObject(this,{boxText:"Lang","Layout.preferredWidth":langWidth})
                    headerText.createObject(this,{"boxText":commentListRoot.headerText,"Layout.preferredWidth":commentWidth,"Layout.fillWidth":true})
                    mainContainer.headerRemoveButton = headerButton.createObject(this,{"onClickIndex":index,"imageSrc":"qrc:/icons/buttons/remove.png","imageDisabledSrc":"qrc:/icons/buttons/remove_disabled.png"})
                    mainContainer.headerAddButton = headerButton.createObject(this,{"onClickIndex":-1,"imageSrc":"qrc:/icons/buttons/add.png"})
                }
            }

            //commentsList view
            ListView{
                id: commentListView
                objectName: "commentListView"
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                model: commentModel.keys

                delegate: commentListDelegate

                onCurrentIndexChanged: {
                    index = currentIndex
                    mainContainer.headerRemoveButton.onClickIndex = currentIndex
                }

                Component.onCompleted: tabRoot.reset.connect(clearSelection)

                function clearSelection() {
                    currentIndex = -1;
                }

            }

            Component {
                id: commentListDelegate

                Item {
                    id: delegateContainer

                    property real editorOpacity : 0

                    property var langView:listViewCell.createObject(delegateRowLayout,{"columnName":"lang", "dataName":"value"})
                    property var commentView: listViewCell.createObject(delegateRowLayout,{"columnName":"comment","Layout.fillWidth":true})

                    property var langEditor: listViewCellEditor.createObject(delegateRowLayout,{"columnName":"lang"});
                    property var commentEditor:listViewCellEditor.createObject(delegateRowLayout,{"columnName":"comment","Layout.fillWidth":true});

                    width:  ListView.view.width

                    height: Math.max(rowHeight, commentView.ptrpaintedheight)

                    MouseArea {
                        id: commentListDelegateMouseArea
                        anchors.fill: parent
                        hoverEnabled: true

                        property bool hovered: false
                        onEntered: hovered = true
                        onExited: {hovered = false; }
                    }


                    Component {
                        id: listViewCell
                        Rectangle {

                            property string columnName
                            property alias ptrpaintedheight: listViewCellText.paintedHeight
                            visible:  !delegateContainer.editorOpacity
                            Layout.preferredWidth: commentListRoot[columnName+"Width"]
                            Layout.fillHeight: true
                            color: commentListDelegateMouseArea.hovered ? cellBackgroundHover : (delegateContainer.ListView.isCurrentItem ? cellBackgroundSelected : cellBackground)

                            Text {
                                id: listViewCellText
                                anchors.fill: parent

                                color: commentListDelegateMouseArea.hovered ? cellForegroundHover: (delegateContainer.ListView.isCurrentItem ? cellForegroundSelected: cellForeground)
                                font.family: cellFontFamily
                                font.pixelSize: cellFontSize
                                wrapMode: Text.WordWrap
                                verticalAlignment:   cellVAlign
                                horizontalAlignment: cellHAlign
                                text: columnName == "lang" ? modelData : commentModel.value(modelData)
                            }
                            MouseArea {

                                anchors.fill: parent

                                onClicked: {
                                    if(delegateContainer.ListView.view.currentIndex === index) {
                                        delegateContainer.state = 'Details';
                                    } else {
                                        delegateContainer.ListView.view.currentIndex = index
                                    }

                                }
                            }
                        }
                    }

                    Component {
                        id: listViewCellEditor
                        Rectangle {

                            property string columnName

                            property string editingText: columnName == "lang" ? modelData : commentModel.value(modelData)
                            visible: delegateContainer.editorOpacity
                            Layout.fillHeight: true
                            Layout.preferredWidth: commentListRoot[columnName + "Width"]

                            TextField {
                                id: listViewCellEditorField
                                anchors.fill: parent
                                text: editingText
                                font.pixelSize: cellFontSize
                                onOpacityChanged: if(opacity)  forceActiveFocus() ;
                                onEditingFinished: {
                                    editingText = text
                                    delegateContainer.state = ""
                                    if (columnName == "lang") {
                                        tabRoot.textChanged(preName+"-lang-"+delegateContainer.commentEditor.editingText+"-"+postName,text)
                                    } else {
                                        tabRoot.textChanged(preName+"-comm-"+delegateContainer.langEditor.editingText+"-"+postName,text)
                                    }
                                }
                            }
                        }
                    }

                    RowLayout{
                        id:delegateRowLayout
                        anchors.fill: parent
                        spacing: columnSpacing
                    }

                    states: State {
                        name: "Details"
                        PropertyChanges {target: delegateContainer; editorOpacity: 1;}

                    }
                }
            }//END DELEGATE
        }
    }
}
