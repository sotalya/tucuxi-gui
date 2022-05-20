import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0


Item {
    id: root
    width: 200
    height: 35

    // inputs
    property alias text: textInput.text
    property alias suffix: textSuffix.text

    // outputs
    property bool textIsOk
    property string textValue
    property int maxLength

    // output singnals
    signal editingFinished


    function textInputControl()
    {
        var minValue = 0
        var maxValue = 10000000
        textValue = textInput.text

        if (isNaN(textInput.text) || textInput.text > maxValue || textInput.text < minValue || textInput === "")
        {
            console.log("NaN");
            // textInputFrame.border.color = 'red';
            textInput.color = 'red'

            if (textInput.text == "")
                textInput.color = 'black';

            textIsOk = false;
        }
        else
        {
            textInput.color = 'black'
            console.log(textInput.text);
            console.log(parseFloat(textInput.text));
            textIsOk = true;
        }
    }

        Rectangle {
        id: textInputFrame
        Layout.preferredWidth: 200
        height: 35
        anchors.fill: parent
        border.color: 'lightGray'
        border.width: 1

        TextInput {
            id: textInput
            objectName: "textInput"
            onTextChanged: textInputControl()
            validator: DoubleValidator { decimals: 2 }
            font.family: Style.form.font.input
            anchors.fill: parent
            anchors.margins: 4
            verticalAlignment: TextInput.AlignVCenter
            onEditingFinished: {
                if (textInput.text < 1)
                    textInput.text = parseFloat(textInput.text);
                covariateDialog.validate()
            }
            maximumLength: maxLength
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


