import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.controls 1.0

Item {
    id: root
    implicitWidth: 500
    implicitHeight: 310

    Item {
        id: impl
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: 5

            Rectangle {
                id: parametersContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 105
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    RowLayout {
                        Text {
                            Layout.minimumWidth: 75
                            Layout.maximumWidth: 75
                            horizontalAlignment: Text.AlignRight
                            text: "From:"
                        }
                        DatePicker {
                            id: fromDate
                            Layout.fillWidth: true
                        }
                    }
                    RowLayout {
                        Text {
                            Layout.minimumWidth: 75
                            Layout.maximumWidth: 75
                            horizontalAlignment: Text.AlignRight
                            text: "at:"
                        }
                        TimePicker {
                            Layout.fillWidth: true
                            hours: 12
                            minutes: 0
                        }
                    }
                    RowLayout {
                        Text {
                            Layout.minimumWidth: 75
                            Layout.maximumWidth: 75
                            horizontalAlignment: Text.AlignRight
                            text: "Use:"
                        }
                        ExclusiveGroup {
                            id: steadyStateGroup
                        }
                        RadioButton {
                            text: "Steady state"
                            exclusiveGroup: steadyStateGroup
                            checked: true
                        }
                        RadioButton {
                            id: nbIntake
                            text: "Nb. intake(s) of"
                            exclusiveGroup: steadyStateGroup
                        }
                        TextField {
                            readOnly: !(nbIntake.checked)
                            Layout.fillWidth: true
                            placeholderText : "intakes"
                        }
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "transparent"
                    }
                }
            }

            Rectangle {
                id: propositionsContainer
                Layout.fillWidth: true
                Layout.minimumHeight: 120
                Layout.maximumHeight: 120
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                ListView {
                    id: propositionsView
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 5
                    clip: true

                    model: propositionsModel
                    delegate: propositionsDelegate
                }
            }

            Rectangle {
                id: userPropositionContainer
                Layout.fillWidth: true
                Layout.minimumHeight: 55
                Layout.maximumHeight: 55
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        radius: 3
                        border.color: "black"
                        color: "white"

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 10

                            Text {
                                Layout.minimumWidth: contentWidth
                                Layout.maximumWidth: contentWidth
                                text: "Dose:"
                            }
                            TextField {
                                Layout.fillWidth: true
                                placeholderText : "dose"
                            }
                            ComboBox {
                                Layout.minimumWidth: 60
                                Layout.maximumWidth: 60
                                model: ["μg", "mg", "g"]
                                currentIndex: 1
                            }
                            Text {
                                Layout.minimumWidth: contentWidth
                                Layout.maximumWidth: contentWidth
                                text: "Interval:"
                            }
                            TextField {
                                Layout.fillWidth: true
                                placeholderText : "interval"
                            }
                            ComboBox {
                                Layout.minimumWidth: 60
                                Layout.maximumWidth: 60
                                model: ["min", "h", "days", "weeks"]
                                currentIndex: 1
                            }
                            CheckBox {}
                        }
                    }
                }
            }

            Rectangle {
                id: nextControlContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 70
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    RowLayout {
                        Text {
                            Layout.minimumWidth: 75
                            Layout.maximumWidth: 75
                            horizontalAlignment: Text.AlignRight
                            text: "Next control:"
                        }
                        DatePicker {
                            Layout.fillWidth: true
                        }
                    }

                    RowLayout {
                        Text {
                            Layout.minimumWidth: 75
                            Layout.maximumWidth: 75
                            horizontalAlignment: Text.AlignRight
                            text: "at:"
                        }
                        TimePicker {
                            Layout.fillWidth: true
                            hours: 12
                            minutes: 0
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"
            }
        }

        ListModel {
            id: propositionsModel
            ListElement { dose: 200; interval: 12; unit: "mg"; residualValue: 800.0; residualUnit: "mg/l" }
            ListElement { dose: 300; interval: 12; unit: "mg"; residualValue: 850.0; residualUnit: "mg/l" }
            ListElement { dose: 600; interval: 24; unit: "mg"; residualValue: 750.0; residualUnit: "mg/l" }
            ListElement { dose: 800; interval: 24; unit: "mg"; residualValue: 900.0; residualUnit: "mg/l" }
        }

        Component {
            id: propositionsDelegate

            Item {
                width: ListView.view.width
                height: 30

                Rectangle {
                    id: delegateContainer
                    anchors.fill: parent
                    radius: 3
                    border.color: "black"
                    color: "white"
                }

                RowLayout {
                    anchors.fill: delegateContainer
                    anchors.margins: 2
                    anchors.leftMargin: 10

                    Text {
                        Layout.fillWidth: true
                        text: model.dose + model.unit + " every " + model.interval + "h"
                    }
                    Text {
                        Layout.minimumWidth: contentWidth
                        Layout.maximumWidth: contentWidth
                        text: "Residual: " + model.residualValue + model.residualUnit
                        font.italic: true
                    }
                    CheckBox {
                        checked: index === 0
                    }
                }
            }
        }
    }
}
