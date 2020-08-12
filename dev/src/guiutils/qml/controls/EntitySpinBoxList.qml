import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

EntitySpinBox {

    property var valuesList: null

    up.onPressedChanged: {adaptStepUp()}
    down.onPressedChanged: {adaptStepDown()}

    function valueValidation()
    {
        console.log("value : " + value)
    }

    function adaptStepUp()
    {
        stepSize = 0
        for (var i = 0; i < valuesList.length; i++){
            if(valuesList[i] > getRealValue()){
                stepSize = (valuesList[i] - getRealValue()) * Math.pow(10, decimals)
                return
            }
        }

    }

    function adaptStepDown()
    {
        stepSize = 0
        for (var i = valuesList.length - 1; i >= 0; i--){
            if(valuesList[i] < getRealValue()){
                stepSize = (getRealValue() - valuesList[i]) * Math.pow(10, decimals)
                return
            }
        }
    }

}
