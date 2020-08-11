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
        for (var i = 0; i < valuesList.length; i++){
            if(valuesList[i] === getRealValue()){
                stepSize = (valuesList[i + 1] - valuesList[i]) * Math.pow(10, decimals)
                return;
            }
        }
    }

    function adaptStepDown()
    {
        for (var i = 0; i < valuesList.length; i++){
            if(valuesList[i] === getRealValue()){
                stepSize = (valuesList[i] - valuesList[i - 1]) * Math.pow(10, decimals)
                return;
            }
        }
    }

}
