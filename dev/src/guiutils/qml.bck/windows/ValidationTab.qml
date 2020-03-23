import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

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
                Layout.fillWidth: true
                Layout.minimumHeight: 85
                Layout.maximumHeight: 85
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Text {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
                        Layout.alignment: Qt.AlignTop
                        horizontalAlignment: Text.AlignRight
                        text: "Expectedness:"
                    }
                    TextArea {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 85
                Layout.maximumHeight: 85
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Text {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
                        Layout.alignment: Qt.AlignTop
                        horizontalAlignment: Text.AlignRight
                        text: "Suitability:"
                    }
                    TextArea {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 85
                Layout.maximumHeight: 85
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Text {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
                        Layout.alignment: Qt.AlignTop
                        horizontalAlignment: Text.AlignRight
                        text: "Prediction:"
                    }
                    TextArea {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 85
                Layout.maximumHeight: 85
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Text {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
                        Layout.alignment: Qt.AlignTop
                        horizontalAlignment: Text.AlignRight
                        text: "Remonitoring:"
                    }
                    TextArea {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 85
                Layout.maximumHeight: 85
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Text {
                        Layout.minimumWidth: 100
                        Layout.maximumWidth: 100
                        Layout.alignment: Qt.AlignTop
                        horizontalAlignment: Text.AlignRight
                        text: "Warning:"
                    }
                    TextArea {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 75
                Layout.maximumHeight: 75
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 2

                    RowLayout {
                        Text {
                            Layout.minimumWidth: 100
                            Layout.maximumWidth: 100
                            horizontalAlignment: Text.AlignRight
                            text: "Interpreted by:"
                        }
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["Manel Aouri", "Thierry Buclin", "Pascal André"]
                        }
                    }
                    RowLayout {
                        Text {
                            Layout.minimumWidth: 100
                            Layout.maximumWidth: 100
                            horizontalAlignment: Text.AlignRight
                            text: "Validated by:"
                        }
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["Manel Aouri", "Thierry Buclin", "Pascal André"]
                        }
                    }
                    RowLayout {}
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 65
                Layout.maximumHeight: 65
                border.color: Qt.darker(color)
                color: "lightsteelblue"
                radius: 3

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 2

                    CheckBox { text: "Send to medical database" }
                    CheckBox { text: "Generate a report" }
                    RowLayout {}
                }
            }

            Button {
                Layout.fillWidth: true
                text: "Validate"
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"
            }
        }
    }
}
