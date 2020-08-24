import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    title: "Analyst informations"
    width: 600
    height: 400
    Layout.minimumHeight: 400

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
        self = this
    }

    function apply()
    {
        interpretation.analyst.saveToSettings()
        return true
    }

    //This has to be implemented in report tab
    function loadAnalystFromSettings()
    {
        var analyst = interpretation.analyst.loadSettings()

        interpretation.analyst.person.name = analyst["person"]["name"]
        interpretation.analyst.person.firstname = analyst["person"]["firstname"]
        interpretation.analyst.person.location.city = analyst["person"]["location"]["city"]
        interpretation.analyst.person.location.postcode = analyst["person"]["location"]["postcode"]
        interpretation.analyst.person.location.state = analyst["person"]["location"]["state"]
        interpretation.analyst.person.location.country = analyst["person"]["location"]["country"]
        interpretation.analyst.title = analyst["title"]
    }


    GridLayout {
        anchors.fill: parent
        rows: 3
        columns: 3

        Rectangle {
            Layout.row: 0
            Layout.column: 0
            Layout.fillWidth:  true
            Layout.fillHeight:  true
        }
        Rectangle {
            Layout.row: 2
            Layout.column: 2
            Layout.fillWidth:  true
            Layout.fillHeight:  true
        }
        ColumnLayout {
            Layout.row: 1
            Layout.column:1
            width: parent.parent.width - 100
            height: parent.parent.height - 50

            //Physician
            EntityView {
                id: analyst
                Layout.fillHeight: true
                Layout.fillWidth: true

                columnlayout.children: [

                    RowLayout {
                        Layout.preferredHeight: analyst.rowHeight
                        Layout.minimumHeight:   analyst.rowHeight
                        spacing: 2

                        EntityLabel {
                            text: "Title:"
                        }
                        EntityTextField {
                            id:titleET
                            placeholderText: "title"
                            Layout.maximumWidth: 40
                            onTextChanged: interpretation.analyst.title = text
                        }
                    }
                    ,
                    RowLayout {
                        Layout.preferredHeight: analyst.rowHeight
                        Layout.minimumHeight:   analyst.rowHeight
                        spacing: 2

                        EntityLabel {
                            text: "First name:"
                        }

                        EntityTextField {
                            Layout.fillWidth:  true
                            onTextChanged: interpretation.analyst.person.firstname = text
                            placeholderText: "first name"
                        }
                        EntityLabel {
                            text: "Last name:"
                            Layout.preferredWidth: 100
                        }
                        EntityTextField {
                            placeholderText: "last name"
                            onTextChanged: interpretation.analyst.person.name = text
                            Layout.fillWidth:  true
                        }
                    }
                    ,
                    RowLayout {
                        Layout.preferredHeight: physicianInCharge.rowHeight
                        Layout.minimumHeight:   physicianInCharge.rowHeight
                        spacing: 2

                        EntityLabel {
                            Layout.preferredWidth: 75
                            text: "Affiliation:"
                        }
                        EntityTextField {
                            Layout.fillWidth:  true
                            placeholderText: "affiliation"
                            onTextChanged: interpretation.analyst.institute = text

                        }
                    }
                    ,
                    RowLayout {
                        Layout.preferredHeight: physicianInCharge.rowHeight
                        Layout.minimumHeight:   physicianInCharge.rowHeight
                        spacing: 2

                        EntityLabel {
                            text: "City:"
                        }
                        EntityTextField {
                            Layout.fillWidth:  true
                            placeholderText: "city"
                            onTextChanged: interpretation.analyst.person.location.city = text
                        }

                        EntityLabel {
                            text: "Postcode:"
                            Layout.preferredWidth: 100
                        }
                        EntityTextField {
                            Layout.fillWidth:  true
                            placeholderText: "postcode"
                            onTextChanged: interpretation.analyst.person.location.postcode = text
                        }
                    }
                    ,
                    RowLayout {
                        Layout.preferredHeight: physicianInCharge.rowHeight
                        Layout.minimumHeight:   physicianInCharge.rowHeight
                        spacing: 2

                        EntityLabel {
                            text: "State:"
                        }
                        EntityTextField {
                            Layout.fillWidth:  true
                            placeholderText: "state"
                            onTextChanged: interpretation.analyst.person.location.state = text
                        }

                        EntityLabel {
                            text: "Country:"
                            Layout.preferredWidth: 100
                        }
                        EntityTextField {
                            Layout.fillWidth:  true
                            placeholderText: "country"
                            onTextChanged: interpretation.analyst.person.location.country = text
                        }
                    }
                ]
            }

            Rectangle{
                height: 20
            }


            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter

                //TODO Implement ok and cancel button

                Button {
                    id: acceptBtn
                    text: "Ok"
                    onClicked: function() {
//                        self.exit(true);
                    }
                }
                Rectangle{
                    Layout.preferredWidth: 125
                }

                Button {
                    id: cancelBtn
                    text: "Cancel"
                    onClicked: function() {
//                        self.exit(false);
                    }
                }
            }
        }
    }
}
