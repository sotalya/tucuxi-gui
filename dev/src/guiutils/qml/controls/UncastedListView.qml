import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {
    id: uncastedL
    property int index:  -1;
    property int status: Status.invalid;
    property bool enabled: false;
    property alias values : uncastedListView.values;
    property var controller;

    visible : values ? (values.size() > 0) : false;

    signal currentIndexChanged(int index)
    signal reset()

    onIndexChanged:  currentIndexChanged(index)

    color: Style.table.background.table

    Layout.fillWidth:  true

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        EntityTitle {
            label.text: "Ambiguous Imported Values List"
            id: uncastedListExpandHeader
            tooltipText: ToolTips.uncastedList.listTitle
        }


        //UncastedList
        EntityList {
            id: uncastedList


            visible : uncastedListExpandHeader.expanded

            columnlayout.children: [
                //UncastedList header
                EntityListHeader {
                    EntityListHeaderItem {
                        Layout.preferredWidth: 100
                        label.text: "Field"
                        tooltipText: ToolTips.uncastedList.field
                    }
                    EntityListHeaderItem {
                        Layout.preferredWidth: 180
                        label.text: "Value"
                        tooltipText: ToolTips.uncastedList.value
                    }
                    EntityListHeaderItem {
                        Layout.preferredWidth: 180
                        label.text: "Comment"
                        tooltipText: ToolTips.uncastedList.comment
                    }
                },

                EntityHeaderEnd {
                    visible: true
                },

                //UncastedList view
                EntityListView {
                    id: uncastedListView
                    Layout.fillWidth:  true

                    property var values;

                    model: values ? values.objlist : 0
                    //UncastedList delegate
                    delegate: EntityListDelegate {
                        height : 60
                        id: root
                        property var selected_model: model
                        mousearea.onClicked: {
                            root.ListView.view.currentIndex = index
                            currentIndexChanged(index);
                        }

                        rowlayout.children: [

                            TextArea {
                                id : t1
                                background: Rectangle {
                                    implicitHeight:  Math.max(t1.paintedHeight + t1.topPadding + t1.bottomPadding,
                                                              t2.paintedHeight + t2.topPadding + t2.bottomPadding,
                                                              t3.paintedHeight + t3.topPadding + t3.bottomPadding)
                                    color: modelData.validated ? "#3EFF92" : (modelData.ispartiallycasted ? "#FFD25C" : "#FF6658")
                                }
                                Layout.preferredWidth: 100
                                text: modelData.field
                                wrapMode: TextInput.WordWrap
                                Layout.minimumHeight: contentHeight
                            },

                            TextArea {
                                id : t2
                                background: Rectangle {
                                    implicitHeight:  Math.max(t1.paintedHeight + t1.topPadding + t1.bottomPadding,
                                                              t2.paintedHeight + t2.topPadding + t2.bottomPadding,
                                                              t3.paintedHeight + t3.topPadding + t3.bottomPadding)
                                    color: modelData.validated ? "#3EFF92" : (modelData.ispartiallycasted ? "#FFD25C" : "#FF6658")
                                }

                                Layout.preferredWidth: 180
                                text: modelData.text
                                wrapMode: TextInput.WordWrap
                                Layout.minimumHeight: contentHeight
                            },

                            TextArea {
                                id : t3
                                background: Rectangle {
                                    implicitHeight:  Math.max(t1.paintedHeight + t1.topPadding + t1.bottomPadding,
                                                              t2.paintedHeight + t2.topPadding + t2.bottomPadding,
                                                              t3.paintedHeight + t3.topPadding + t3.bottomPadding)
                                    color: modelData.validated ? "#3EFF92" : (modelData.ispartiallycasted ? "#FFD25C" : "#FF6658")
                                }
                                Layout.fillWidth: true
                                Layout.preferredWidth: 180
                                text: modelData.comment
                                wrapMode: TextInput.WordWrap
                                Layout.minimumHeight: contentHeight
                            },

                            EntityListImage {
                                Layout.minimumWidth: 25
                                image.source: "qrc:/icons/buttons/check.png"
                                mousearea.onClicked: {
                                    if (modelData.validated)
                                        modelData.validated = false;
                                    else
                                        modelData.validated = true;
                                    interpretationController.evaluateFlow();
                                }
                                tooltipText: ToolTips.uncastedList.check
                            }
                        ]

                    }

                    onCurrentIndexChanged: index = currentIndex
                    Component.onCompleted: reset.connect(clearSelection)

                    function clearSelection() {
                        currentIndex = -1;
                    }
                }
            ]
        }


        EntityBlockEnd {
            visible : uncastedListExpandHeader.visible && uncastedListExpandHeader.expanded
        }

    }
}
