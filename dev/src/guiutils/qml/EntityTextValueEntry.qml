import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel


Item {
    id: root
    width: 200
    height: 38

    // inputs
    property alias text: textInput.text
    property alias suffix: textSuffix.text

    // outputs
    property bool textIsOk
    property string textValue
    property int maxLength
    property bool trigger: false

    // output singnals
    signal editingFinished

    function textInputControl()
    {
        var minValue = 0.0
        var maxValue = 10000000
        textValue = textInput.text

        if (isNaN(textInput.text) || textInput.text > maxValue || textInput.text < minValue || !textInput.text)
        {
            console.log("NaN");
            // textInputFrame.border.color = 'red';
            textInput.color = 'red'

            if (!textInput.text) {
                textInput.color = 'black';
//                console.log("EMPTY STRING")
            }
            textIsOk = false;
        }
        else
        {
            textInput.color = 'black'
//            console.log(textInput.text);
//            console.log(parseFloat(textInput.text));
            textIsOk = true;
        }
    }

    Rectangle {
        id: textInputFrame
        Layout.preferredWidth: 200
        anchors.fill: parent
        border.color: 'lightGray'
        border.width: 1

        TextInput {
            id: textInput
            objectName: "textInput"
            onTextChanged: textInputControl()
            validator: DoubleValidator { decimals: 2 }
            font.family: Style.form.font.input
            font.pixelSize: 13
            anchors.fill: parent
            anchors.margins: 4
            anchors.leftMargin: 10
            verticalAlignment: TextInput.AlignVCenter
            onEditingFinished: {
//                if (textInput.text < 1 && !textInput)
//                    textInput.text = parseFloat(textInput.text);

                if (root.objectName === "covarValueEntry")
                    covariateDialog.validate()
                else if (root.objectName === "measureValueEntry")
                    measureDialog.validate()
            }
            maximumLength: maxLength
            onActiveFocusChanged: {
                if (trigger == false) {
                    textInputFrame.border.color = 'blue'
                    textInputFrame.border.width = 2
                    trigger = true

                }
                else {
                    textInputFrame.border.color = 'lightGray'
                    textInputFrame.border.width = 1
                    trigger = false
                }
            }
        }

        Text {
            id: textSuffix
            font: textInput.font
            color: 'black'
            anchors.fill: parent
            anchors.margins: 4
            horizontalAlignment: Qt.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }
}


