import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

SpinBox {

    id: root

    property bool isOk: true

    // doValidation is used as a function, so can be set by the instantiator
    property var doValidation: function() { return true;}
    function validate()
    {
        isOk = doValidation();
        return isOk;
    }


    editable : true

    stepSize : Math.pow(10,decimals)

    //    Layout.fillWidth:  true
    font.family: Style.form.font.input
    font.pixelSize: Style.form.size.input
    property int  horizontalAlignment:  Text.AlignHCenter
    property int  decimals: 0
    property int  minimumValue: 0
    property int maximumValue: 1000000
    property string suffix : ""

    // Avoids the 1'000 or 1,000 separators
    locale : Qt.locale("C");

    signal editingFinished

    function setRealValue(val)
    {
        value = Math.pow(10, decimals) * val;
        txtInput.text = root.textFromValue(root.value, root.locale);
    }

    function getRealValue()
    {
        return value / Math.pow(10,decimals);
    }

    from : minimumValue * Math.pow(10,decimals)
    to : maximumValue * Math.pow(10,decimals)

    // Format the value after a change
    // There is some code to avoid strange behavior of the cursor when editing the value. These problems are
    // caused by the formatting code which suddently adds separator for thousands and ".00" for decimals when
    // they are missing. The current code fixes problems linked to thousands separators but does nothing for
    // the ones linked to decimals.
    onValueChanged: {
        var t = root.textFromValue(root.value, root.locale);
        if (!(t === txtInput.text) && root.value !== 0) {
            var curPosFromBack = txtInput.text.length - txtInput.cursorPosition
            txtInput.text = t;
            txtInput.cursorPosition = txtInput.text.length - curPosFromBack;
        }
        if (root.value === 0) {
            txtInput.cursorPosition = 0; // Keep the cursor on the left when deleting a char results in a value of 0.
        }
        validate();
    }

    property string baseColor: "black" // "#007194"


    // From https://stackoverflow.com/questions/41879198/suffix-option-for-spinbox-on-qt-quick-controls-2
    contentItem: RowLayout{
        z: 2
        TextInput {
            id: txtInput
            Layout.fillWidth: true
            Layout.fillHeight: true
           // text: root.textFromValue(root.value, root.locale)
            font: root.font
            color: isOk ? Qt.darker(baseColor, root.enabled ? 1 : 0.7) : "red"
            selectionColor: baseColor
            selectedTextColor: "#ffffff"
            horizontalAlignment: Qt.AlignRight
            verticalAlignment: Qt.AlignVCenter

            readOnly: !root.editable
            validator: (root.decimals === 0) ? root.intValidator : root.doubleValidator
            inputMethodHints: Qt.ImhFormattedNumbersOnly

            onTextChanged: {
                root.value = valueFromText(text, root.locale);
            }

            // Handle delete keys manually because standard behavior is user friendly when
            // deleting the left most character
            Keys.onPressed: {
                if (event.key === Qt.Key_Backspace) {
                    if (cursorPosition == 1) {
                        text = text.substring(1);
                        cursorPosition = 0;
                        event.accepted = true
                    }
                }
                if (event.key === Qt.Key_Delete) {
                    var curPosFromBack = text.length - cursorPosition
                    if (cursorPosition == 0) {
                        text = text.substring(1);
                        cursorPosition = 0;
                        event.accepted = true
                    }
                }
            }

            onEditingFinished: root.editingFinished
        }
        Text{
            Layout.preferredWidth: contentWidth
            Layout.fillHeight: true
            z: -1
            font: txtInput.font
            color: txtInput.color
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            text: suffix
        }
    }

    property var intValidator: IntValidator {
        bottom: Math.min(root.from, root.to)
        top:  Math.max(root.from, root.to)
    }


    property var doubleValidator: DoubleValidator {
        bottom: Math.min(root.from, root.to)
        top:  Math.max(root.from, root.to)
    }

    textFromValue: function(value, locale) {
        if (root.decimals === 0)
            return value;
        else
            return Number(value / 100).toLocaleString(locale, 'f', root.decimals)
    }

    valueFromText: function(text, locale) {
        var val;
        try {
            var spaceFreeText = text.replace(/\s/g, ''); // Avoid problems with misplaced separators after a deletion
            val = Number.fromLocaleString(locale, spaceFreeText) * Math.pow(10, root.decimals);
        }
        catch (e) {
            val = root.value;
            isOk = false;
        }
        return val;
    }

}