import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    title: "Validation text"
    width: 600
    height: 400

    property var self

    property var stdTreatment

    // Intercept Return to validate the dialog
    Shortcut {
        sequence: "Return"
        onActivated: {
            if (self.apply()) {
                self.exit(true);
            }
        }
    }

    function init()
    {

    }

    function extValidateDialog(tct){
        validationDialog.exit();
    }

    Rectangle {
        Layout.fillWidth:  true
        Layout.minimumHeight:   analysis.rowHeight * 1
        border.width: 1
        border.color: "#cccccc"

        Flickable{
            id: flickable
            anchors.fill: parent

            TextArea.flickable: TextArea {
                id: expectednessInput
                placeholderText: "Please fill in"
                clip: true
                anchors.margins:1
                wrapMode: TextArea.Wrap
                text: analysisFrame.model.expectedness
                onTextChanged: analysisFrame.model.expectedness = text
                font.family:    analysis.inputFontFamily
                font.pixelSize: analysis.inputFontSize
                verticalAlignment:   analysis.inputVAlign
                horizontalAlignment: analysis.inputHAlign

                EntityToolTip {
                    tooltipText: ToolTips.validationTab.expectedness
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        validationDialog.exec()
                    }
                }
            }


            ScrollBar.vertical: ScrollBar {}
        }
    }
}
