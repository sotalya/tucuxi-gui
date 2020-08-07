import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

Rectangle {
    id: validationPanel
    property int status: Status.invalid
    property bool enabled: false;

    signal reset()    

    function toggleShow(index, value)
    {
        graphInformationSelection.setVisible(index, value);
        return 1;
    }
	
    //property var show: [1,1,1,1,1,1,1,1,1,1]
    property int pop: 0
    property int pop_perc: 1
    property int apr: 2
    property int apr_perc: 3
    property int apo: 4
    property int apo_perc: 5
    property int mea: 6
    property int tar: 7
    property int rev: 8 // list of possible adjustments
    property int sad: 9 // the selected adjustment

    Connections {
        target: interpretation.drugResponseAnalysis.treatment.adjustments
        onObjListChanged: {
            var adjs = []
            var nextControl = 0;
            var lst = interpretation.drugResponseAnalysis.treatment.adjustments;
            for (var i=0; i<lst.size(); i++) {
                var dosage = lst.objat(i);
                if (dosage.endtime > nextControl) nextControl = dosage.endtime;

                var route = "";
                switch (dosage.route.route) {
                    case 0:  route = "(bolus)"; break;
                    case 1:  route = "(infusion of " +  dosage.dbtinf + "min)"; break;
                    case 2:  route = "(extravascular)"; break;
                    default: break;
                }

                adjs.push("From " + dosage.applied.toLocaleDateString(Qt.locale(), "dd.MM.yyyy") +
                          " at " + dosage.applied.toLocaleTimeString(Qt.locale(), "HH:mm : ") +
                          dosage.quantity.dbvalue + dosage.quantity.unitstring +
                          " every " + dosage.dbinterval + " hours " +
                          route);
            }
            var msg = "";
            for (i in adjs) {
                if (i != 0) msg = msg + "\r\n";
                msg = msg + adjs[i];
            }
            analysisFrame.model.dosageDescription = msg;

            analysisFrame.model.nextControl = new Date();
            if (nextControl != 0) {
                analysisFrame.model.nextControl = nextControl;
            }
        }
    }
    
    Connections {
        target: flow.chart.mouseArea
        onPositionChanged: {
            if (flow.chart.state !== "validation") {
                return;
            }

            var poppd;
            if (flow.chart.popP && flow.chart.popP.predictive && flow.chart.popP.predictive.predictionData
                    && flow.chart.popP.predictive.predictionData.points) {
                poppd = flow.chart.popP.predictive.predictionData;
                if (poppd.closestPoint) {
                    var popi = poppd.closestPoint.currentindex;
                    poppt = poppd.points.objat(popi).pset.parameters.objlist;
                    actualDrugModel = poppt
                }
            }

            var aprpd;
            if (flow.chart.aprP && flow.chart.aprP.predictive && flow.chart.aprP.predictive.predictionData
                    && flow.chart.aprP.predictive.predictionData.points) {
                aprpd = flow.chart.aprP.predictive.predictionData;
                if (aprpd.closestPoint) {
                    var apri = aprpd.closestPoint.currentindex;
                    aprpt = aprpd.points.objat(apri).pset.parameters.objlist;
                    actualDrugModel = aprpt
                }
            }

            var apopd;
            if (flow.chart.apoP && flow.chart.apoP.predictive && flow.chart.apoP.predictive.predictionData
                    && flow.chart.apoP.predictive.predictionData.points) {
                apopd = flow.chart.apoP.predictive.predictionData;
                if (apopd.closestPoint) {
                    var apoi = apopd.closestPoint.currentindex;
                    apopt = apopd.points.objat(apoi).pset.parameters.objlist;
                    actualDrugModel = apopt
                }
            }

            //            console.log(poppd.closestPoint.currentIndex);
            //            console.log(aprpd.closestPoint.currentindex);
            //            console.log(apopd.closestPoint.currentindex);

            //            console.log("pop: " + flow.chart.currentPoints[0].currentindex)
            //            console.log("apr: " + flow.chart.currentPoints[1].currentindex)
            //            console.log("apo: " + flow.chart.currentPoints[2].currentindex)


            //            var popi = flow.chart.currentPoints[0].currentindex;
            //            var apri = flow.chart.currentPoints[1].currentindex;
            //            var apoi = flow.chart.currentPoints[2].currentindex;
        }
    }

    property var actualDrugModel: ({})

    property var poppt: ({})
    onPopptChanged: {
        //        for (var i = 0; i < poppt.length; ++i) {
        //            console.log(poppt[i].name + poppt[i].quantity.dbvalue);
        //        }
    }

    property var aprpt: ({})
    onAprptChanged: {
        //        for (var i = 0; i < aprpt.length; ++i) {
        //            console.log(aprpt[i].name + aprpt[i].quantity.dbvalue);
        //        }
    }
    property var apopt: ({})
    onApoptChanged: {
        //        for (var i = 0; i < apopt.length; ++i) {
        //            console.log(apopt[i].name + apopt[i].quantity.dbvalue);
        //        }
    }

    anchors.fill: parent
    color: Style.form.background.frame

    ColumnLayout {
        id: lyt
        anchors.fill: parent
        anchors.margins: 5

        property var dprms: interpretation.drugResponseAnalysis.drugModel? interpretation.drugResponseAnalysis.drugModel.parameters.parameters : 0
        property var lamodel: dprms ? dprms.objlist : 0
        onDprmsChanged: {
            //        console.log("# defparams: " + dprms.size());
            //        console.log("# lamodel: " + dprms.size());
        }

        onLamodelChanged: {
            //        for (var i = 0; i < lamodel.length; ++i) {
            //            console.log("param: " + lamodel[i].name);
            //        }
        }

        EntityTitle {
            label.text: "Parameters"
            id: parameterListExpandHeader
            tooltipText: ToolTips.validationTab.parametersListTitle
        }


        EntityList {
            id: parameter

            columnlayout.children: [

                EntityListHeader {
                    id: popParametersListHeaders

                    EntityListHeaderItem { Layout.minimumHeight: 20; Layout.preferredWidth: 115; label.text: ""}
                    EntityListHeaderItem { Layout.minimumHeight: 20; Layout.preferredWidth: 115; label.text: "Population"; }
                    EntityListHeaderItem { Layout.minimumHeight: 20; Layout.preferredWidth: 115; label.text: "A priori"}
                    EntityListHeaderItem { Layout.minimumHeight: 20; Layout.preferredWidth: 115; label.text: "A posteriori"}
                    EntityListHeaderItem { Layout.minimumHeight: 20; Layout.preferredWidth: 115; label.text: ""}

                },

                RowLayout{
                    Layout.fillWidth: true;
                    Layout.minimumWidth: 100;
                    Layout.preferredWidth: 100;
                    EntityListView{
                        Layout.fillWidth: true;
                        Layout.preferredHeight: 20
                        Layout.minimumHeight:   20
                        Layout.minimumWidth: 100
                        Layout.preferredWidth: 100
                        visible : parameterListExpandHeader.expanded
                        orientation: Qt.Vertical
                        model: actualDrugModel
                        delegate:
                            Text {
                                text: modelData.name
                                color: Style.table.foreground.header
                            }
                    }

                    EntityListView{
                        id: deflv
                        Layout.fillWidth: true;
                        Layout.preferredHeight: 20
                        Layout.minimumHeight:   20
                        Layout.minimumWidth: 100
                        Layout.preferredWidth: 100
                        visible : parameterListExpandHeader.expanded
                        model: poppt // lyt.lamodel
                        orientation: Qt.Vertical
                        delegate:
                            RowLayout{
                            Text {
                                text: (modelData.quantity.dbvalue).toFixed(3)
                            }
                        }
                        MouseArea {
                            id: popMousearea
                            anchors.fill: parent
                            hoverEnabled: true
                            ToolTip {
                                property string tooltipText : ToolTips.validationTab.populationParameters
                                background: Rectangle {
                                    color: Style.tooltip.color
                                    border.color: Style.tooltip.border_color
                                }
                                visible : (show_tooltip) ? (popMousearea.containsMouse && (tooltipText != "")) : false
                                text : tooltipText
                            }
                        }


                    }

                    EntityListView {
                        id: aprlv
                        Layout.fillWidth: true;
                        Layout.preferredHeight: 20
                        Layout.minimumHeight:   20
                        Layout.minimumWidth: 115
                        Layout.preferredWidth: 115
                        visible : parameterListExpandHeader.expanded
                        model: aprpt
                        orientation: Qt.Vertical
                        spacing: 2
                        delegate: RowLayout{
                            Text {
                                text: (modelData.quantity.dbvalue).toFixed(3)
                            }
                        }

                        MouseArea {
                            id: aprMousearea
                            anchors.fill: parent
                            hoverEnabled: true
                            ToolTip {
                                property string tooltipText : ToolTips.validationTab.aprioriParameters
                                background: Rectangle {
                                    color: Style.tooltip.color
                                    border.color: Style.tooltip.border_color
                                }
                                visible : (show_tooltip) ? (aprMousearea.containsMouse && (tooltipText != "")) : false
                                text : tooltipText
                            }
                        }
                    }

                    EntityListView {
                        id: apolv
                        Layout.fillWidth: true;
                        Layout.preferredHeight: 20
                        Layout.minimumHeight:   20
                        Layout.minimumWidth: 115
                        Layout.preferredWidth: 115
                        headerPositioning: ListView.OverlayHeader
                        visible : parameterListExpandHeader.expanded
                        model: apopt
                        orientation: Qt.Vertical
                        spacing: 2
                        delegate: RowLayout {
                            Text {
                                text: (modelData.quantity.dbvalue).toFixed(3)
                            }
                        }

                        MouseArea {
                            id: apostMousearea
                            anchors.fill: parent
                            hoverEnabled: true
                            ToolTip {
                                property string tooltipText : ToolTips.validationTab.aposterioriParameters
                                background: Rectangle {
                                    color: Style.tooltip.color
                                    border.color: Style.tooltip.border_color
                                }
                                visible : (show_tooltip) ? (apostMousearea.containsMouse && (tooltipText != "")) : false
                                text : tooltipText
                            }
                        }
                    }

                }

            ]
        }

        EntityTitle {
            label.text: "Display selection"
            id: displaySelectionListExpandHeader
        }

        Rectangle {
            Layout.fillWidth:  true
            Layout.preferredHeight: 30
            Layout.minimumHeight:   20
            visible : displaySelectionListExpandHeader.expanded
            RowLayout {
                Layout.preferredHeight: 25
                Layout.minimumHeight:   25
                spacing: 2

                CheckBox {
                    text: "Measures"
                    checked: true
                    onCheckedChanged: {
                        //show[mea] = checked ? 1 : 0;
                        //toggleShow(mea, show[mea]);
                        toggleShow(mea, checked ? 1 : 0);
                    }
                }
                CheckBox {
                    text: "Targets"
                    checked: true
                    onCheckedChanged: {
                        //show[tar] = checked ? 1 : 0;
                        //toggleShow(tar, show[tar]);
                        toggleShow(tar, checked ? 1 : 0);
                    }
                }
            }
        }

        EntityTitle {
            label.text: "Validation"
            id: validationExpandHeader
        }

        //InterpretationAnalysis
        Rectangle {
            id: analysis
            Layout.fillWidth: true
            Layout.preferredHeight: 5 * 40
            visible : validationExpandHeader.expanded

            property int rowHeight: 35

            property string frameBackground: Style.form.background.frame
            property string frameColor:      Style.form.border.color
            property int frameWidth:   Style.form.border.width
            property int frameRadius:  Style.form.border.radius
            property int framePadding: 5

            property string labelForeground: Style.form.foreground.label
            property string labelFontFamily: Style.form.font.label
            property string labelFontSize:   Style.form.size.label
            property bool labelFontBold: false
            property int labelVAlign: Text.AlignVCenter
            property int labelHAlign: Text.AlignRight

            property string inputFontFamily: Style.form.font.input
            property string inputFontSize:   Style.form.size.input
            property int inputVAlign: Text.AlignVCenter
            property int inputHAlign: Text.AlignLeft

            Rectangle {
                id: analysisFrame
                property var model: interpretation.analysis
                anchors.fill: parent
                anchors.margins: 0
                color:  analysis.frameBackground
                radius: analysis.frameRadius
                border.width: analysis.frameWidth
                border.color: analysis.frameColor

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: analysis.framePadding
                    spacing: 5

                    RowLayout {
                        Layout.preferredHeight: analysis.rowHeight
                        Layout.minimumHeight:   analysis.rowHeight
                        spacing: 2

                        Text {
                            id: posologyLabel
                            Layout.preferredWidth: 100
                            text: "Dosage:"
                            elide: Text.ElideRight
                            color: analysis.labelForeground
                            font.family:    analysis.labelFontFamily
                            font.pixelSize: analysis.labelFontSize
                            font.bold: analysis.labelFontBold
                            verticalAlignment:   analysis.labelVAlign
                            horizontalAlignment: analysis.labelHAlign
                        }
                        Text{
                            Layout.fillWidth:  true
                            id: posologyText
                            wrapMode: TextEdit.WordWrap
                            text: analysisFrame.model.dosageDescription
                            font.family:    analysis.inputFontFamily
                            font.pixelSize: analysis.inputFontSize
                            verticalAlignment:   analysis.inputVAlign
                            horizontalAlignment: analysis.inputHAlign
                        }
                    }
                    RowLayout {
                        Layout.preferredHeight: analysis.rowHeight
                        Layout.minimumHeight:   analysis.rowHeight
                        spacing: 2
                        Text {
                            id: nextControlLabel
                            Layout.preferredWidth: 100
                            text: "Next control:"
                            elide: Text.ElideRight
                            color: analysis.labelForeground
                            font.family:    analysis.labelFontFamily
                            font.pixelSize: analysis.labelFontSize
                            font.bold: analysis.labelFontBold
                            horizontalAlignment: analysis.labelHAlign
                        }
                        DatePicker {
                            id: nextControlDate
                            Layout.preferredWidth: 150
                            date: analysisFrame.model.nextControl
                            onEditingFinished: {
                                analysisFrame.model.nextControl = (new Date(nextControlDate.date.getFullYear(), nextControlDate.date.getMonth(), nextControlDate.date.getDate(), nextControlTime.date.getHours(), nextControlTime.date.getMinutes()));
                            }
                        }
                        TimePicker {
                            id: nextControlTime
                            Layout.preferredWidth: 60
                            date: analysisFrame.model.nextControl
                            onEditingFinished: {
                                analysisFrame.model.nextControl = (new Date(nextControlDate.date.getFullYear(), nextControlDate.date.getMonth(), nextControlDate.date.getDate(), nextControlTime.date.getHours(), nextControlTime.date.getMinutes()));
                            }
                        }
                    }
                    RowLayout {
                        Layout.preferredHeight: analysis.rowHeight
                        Layout.minimumHeight:   analysis.rowHeight
                        spacing: 2

                        Text {
                            id: expectednessLabel
                            Layout.preferredWidth: 100
                            text: "Expectedness:"
                            elide: Text.ElideRight
                            color: analysis.labelForeground
                            font.family:    analysis.labelFontFamily
                            font.pixelSize: analysis.labelFontSize
                            font.bold: analysis.labelFontBold
                            verticalAlignment:   analysis.labelVAlign
                            horizontalAlignment: analysis.labelHAlign
                        }
                        Rectangle {
                            Layout.fillWidth:  true
                            Layout.minimumHeight:   analysis.rowHeight * 1
                            border.width: 1
                            border.color: "#cccccc"

                            Flickable{
                                id: expectedFlick
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

//                                    MouseArea {
//                                        anchors.fill: parent
//                                        onClicked: {
//                                            validationDialog.exec()
//                                        }
//                                    }
                                }


                                ScrollBar.vertical: ScrollBar {}
                            }
                        }

                    }
                    RowLayout {
                        Layout.preferredHeight: analysis.rowHeight
                        Layout.minimumHeight:   analysis.rowHeight
                        spacing: 2

                        Text {
                            id: suitabilityLabel
                            Layout.preferredWidth: 100
                            text: "Suitability:"
                            elide: Text.ElideRight
                            color: analysis.labelForeground
                            font.family:    analysis.labelFontFamily
                            font.pixelSize: analysis.labelFontSize
                            font.bold: analysis.labelFontBold
                            verticalAlignment:   analysis.labelVAlign
                            horizontalAlignment: analysis.labelHAlign
                        }
                        Rectangle {
                            Layout.fillWidth:  true
                            Layout.minimumHeight:   analysis.rowHeight * 1
                            border.width: 1
                            border.color: "#cccccc"

                            Flickable{
                                id: suitableFlick
                                anchors.fill: parent

                                TextArea.flickable: TextArea {
                                    id: suitabilityInput
                                    placeholderText: "Please fill in"
                                    anchors.margins: 1
                                    clip: true
                                    wrapMode: TextEdit.WordWrap
                                    text: analysisFrame.model.suitability
                                    onTextChanged: analysisFrame.model.suitability = text
                                    font.family:    analysis.inputFontFamily
                                    font.pixelSize: analysis.inputFontSize
                                    verticalAlignment:   analysis.inputVAlign
                                    horizontalAlignment: analysis.inputHAlign

                                EntityToolTip {
                                    tooltipText: ToolTips.validationTab.suitability
                                }
                            }

                                ScrollBar.vertical: ScrollBar {}
                            }

                        }

                    }
                    RowLayout {
                        Layout.preferredHeight: analysis.rowHeight
                        Layout.minimumHeight:   analysis.rowHeight
                        spacing: 2

                        Text {
                            id: predictionLabel
                            Layout.preferredWidth: 100
                            text: "Prediction:"
                            elide: Text.ElideRight
                            color: analysis.labelForeground
                            font.family:    analysis.labelFontFamily
                            font.pixelSize: analysis.labelFontSize
                            font.bold: analysis.labelFontBold
                            verticalAlignment:   analysis.labelVAlign
                            horizontalAlignment: analysis.labelHAlign
                        }
                        Rectangle {
                            Layout.fillWidth:  true
                            Layout.minimumHeight:   analysis.rowHeight * 1
                            border.width: 1
                            border.color: "#cccccc"

                            Flickable{
                                id: predictionFlick
                                anchors.fill: parent

                                TextArea.flickable: TextArea {
                                id: predictionInput
                                placeholderText: "Please fill in"
                                anchors.margins: 1
                                clip: true
                                wrapMode: TextEdit.WordWrap
                                text: analysisFrame.model.prediction
                                onTextChanged: analysisFrame.model.prediction = text
                                font.family:    analysis.inputFontFamily
                                font.pixelSize: analysis.inputFontSize
                                verticalAlignment:   analysis.inputVAlign
                                horizontalAlignment: analysis.inputHAlign

                                EntityToolTip {
                                    tooltipText: ToolTips.validationTab.prediction
                                }
                            }
                                ScrollBar.vertical: ScrollBar {}
                            }
                        }

                    }
                    RowLayout {
                        Layout.preferredHeight: analysis.rowHeight
                        Layout.minimumHeight:   analysis.rowHeight
                        spacing: 2

                        Text {
                            id: remonitoringLabel
                            Layout.preferredWidth: 100
                            text: "Remonitoring:"
                            elide: Text.ElideRight
                            color: analysis.labelForeground
                            font.family:    analysis.labelFontFamily
                            font.pixelSize: analysis.labelFontSize
                            font.bold: analysis.labelFontBold
                            verticalAlignment:   analysis.labelVAlign
                            horizontalAlignment: analysis.labelHAlign
                        }
                        Rectangle {
                            Layout.fillWidth:  true
                            Layout.minimumHeight:   analysis.rowHeight * 1
                            border.width: 1
                            border.color: "#cccccc"

                            Flickable{
                                id: remonitoringFlick
                                anchors.fill: parent

                                TextArea.flickable: TextArea {
                                id: remonitoringInput
                                placeholderText: "Please fill in"
                                anchors.margins: 1
                                clip: true
                                wrapMode: TextEdit.WordWrap
                                text: analysisFrame.model.remonitoring
                                onTextChanged: analysisFrame.model.remonitoring = text
                                font.family:    analysis.inputFontFamily
                                font.pixelSize: analysis.inputFontSize
                                verticalAlignment:   analysis.inputVAlign
                                horizontalAlignment: analysis.inputHAlign

                                EntityToolTip {
                                    tooltipText: ToolTips.validationTab.remonitoring
                                }
                            }
                            ScrollBar.vertical: ScrollBar {}
                        }
                        }

                    }
                    RowLayout {
                        Layout.preferredHeight: analysis.rowHeight
                        Layout.minimumHeight:   analysis.rowHeight
                        spacing: 2

                        Text {
                            id: warningLabel
                            Layout.preferredWidth: 100
                            text: "Warning:"
                            elide: Text.ElideRight
                            color: analysis.labelForeground
                            font.family:    analysis.labelFontFamily
                            font.pixelSize: analysis.labelFontSize
                            font.bold: analysis.labelFontBold
                            verticalAlignment:   analysis.labelVAlign
                            horizontalAlignment: analysis.labelHAlign
                        }
                        Rectangle {
                            Layout.fillWidth:  true
                            Layout.minimumHeight:   analysis.rowHeight * 1
                            border.width: 1
                            border.color: "#cccccc"

                            Flickable{
                                id: warningFlick
                                anchors.fill: parent

                                TextArea.flickable: TextArea {
                                id: warningInput
                                placeholderText: "Please fill in"
                                anchors.margins: 1
                                clip: true
                                wrapMode: TextEdit.WordWrap
                                text: analysisFrame.model.warning
                                onTextChanged: analysisFrame.model.warning = text
                                font.family:    analysis.inputFontFamily
                                font.pixelSize: analysis.inputFontSize
                                verticalAlignment:   analysis.inputVAlign
                                horizontalAlignment: analysis.inputHAlign

                                EntityToolTip {
                                    tooltipText : ToolTips.validationTab.warning
                                }
/*
                                ToolTip {
                                    visible: (show_tooltip) ? parent.hovered : false
                                    text: (show_tooltip) ? ToolTips.validationTab.warning : ""
                                    background: Rectangle {
                                        color: Style.tooltip.color
                                        border.color: Style.tooltip.border_color
                                    }
                                }
*/
                            }
                            ScrollBar.vertical: ScrollBar {}
                        }
                        }
                    }

                    TucuButton {
                        objectName: "validateInterpretation"
                        Layout.fillWidth: true
                        Layout.preferredHeight: analysis.rowHeight
                        Layout.minimumHeight:   analysis.rowHeight
                        text: "Validate"
                        onClicked: {
                            interpretationController.isInterpretationValidated = true;
                        }

                        tooltipText: ToolTips.validationTab.validate
                    }
                }
            }
        }
        Rectangle{
            Layout.fillHeight: true
        }
    }

    ValidationTextDialog {
        id: validationDialog
        objectName: "validationTextDialog"

        function exec()
        {
            this.init();
        }
    }
}
