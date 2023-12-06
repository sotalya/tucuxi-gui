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

    property int m_modifiers: 0x0
    property int m_key: 0x0

    function toggleModifier(_checkState, _modifier){
        if(_checkState){
            m_modifiers = m_modifiers | _modifier
        }
        else{
            m_modifiers = m_modifiers & (~_modifier)
        }
        validateKeyCombination()
    }

    function init(_modifier, _key, _sentence){
        m_modifiers = _modifier
        m_key = _key

        // Reset all CheckBoxes
        controlCheckBox.checked = false
        shiftCheckBox.checked = false
        altCheckBox.checked = false
        // Set the ones that are needed
        if(_modifier & Qt.ControlModifier) controlCheckBox.checked = true
        if(_modifier & Qt.ShiftModifier) shiftCheckBox.checked = true
        if(_modifier & Qt.AltModifier) altCheckBox.checked = true

        sentenceInput.text = _sentence
        keyShortcut.text = String.fromCharCode(_key)
    }

    function reset(){
        m_modifiers = 0x0
        m_key = 0x0
        controlCheckBox.checked = false
        shiftCheckBox.checked = false
        altCheckBox.checked = false
        sentenceInput.text = ""
        keyShortcut.text = ""
        shortcutIsGlobalInfo.visible = false
    }

    function validateKeyCombination() {
        var bOk = true
        // Check if the keys combination used is already in use
        if(m_modifiers === (Qt.ControlModifier|Qt.ShiftModifier)){
            switch (m_key) {
                case Qt.Key_F2:
                    bOk = false
                    break
                case Qt.Key_T:
                    bOk = false
                    break
                case Qt.Key_O:
                    bOk = false
                    break
                case Qt.Key_E:
                    bOk = false
                    break
                case Qt.Key_A:
                    bOk = false
                    break
                case Qt.Key_S:
                    bOk = false
                    break
                case Qt.Key_D:
                    bOk = false
                    break
                case Qt.Key_F:
                    bOk = false
                    break
                case Qt.Key_G:
                    bOk = false
                    break
                case Qt.Key_X:
                    bOk = false
                    break
                default:
                    break
            }
        }
        // Check if only a number was used (already in use to switch tabs)
        else if(m_modifiers === Qt.NoModifier){
            if((m_key - Qt.Key_0) <= 9){
                bOk = false
            }
        }

        // Change text color if combination not ok
        if(bOk){
            keyShortcut.color = "black"
            controlCheckBox.setErrorColor(!bOk)
            shiftCheckBox.setErrorColor(!bOk)
            altCheckBox.setErrorColor(!bOk)
            shortcutIsGlobalInfo.visible = false
        }
        else{
            keyShortcut.color = "red"
            controlCheckBox.setErrorColor(!bOk)
            shiftCheckBox.setErrorColor(!bOk)
            altCheckBox.setErrorColor(!bOk)
            shortcutIsGlobalInfo.visible = true
        }

        return bOk
    }


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

                            rightPadding: 15
                        }

                        ScrollBar.vertical: ScrollBar {
                            active: true
                        }
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
                    font.family: Style.form.font.input
                    font.pixelSize: Style.form.size.input
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            RowLayout{
                spacing: 2
                Layout.alignment: Qt.AlignHCenter

                EntityCheckBox {
                    id: controlCheckBox
                    objectName: "controlCheckBox"
                    checkBoxtext: "CTRL"

                    onCheckStateChanged: {
                        toggleModifier(checked, Qt.ControlModifier)
                    }
                }

                EntityCheckBox{
                    id: shiftCheckBox
                    objectName: "shiftCheckBox"
                    checkBoxtext: "SHIFT"

                    onCheckStateChanged: {
                        toggleModifier(checked, Qt.ShiftModifier)
                    }
                }

                EntityCheckBox{
                    id: altCheckBox
                    objectName: "altCheckBox"
                    checkBoxtext: "ALT"

                    onCheckStateChanged: {
                        toggleModifier(checked, Qt.AltModifier)
                    }
                }
            }

            RowLayout {
                spacing: 2
                Label {
                    id: keyShortcutLabel
                    Layout.fillWidth: true
                    Layout.fillHeight:  true
                    text: "+"
                    font.family: Style.form.font.input
                    font.pixelSize: Style.form.size.input
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            RowLayout {
                spacing: 2
                Item {
                    // spacer
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Rectangle {
                    Layout.preferredWidth: 30
                    Layout.preferredHeight: 30
                    Layout.alignment: Qt.AlignHCenter
                    border.width: 1
                    border.color: "#cccccc"

                    TextField {
                        id: keyShortcut
                        objectName: "keyShortcut"
                        anchors.fill: parent
                        anchors.margins: 1
                        validator: RegularExpressionValidator { regularExpression: /[A-Za-z0-9]/ }
                        font.family: Style.form.font.input
                        font.pixelSize: Style.form.size.input
                        font.bold: true
                        verticalAlignment: TextField.AlignVCenter
                        horizontalAlignment: TextField.AlignHCenter
                        padding: 0
                        leftPadding: 0
                        rightPadding: 0

                        // This function is called whenever a key is pressed
                        Keys.onPressed: function(event) {
                            if ((event.modifiers === Qt.NoModifier || event.modifiers === Qt.KeypadModifier ||
                                 event.modifiers === Qt.ShiftModifier) && event.key !== Qt.Key_Backspace) {
                                // Check if the pressed key is between F1 and F12 else is a normal key
                                if (0 <= (event.key - Qt.Key_F1) && (event.key - Qt.Key_F1) <= 12){
                                    text = "F" + (event.key - Qt.Key_F1 + 1)
                                } else{
                                    text = event.text.toUpperCase()
                                }

                                m_key = event.key
                                event.accepted = true
                                validateKeyCombination()
                            }
                        }
                    }
                }

                Item {
                    // spacer
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                spacing: 2

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 15
                    Layout.alignment: Qt.AlignHCenter

                    Text {
                        id: shortcutIsGlobalInfo
                        anchors.fill: parent
                        text: "The selected key combination is not available because it is part of the global ones"
                        color: "red"
                        font.bold: true
                        font.pixelSize: Style.form.size.input

                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        visible: false
                    }
                }
            }

            RowLayout{
                Layout.alignment: Qt.AlignHCenter
                spacing: 50
                Button {
                    id: saveBtn
                    Layout.alignment: Qt.AlignLeft
                    objectName: "sentencesSaveButton"
                    text: "Save"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        if(validateKeyCombination(m_key, m_modifiers)){
                            root.exit(false);
                        }
                    }
                }

                Button {
                    id: cancelBtn
                    Layout.alignment: Qt.AlignRight
                    text: "Cancel"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        reset()
                        root.exit(false);
                    }
                }
            }
        }
    }
}
