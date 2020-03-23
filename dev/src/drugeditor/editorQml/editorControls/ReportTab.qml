import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4

import guiutils.qml.styles 1.0
import drugeditor.qml.controls 1.0

//import ezechiel 1.0

FlowTab {
    id: root

    property int status: Status.complete
    property var show: [1,1,1,1,1,1]
    property int pop: 0
    property int apr: 1
    property int apo: 2
    property int rev: 3
    property int mea: 4
    property int tar: 5

    signal currentStatusChanged(int status)
    signal publishReport(int output)
    signal reset()

    onStatusChanged: currentStatusChanged(status)

    leftPanel: Component {
        id:reportPanel

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
//                    onClicked: root.publishReport(InterpretationController.Pdf)
                    onClicked: interpretationController.publishReport()
                }
                RowLayout {
                    RadioButton {
                        text: "Population"
                        checked: true
                        onCheckedChanged: {
                            show[pop] = checked ? 1 : 0;
                            interpretationController.setReportTabShow(show);
                        }
                    }
                    RadioButton {
                        text: "Apriori"
                        checked: true
                        onCheckedChanged: {
                            show[apr] = checked;
                        }
                    }
                    RadioButton {
                        text: "Aposteriori"
                        checked: true
                        onCheckedChanged: {
                            show[apo] = checked;
                        }
                    }
                    RadioButton {
                        text: "Reverse"
                        checked: true
                        onCheckedChanged: {
                            show[rev] = checked;
                        }
                    }
                    RadioButton {
                        text: "Measures"
                        checked: true
                        onCheckedChanged: {
                            show[mea] = checked;
                        }
                    }
                    RadioButton {
                        text: "Targets"
                        checked: true
                        onCheckedChanged: {
                            show[tar] = checked;
                        }
                    }
                }

//                Button {
//                    Layout.fillWidth: true
//                    text: "Print report"
//                    onClicked: root.publishReport(InterpretationController.Printer)
//                }
//                Button {
//                    Layout.fillWidth: true
//                    text: "Send report"
//                    onClicked: root.publishReport(InterpretationController.Remote)
//                }
            }
        }
    }
}
