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

    function applyToAnalyst(analyst) {
        analyst.person.firstname = analystFirstname.text
        analyst.person.name = analystLastname.text
        analyst.role = analystRole.text
        analyst.person.location.city = analystCity.text
        analyst.person.location.postcode = analystPostcode.text
        analyst.person.location.state = analystState.text
        analyst.person.location.country = analystCountry.text
        analyst.title = analystTitle.text
        analyst.institute.name = analystAffiliation.text
        analyst.person.setPrimaryPhone(analystPhone.text)
    }

    function onUpdated(bApplied, bCreatingNewItem) {
        if (bApplied) {
            applyToAnalyst(interpretation.analyst)

            applyToAnalyst(appGlobals.getAnalyst())
            appGlobals.saveAnalystSettings()

            sentencesPalettes.filename = xmlPathETF.text
            sentencesPalettes.exportToXml();
            sentencesPalettes.saveXMLPath()

            if (showProcessingTime.checkState == Qt.Checked)
                appGlobals.setShowProcessingTime(true);
            else
                appGlobals.setShowProcessingTime(false);
        }
    }

    function applyChanges()
    {
        if (pathChanged){
            if (sentencesPalettes.isPathExisting(xmlPathETF.text)){
                messageDialog.open()
            }
        }
        return true;
    }

    function init()
    {
        self = this
        loadAnalyst()
        if (appGlobals.showProcessingTime())
            showProcessingTime.checkState = Qt.Checked
        else
            showProcessingTime.checkState = Qt.Unchecked

        pathChanged = false
    }

    function loadAnalyst()
    {
        var analyst = interpretation.analyst
        analystLastname.text = analyst.person.name
        analystFirstname.text = analyst.person.firstname

        analystCity.text = analyst.person.location.city
        analystPostcode.text = analyst.person.location.postcode
        analystState.text = analyst.person.location.state
        analystCountry.text = analyst.person.location.country
        analystTitle.text = analyst.title
        analystRole.text = analyst.role
        if (analyst.person.phones.size() > 0)
            analystPhone.text = analyst.person.phones.at(0).number
        if (analyst.institute)
            analystAffiliation.text = analyst.institute.name

        xmlPathETF.text = sentencesPalettes.loadXMLPath()

    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: bar
            width: parent.width
            TabButton {
                text: qsTr("Analyst")
                width: implicitWidth
            }

            TabButton {
                 text: qsTr("General")
                 width: implicitWidth
             }

           TabButton {
                text: qsTr("Sentences")
                width: implicitWidth
            }

            TabButton {
                text: qsTr("Connectivity")
                width: implicitWidth
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
                                text: "Role:"
                            }
                            EntityTextField {
                                id: analystRole
                                Layout.fillWidth:  true
                                placeholderText: "role"
                            }

                            EntityLabel {
                                text: "Phone number:"
                            }
                            EntityTextField {
                                id: analystPhone
                                Layout.fillWidth:  true
                                placeholderText: "phone"
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
                id: generalTab
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
                            CheckBox {
                                id: showProcessingTime
                                text: "Show interpretation processing time when you validate?"
                                checked: true
                                enabled: !appMode.isDemo()

                                //ToolTip.visible: (show_tooltip) ? steadyStateMousearea.containsMouse : false
                                //ToolTip.text: ToolTips.dosageDialog.steadyState

                                MouseArea {
                                    id: steadyStateMousearea
                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom
                                    anchors.right: parent.right
                                    width: parent.width - 30
                                    hoverEnabled: true
                                }
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

            Item{
                Layout.fillWidth:  true
            }
            Button {
                id: acceptBtn
                text: "Ok"
                onClicked: {
                    if (pathChanged){
                        if (sentencesPalettes.isPathExisting(xmlPathETF.text)){
                            messageDialog.open()
                        }
                    }
                    else{
                        self.exit(true);
                    }
                }
            }

            Button {
                id: applyBtn
                text: "Apply"
                onClicked: function() {
                    if (pathChanged){
                        if (sentencesPalettes.isPathExisting(xmlPathETF.text)){
                            messageDialog.open()
                        }
                    }
                }
            }

            Button {
                id: cancelBtn
                text: "Cancel"
                onClicked: function() {
                    self.exit(false);
                }
            }

            Item{
                Layout.fillWidth:  true
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
                    pathChanged = false;
                }

                onRejected: {
                    messageDialog.close()
                }

                Component.onCompleted: visible = false
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
        nameFilters: "*.xml"
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
