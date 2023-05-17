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

    property var savedDate

    signal editingFinished()

    //    property bool hasFocus: daysInput.activeFocus || monthsInput.activeFocus || yearsInput.activeFocus

    property bool opacityOnOff: false

    function opacityHandler()
    {
        if (opacityOnOff == false)  {
//            wholeDate.opacity = 0.5
//            dateFormatHint.opacity = 1
            dateFormatHint.visible = true
            opacityOnOff = true
        } else {
//            wholeDate.opacity = 1
//            dateFormatHint.opacity = 0
            dateFormatHint.visible = false
            opacityOnOff = false
        }
    }

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
        objectName: "wholeDate"
        anchors.top: parent.top
        anchors.left: parent.left
        font.family: parent.fontFamily
        font.pixelSize: parent.fontSize
        implicitWidth: 150
        inputMask: "00/00/0000"
        inputMethodHints : Qt.ImhDate
        text: parent.date.toLocaleString(Qt.locale(),"dd/MM/yyyy")
        color: isOk ? "black" : "red"
        enabled: isEnabled

        onFocusChanged: if (activeFocus)
                            selectAll()

        // Not OK with Qt5.7 on Windows:
        //onHoveredChanged: {
        //    opacityHandler()
        //}

//        onTextChanged: {
//            var theDate = Date.fromLocaleString(Qt.locale(), text, "dd/MM/yyyy");
//            if (theDate.toLocaleString(Qt.locale(),"dd/MM/yyyy") === text) {
//                parent.date = theDate;
//                parent.editingFinished();
//                color = "black";
//            }
//            else color = "#FF0000"
//        }

        onEditingFinished: {
            var theDate = Date.fromLocaleString(Qt.locale(), text, "dd/MM/yyyy");
            if (theDate.toLocaleString(Qt.locale(),"dd/MM/yyyy") === text) {
                parent.date = theDate;
                parent.editingFinished();
                color = "black";
            }
            else {
                color = "#FF0000";
            }
        }

//        MouseArea {
//            anchors.fill: parent
//            hoverEnabled: true
//            onEntered: {
////                wholeDate.opacity = 0.2

//            }
//            onExited: { console.log("EXITED zone")
////                wholeDate.opacity = 1
//                //            wholeDate.text = defaultStyle.date.toLocaleString(Qt.locale(),"dd/MM/yyyy")
//            }
//            onClicked: {
////                wholeDate.opacity = 1
//                //            wholeDate.text = defaultStyle.date.toLocaleString(Qt.locale(), "dd/MM/yyyy")
//            }
//        }
    }

    Text {
        id: dateFormatHint
        anchors {
            //        anchors.horizontalCenter: wholeDate.horizontalCenter
            left: wholeDate.left
            leftMargin: 12
            top: wholeDate.bottom
            topMargin: -13
        }
        font.pixelSize: parent.fontSize-3
        text: "dd / mm / yyyy"
        color: "gray" // "blue"
        enabled: isEnabled
        visible: false
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

