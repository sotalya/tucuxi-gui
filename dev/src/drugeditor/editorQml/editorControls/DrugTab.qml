import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

import ezechiel 1.0

FlowTab {
    id: tabRoot

    property int index:  -1
    property int status: Status.invalid

    property string drugName: ""

    signal currentIndexChanged(string view,int index)
    signal mainIndexChanged(int index)
    signal currentStatusChanged(int status)
    signal reset()

    signal textChanged(string propertyName, var newValue)
    // `index == -1` when adding new line
    signal entityChanged(string listViewName, int index)


    onIndexChanged: {

//        if(index != drugPanel.drugList.drugListView.currentIndex)
//            drugListView.currentIndex = index;
        mainIndexChanged(index)
    }
    onStatusChanged: currentStatusChanged(status)

    leftPanel: Component {
        id: drugPanel

        Rectangle {
            anchors.fill: parent
            color: Style.table.background.table

            property int status: {
                if (drugListView.status === Status.invalid || domainListView.status === Status.invalid || studyListView.status === Status.invalid)
                    return Status.invalid
                if (drugListView.status === Status.valid   || domainListView.status === Status.valid   || studyListView.status === Status.valid)
                    return Status.valid

                return Status.complete
            }

            onStatusChanged: tabRoot.status = status

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5

                //DrugList
                Rectangle {

                    id: drugList
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

                    property int drugNameColumnWidth: 125

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

                        //DrugList header
                        RowLayout {
                            spacing: 2

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: drugList.headerHeight
                                border.width: drugList.cellFrameWidth
                                radius: drugList.cellFrameRadius
                                color: drugList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Drug"
                                    elide: Text.ElideRight
                                    color: drugList.headerForeground
                                    font.family: drugList.headerFontFamily
                                    font.pixelSize: drugList.headerFontSize
                                    font.bold: drugList.headerFontBold
                                    font.underline: drugList.headerFontUnderline
                                    verticalAlignment:   drugList.headerVAlign
                                    horizontalAlignment: drugList.headerHAlign
                                }
                            }
                        }

                        //DrugList view
                        ListView {
                            id: drugListView
                            objectName: "drugListView"
                            Layout.fillWidth:  true
                            Layout.fillHeight: true
                            highlightFollowsCurrentItem: true
                            snapMode: ListView.SnapToItem
                            spacing: 2
                            clip: true

                            model: undoRedoController.drugs.objlist
                            delegate: drugListDelegate

                            property string currentDrugName: currentIndex === -1 ? "" : undoRedoController.drugs.objat(currentIndex).name.value()
                            property int status:           currentIndex === -1 ? Status.invalid : Status.complete

                            onCurrentDrugNameChanged: tabRoot.drugName = currentDrugName

                            onCurrentIndexChanged: {
//                                tabRoot.currentIndexChanged("drugListView",currentIndex)
                                if (currentIndex !== -1) {
                                    domainListView.currentIndex = 0;
                                    studyListView.currentIndex  = 0;
                                }
                            }

                            Component.onCompleted: {

                                tabRoot.reset.connect(clearSelection)
                                tabRoot.indexChanged.connect(refreshIndex)
                            }

                            function clearSelection() {
                                currentIndex = -1;
                            }

                            function refreshIndex() {
                                if(currentIndex !== tabRoot.index){
                                    currentIndex = tabRoot.index;
                                }
                            }
                        }

                        //DrugList delegate
                        Component {
                            id: drugListDelegate

                            Item {
                                id: delegateContainer
                                width:  ListView.view.width
                                height: drugList.rowHeight

                                MouseArea {
                                    id: drugListDelegateMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    property bool hovered: false

                                    onClicked: tabRoot.currentIndexChanged("drugsView",index) //tabRoot.ListView.view.currentIndex = index
                                    onEntered: hovered = true
                                    onExited:  hovered = false
                                }

                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 2

                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        border.width: drugList.cellFrameWidth
                                        radius: drugList.cellFrameRadius
                                        color: drugListDelegateMouseArea.hovered ? drugList.rowBackgroundHover : (delegateContainer.ListView.isCurrentItem ? drugList.rowBackgroundSelected : drugList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: drugList.cellMargin
                                            text: modelData.name.value()
                                            elide: Text.ElideRight
                                            color: drugListDelegateMouseArea.hovered ? drugList.rowForegroundHover : (delegateContainer.ListView.isCurrentItem ? drugList.rowForegroundSelected : drugList.rowForeground)
                                            font.family:    drugList.rowFontFamily
                                            font.pixelSize: drugList.rowFontSize
                                            verticalAlignment:   drugList.rowVAlign
                                            horizontalAlignment: drugList.rowHAlign
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                //DomainList
                Rectangle {
                    id: domainList
                    objectName: "domainsView"
                    Layout.fillWidth: true
                    Layout.preferredHeight: headerHeight + 3 + (rowHeight + domainListView.spacing) * 3
                    Layout.minimumHeight:   headerHeight + 3 + (rowHeight + domainListView.spacing) * 3

                    property string headerBackground: Style.table.background.header
                    property string headerForeground: Style.table.foreground.header
                    property string headerFontFamily: Style.table.font.header
                    property string headerFontSize:   Style.table.size.header
                    property bool headerFontBold:      true
                    property bool headerFontUnderline: false
                    property int headerHeight: 25
                    property int headerVAlign: Text.AlignVCenter
                    property int headerHAlign: Text.AlignHCenter

                    property int domainNameColumnWidth: 125

                    property string rowBackground:         Style.table.background.row
                    property string rowBackgroundHover:    Style.table.background.row_hover
                    property string rowBackgroundSelected: Style.table.background.row_selected
                    property string rowForeground:         Style.table.foreground.row
                    property string rowForegroundHover:    Style.table.foreground.row_hover
//                    property string rowForegroundSelected: Style.stringtable.foreground.row_selected
                    property string rowFontFamily:  Style.table.font.row
                    property string rowFontSize:    Style.table.size.row
                    property int rowVAlign: Text.AlignVCenter
                    property int rowHAlign: Text.AlignLeft
                    property int rowHeight: 25

                    property int cellFrameRadius: 1
                    property int cellFrameWidth:  0
                    property int cellMargin:      5

                    signal currentIndexChanged(int index)

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 0

                        //DomainList header
                        RowLayout {
                            spacing: 2

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: domainList.headerHeight
                                border.width: domainList.cellFrameWidth
                                radius: domainList.cellFrameRadius
                                color: domainList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Domain"
                                    elide: Text.ElideRight
                                    color: domainList.headerForeground
                                    font.family: domainList.headerFontFamily
                                    font.pixelSize: domainList.headerFontSize
                                    font.bold: domainList.headerFontBold
                                    font.underline: domainList.headerFontUnderline
                                    verticalAlignment:   domainList.headerVAlign
                                    horizontalAlignment: domainList.headerHAlign
                                }
                            }
                        }

                        //DomainList view
                        ListView {
                            id: domainListView
                            Layout.fillWidth:  true
                            Layout.fillHeight: true
                            highlightFollowsCurrentItem: true
                            snapMode: ListView.SnapToItem
                            spacing: 2
                            clip: true

                            model: domainListModel
                            delegate: domainListDelegate

                            property int status: currentIndex === -1 ? Status.invalid : Status.complete

                            onCurrentIndexChanged: domainList.currentIndexChanged(currentIndex)
                            Component.onCompleted: tabRoot.reset.connect(clearSelection)

                            function clearSelection() {
                                currentIndex = -1;
                            }
                        }

                        //DomainList delegate
                        Component {
                            id: domainListDelegate

                            Item {
                                id: tabRoot
                                width:  ListView.view.width
                                height: domainList.rowHeight

                                MouseArea {
                                    id: domainListDelegateMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    property bool hovered: false

                                    onClicked: tabRoot.ListView.view.currentIndex = index
                                    onEntered: hovered = true
                                    onExited:  hovered = false
                                }

                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 2

                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        border.width: domainList.cellFrameWidth
                                        radius: domainList.cellFrameRadius
                                        color: domainListDelegateMouseArea.hovered ? domainList.rowBackgroundHover : (tabRoot.ListView.isCurrentItem ? domainList.rowBackgroundSelected : domainList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: domainList.cellMargin
                                            text: model.name
                                            elide: Text.ElideRight
                                            color: domainListDelegateMouseArea.hovered ? domainList.rowForegroundHover : (tabRoot.ListView.isCurrentItem ? domainList.rowForegroundSelected : domainList.rowForeground)
                                            font.family:    domainList.rowFontFamily
                                            font.pixelSize: domainList.rowFontSize
                                            verticalAlignment:   domainList.rowVAlign
                                            horizontalAlignment: domainList.rowHAlign
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                //StudyList
                Rectangle {
                    id: studyList
                    objectName: "studiesView"
                    Layout.fillWidth:  true
                    Layout.preferredHeight: headerHeight + 3 + (rowHeight + studyListView.spacing) * 3
                    Layout.minimumHeight:   headerHeight + 3 + (rowHeight + studyListView.spacing) * 3

                    property string headerBackground: Style.table.background.header
                    property string headerForeground: Style.table.foreground.header
                    property string headerFontFamily: Style.table.font.header
                    property string headerFontSize:   Style.table.size.header
                    property bool headerFontBold:      true
                    property bool headerFontUnderline: false
                    property int headerHeight: 25
                    property int headerVAlign: Text.AlignVCenter
                    property int headerHAlign: Text.AlignHCenter

                    property int studyNameColumnWidth: 125

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

                    signal currentIndexChanged(int index)

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 0

                        //StudyList header
                        RowLayout {
                            spacing: 2

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: studyList.headerHeight
                                border.width: studyList.cellFrameWidth
                                radius: studyList.cellFrameRadius
                                color: studyList.headerBackground

                                Text {
                                    anchors.fill: parent
                                    text: "Study"
                                    elide: Text.ElideRight
                                    color: studyList.headerForeground
                                    font.family: studyList.headerFontFamily
                                    font.pixelSize: studyList.headerFontSize
                                    font.bold: studyList.headerFontBold
                                    font.underline: studyList.headerFontUnderline
                                    verticalAlignment:   studyList.headerVAlign
                                    horizontalAlignment: studyList.headerHAlign
                                }
                            }
                        }

                        //StudyList view
                        ListView {
                            id: studyListView
                            Layout.fillWidth:  true
                            Layout.fillHeight: true
                            highlightFollowsCurrentItem: true
                            snapMode: ListView.SnapToItem
                            spacing: 2
                            clip: true

                            model: studyListModel
                            delegate: studyListDelegate

                            property int status: currentIndex === -1 ? Status.invalid : Status.complete

                            onCurrentIndexChanged: studyList.currentIndexChanged(currentIndex)
                            Component.onCompleted: tabRoot.reset.connect(clearSelection)

                            function clearSelection() {
                                currentIndex = -1;
                            }
                        }

                        //StudyList delegate
                        Component {
                            id: studyListDelegate

                            Item {
                                id: tabRoot
                                width:  ListView.view.width
                                height: studyList.rowHeight

                                MouseArea {
                                    id: studyListDelegateMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    property bool hovered: false

                                    onClicked: tabRoot.ListView.view.currentIndex = index
                                    onEntered: hovered = true
                                    onExited:  hovered = false
                                }

                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 2

                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        border.width: studyList.cellFrameWidth
                                        radius: studyList.cellFrameRadius
                                        color: studyListDelegateMouseArea.hovered ? studyList.rowBackgroundHover : (tabRoot.ListView.isCurrentItem ? studyList.rowBackgroundSelected : studyList.rowBackground)

                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: studyList.cellMargin
                                            text: model.name
                                            elide: Text.ElideRight
                                            color: studyListDelegateMouseArea.hovered ? studyList.rowForegroundHover : (tabRoot.ListView.isCurrentItem ? studyList.rowForegroundSelected : studyList.rowForeground)
                                            font.family:    studyList.rowFontFamily
                                            font.pixelSize: studyList.rowFontSize
                                            verticalAlignment:   studyList.rowVAlign
                                            horizontalAlignment: studyList.rowHAlign
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

rightPanel:    CommentsView {
                            commentModel: undoRedoController.drugGeneralComments
                            objectName: "drugCommentsView"
                        }

}
