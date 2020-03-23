import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: root
    width: 800
    height: 600

    MouseArea {
        anchors.fill: parent
        onClicked: parent.forceActiveFocus()
    }

    ListView {
        id: covariatesView
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        width: parent.width / 2 - 20
        spacing: 5
        clip: true

        model: covariateListModel
        delegate: covariateDelegate
        footer: covariateFooter
    }

    Component {
        id: valueDoubleDelegate

        TextField {
            placeholderText : "value"
            text: modelData.value
            onEditingFinished: modelData.value = text
        }
    }

    Component {
        id: valueBoolDelegate

        ComboBox {
            id: valueBoolDelegateControl
            model: [ "No", "Yes" ]
            onActivated: modelData.value = index
            Binding { target: valueBoolDelegateControl; property: "currentIndex"; value: modelData.value }
        }
    }

    Component {
        id: covariateFooter

        Item {
            width: ListView.view.width
            height: 40

            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 5
                width: parent.width
                height: parent.height - 5
                radius: 10

                color: "LightSteelBlue"
                border.color: Qt.darker(color)

                Text {
                    anchors.centerIn: parent
                    font.bold: true
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                    text: "New covariate..."
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: covariateListModel.insertRows(covariateListModel.rowCount(), 1)
                }
            }
        }
    }

    Component {
        id: covariateDelegate

        Item {
            width: ListView.view.width
            height: 40

            MouseArea {
                anchors.fill: parent
                onClicked: parent.forceActiveFocus()
            }

            Rectangle {
                id: delegateContainer
                anchors.fill: parent
                radius: 10
                color: "LightSteelBlue"
                border.color: Qt.darker(color)
            }

            RowLayout {
                anchors.fill: delegateContainer
                anchors.margins: 5

                Text {
                    Layout.minimumWidth: contentWidth
                    Layout.preferredWidth: 50
                    Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                    text: model.name
                    font.bold: true
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                }
                Text {
                    Layout.minimumWidth: contentWidth
                    Layout.preferredWidth: 75
                    Layout.maximumWidth: Math.max(Layout.minimumWidth, Layout.preferredWidth)
                    text: "Value (in " + model.unit + "):"
                }
                Loader {
                    id: valueDelegateLoader
                    Layout.minimumWidth: 55
                    Layout.preferredWidth: 55
                    Layout.maximumWidth: 55

                    property var modelData: model

                    sourceComponent: {
                        switch(model.type) {
                        case "bool": return valueBoolDelegate;
                        default:     return valueDoubleDelegate;
                        }
                    }
                }
                Text {
                    Layout.minimumWidth: contentWidth
                    Layout.preferredWidth: contentWidth
                    Layout.maximumWidth: contentWidth
                    text: "Date:"
                }
                TextField {
                    Layout.fillWidth: true
                    placeholderText : "date"
                    text: Qt.formatDateTime(model.date)
                    onEditingFinished: model.date = Date.fromLocaleString(Qt.locale(), text, Qt.DefaultLocaleShortDate)
                }
                Rectangle {
                    Layout.minimumWidth: 18
                    Layout.maximumWidth: Layout.minimumWidth
                    Layout.minimumHeight: Layout.minimumWidth
                    Layout.maximumHeight: Layout.minimumWidth
                    radius: Layout.minimumWidth / 2
                    antialiasing: true
                    color: "white"
                    border.color: "black"
                    Image {
                        anchors.fill: parent
                        anchors.margins: 2
                        source: "qrc:/icons/remove.png"
                        fillMode: Image.PreserveAspectCrop
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: covariateListModel.removeRows(index, 1)
                    }
                }
            }
        }
    }
}
