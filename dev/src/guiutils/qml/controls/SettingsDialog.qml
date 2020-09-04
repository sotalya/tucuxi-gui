import QtQuick 2.5
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

DialogBase {
    id: root
    title: "Application settings"
    width: 600
    height: 400

    property var self

    property var stdTreatment

    property var pathChanged

    function onUpdated(bApplied, bCreatingNewItem) {
        if (bApplied) {
            interpretation.analyst.person.firstname = analystFirstname.text
            interpretation.analyst.person.name = analystLastname.text
            interpretation.analyst.person.location.city = analystCity.text
            interpretation.analyst.person.location.postcode = analystPostcode.text
            interpretation.analyst.person.location.state = analystState.text
            interpretation.analyst.person.location.country = analystCountry.text
            interpretation.analyst.title = analystTitle.text
            //interpretation.analyst.institute.name = analystAffiliation.text
            //interpretation.analyst.person.setPhones()
            interpretation.analyst.saveToSettings()

            sentencesPalettes.filename = xmlPathETF.text
            sentencesPalettes.saveXMLPath()
        }
    }


    function init()
    {
        self = this
        loadAnalystFromSettings()
        pathChanged = false
    }

    function loadAnalystFromSettings()
    {
        var analyst = interpretation.analyst.loadSettings()

        analystLastname.text = analyst["person"]["name"].toString()
        analystFirstname.text = analyst["person"]["firstname"].toString()
        analystCity.text = analyst["person"]["location"]["city"].toString()
        analystPostcode.text = analyst["person"]["location"]["postcode"].toString()
        analystState.text = analyst["person"]["location"]["state"].toString()
        analystCountry.text = analyst["person"]["location"]["country"].toString()
        analystTitle.text = analyst["title"].toString()
        xmlPathETF.text = sentencesPalettes.loadXMLPath()

        //analystAffiliation.text = analyst["institute"]["name"].toString()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: bar
            width: parent.width
            TabButton {
                text: qsTr("Analyst")
            }

           TabButton {
                text: qsTr("Sentences")
            }

            TabButton {
                text: qsTr("Connectivity")
            }
            /*
            TabButton {
                text: qsTr("Activity")
            }
 */
        }

        StackLayout {
            width: parent.width
            currentIndex: bar.currentIndex
            Item {
                id: analystTab
                width: parent.width

                GridLayout {
                    anchors.fill: parent
                    rows: 3
                    columns: 3

                    Rectangle {
                        Layout.fillWidth:  true
                        Layout.fillHeight:  true
                    }
                    ColumnLayout {
                        Layout.row: 1
                        Layout.column:1
                        width: analystTab.width - 100
                        height: analystTab.height - 50

                        RowLayout {
                            //Layout.preferredHeight: analyst.rowHeight
                            //Layout.minimumHeight:   analyst.rowHeight
                            Layout.minimumWidth: 400
                            spacing: 2

                            EntityLabel {
                                text: "Title:"
                            }
                            EntityTextField {
                                id: analystTitle
                                placeholderText: "title"
                                Layout.maximumWidth: 40
                                onTextChanged: interpretation.analyst.title = text
                            }

                            Item{
                                Layout.fillWidth:  true
                            }
                        }

                        RowLayout {
                            //Layout.preferredHeight: analyst.rowHeight
                            //Layout.minimumHeight:   analyst.rowHeight
                            Layout.minimumWidth: 400
                            Layout.preferredWidth: parent.width
                            spacing: 2

                            EntityLabel {
                                text: "First name:"
                            }

                            EntityTextField {
                                id: analystFirstname
                                Layout.fillWidth:  true
                                placeholderText: "first name"
                            }
                            EntityLabel {
                                text: "Last name:"
                                Layout.preferredWidth: 100
                            }
                            EntityTextField {
                                id: analystLastname
                                placeholderText: "last name"
                                Layout.fillWidth:  true
                            }
                        }

                        RowLayout {
                            //Layout.preferredHeight: physicianInCharge.rowHeight
                            //Layout.minimumHeight:   physicianInCharge.rowHeight
                            Layout.minimumWidth: 400
                            spacing: 2

                            EntityLabel {
                                Layout.preferredWidth: 75
                                text: "Affiliation:"
                            }
                            EntityTextField {
                                id: analystAffiliation
                                Layout.fillWidth:  true
                                placeholderText: "affiliation"

                            }
                        }

                        RowLayout {
                            //Layout.preferredHeight: physicianInCharge.rowHeight
                            //Layout.minimumHeight:   physicianInCharge.rowHeight
                            Layout.minimumWidth: 400
                            spacing: 2

                            EntityLabel {
                                text: "City:"
                            }
                            EntityTextField {
                                id: analystCity
                                Layout.fillWidth:  true
                                placeholderText: "city"
                            }

                            EntityLabel {
                                text: "Postcode:"
                                Layout.preferredWidth: 100
                            }
                            EntityTextField {
                                id: analystPostcode
                                Layout.fillWidth:  true
                                placeholderText: "postcode"
                            }
                        }

                        RowLayout {
                            //Layout.preferredHeight: physicianInCharge.rowHeight
                            //Layout.minimumHeight:   physicianInCharge.rowHeight
                            spacing: 2

                            EntityLabel {
                                text: "State:"
                            }
                            EntityTextField {
                                id: analystState
                                Layout.fillWidth:  true
                                placeholderText: "state"
                            }

                            EntityLabel {
                                text: "Country:"
                                Layout.preferredWidth: 100
                            }
                            EntityTextField {
                                id: analystCountry
                                Layout.fillWidth:  true
                                placeholderText: "country"
                            }
                        }
                    }
                    Rectangle {
                        Layout.fillWidth:  true
                        Layout.fillHeight:  true
                    }
                }

            }

            Item {
                id: sentencesPalettesTab
                width: parent.width

                GridLayout {
                    anchors.fill: parent
                    rows: 3
                    columns: 3

                    Rectangle {
                        Layout.fillWidth:  true
                        Layout.fillHeight:  true
                    }
                    ColumnLayout{
                        Layout.row: 1
                        Layout.column:1
                        width: parent.width
                        height: parent.height

                        RowLayout {
                            width: parent.width
                            spacing: 2

                            EntityLabel {
                                Layout.preferredWidth: 180
                                text: "Sentences XML file saving path"
                            }
                            EntityTextField {
                                id: xmlPathETF
                                Layout.fillWidth:  true
                                onTextChanged: {
                                    pathChanged = true
                                }
                            }
                        }

                        Button{
                            Layout.alignment: Qt.AlignHCenter
                            text: "Import sentences"
                            onClicked: {
                                fileDialog.open();
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth:  true
                        Layout.fillHeight:  true
                    }
                }
            }

            Item {
                id: connectivityTab
                width: parent.width
                                }
            /*
            Item {
                id: activityTab
            }
            */
        }

        // Intercept Return to validate the dialog
        Shortcut {
            sequence: "Return"
            onActivated: {
                if (self.apply()) {
                    sentencesPalettes.filename = xmlPathETF.text
                    sentencesPalettes.exportToXml();
                    self.exit(true);
                }
            }
        }

        Rectangle{
            height: 20
        }


        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            // anchors.horizontalCenter: parent.horizontalCenter

            //TODO Implement ok and cancel button

            Item{
                Layout.fillWidth:  true
                //Layout.fillHeight:  true
            }
            Button {
                id: acceptBtn
                text: "Save and Close"
                onClicked: {
                    if (pathChanged){
                        if (sentencesPalettes.isPathExisting(xmlPathETF.text)){
                            messageDialog.open()
                        }
                    }
                    else{
                        sentencesPalettes.filename = xmlPathETF.text
                        sentencesPalettes.exportToXml();
                        root.exit(true);
                    }
                }
            }
            MessageDialog {
                id: messageDialog
                title: "Warning"
                text: "The path you want to use is already used. If you continue, the older file will be overwritten."
                standardButtons: StandardButton.Abort | StandardButton.Yes
                onAccepted: {
                    messageDialog.close()
                    sentencesPalettes.filename = xmlPathETF.text
                    sentencesPalettes.exportToXml();
                    root.exit(true);
                }

                onRejected: {
                    messageDialog.close()
                    sentencesPalettes.filename = xmlPathETF.text
                    sentencesPalettes.exportToXml();
                }

                Component.onCompleted: visible = false
            }
            Button {
                id: cancelBtn
                text: "Cancel"
                onClicked: function() {
                    root.exit(false);
                }
            }
            Item{
                Layout.fillWidth:  true
                //Layout.fillHeight:  true
            }
        }

        Rectangle{
            height: 20
        }
    }

    FileDialog {
        id: fileDialog
        title: "Import"
        folder: shortcuts.home
        modality: Qt.WindowModal
        selectExisting: true
        selectMultiple: false
        onAccepted: {
            sentencesPalettes.manualImport(fileDialog.fileUrls.toString().replace(/^(file:\/{2})/,""))
            fileDialog.close();
        }
        onRejected: {
            fileDialog.close();
        }
        Component.onCompleted: visible = false
    }

}
