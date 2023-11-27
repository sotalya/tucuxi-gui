import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtWebEngine
import QtWebChannel
import QtQuick.Dialogs

import Qt.labs.platform

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

Rectangle {
    id:reportPanel
    property alias daweb: daweb
    property int status: Status.invalid
    property bool enabled: false;
    property var show: [1,1,1,1,1,1]
    property int pop: 0
    property int apr: 1
    property int apo: 2
    property int rev: 3
    property int mea: 4
    property int tar: 5

    property bool isWebViewInitialized: false

    signal publishReport(int output)
    signal reset()
    signal toggleShow(int index, int value)

    anchors.fill: parent
    color: Style.form.background.frame

    function getParameters(predData)
    {
        var index = 0;
        var params = "";
        var nbPoints = predData.points.size();
        if (nbPoints === 0)
            return "{}";
        var current = predData.points.objat(index);
        var adjTime = adjustmentTabController.adjustmentDate/1000;
        while (index + 1 < nbPoints && current.time < adjTime) {
            // console.log(current.time + "<" + adjTime);
            current = predData.points.objat(++index);
        }
        //if (index < nbPoints) {

            var pset = current.pset.parameters.objlist;
            for (var i = 0; i < pset.length; ++i) {
                if (i !== 0) params = params + ",";
                params = params + "\"" + pset[i].name + "\":" + "\"" + roundParameters(pset[i].quantity.dbvalue) + "\"";
            }
        //}
        return "{" + params + "}";
    }

    function roundParameters(param)
    {
        var valueInt = (param.toFixed(0) * 100).toFixed(0)   //x
        var valueOne = (param.toFixed(1) * 100).toFixed(0)  //x.0
        var valueTwo = (param.toFixed(2) * 100).toFixed(0)   //x.00
        var result = param.toFixed(2)
        if (valueTwo === valueInt){
            result = param.toFixed(0)
        }
        else if(valueTwo === valueOne){
            result = param.toFixed(1)
        }

        return result
    }

    function updateReport()
    {
        publishReport(InterpretationController.Pdf);
        if (!isWebViewInitialized) {
            daweb.register();
            isWebViewInitialized = true;
        }
        else {
            daweb.update();
            //daweb.webChannel.registerObject("interpretation", interpretation);
            reportInfo.dataUpdated();
            daweb.reload();
        }
        printbutton.enabled = true;
    }

    Connections {
        target: interpretationController
        // function onInterpretationValidated()
        //TODO -> check if its still work as intended
        //onInterpretationValidated: {
        function onInterpretationValidated() {
            updateReport();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        QtObject {
            id: reportInfo
            signal dataUpdated();
            property variant covariates: []
            property variant parameters: []
            property variant drugvariates: []
            property variant patientvariates: []
            property variant measures: []
            property variant dosages: []
            property string popParameters: "";
            property string aPrioriParameters: "";
            property string aPosterioriParameters: "";
            property string posology: "";
            property string nextControl: "";
            property string validationDate: "";
            property string graphPath: "";
            property string steadyStateAUC24: "";
            property string steadyStateMin: "";
            property string steadyStateMax: "";
            property string fullJson: "";
        }

        WebEngineView {
            id: daweb
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            webChannel: interpretationController.webchannel;

            function update()
            {
                reportInfo.fullJson = interpretationController.interpretationToJson();

//                reportInfo.steadyStateMax = (interpretation.analysis.chartData.getInfo("steadyStateMax")/1000).toFixed(2) + "mg/l";
//                reportInfo.steadyStateMin = (interpretation.analysis.chartData.getInfo("steadyStateMin")/1000).toFixed(2) + "mg/l";
//                reportInfo.steadyStateAUC24 = (interpretation.analysis.chartData.getInfo("steadyStateAUC24")/1000).toFixed(2) + "mg/l*h";

                // Get parameter sets at the adjustment date
//                reportInfo.popParameters = "{}"
//                if (flow.chart.popP) {
//                    reportInfo.popParameters = getParameters(flow.chart.popP.predictive.predictionData);
//                }
//                reportInfo.aPrioriParameters = "{}"
//                if (flow.chart.aprP) {
//                    reportInfo.aPrioriParameters = getParameters(flow.chart.aprP.predictive.predictionData);
//                }
//                reportInfo.aPosterioriParameters = "{}"
//                if (flow.chart.apoP) {
//                    reportInfo.aPosterioriParameters = getParameters(flow.chart.apoP.predictive.predictionData);
//                }

                // Validation info:
//                reportInfo.posology = interpretation.analysis.dosageDescription
//                reportInfo.nextControl = interpretation.analysis.nextControl.toLocaleDateString(Qt.locale(), "dd.MM.yyyy") + " " +
//                                         interpretation.analysis.nextControl.toLocaleTimeString(Qt.locale(), "HH:mm");
//                reportInfo.validationDate = interpretationController.interpretation.validateInterpretationTime.toLocaleDateString(Qt.locale(), "dd.MM.yyyy") + " " +
//                        interpretationController.interpretation.validateInterpretationTime.toLocaleTimeString(Qt.locale(), "HH:mm");
            }

            function register()
            {
                // Init our report info object and add it to the channel
                daweb.update();
                //webChannel.registerObject("interpretation", interpretation);
                //webChannel.registerObject("something", interpretation.analysis.chartData.adjPred.predictive.predictionData);
                webChannel.registerObject("info", reportInfo);

                // Now load the web page into the web browser
                if (interpretationController.isDemo) {
                    url = "qrc:/report/report.html"
                }
                else {
                    url = "file:" + appPath + "/reports/report.html"
                }
            }

            Component.onCompleted: {
                //register();
                //                userScripts = [jojo];
                //                interpretation.analysis.expectedness = "what ya"
                //                webChannel.registerObject("interp", interpretation);
            }
            //            url: "qrc:/controls/ezechiel-default.html"
            //            url: "qrc:/controls/index.html"
        }

        //ToDo: custom look for the buttons, see with Sandrine
        //ToDo: later on, the right panel could display the report
        TucuButton {
            id: printbutton
            objectName: "printButton"
            Layout.fillWidth: true
            text: "Generate pdf"
            enabled: false
            //        onClicked: publishReport(InterpretationController.Pdf)
            onClicked: {
                forceActiveFocus();
                if (interpretationController.isTesting()) {
                    daweb.printToPdf(interpretationController.reportFileName);
                }
                else {
                    fileDialog.open();
                }
            }
        }

        FileDialog {
            id: fileDialog
            title: "Generate pdf report..."
            fileMode: FileDialog.SaveFile
            folder : StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
            modality: Qt.WindowModal
            onAccepted: {
                // Idea from https://stackoverflow.com/questions/24927850/get-the-path-from-a-qml-url
                var filename = interpretationController.handleFileChosen(fileDialog.file);

                daweb.printToPdf(filename);
                fileDialog.close();
            }
            onRejected: {
                fileDialog.close();
            }
            Component.onCompleted: visible = false
        }
    }
}
