import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

//ToDo: fix the height (slightly above the a basic text field)

TextField {
    id: root
    objectName: "timePickerObj"
    implicitWidth: 60

    property string separator: ":"

    property var date: new Date()
    property bool isEnabled: true
    property string lastTextOk

    font.family:    Style.form.font.input
    font.pixelSize: Style.form.size.input

    //signal editingFinished()

    property bool isOk: true
    property var doValidation: function() { return true;}
    function validate()
    {
        if (getValues()) {
            isOk = doValidation();
        }
        else {
            isOk = false;
        }

        return isOk
    }

    inputMask: "99:99"
    inputMethodHints : Qt.ImhTime
    text: date.toLocaleString(Qt.locale(),"hh:mm")
    lastTextOk: date.toLocaleString(Qt.locale(),"hh:mm")
    color: isOk ? "black" : "red"

    enabled: isEnabled

    onFocusChanged: if (activeFocus) selectAll()
/*
    onTextChanged: {
        var curPos = cursorPosition

        // Separate hours and minutes
        var parts = text.split(":")

        // Check if we're missing the colon...
        if (parts.length != 2) {
            // If yes, use the last correct value
            parts = lastTextOk.split(":")
        }

        // Handle the "hours" part
        if (parts[0].length >= 2) {
            if (curPos == 2) {
                curPos = 3 // Move after the colon to let the user modify the minutes part
            }
            if (curPos < 2) {
                // Override digit instead of insert
                parts[0] = parts[0].slice(curPos-1,curPos) + parts[0].slice(curPos+1, curPos+2)
            }
            parts[0] = parts[0].slice(0,2)
        }

        // Handle the "minutes" part
        if (parts[1].length > 2) {
            var pos = curPos - parts[0].length - 1
            if (pos == 1) {
                parts[1] = parts[1].slice(0, 1) + parts[1].slice(2, 3);
            }
            parts[1] = parts[1].slice(0,2);
        }

        // Set the "fixed" text
        text = parts[0] + ":" + parts[1]
        lastTextOk = text

        // After the modification of the text we need to place the cursor at the correct position
        cursorPosition = curPos;
    }
*/
    onEditingFinished: {
        getValues();
    }

    function getValues() {
        var curPos = cursorPosition

        // Separate hours and minutes
        var parts = text.split(":")

        // Check if we have hours and minutes
        if (parts.length !== 2) {
            return false
        }

        // Handle cases we do not have 2 digits
        for (var i in parts) {
            if (parts[i].length === 0) parts[i] = "00";
            var val = parseInt(parts[i])
            if (val < 10) parts[i] = "0" + val
        }

        // Check validity
        var dateText = parts[0] + ":" + parts[1]
        var theDate = Date.fromLocaleTimeString(Qt.locale(), dateText, "hh:mm");
        if (theDate.toLocaleTimeString(Qt.locale(),"hh:mm") !== dateText) {
            return false;
        }

        // Ok, update the display and set the date variable
        text = dateText
        cursorPosition = curPos
        date.setHours(theDate.getHours())
        date.setMinutes(theDate.getMinutes())
        return true;
    }

    function setDate(newDate)
    {
        date = newDate;
        text = date.toLocaleTimeString(Qt.locale(),"hh:mm")
        lastTextOk = date.toLocaleTimeString(Qt.locale(),"hh:mm")
    }

    onDateChanged: {
        text = date.toLocaleTimeString(Qt.locale(),"hh:mm")
        lastTextOk = date.toLocaleTimeString(Qt.locale(),"hh:mm")
    }
}
