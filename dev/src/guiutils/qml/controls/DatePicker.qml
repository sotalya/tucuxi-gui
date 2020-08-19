import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQml 2.2

import guiutils.qml.styles 1.0

//ToDo: fix the height (slightly above the a basic text field)

Rectangle {
    id: defaultStyle
//    implicitWidth: wholeDate.implicitWidth - 20
    implicitHeight:wholeDate.height

    width : 150
    Layout.minimumWidth: 150

    property bool isOk: true
    property var doValidation: function() {return true;}
    function validate()
    {
        isOk = doValidation();
        return isOk;
    }

    property var date: new Date()
    property string separator: "."

    property bool isEnabled: true

    property string fontFamily: Style.form.font.input
    property int fontSize:      Style.form.size.input

    signal editingFinished()

//    property bool hasFocus: daysInput.activeFocus || monthsInput.activeFocus || yearsInput.activeFocus

    anchors.bottomMargin: -1
    color: "#44ffffff"
    radius: 0 // baserect.radius

    Rectangle {
        id: baserect
        anchors.fill: parent

        //radius: height * 0.16
        //border.color: defaultStyle.hasFocus ? "#47b" : "#999"

        //gradient: Gradient {
        //    GradientStop {color: "#e0e0e0" ; position: 0}
        //    GradientStop {color: "#fff" ; position: 0.1}
        //    GradientStop {color: "#fff" ; position: 1}
        //}
    }


    TextField {
        id : wholeDate
        anchors.left: parent.left
        font.family:    parent.fontFamily
        font.pixelSize: parent.fontSize
        implicitWidth: 150
        inputMask: "00/00/0000"
        inputMethodHints : Qt.ImhDate
        text: parent.date.toLocaleString(Qt.locale(),"dd/MM/yyyy")
        color: isOk ? "black" : "red"
        enabled: isEnabled

        onFocusChanged: if (activeFocus) selectAll()

        onEditingFinished: {
            var theDate = Date.fromLocaleString(Qt.locale(), text, "dd/MM/yyyy");
            if (theDate.toLocaleString(Qt.locale(),"dd/MM/yyyy") === text) {
                parent.date = theDate;
                parent.editingFinished();
            }
            else {
                color = "#FF0000";
            }
        }
    }

    Image {
        id: calendarInput
        anchors.right: wholeDate.right
        anchors.top: wholeDate.top
        anchors.bottom: wholeDate.bottom
        anchors.margins: 5
        width: height
        mipmap: true
        source: "qrc:/icons/buttons/calendar.png"
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            onClicked: {
                calendar.open(date);
            }
        }
    }

    Component {
        id: borderlessStyle

        TextFieldStyle {
            background: Rectangle {
                anchors.fill: parent
                anchors.margins: 0
                color: "transparent"
            }
        }
    }

    TucuCalendar {
        id: calendar
        onAccepted: {
            defaultStyle.date = date;
            editingFinished();
        }
    }
}
