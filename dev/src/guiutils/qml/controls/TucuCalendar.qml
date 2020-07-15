import QtQuick 2.5
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4

Window {
    visible: false
    modality: Qt.ApplicationModal
    title: "Calendar"
    flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint | Qt.MSWindowsFixedSizeDialogHint
    width: 300
    height: 250

    signal accepted(var date)

    function open(date) {
        qtCalendarControl.selectedDate = date
        showNormal()
    }

    ColumnLayout {
        anchors.fill: parent

        Calendar {
            id: qtCalendarControl
            Layout.fillWidth:  true
            Layout.fillHeight:  true
            onDoubleClicked: {
                accepted(selectedDate)
                close()
            }
        }

        RowLayout {
            Layout.fillWidth:  true
            Layout.fillHeight:  true
            spacing: 10

            Button {
                Layout.fillWidth:  true
                text: "Ok"
                onClicked: {
                    accepted(qtCalendarControl.selectedDate)
                    close()
                }
            }
            Button {
                Layout.fillWidth:  true
                text: "Cancel"
                onClicked: {
                    close()
                }
            }
        }
    }
}