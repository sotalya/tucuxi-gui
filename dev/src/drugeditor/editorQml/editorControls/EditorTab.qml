 import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

import ezechiel 1.0

FlowTab {
    id: tabRoot

    property int index:  -1
    property int status: Status.invalid

    //ToDo: the way this updated is a mess, find a better solution, either in QML, or through the interpretation controler
    property string editorName:      ""
    property string editorInstitution: ""
    property string editionDate: ""
    property string editorEmail: ""



    signal currentIndexChanged(string view,int index)
    signal mainIndexChanged(int index)
    signal currentStatusChanged(int status)
    signal reset()

    signal textChanged(string propertyName, var newValue)
    //used when an Editor is added or removed; `index == -1` when added
    signal entityChanged(string listViewName, int index)

    onIndexChanged:  mainIndexChanged(index)
    onStatusChanged: currentStatusChanged(status)

    //    onCurrentIndexChanged: editorListView.currentIndex = index

    leftPanel: Component {
        id: editorPanel

        Rectangle {
            anchors.fill: parent
            color: Style.table.background.table

            property int status: {
                if (/*editorListView.status === Status.invalid ||*/ editorView.status === Status.invalid)
                    return Status.invalid
                if (/*editorListView.status === Status.valid   ||*/ editorView.status === Status.valid)
                    return Status.valid

                return Status.complete
            }

            onStatusChanged: tabRoot.status = status

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5

                //EditorList
                Rectangle {
                    id: editorList
                    Layout.fillWidth:  true
                    Layout.fillHeight: true

                    property string headerBackground: Style.table.background.header
                    property string headerForeground: Style.table.foreground.header
                    property string headerFontFamily: Style.table.font.header
                    property string headerFontSize:   Style.table.size.header
                    property bool headerFontBold:      true
                    property bool headerFontUnderline: false
                    property int headerHeight: 25
                    property int headerVAlign: Text.AlignVCenter
                    property int headerHAlign: Text.AlignHCenter

                    property int editorNameColumnWidth: 100
                    property int editorInstitutionColumnWidth: 100
                    property int editorDateColumnWidth: 75

                    property string rowBackground:         Style.table.background.row
                    property string rowBackgroundHover:    Style.table.background.row_hover
                    property string rowBackgroundSelected: Style.table.background.row_selected
                    property string rowForeground:         Style.table.foreground.row
                    property string rowForegroundHover:    Style.table.foreground.row_hover
                    property string rowForegroundSelected: Style.table.foreground.row_selected
                    property string rowFontFamily:  Style.table.font.row
                    property string rowFontSize:    Style.table.size.row
                    property int rowVAlign: Text.AlignVCenter
                    property int rowHAlign: Text.AlignLeft
                    property int rowHeight: 25

                    property int cellFrameRadius: 1
                    property int cellFrameWidth:  0
                    property int cellMargin:      5

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 0

                        //PatientList header
                        RowLayout {
                            spacing: 2


                            Rectangle {
                                Layout.preferredWidth: editorList.editorDateColumnWidth
                                Layout.preferredHeight: editorList.headerHeight
                                border.width: editorList.cellFrameWidth
                                radius: editorList.cellFrameRadius
                                color: editorList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Date"
                                    elide: Text.ElideRight
                                    color: editorList.headerForeground
                                    font.family: editorList.headerFontFamily
                                    font.pixelSize: editorList.headerFontSize
                                    font.bold: editorList.headerFontBold
                                    font.underline: editorList.headerFontUnderline
                                    verticalAlignment:   editorList.headerVAlign
                                    horizontalAlignment: editorList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  editorList.editorInstitutionColumnWidth
                                Layout.preferredHeight: editorList.headerHeight
                                border.width: editorList.cellFrameWidth
                                radius: editorList.cellFrameRadius
                                color: editorList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Institution"
                                    elide: Text.ElideRight
                                    color: editorList.headerForeground
                                    font.family: editorList.headerFontFamily
                                    font.pixelSize: editorList.headerFontSize
                                    font.bold: editorList.headerFontBold
                                    font.underline: editorList.headerFontUnderline
                                    verticalAlignment:   editorList.headerVAlign
                                    horizontalAlignment: editorList.headerHAlign
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  editorList.editorNameColumnWidth
                                Layout.preferredHeight: editorList.headerHeight
                                border.width: editorList.cellFrameWidth
                                radius: editorList.cellFrameRadius
                                color: editorList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Email"
                                    elide: Text.ElideRight
                                    color: editorList.headerForeground
                                    font.family: editorList.headerFontFamily
                                    font.pixelSize: editorList.headerFontSize
                                    font.bold: editorList.headerFontBold
                                    font.underline: editorList.headerFontUnderline
                                    verticalAlignment:   editorList.headerVAlign
                                    horizontalAlignment: editorList.headerHAlign
                                }
                            }

                            Rectangle {
                                Layout.preferredWidth:  editorList.editorNameColumnWidth
                                Layout.preferredHeight: editorList.headerHeight
                                border.width: editorList.cellFrameWidth
                                radius: editorList.cellFrameRadius
                                color: editorList.headerBackground
                                Layout.fillWidth: true

                                Text {
                                    anchors.fill: parent

                                    text: "Name"
                                    elide: Text.ElideRight
                                    color: editorList.headerForeground
                                    font.family: editorList.headerFontFamily
                                    font.pixelSize: editorList.headerFontSize
                                    font.bold: editorList.headerFontBold
                                    font.underline: editorList.headerFontUnderline
                                    verticalAlignment:   editorList.headerVAlign
                                    horizontalAlignment: editorList.headerHAlign
                                }
                            }

                            Rectangle {
                                Layout.preferredWidth:  editorList.headerHeight
                                Layout.preferredHeight: editorList.headerHeight
                                border.width: editorList.cellFrameWidth
                                radius: editorList.cellFrameRadius
                                color:  editorList.headerBackground

                                Image {
                                    anchors.fill: parent
                                    source:  "qrc:/icons/buttons/remove.png"//editorListView.currentIndex !== -1 ? "qrc:/icons/buttons/remove.png" : "qrc:/icons/buttons/remove_disabled.png"
                                    fillMode: Image.PreserveAspectFit
                                    antialiasing: true
                                    mipmap: true

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            var currentIndex =  editorListView.currentIndex;

                                            tabRoot.entityChanged("editor",currentIndex)

                                            if (editorListModel.rowCount() !== currentIndex) {
                                                console.assert(tabRoot.index === currentIndex, "Critical: indexes shouldn't differ");
                                                tabRoot.currentIndexChanged(currentIndex);
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth:  editorList.headerHeight
                                Layout.preferredHeight: editorList.headerHeight
                                border.width: editorList.cellFrameWidth
                                radius: editorList.cellFrameRadius
                                color:  editorList.headerBackground

                                Image {
                                    anchors.fill: parent
                                    source: "qrc:/icons/buttons/add.png"
                                    fillMode: Image.PreserveAspectFit
                                    antialiasing: true
                                    mipmap: true

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            tabRoot.entityChanged("editor",-1)
                                        }
                                    }
                                }
                            }
                        }

                        //EditorList view
                        ListView {
                            id: editorListView
                            objectName: "editorListView"
                            Layout.fillWidth:  true
                            Layout.fillHeight: true
                            highlightFollowsCurrentItem: true
                            snapMode: ListView.SnapToItem
                            spacing: 2
                            clip: true

                            model: editorListModel
                            delegate: editorListDelegate


                            //TODO proper status, check if there is an empty line in the list


                            property int status: {

                                if(currentIndex === -1)
                                    return Status.invalid;
                                if (model.name === ""      ||
                                        model.email === ""   )
                                    return Status.invalid;
                                if (model.institution === "")
                                    return Status.valid;

                                return Status.complete;
                            }

                            onCurrentIndexChanged: tabRoot.index = currentIndex

                            Component.onCompleted: tabRoot.reset.connect(clearSelection)

                            function clearSelection() {
                                currentIndex = -1;
                            }
                        }

                        //PatientList delegate
                        Component {
                            id: editorListDelegate

                            Item {
                                id: delegateContainer
                                width:  ListView.view.width
                                height: editorList.rowHeight

                                MouseArea {
                                    id: editorListDelegateMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    property bool hovered: false

                                    onClicked: {

                                        tabRoot.currentIndexChanged("editorListView",index)
//                                    console.log(undoRedoController.currentDrug.metadata.editors.comments)
                                    console.log(undoRedoController.editors.objat(tabRoot.index).comments)
                                    }
                                    onEntered: hovered = true
                                    onExited:  hovered = false
                                }

                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 2

                                    Rectangle {
                                        Layout.preferredWidth: editorList.editorDateColumnWidth
                                        Layout.fillHeight: true
                                        border.width: editorList.cellFrameWidth
                                        radius: editorList.cellFrameRadius
                                        color: editorListDelegateMouseArea.hovered ? editorList.rowBackgroundHover : (delegateContainer.ListView.isCurrentItem ? editorList.rowBackgroundSelected : editorList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: editorList.cellMargin
                                            text: Qt.formatDate(model.date)
                                            elide: Text.ElideRight
                                            color: editorListDelegateMouseArea.hovered ? editorList.rowForegroundHover : (delegateContainer.ListView.isCurrentItem ? editorList.rowForegroundSelected : editorList.rowForeground)
                                            font.family:    editorList.rowFontFamily
                                            font.pixelSize: editorList.rowFontSize
                                            verticalAlignment:   editorList.rowVAlign
                                            horizontalAlignment: editorList.rowHAlign
                                        }
                                    }
                                    Rectangle {
                                        Layout.preferredWidth:  editorList.editorInstitutionColumnWidth
                                        Layout.fillHeight: true
                                        border.width: editorList.cellFrameWidth
                                        radius: editorList.cellFrameRadius
                                        color: editorListDelegateMouseArea.hovered ? editorList.rowBackgroundHover : (delegateContainer.ListView.isCurrentItem ? editorList.rowBackgroundSelected : editorList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: editorList.cellMargin
                                            text: model.institution
                                            elide: Text.ElideRight
                                            color: editorListDelegateMouseArea.hovered ? editorList.rowForegroundHover : (delegateContainer.ListView.isCurrentItem ? editorList.rowForegroundSelected : editorList.rowForeground)
                                            font.family:    editorList.rowFontFamily
                                            font.pixelSize: editorList.rowFontSize
                                            verticalAlignment:   editorList.rowVAlign
                                            horizontalAlignment: editorList.rowHAlign
                                        }
                                    }
                                    Rectangle {
                                        Layout.preferredWidth:  editorList.editorNameColumnWidth
                                        Layout.fillHeight: true
                                        border.width: editorList.cellFrameWidth
                                        radius: editorList.cellFrameRadius
                                        color: editorListDelegateMouseArea.hovered ? editorList.rowBackgroundHover : (delegateContainer.ListView.isCurrentItem ? editorList.rowBackgroundSelected : editorList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: editorList.cellMargin
                                            text: model.email
                                            elide: Text.ElideRight
                                            color: editorListDelegateMouseArea.hovered ? editorList.rowForegroundHover : (delegateContainer.ListView.isCurrentItem ? editorList.rowForegroundSelected : editorList.rowForeground)
                                            font.family:    editorList.rowFontFamily
                                            font.pixelSize: editorList.rowFontSize
                                            verticalAlignment:   editorList.rowVAlign
                                            horizontalAlignment: editorList.rowHAlign
                                        }
                                    }


                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        border.width: editorList.cellFrameWidth
                                        radius: editorList.cellFrameRadius
                                        color: editorListDelegateMouseArea.hovered ? editorList.rowBackgroundHover : (delegateContainer.ListView.isCurrentItem ? editorList.rowBackgroundSelected : editorList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: editorList.cellMargin
                                            text: model.name
                                            elide: Text.ElideRight
                                            color: editorListDelegateMouseArea.hovered ? editorList.rowForegroundHover : (delegateContainer.ListView.isCurrentItem ? editorList.rowForegroundSelected : editorList.rowForeground)
                                            font.family:    editorList.rowFontFamily
                                            font.pixelSize: editorList.rowFontSize
                                            verticalAlignment:   editorList.rowVAlign
                                            horizontalAlignment: editorList.rowHAlign
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                /**/
                //Patient
                Rectangle {
                    id: editor
                    Layout.fillWidth: true
                    Layout.preferredHeight: 190
                    Layout.minimumHeight:   190

                    property int rowHeight: 25

                    property string frameBackground: Style.form.background.frame
                    property string frameColor:      Style.form.border.color
                    property int frameWidth:   Style.form.border.width
                    property int frameRadius:  Style.form.border.radius
                    property int framePadding: 5

                    property string labelForeground: Style.form.foreground.label
                    property string labelFontFamily: Style.form.font.label
                    property string labelFontSize:   Style.form.size.label
                    property bool labelFontBold: false
                    property int labelVAlign: Text.AlignVCenter
                    property int labelHAlign: Text.AlignRight

                    property string inputFontFamily: Style.form.font.input
                    property string inputFontSize:   Style.form.size.input
                    property int inputVAlign: Text.AlignVCenter
                    property int inputHAlign: Text.AlignLeft

                    //Patient view
                    ListView {
                        id: editorView
                        anchors.fill: parent
                        anchors.margins: 0
                        highlightFollowsCurrentItem: true
                        snapMode: ListView.SnapToItem
                        clip: true
                        interactive: false

                        model: editorModel
                        delegate: editorDelegate

                        property int delegateStatus
                        property int status: count === 0 ? Status.valid : delegateStatus

                        property string delegateEditionDate
                        property string editionDate: count === 0 ? "" : delegateEditionDate

                        property int delegateGender
                        property int gender: count === 0 ? 0 : delegateGender

                        property string delegateFullName
                        property string fullName: count === 0 ? "" : delegateFullName

                        onEditionDateChanged: tabRoot.editionDate = editionDate
                        onFullNameChanged:  tabRoot.editorName      = fullName
                    }

                    //Patient delegate
                    Component {
                        id: editorDelegate

                        Item {
                            id: container
                            width:  ListView.view.width
                            height: ListView.view.height

                            property string fullName: nameInput.text

                            property string editionDate: isNaN(editionDateInput.date) ? "" : Qt.formatDate(editionDateInput.date)

                            property int status: {
                                if (nameInput.text  === ""      ||
                                        emailInput.text    === ""   )
                                    return Status.invalid;
                                if (institutionInput.text === "")
                                    return Status.valid;

                                return Status.complete;
                            }

                            onFullNameChanged:  ListView.view.delegateFullName  = fullName
                            onEditionDateChanged: ListView.view.delegateEditionDate = editionDate
                            onStatusChanged:    ListView.view.delegateStatus    = status

                            Component.onCompleted: {
                                ListView.view.delegateFullName  = fullName
                                ListView.view.delegateEditionDate = editionDate
                                ListView.view.delegateStatus    = status
                            }

                            Rectangle {
                                id: editorFrame
                                anchors.fill: parent
                                anchors.margins: 0
                                color:  editor.frameBackground
                                radius: editor.frameRadius
                                border.width: editor.frameWidth
                                border.color: editor.frameColor
                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.margins: editor.framePadding
                                    spacing: 5

                                    RowLayout {
                                        Layout.preferredHeight: editor.rowHeight
                                        Layout.minimumHeight:   editor.rowHeight
                                        spacing: 2

                                        Text {
                                            id: nameLabel
                                            Layout.preferredWidth: 75
                                            text: "Name:"
                                            elide: Text.ElideRight
                                            color: editor.labelForeground
                                            font.family:    editor.labelFontFamily
                                            font.pixelSize: editor.labelFontSize
                                            font.bold:      editor.labelFontBold
                                            verticalAlignment:   editor.labelVAlign
                                            horizontalAlignment: editor.labelHAlign
                                        }
                                        TextField {
                                            id: nameInput
                                            Layout.fillWidth:  true
                                            placeholderText: "Name"
                                            text: model.name
                                            onTextChanged:{
                                                tabRoot.textChanged("name",text)
                                            }
                                            font.family:    editor.inputFontFamily
                                            font.pixelSize: editor.inputFontSize
                                            verticalAlignment:   editor.inputVAlign
                                            horizontalAlignment: editor.inputHAlign
                                        }

                                    }


                                    RowLayout {
                                        Layout.preferredHeight: editor.rowHeight
                                        Layout.minimumHeight:   editor.rowHeight
                                        spacing: 2

                                        Text {
                                            id: editionDateLabel
                                            Layout.preferredWidth: 75
                                            text: "Edition date:"
                                            elide: Text.ElideRight
                                            color: editor.labelForeground
                                            font.family:    editor.labelFontFamily
                                            font.pixelSize: editor.labelFontSize
                                            font.bold: editor.labelFontBold
                                            verticalAlignment:   editor.labelVAlign
                                            horizontalAlignment: editor.labelHAlign
                                        }
                                        DatePicker {
                                            id: editionDateInput
                                            Layout.fillWidth: true
                                            Binding { target: editionDateInput; property: "date"; value: model.date }
                                            onEditingFinished: tabRoot.textChanged("date", date.toLocaleDateString(Qt.locale(),"yyyy.MM-dd"))
                                        }

                                    }

                                    RowLayout {
                                        Layout.preferredHeight: editor.rowHeight
                                        Layout.minimumHeight:   editor.rowHeight
                                        spacing: 2

                                        Text {
                                            id: emailLabel
                                            Layout.preferredWidth: 75
                                            text: "Email:"
                                            elide: Text.ElideRight
                                            color: editor.labelForeground
                                            font.family:    editor.labelFontFamily
                                            font.pixelSize: editor.labelFontSize
                                            font.bold: editor.labelFontBold
                                            verticalAlignment:   editor.labelVAlign
                                            horizontalAlignment: editor.labelHAlign
                                        }
                                        TextField {
                                            id: emailInput
                                            Layout.fillWidth:  true
                                            placeholderText: "email"
                                            text: model.email
                                            onTextChanged: {
                                                tabRoot.textChanged("email",text)
                                            }
                                            font.family:    editor.inputFontFamily
                                            font.pixelSize: editor.inputFontSize
                                            verticalAlignment:   editor.inputVAlign
                                            horizontalAlignment: editor.inputHAlign
                                        }
                                    }

                                    RowLayout {
                                        Layout.preferredHeight: editor.rowHeight
                                        Layout.minimumHeight:   editor.rowHeight
                                        spacing: 2

                                        Text {
                                            id: institutionLabel
                                            Layout.preferredWidth: 75
                                            text: "Institution:"
                                            elide: Text.ElideRight
                                            color: editor.labelForeground
                                            font.family:    editor.labelFontFamily
                                            font.pixelSize: editor.labelFontSize
                                            font.bold: editor.labelFontBold
                                            verticalAlignment:   editor.labelVAlign
                                            horizontalAlignment: editor.labelHAlign
                                        }
                                        TextField {
                                            id: institutionInput
                                            Layout.fillWidth:  true
                                            placeholderText: "institution"
                                            text: model.institution
                                            onTextChanged: {
                                                tabRoot.textChanged("institution",text)
                                            }
                                            font.family:    editor.inputFontFamily
                                            font.pixelSize: editor.inputFontSize
                                            verticalAlignment:   editor.inputVAlign
                                            horizontalAlignment: editor.inputHAlign
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    rightPanel: CommentsView {
        commentModel: undoRedoController.editorComments//(tabRoot.index == -1) ? "" :undoRedoController.editors.objat(tabRoot.index).comments
        objectName: "editorCommentsView"
    }

}
