import QtQuick
import Qt5Compat.GraphicalEffects
import ezechiel

Rectangle {
    id: container
    width: (Math.ceil(Math.sqrt(menuModel.count)) * menuView.cellWidth) + (menuView.anchors.margins * 2);
    height: (Math.ceil(Math.sqrt(menuModel.count)) * menuView.cellHeight) + (menuView.anchors.margins * 2);

    signal optionSelected(int option);

    border.color: Qt.darker("lightsteelblue", 1.2)
    border.width: 2

    gradient: Gradient {
        GradientStop { position: 0.0; color: "white" }
        GradientStop { position: 1.0; color: "lightsteelblue" }
    }

    GridView {
        id: menuView
        anchors.fill: container
        anchors.margins: 20

        cellWidth: 150
        cellHeight: 150
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        model: menuModel
        delegate: menuDelegate
    }

    ListModel {
        id: menuModel

        ListElement {
            name: "EXISTING"
            icon: "qrc:/icons/toolbar/folder.png"
            option: StartupWindow.PendingRequests
        }
        ListElement {
            name: "REMOTE"
            icon: "qrc:/icons/toolbar/requests.png"
            option: StartupWindow.PendingRequestGivenPatient
        }
        ListElement {
            name: "NEW"
            icon: "qrc:/icons/toolbar/folder_patient_add.png"
            option: StartupWindow.NewPatient
        }
        ListElement {
            name: "SHUTDOWN"
            icon: "qrc:/icons/toolbar/shutdown.png"
            option: StartupWindow.Shutdown
        }
    }

    Component {
        id: menuDelegate

        Item {
            width: menuView.cellWidth
            height: menuView.cellHeight

            Rectangle {
                id: menuDelegateBox
                anchors.centerIn: parent
                width: parent.width - 25
                height: parent.height - 25
                visible: false

                color: menuDelegateMouseArea.containsMouse ? "#dbe4f0" : "#edf2f7"
                radius: 10
                border.color: Qt.darker(color, 1.5)
            }

            DropShadow {
                id: menuDelegateBoxShadow
                anchors.fill: menuDelegateBox
                horizontalOffset: 3
                verticalOffset: 3
                radius: 2
                samples: 8
                color: "#80000000"
                source: menuDelegateBox
            }

            MouseArea {
                id: menuDelegateMouseArea
                anchors.fill: menuDelegateBox
                hoverEnabled: true

                onClicked: container.optionSelected(option)

                onPressed: {
                    menuDelegateBoxShadow.visible = false
                    menuDelegateBox.visible = true;
                }

                onReleased: {
                    menuDelegateBoxShadow.visible = true
                    menuDelegateBox.visible = false;
                }
            }

            Image {
                id: image
                anchors.horizontalCenter: menuDelegateBox.horizontalCenter
                anchors.top: menuDelegateBox.top
                anchors.topMargin: menuDelegateBox.height * 0.15
                width: menuDelegateBox.width / 2
                height: menuDelegateBox.height / 2

                source: icon
                fillMode: Image.PreserveAspectFit
            }

            Text {
                anchors.horizontalCenter: image.horizontalCenter
                anchors.top: image.bottom
                anchors.topMargin: 10
                text: name
                font.family: "Karmakooma"
                fontSizeMode: Text.Fit
                minimumPixelSize: 10
                font.pixelSize: 10
            }
        }
    }
}
