import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import guiutils.qml.styles 1.0

Item {

    height: 128
    property string drugName:         interpretationController.currentActiveSubstance ? interpretationController.currentActiveSubstance.name.value() : ""
    property date patientBirthDate: interpretationController.currentPatient ? interpretationController.currentPatient.person.birthday : ""
    property string patientName:      interpretationController.currentPatient ? interpretationController.currentPatient.person.firstname.charAt(0) + " " + interpretationController.currentPatient.person.name : ""

    property int patientGender: interpretationController.currentPatient ? interpretationController.currentPatient.person.gender : 0

    property int topMargin:    28
    property int bottomMargin: 28
    property int leftMargin:   50

    property int iconSpacing: 5

    property int totalWidth :  patientNameLabel.contentWidth - 10

    //Name label
    Text {
        id: patientNameLabel
        anchors.top:  parent.top
        anchors.left: parent.left
        anchors.topMargin:  topMargin
        anchors.leftMargin: leftMargin
        text: patientName === "" ? "select a patient..." : patientName
        elide: Text.ElideRight
        color: Style.flow.foreground.information
        font.family:    Style.flow.font.information
        font.pixelSize: Style.flow.size.information
        font.bold:   true
        font.italic: patientName === ""
        verticalAlignment:   Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    //Birth date label
    Text {
        id: patientBirthDateLabel
        anchors.top:  patientNameLabel.bottom
        anchors.left: patientNameLabel.left
        anchors.topMargin: 4
        text: patientName === "" ? "" : patientBirthDate.getDate()  + "." + (patientBirthDate.getMonth()+1) + "." + patientBirthDate.getFullYear();
        elide: Text.ElideRight
        color: Style.flow.foreground.information
        font.family:    Style.flow.font.information_birthdate
        font.pixelSize: Style.flow.size.information_birthdate
        font.bold:   true
        verticalAlignment:   Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    //Gender icon
    Image {
        id: genderIcon
        anchors.verticalCenter: patientNameLabel.verticalCenter
        anchors.right:          patientNameLabel.left
        anchors.rightMargin: iconSpacing
        width:  32
        height: 32
        source:  patientGender !== 0 ? "qrc:/icons/other/gender_male.png" : "qrc:/icons/other/gender_female.png";
        antialiasing: true
        mipmap:       true
        fillMode: Image.PreserveAspectFit
    }

    //Drug label
    Text {
        id: drugNameLabel
        anchors.bottom:  parent.bottom
        anchors.left:    parent.left
        anchors.bottomMargin: bottomMargin
        anchors.leftMargin:   leftMargin
        text: drugName === "" ? "select a drug..." : drugName
        elide: Text.ElideRight
        color: Style.flow.foreground.information
        font.family:    Style.flow.font.information
        font.pixelSize: Style.flow.size.information
        font.bold:   true
        font.italic: drugName === ""
        verticalAlignment:   Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    //Drug icon
    Image {
        id: drugIcon
        anchors.verticalCenter: drugNameLabel.verticalCenter
        anchors.right:          drugNameLabel.left
        anchors.rightMargin: iconSpacing
        width:  32
        height: 32
        source:  "qrc:/icons/other/drug.png";
        antialiasing: true
        mipmap:       true
        fillMode: Image.PreserveAspectFit
    }
}
