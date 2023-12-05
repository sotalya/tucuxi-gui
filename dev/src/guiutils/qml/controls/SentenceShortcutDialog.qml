import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles
import QtQuick.Window

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

DialogBase {
    id: root
    title: "Sentences shorcuts"
    width: 550
    height: 350

    property int modifiers: 0x0
    property int key: 0x0


    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            root.exit(true);
        }
    }

    GridLayout {
        anchors.fill: parent
        rows: 3
        columns: 3

        Rectangle {
            Layout.row: 0
            Layout.column: 0
            height: 25
            width: 50
        }
        Rectangle {
            Layout.row: 2
            Layout.column: 2
            height: 25
            width: 50
        }

        ColumnLayout {
            Layout.row: 1
            Layout.column:1
            height: parent.parent.height - 50
            width: parent.parent.width - 100

            RowLayout {
                spacing: 2
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    Layout.alignment: Qt.AlignHCenter
                    border.width: 1
                    border.color: "#cccccc"

                    Flickable{
                        id: sentenceFlick
                        anchors.fill: parent

                        TextArea.flickable: TextArea {
                            id: sentenceInput
                            objectName: "sentenceInput"
                            placeholderText: "Please fill in"
                            clip: true
                            anchors.margins:1
                            wrapMode: TextArea.Wrap
                            font.family: Style.form.font.input
                            font.pixelSize: Style.form.size.input
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }

                        ScrollBar.vertical: ScrollBar {}
                    }
                }
            }

            RowLayout {
                spacing: 2
                Label {
                    id: checkBoxesLabel
                    Layout.fillWidth: true
                    Layout.fillHeight:  true
                    text: "Select a keys combination for the shortcut"
                    font.bold: true
                    font.family: Style.form.font.input
                    font.pixelSize: Style.form.size.input
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            RowLayout{
                spacing: 2
                Layout.alignment: Qt.AlignHCenter
                CheckBox {
                    id: controlCheckBox
                    objectName: "controlCheckBox"
                    checked: false
                    text: "CTRL"
                    font.bold: true
                    leftPadding: indicator.width
                }

                CheckBox {
                    id: shiftCheckBox
                    objectName: "shiftCheckBox"
                    checked: false
                    text: "SHIFT"
                    font.bold: true
                    leftPadding: indicator.width
                }

                CheckBox {
                    id: altCheckBox
                    objectName: "altCheckBox"
                    checked: false
                    text: "ALT"
                    font.bold: true
                    leftPadding: indicator.width
                }
            }

            RowLayout {
                spacing: 2
                Label {
                    id: keyShortcutLabel
                    Layout.fillWidth: true
                    Layout.fillHeight:  true
                    text: "+"
                    font.bold: true
                    font.family: Style.form.font.input
                    font.pixelSize: Style.form.size.input
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            RowLayout {
                spacing: 2
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    Layout.alignment: Qt.AlignHCenter
                    border.width: 1
                    border.color: "#cccccc"

                    TextField {
                        id: keyShortcut
                        objectName: "keyShortcut"
                        anchors.fill: parent
                        anchors.margins:1
                        inputMask: "N"
                        font.family: Style.form.font.input
                        font.pixelSize: Style.form.size.input
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        onTextEdited: {
                            // Check if the text is longer than 1 character
                            if (text.length > 1) {
                                // If it is, update the text with only the first character
                                text = text.charAt(0);
                            }
                        }
                    }
                }
            }

            RowLayout{
                spacing: 2
                Button {
                    id: saveBtn
                    Layout.alignment: Qt.AlignLeft
                    objectName: "sentencesSaveButton"
                    text: "Save"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        root.exit(false);
                    }
                }

                Button {
                    id: cancelBtn
                    Layout.alignment: Qt.AlignRight
                    text: "Cancel"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        root.exit(false);
                    }
                }
            }
        }
    }
}
