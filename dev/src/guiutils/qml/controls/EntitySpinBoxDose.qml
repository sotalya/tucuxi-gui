import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

EntitySpinBox {

    id: doseSpinBox

    property var doses: null

    function valueValidation()
    {
        console.log("value : " + value)
    }

    function adaptStepUp()
    {
        for (var i = 0; i < doses.dosesList.length; i++){
            if(doses.dosesList[i] === getRealValue()){
                stepSize = (doses.dosesList[i + 1] - doses.dosesList[i]) * Math.pow(10, decimals)
            }
        }
    }

    function adaptStepDown()
    {
        for (var i = 0; i < doses.dosesList.length; i++){
            if(doses.dosesList[i] === getRealValue()){
                stepSize = (doses.dosesList[i] - doses.dosesList[i - 1]) * Math.pow(10, decimals)
            }
        }
    }

}
