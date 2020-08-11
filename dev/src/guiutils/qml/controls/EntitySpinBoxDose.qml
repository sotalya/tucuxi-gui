import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

EntitySpinBox {

    id: doseSpinBox

    property int currentDose: 0
    property var doses: null

    property bool upEnable : true
    property bool downEnable : true

    function valueValidation()
    {
        console.log("value : " + value)
    }

    function adaptStepUp()
    {
        if ((doseSpinBox.currentDose <= doses.dosesList.length - 2) && upEnable){
            stepSize = (doses.dosesList[doseSpinBox.currentDose + 1] - doses.dosesList[doseSpinBox.currentDose]) * Math.pow(10, decimals)
            console.log("i : " + doseSpinBox.currentDose + " | value : " + doses.dosesList[doseSpinBox.currentDose] + " | Step : " + stepSize)
            doseSpinBox.currentDose++;
        }
        upEnable = !upEnable;
    }

    function adaptStepDown()
    {
        if ((doseSpinBox.currentDose > 0) && downEnable){
            stepSize = (doses.dosesList[doseSpinBox.currentDose] - doses.dosesList[doseSpinBox.currentDose - 1]) * Math.pow(10, decimals)
            console.log("i : " + doseSpinBox.currentDose + " | value : " + doses.dosesList[doseSpinBox.currentDose] + " | Step : " + stepSize)
            doseSpinBox.currentDose--;
        }
        downEnable = !downEnable;
    }

}
