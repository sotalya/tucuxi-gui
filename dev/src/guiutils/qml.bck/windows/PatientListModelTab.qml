import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.0
import ezechiel 1.0

Item {
    id: root
    width: Screen.desktopAvailableWidth / 2
    height: Screen.desktopAvailableHeight - frame.height

    Rectangle {
        id: mainContainer
        anchors.fill: parent
        color: "white"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: parent.forceActiveFocus()
    }

    ColumnLayout {
        id: mainLayout
        anchors.left: mainContainer.left
        anchors.leftMargin: 10
        anchors.top: mainContainer.top
        anchors.topMargin: 10
        width: mainContainer.width / 2 - 20
//        minimumWidth: patientView

        ComboBox {
            id: patientList
            Layout.fillWidth: true
            model: patientListModel
            textRole: "lastName"
            onActivated: patientView.currentIndex = index
        }

        ListView {
            id: patientView
            Layout.fillWidth: true

            highlightMoveDuration: 750
            interactive: false
            clip: true
            spacing: 5

            model: patientListModel
            delegate: patientDelegate

            state: "simple"
            states: [
                State {
                    name: "simple"
                    PropertyChanges { target: patientView; height: 125 }
                },
                State {
                    name: "detailed"
                    PropertyChanges { target: patientView; height: 350 }
                }
            ]

            transitions: [
                Transition {
                    NumberAnimation {
                        duration: 200;
                        properties: "height"
                    }
                }
            ]
        }
    }

    Component {
        id: patientDelegate

        Item {
            width: ListView.view.width
            height: ListView.view.height

            state: "simple"
            states: [
                State {
                    name: "simple"
                    when: patientView.state === "simple"
                    PropertyChanges { target: adminImage; source: "qrc:/icons/zoomin.png" }
                    PropertyChanges { target: externalIdRow; visible: false }
                    PropertyChanges { target: stayNumberRow; visible: false }
                    PropertyChanges { target: addressRow; visible: false }
                    PropertyChanges { target: postcodeRow; visible: false }
                    PropertyChanges { target: cityRow; visible: false }
                    PropertyChanges { target: stateRow; visible: false }
                    PropertyChanges { target: countryRow; visible: false }
                },
                State {
                    name: "detailed"
                    when: patientView.state === "detailed"
                    PropertyChanges { target: adminImage; source: "qrc:/icons/zoomin.png" }
                    PropertyChanges { target: externalIdRow; visible: true }
                    PropertyChanges { target: stayNumberRow; visible: true }
                    PropertyChanges { target: addressRow; visible: true }
                    PropertyChanges { target: postcodeRow; visible: true }
                    PropertyChanges { target: cityRow; visible: true }
                    PropertyChanges { target: stateRow; visible: true }
                    PropertyChanges { target: countryRow; visible: true }
                }
            ]

            transitions: [
                Transition {
                    NumberAnimation {
                        duration: 200;
                        properties: "visible"
                    }
                }
            ]

            Rectangle {
                id: patientDelegateContainer
                anchors.fill: parent
                radius: 1
                color: "LightSteelBlue"
                border.color: Qt.darker(color)

                RowLayout {
                anchors.fill: patientDelegateContainer
                anchors.margins:2

                    Rectangle {
                        id: rect_zoompatdel
                        Layout.fillHeight: true
                        //width: patientView.width / 4
                        width: 25
                        // color: "transparent"
                        color: "yellow"

                    Image {
                        id: adminImage
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        fillMode: Image.PreserveAspectFit
                        antialiasing: true
                        source: "qrc:/icons/zoomin.png"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: patientView.state = (patientView.state === "simple" ? "detailed" : "simple")
                    }
                }
                Rectangle {
                    width: patientDelegateContainer.width - 25
                    height: patientDelegateContainer.height
                    anchors.left: rect_zoompatdel.right
                    color: "green"
                    Rectangle {
                        x: 5
                    ColumnLayout {
                        RowLayout {
                        id: firstNameRow

                    Text {
                    Layout.minimumWidth: contentWidth
                    Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                    text: "First Name:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "first name"
                            text: firstName
                            onEditingFinished: firstName = text
                        }
                    }

                    RowLayout {
                        id: lastNameRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "Last Name:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "last name"
                            text: lastName
                            onEditingFinished: lastName = text
                        }
                    }

                    RowLayout {
                        id: birthDateRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "Birth Date:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            id: birthDateInput
                            Layout.fillWidth: true
                            placeholderText: "date of birth"
                            text: Qt.formatDate(birthDate, Qt.SystemLocaleShortDate)

                            Loader {
                                id: birthDatePicker
                                source: "DatePicker.qml"
                            }
                            Connections {
                                target: birthDatePicker.item
                                onDateChosen: birthDate = date;
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: birthDatePicker.item.show()
                            }
                        }
                    }

                    RowLayout {
                        id: genderRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "Gender:"
                            horizontalAlignment: Text.AlignRight
                        }
                        ExclusiveGroup {
                            id: genderGroup
                        }
                        RadioButton {
                            id: maleRadioButton
                            text: "Male"
                            exclusiveGroup: genderGroup
                            onClicked: gender = 0
                        }
                        Binding {
                            target: maleRadioButton
                            property: "checked"
                            value: gender == 0 ? true : false
                        }
                        RadioButton {
                            id: femaleRadioButton
                            text: "Female"
                            exclusiveGroup: genderGroup
                            onClicked: gender = 1
                        }
                        Binding {
                            target: femaleRadioButton
                            property: "checked"
                            value: gender == 1 ? true : false
                        }
                    }

                    RowLayout {
                        id: externalIdRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "Patient Id:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "patient id"
                            text: externalId
                            onEditingFinished: externalId = text
                        }
                    }

                    RowLayout {
                        id: stayNumberRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "Stay Number:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "stay number"
                            text: stayNumber
                            onEditingFinished: stayNumber = text
                        }
                    }

                    RowLayout {
                        id: addressRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "Address:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "address"
                            text: address
                            onEditingFinished: address = text
                        }
                    }

                    RowLayout {
                        id: postcodeRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "Postcode:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "postcode"
                            text: postcode
                            onEditingFinished: postcode = text
                        }
                    }

                    RowLayout {
                        id: cityRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "City:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "city"
                            text: city
                            onEditingFinished: city = text
                        }
                    }

                    RowLayout {
                        id: stateRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "State:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "state"
                            text: model.state
                            onEditingFinished: model.state = text
                        }
                    }

                    RowLayout {
                        id: countryRow

                        Text {
                            Layout.minimumWidth: contentWidth
                            Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                            text: "Country:"
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "country"
                            text: country
                            onEditingFinished: country = text
                        }
                    }

                    RowLayout {
                        id: spacer

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "transparent"
                        }
                    }

                    }
                    }
                }
            }

            }

        }
    }
}
