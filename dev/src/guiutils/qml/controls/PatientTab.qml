import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {
    id: patientPanel
    property int index:  -1
    property int status: Status.invalid
    property bool enabled: false;

    //ToDo: the way this updated is a mess, find a better solution, either in QML, or through the interpretation controler
    property string patientName:      ""
    property string patientBirthDate: ""
    // property int patientGender:       0

    signal imUpdated();
    signal currentIndexChanged(int index)
    signal reset();

    function clearSelection() {
        patientListView.clearSelection();
    }

    function extPatientData(yearPat, monthPat, dayPat, firstName, lastName, identifier, stayNumber) {
        patientExtraExpandHeader.expanded = true
        physicianInChargeHeader.expanded = true

        patientRequired.model.person.birthday = (new Date(yearPat.toString(), monthPat.toString(), dayPat.toString()));
//        patientRequired.model.person.birthday = new Date("1912", "11", "01");         // for manual testing only
        patientFirstNameId.text     = firstName
        patientLastNameId.text      = lastName
        patientIdentifierId.text    = identifier
        patientStayNumberId.text    = stayNumber
    }

    function extPatientContactData(address, city, state, postcode, country){
        patientAddressId.text   = address
        patientCityId.text      = city
        patientStateId.text     = state
        patientPostcodeId.text  = postcode
        patientCountryId.text   = country
    }

    function extPhysicianData(titlePhy, firstNamePhy, lastNamePhy, affiliationPhy, addressPhy, cityPhy, statePhy, postcodePhy, countryPhy){
        titlePhyId.text             = titlePhy
        firstNamePhyId.text         = firstNamePhy
        lastNamePhyId.text          = lastNamePhy
        affiliationPhyId.text       = affiliationPhy
        addressPhyId.text           = addressPhy
        cityPhyId.text              = cityPhy
        statePhyId.text             = statePhy
        postcodePhyId.text          = postcodePhy
        countryPhyId.text           = countryPhy
    }


    onIndexChanged:  currentIndexChanged(index)

    anchors.fill: parent
    color: Style.table.background.table

    MessageDialog {
        id: messageDialog
        title: "Demo version"
        text: "Adding or removing a new patient is not possible in the demo version"
        standardButtons: StandardButton.Close
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        EntityTitle {
            label.text: "Patients List"
            id: patientsListExpandHeader
            visible : false
        }

        //PatientList
        EntityList {
            id: patientList

            visible : false
            //visible : patientsListExpandHeader.expanded

            columnlayout.children: [
                EntityListHeader {

                    // Here the preferredWidth values should be identical
                    // for the headers and items

                    EntityListHeaderItem {
                        Layout.preferredWidth: 50
                        label.text: "ID"
                    }

                    EntityListHeaderItem {
                        Layout.preferredWidth: 150
                        label.text: "First Name"
                    }
                    EntityListHeaderItem {
                        Layout.preferredWidth: 150
                        label.text: "Last Name"
                    }
                    EntityListHeaderItem {
                        Layout.fillWidth: true
                        label.text: "Birth Date"
                    }
                    EntityListHeaderImage {
                        image.source: "qrc:/icons/buttons/add.png"
                        mousearea.onClicked: {
                            // messageDialog.open()
                            interpretationController.addPatient();
                            patientListView.currentIndex = patientListView.count - 1
                            //                        patientListView.add();
                            // imUpdated();
                        }
                    }
                },

                EntityHeaderEnd {
                    visible: true
                },

                //PatientList view
                EntityListView {
                    id: patientListView
                    property var patients: interpretationController.patients;
                    onPatientsChanged: {
                        //                    console.log("# patients: " + interpretationController.patients.size());
                    }

                    model: patients.objlist//patientListModel
                    onModelChanged: {
                        //                    console.log("# patients: " + interpretationController.patients.size());
                        //                    for (var i = 0; i < patients.size(); ++i) {
                        //                        console.log("patient " + i + ": " + patients.at(i).person.firstname);
                        //                        console.log("patient " + i + ": " + patients.at(i).person.name);
                        //                    }
                    }

                    //PatientList delegate
                    delegate: EntityListDelegate {
                        id: root
                        property var selected_model: model.modelData
                        mousearea.onClicked: root.ListView.view.currentIndex = index

                        ToolTip.visible: (show_tooltip) ? (mousearea.containsMouse) : false
                        ToolTip.text: ToolTips.patientTab.patientList

                        rowlayout.children: [
                            EntityListDelegateItem {
                                Layout.preferredWidth: 50
                                color: root.mousearea.hovered ? patientList.rowBackgroundHover : (root.ListView.isCurrentItem ? patientList.rowBackgroundSelected : patientList.rowBackground)
                                label.text: modelData.externalId
                                label.color: root.mousearea.hovered ? patientList.rowForegroundHover : (root.ListView.isCurrentItem ? patientList.rowForegroundSelected : patientList.rowForeground)
                            },

                            EntityListDelegateItem {
                                Layout.preferredWidth: 150
                                color: root.mousearea.hovered ? patientList.rowBackgroundHover : (root.ListView.isCurrentItem ? patientList.rowBackgroundSelected : patientList.rowBackground)
                                label.text: modelData.person.firstname
                                label.color: root.mousearea.hovered ? patientList.rowForegroundHover : (root.ListView.isCurrentItem ? patientList.rowForegroundSelected : patientList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.preferredWidth: 150
                                color: root.mousearea.hovered ? patientList.rowBackgroundHover : (root.ListView.isCurrentItem ? patientList.rowBackgroundSelected : patientList.rowBackground)
                                label.text: modelData.person.name
                                label.color: root.mousearea.hovered ? patientList.rowForegroundHover : (root.ListView.isCurrentItem ? patientList.rowForegroundSelected : patientList.rowForeground)
                            },
                            EntityListDelegateItem {
                                Layout.fillWidth: true
                                color: root.mousearea.hovered ? patientList.rowBackgroundHover : (root.ListView.isCurrentItem ? patientList.rowBackgroundSelected : patientList.rowBackground)
                                label.text: Qt.formatDate(modelData.person.birthday)
                                label.color: root.mousearea.hovered ? patientList.rowForegroundHover : (root.ListView.isCurrentItem ? patientList.rowForegroundSelected : patientList.rowForeground)
                            },
                            EntityListImage {
                                image.source: "qrc:/icons/buttons/remove.png"
                                mousearea.onClicked: {
                                //    messageDialog.open()
                                    interpretationController.removePatient(index);
                                    // patientListView.patients.remove(index);
                                    // imUpdated();
                                }
                            }
                        ]
                    }

                    onCurrentIndexChanged: index = currentIndex
                    Component.onCompleted: reset.connect(clearSelection)

                    function clearSelection() {
                        currentIndex = -1;
                        currentIndex = 0;
                    }
                }


            ]
        }

        EntityBlockEnd {
            visible : false
            //visible : patientsListExpandHeader.expanded && (patientRequired.model ? 1 : 0)
        }

        EntityTitle {
            label.text: "Patient Required Information"
            id: patientRequiredExpandHeader
            visible: patientRequired.model ? 1 : 0
        }

        //Patient
        EntityView {
            //            visible : patientRequiredExpandHeader.expanded
            id: patientRequired
            Layout.preferredHeight: 100
            Layout.minimumHeight:   50

            property var model: patientListView.currentItem? patientListView.currentItem.selected_model : 0
            onModelChanged: {
                // Update the gender field
                if (patientRequired.model) {
                    if (patientRequired.model.person.gender == 1)
                        maleRadioInput.checked = true;
                    else
                        femaleRadioInput.checked = true;
                }
            }

            visible: (patientListView.count > 0) && patientRequiredExpandHeader.expanded && (patientRequired.model ? 1 : 0);
            columnlayout.children: [
                RowLayout {
                    Layout.preferredHeight: patientRequired.rowHeight
                    Layout.minimumHeight:   patientRequired.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "First name:"
                    }

                    EntityTextField {
                        id: patientFirstNameId
                        objectName: "patientFirstName"
                        Layout.fillWidth:  true
                        text: patientRequired.model ? patientRequired.model.person.firstname : ""
                        onTextChanged: {
                            if (!patientRequired.model) {return;}
                            patientRequired.model.person.firstname = text
                        }
                        placeholderText: "first name"
                    }
                    EntityLabel {
                        text: "Last name:"
                        Layout.preferredWidth: 100
                    }
                    EntityTextField {
                        id: patientLastNameId
                        objectName: "patientLastName"
                        placeholderText: "last name"
                        text: patientRequired.model ? patientRequired.model.person.name : ""
                        onTextChanged: {
                            if (!patientRequired.model) {return;}
                            patientRequired.model.person.name = text
                        }
                        Layout.fillWidth:  true
                    }
                }
                ,
                RowLayout {
                    Layout.preferredHeight: patientRequired.rowHeight
                    Layout.minimumHeight:   patientRequired.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "Birth date:"
                        tooltipText : ToolTips.patientTab.birthdate
                    }
                    DatePicker {
                        id: birthDateInput
                        objectName: "birthDateInput"
                        Layout.fillWidth: true
                        Binding { target: birthDateInput; property: "date"; value: patientRequired.model ? patientRequired.model.person.birthday : new Date() }
                        onEditingFinished: {
                            patientRequired.model.person.birthday = new Date(date.getFullYear(), date.getMonth(), date.getDate())
                            interpretationController.updateSexAndAgeCovariates()
                        }
                    }

                    EntityLabel {
                        text: "Gender:"
                        Layout.preferredWidth: 100
                        tooltipText: ToolTips.patientTab.gender
                    }
//                    ExclusiveGroup {
//                        id: genderGroup
//                    }
                    RadioButton {
                        id: maleRadioInput
                        objectName: "maleRadioInput"
                        text: "Male"
                        //exclusiveGroup: genderGroup
                        //Binding { target: maleRadioInput; property: "checked"; value: patientRequired.model ? (patientRequired.model.person.gender === 0 ? true : false) : false}
                        onClicked: {
                            patientRequired.model.person.gender = 1
                            interpretationController.updateSexAndAgeCovariates()
                        }

/*
                        style: RadioButtonStyle {
                            label: Text {
                                text: control.text
                                elide: Text.ElideRight
                                color: patientRequired.labelForeground
                                font.family:    patientRequired.labelFontFamily
                                font.pixelSize: patientRequired.labelFontSize
                                font.bold: patientRequired.labelFontBold
                            }
                        }
*/                    }
                    RadioButton {
                        id: femaleRadioInput
                        objectName: "femaleRadioInput"
                        text: "Female"
                        //exclusiveGroup: genderGroup
                        //Binding { target: femaleRadioInput; property: "checked"; value: patientRequired.model ? (patientRequired.model.person.gender === 1 ? true : false) : false }
                        onClicked: {
                            patientRequired.model.person.gender = 0
                            interpretationController.updateSexAndAgeCovariates()
                        }

                        /*
                        style: RadioButtonStyle {
                            label: Text {
                                text: control.text
                                elide: Text.ElideRight
                                color: patientRequired.labelForeground
                                font.family:    patientRequired.labelFontFamily
                                font.pixelSize: patientRequired.labelFontSize
                                font.bold: patientRequired.labelFontBold
                            }
                        }
*/                    }
                }
                ,
                RowLayout {} //Spacer
            ]
        }


        EntityBlockEnd {
            visible : patientRequiredExpandHeader.expanded && (patientRequired.model ? 1 : 0)
        }

        /*
        EntityBlock {
            id: ene
            width: parent.width
            Layout.minimumWidth: parent.width
            label.text: "Test";

                Rectangle{
                    id: stack
                    width: 200
                    height: 40
//                    Layout.minimumHeight: 40

                    Text {
                        text : "salut"
                    }
                }
        }
*/

        EntityTitle {
            label.text: "Patient Extra Informations"
            id: patientExtraExpandHeader
            expanded: false  // modified in extPatientData for GUI-tests    JRT 21.03.2022
            visible : patientRequired.model ? 1 : 0
        }

        //Patient
        EntityView {
            id: patientExtra
            Layout.preferredHeight: 180
            Layout.minimumHeight:   100
            property var model: patientListView.currentItem? patientListView.currentItem.selected_model : 0

            visible: (patientListView.count > 0) && patientExtraExpandHeader.expanded && (patientRequired.model ? 1 : 0);

            columnlayout.children: [

                RowLayout {
                    Layout.preferredHeight: patientExtra.rowHeight
                    Layout.minimumHeight:   patientExtra.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "Identifier:"
                    }
                    EntityTextField {
                        id: patientIdentifierId
                        objectName: "patientIdentifier"
                        placeholderText: "id"
                        text: patientExtra.model ? patientExtra.model.externalId: ""
                        onTextChanged: patientExtra.model.externalId = text
                        Layout.fillWidth:  true
                    }

                    EntityLabel {
                        text: "Stay number:"
                        Layout.preferredWidth: 100
                    }
                    EntityTextField {
                        id: patientStayNumberId
                        objectName: "patientStayNumber"
                        placeholderText: "stay number"
                        text: patientExtra.model ? patientExtra.model.stayNumber : ""
                        onTextChanged: patientExtra.model.stayNumber = text
                        Layout.fillWidth:  true
                    }
                }
                ,
                RowLayout {
                    Layout.preferredHeight: patientExtra.rowHeight
                    Layout.minimumHeight:   patientExtra.rowHeight
                    spacing: 2

                    EntityLabel {
                        Layout.preferredWidth: 75
                        text: "Address:"
                    }
                    EntityTextField {
                        id: patientAddressId
                        Layout.fillWidth:  true
                        placeholderText: "address"
                        text: patientExtra.model ? patientExtra.model.person.location.address : ""
                        onTextChanged: {
                            if (!patientExtra.model) {return;}
                            patientExtra.model.person.location.address = text;
                        }
                    }
                }
                ,
                RowLayout {
                    Layout.preferredHeight: patientExtra.rowHeight
                    Layout.minimumHeight:   patientExtra.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "City:"
                    }
                    EntityTextField {
                        id: patientCityId
                        Layout.fillWidth:  true
                        placeholderText: "city"
                        text: patientExtra.model ? patientExtra.model.person.location.city : ""
                        onTextChanged: {
                            if (!patientExtra.model) {return;}
                            patientExtra.model.person.location.city = text
                        }
                    }

                    EntityLabel {
                        text: "Postcode:"
                        Layout.preferredWidth: 100
                    }
                    EntityTextField {
                        id: patientPostcodeId
                        Layout.fillWidth:  true
                        placeholderText: "postcode"
                        text: patientExtra.model ? patientExtra.model.person.location.postcode : ""
                        onTextChanged: {
                            if (!patientExtra.model) {return;}
                            patientExtra.model.person.location.postcode = text
                        }
                    }
                }
                ,
                RowLayout {
                    Layout.preferredHeight: patientExtra.rowHeight
                    Layout.minimumHeight:   patientExtra.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "State:"
                    }
                    EntityTextField {
                        id: patientStateId
                        Layout.fillWidth:  true
                        placeholderText: "state"
                        text: patientExtra.model ? patientExtra.model.person.location.state : ""
                        onTextChanged: {
                            if (!patientExtra.model) {return;}
                            patientExtra.model.person.location.state = text
                        }
                    }

                    EntityLabel {
                        text: "Country:"
                        Layout.preferredWidth: 100
                    }
                    EntityTextField {
                        id: patientCountryId
                        Layout.fillWidth:  true
                        placeholderText: "country"
                        text: patientExtra.model ? patientExtra.model.person.location.country : ""
                        onTextChanged: {
                            if (!patientExtra.model) {return;}
                            patientExtra.model.person.location.country = text
                        }
                    }
                }
                ,
                RowLayout {} //Spacer
            ]
        }


        EntityBlockEnd {
            visible : patientExtraExpandHeader.expanded && (patientRequired.model ? 1 : 0)
        }

        EntityTitle {
            label.text: "Physician in charge"
            id: physicianInChargeHeader
            expanded: false         // modified in extPatientData       JRT 21.03.2022
            visible : patientRequired.model ? 1 : 0
        }

        //Physician
        EntityView {
            id: physicianInCharge
            Layout.preferredHeight: 260
            Layout.minimumHeight:   150
            property var model: patientListView.currentItem? patientListView.currentItem.selected_model : 0

            visible: (patientListView.count > 0) && physicianInChargeHeader.expanded && (patientRequired.model ? 1 : 0);

            columnlayout.children: [

                RowLayout {
                    Layout.preferredHeight: physicianInCharge.rowHeight
                    Layout.minimumHeight:   physicianInCharge.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "Title:"
                    }
                    EntityTextField {
                        id: titlePhyId
                        objectName: "titlePhy"
                        placeholderText: "title"
                        text: interpretation.request.practician ? interpretation.request.practician.title : ""
                        onTextChanged: interpretation.request.practician.title = text
                        Layout.fillWidth:  true
                    }
                }
                ,
                RowLayout {
                    Layout.preferredHeight: physicianInCharge.rowHeight
                    Layout.minimumHeight:   physicianInCharge.rowHeight
                    spacing: 2

                    EntityLabel {
                        text: "First name:"
                    }

                    EntityTextField {
                        id: firstNamePhyId
                        objectName: "firstNamePhy"
                        Layout.fillWidth:  true
                        text: interpretation.request.practician ? interpretation.request.practician.person.firstname : ""
                        onTextChanged: {
                            if (!interpretation.request.practician) {return;}
                            interpretation.request.practician.person.firstname = text
                        }
                        placeholderText: "first name"
                    }
                    EntityLabel {
                        text: "Last name:"
                        Layout.preferredWidth: 100
                    }
                    EntityTextField {
                        id: lastNamePhyId
                        objectName: "lastNamePhy"
                        placeholderText: "last name"
                        text: interpretation.request.practician ? interpretation.request.practician.person.name : ""
                        onTextChanged: {
                            if (!interpretation.request.practician) {return;}
                            interpretation.request.practician.person.name = text
                        }
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
                        id: affiliationPhyId
                        Layout.fillWidth:  true
                        placeholderText: "affiliation"
                        text: interpretation.request.practician ? interpretation.request.practician.institute.name : ""
                        onTextChanged: {
                            if (!patientExtra.model) {return;}
                            interpretation.request.practician.institute.name = text;
                        }
                    }
                }
                ,
                RowLayout {
                    Layout.preferredHeight: physicianInCharge.rowHeight
                    Layout.minimumHeight:   physicianInCharge.rowHeight
                    spacing: 2

                    EntityLabel {
                        Layout.preferredWidth: 75
                        text: "Address:"
                    }
                    EntityTextField {
                        id: addressPhyId
                        Layout.fillWidth:  true
                        placeholderText: "address"
                        text: interpretation.request.practician ? interpretation.request.practician.institute.location.address : ""
                        onTextChanged: {
                            if (!patientExtra.model) {return;}
                            interpretation.request.practician.institute.location.address = text;
                        }
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
                        id: cityPhyId
                        Layout.fillWidth:  true
                        placeholderText: "city"
                        text: interpretation.request.practician ? interpretation.request.practician.person.location.city : ""
                        onTextChanged: {
                            if (!interpretation.request.practician) {return;}
                            interpretation.request.practician.person.location.city = text
                        }
                    }

                    EntityLabel {
                        text: "Postcode:"
                        Layout.preferredWidth: 100
                    }
                    EntityTextField {
                        id: postcodePhyId
                        Layout.fillWidth:  true
                        placeholderText: "postcode"
                        text: interpretation.request.practician ? interpretation.request.practician.person.location.postcode : ""
                        onTextChanged: {
                            if (!interpretation.request.practician) {return;}
                            interpretation.request.practician.person.location.postcode = text
                        }
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
                        id: statePhyId
                        Layout.fillWidth:  true
                        placeholderText: "state"
                        text: interpretation.request.practician ? interpretation.request.practician.person.location.state : ""
                        onTextChanged: {
                            if (!interpretation.request.practician) {return;}
                            interpretation.request.practician.person.location.state = text
                        }
                    }

                    EntityLabel {
                        text: "Country:"
                        Layout.preferredWidth: 100
                    }
                    EntityTextField {
                        id: countryPhyId
                        Layout.fillWidth:  true
                        placeholderText: "country"
                        text: interpretation.request.practician ? interpretation.request.practician.person.location.country : ""
                        onTextChanged: {
                            if (!interpretation.request.practician) {return;}
                            interpretation.request.practician.person.location.country = text
                        }
                    }
                }
                ,
                RowLayout {} //Spacer
            ]
        }

        EntityBlockEnd {
            visible : physicianInChargeHeader.expanded && (patientRequired.model ? 1 : 0)
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
