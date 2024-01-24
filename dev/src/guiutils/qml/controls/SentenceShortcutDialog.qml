import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Qt.labs.platform

import guiutils.qml.styles
import guiutils.qml.controls

DialogBase {
    id: root
    title: "Sentences shorcuts"
    width: 550
    height: 350

    property int m_modifiers: 0x0
    property int m_key:       0x0

    property int m_oldModifiers: 0x0
    property int m_oldKey:       0x0
    property string m_oldText:   ""


    property int m_sectionNb: 0
    property string m_drugId: ""

    property bool saveIsEnabled: true
    property bool isEditing: false
    property int  m_index: 0

    property alias sentenceText: sentenceInput.text

    signal exited()

    // For testing
    function getSaveButtonStatus(){
        return saveIsEnabled
    }
    function setText(_text){
        sentenceText = _text
        validateShortcut()
    }
    function setModifier(_modifier){
        if(_modifier === "CTRL"){
            controlCheckBox.checked = true
        }
        else if(_modifier === "SHIFT"){
            shiftCheckBox.checked = true
        }
        else if(_modifier === "ALT"){
            altCheckBox.checked = true
        }
        validateShortcut()
    }
    function setKey(_key){
        m_key = _key.charCodeAt(0)
        keyShortcut.text = _key.toUpperCase()
        validateShortcut()
    }

    function reset(){
        m_modifiers = 0x0
        m_key = 0x0
        controlCheckBox.checked = false
        shiftCheckBox.checked = false
        altCheckBox.checked = false
        sentenceText = ""
        keyShortcut.text = ""
        shortcutIsGlobalInfo.visible = false
        shortcutIsLocalInfo.visible = false
    }

    function init(_key, _modifier, _text, _sectionNb, _drugId, _isEdit, _index){
        m_modifiers = _modifier
        m_key = _key
        m_sectionNb = _sectionNb
        m_drugId = _drugId
        m_index = _index

        // Reset all CheckBoxes
        controlCheckBox.checked = false
        shiftCheckBox.checked = false
        altCheckBox.checked = false
        // Set the ones that are needed
        if(_modifier & Qt.ControlModifier) controlCheckBox.checked = true
        if(_modifier & Qt.ShiftModifier) shiftCheckBox.checked = true
        if(_modifier & Qt.AltModifier) altCheckBox.checked = true

        if(_text === ""){
            saveIsEnabled = false
        }

        sentenceText = _text
        keyShortcut.text = String.fromCharCode(_key)

        if(validateKeyCombination_global()) validateKeyCombination_local()

        if(_isEdit){
            m_oldKey = m_key
            m_oldModifiers = m_modifiers
            m_oldText = sentenceText
            saveIsEnabled = false
        }
        else{
            m_oldKey = 0x0
            m_oldModifiers = 0x0
            m_oldText = 0x0
        }

        isEditing = _isEdit
    }

    function setClearModifier(_checkState, _modifier){
        if(_checkState){
            m_modifiers = m_modifiers | _modifier
        }
        else{
            m_modifiers = m_modifiers & (~_modifier)
        }

        validateShortcut()
    }

    function setShortcutValidColor(_isValid){
        if(_isValid){
            keyShortcut.color = "black"
            if (m_oldKey === m_key && m_oldModifiers === m_modifiers && m_oldText === sentenceText){
                saveIsEnabled = false
            }
            else{
                saveIsEnabled = true
            }

        }
        else{
            keyShortcut.color = "red"
            saveIsEnabled = false
        }

        controlCheckBox.setErrorColor(!_isValid)
        shiftCheckBox.setErrorColor(!_isValid)
        altCheckBox.setErrorColor(!_isValid)
    }

    function validateText() {
        var bValid = true

        if (m_oldKey !== m_key || m_oldModifiers !== m_modifiers || m_oldText !== sentenceText){
            bValid = validationTabController.validateText(m_sectionNb, sentenceText, m_drugId, isEditing, m_index)
        }

        setShortcutValidColor(bValid)

        if(shortcutIsGlobalInfo.visible) textAlreadyExist.visible = false
        else if (shortcutIsLocalInfo.visible) textAlreadyExist.visible = false
        else if (bValid) textAlreadyExist.visible = false
        else textAlreadyExist.visible = true

        return bValid
    }

    function validateKeyCombination_local() {
        var bValid = true

        if (m_oldKey !== m_key || m_oldModifiers !== m_modifiers || m_oldText !== sentenceText){
            bValid = validationTabController.validateSentenceShortcut(m_sectionNb, m_key, m_modifiers, sentenceText, m_drugId, isEditing, m_index)
        }

        setShortcutValidColor(bValid)

        if(bValid || shortcutIsGlobalInfo.visible) shortcutIsLocalInfo.visible = false
        else {
            textAlreadyExist.visible = false
            shortcutIsLocalInfo.visible = true
        }
        return bValid
    }

    function validateKeyCombination_global() {
        var bOk = true

        if (m_oldKey !== m_key || m_oldModifiers !== m_modifiers || m_oldText !== sentenceText){
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
                if(0 <= (m_key - Qt.Key_0) && (m_key - Qt.Key_0) <= 9){
                    bOk = false
                }
            }
        }

        // Change text color if combination not ok
        setShortcutValidColor(bOk)

        if(bOk) shortcutIsGlobalInfo.visible = false
        else{
            textAlreadyExist.visible = false
            shortcutIsLocalInfo.visible = false
            shortcutIsGlobalInfo.visible = true
        }


        return bOk
    }

    function validateShortcut(){
        if(validateKeyCombination_global()){
            if(validateKeyCombination_local()){
                return validateText()
            }
            else{
                return false
            }
        }
        else{
            return false
        }
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
                        objectName: "sentenceFlick"
                        anchors.fill: parent

                        TextArea.flickable: TextArea {
                            id: sentenceInput
                            objectName: "sentenceInput"
                            placeholderText: "Please fill in"
                            clip: true
                            anchors.margins:1
                            wrapMode: TextInput.Wrap
                            font.family: Style.form.font.input
                            font.pixelSize: Style.form.size.input
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft

                            rightPadding: 15

                            Keys.onReleased: {
                                if(sentenceText === "") saveIsEnabled = false
                                else{
                                    validateShortcut()
                                }
                            }
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
                    font.family: Style.form.font.label
                    font.pixelSize: Style.form.size.label
                    color: Style.form.foreground.label
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
                    checkBoxtext: macos ? "COMMAND" : "CTRL"

                    onCheckStateChanged: {
                        setClearModifier(checked, Qt.ControlModifier)
                    }
                }

                EntityCheckBox{
                    id: shiftCheckBox
                    objectName: "shiftCheckBox"
                    checkBoxtext: "SHIFT"

                    onCheckStateChanged: {
                        setClearModifier(checked, Qt.ShiftModifier)
                    }
                }

                EntityCheckBox{
                    id: altCheckBox
                    objectName: "altCheckBox"
                    checkBoxtext: macos ? "CONTROL" : "ALT"

                    onCheckStateChanged: {
                        setClearModifier(checked, macos ? Qt.MetaModifier : Qt.AltModifier)
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
                    font.family: Style.form.font.label
                    font.pixelSize: Style.form.size.label
                    color: Style.form.foreground.label
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

                                validateShortcut()
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

                    Text {
                        id: shortcutIsLocalInfo
                        anchors.fill: parent
                        text: "The selected key combination is already used for another sentence in this section"
                        color: "red"
                        font.bold: true
                        font.pixelSize: Style.form.size.input

                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        visible: false
                    }

                    Text {
                        id: textAlreadyExist
                        anchors.fill: parent
                        text: "The choosen text already exist"
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
                    objectName: "sentencesSaveButton"
                    Layout.alignment: Qt.AlignLeft
                    text: "Save"
                    Layout.preferredWidth: 125

                    enabled: saveIsEnabled

                    opacity: saveIsEnabled ? 1 : 0.5

                    onClicked: function() {
                        if(validateShortcut()){
                            var sec = sentencesPalettes.getSection(m_sectionNb)

                            if(m_drugId === ""){
                                if(isEditing) sec.editSentenceOfGlobal(m_key, m_modifiers, sentenceText, m_index)
                                else sec.addSentenceToGlobal(m_key, m_modifiers, sentenceText)

                            }
                            else {
                                if(isEditing) sec.editSentenceOfDrugSentencesList(m_drugId, m_key, m_modifiers, sentenceText, m_index)
                                else sec.addSentenceToDrugSentencesList(m_drugId, m_key, m_modifiers, sentenceText)
                            }
                            root.exited()
                            root.exit(true);
                        }
                        else{
                            // This should never appear, however it might still be possible to save even with unavailable shortcuts selected
                            messageNoValidShortcut.open()
                        }
                    }
                }

                Button {
                    id: cancelBtn
                    objectName: "sentencesCancelButton"
                    Layout.alignment: Qt.AlignRight
                    text: "Cancel"
                    Layout.preferredWidth: 125
                    onClicked: function() {
                        reset()
                        root.exited()
                        root.exit(false)
                    }
                }
            }
        }
    }

    MessageDialog {
        id: messageNoValidShortcut
        title: "Shortcut"
        text: "The shortcut choosen is not available as it's already in use"
        buttons: MessageDialog.Close
    }
}
