import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

import ezechiel 1.0

FlowTab {
    id: root

    property int status: Status.complete

    signal currentStatusChanged(int status)
    signal publishReport(int output)
    signal reset()

    onStatusChanged: currentStatusChanged(status)

    leftPanel: Component {
        id: validationPanel

        Rectangle {
            anchors.fill: parent
            color: Style.form.background.frame

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5

                //ToDo: custom look for the buttons, see with Sandrine
                //ToDo: later on, the right panel could display the report

                Button {
                    Layout.fillWidth: true
                    text: "Generate report"
                    onClicked: root.publishReport(InterpretationController.Pdf)
                }
                Button {
                    Layout.fillWidth: true
                    text: "Print report"
                    onClicked: root.publishReport(InterpretationController.Printer)
                }
                Button {
                    Layout.fillWidth: true
                    text: "Send report"
                    onClicked: root.publishReport(InterpretationController.Remote)
                }
                RowLayout {}
            }
        }
    }
}
