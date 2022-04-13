import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4


EntitySpinBox {
    id : root

    property var valuesList: null

    up.onPressedChanged: {adaptStepUp()}
    down.onPressedChanged: {adaptStepDown()}

    // The following do not work to disable the - and +. It disallows to define values outside
    // of the list min and max values
    // minimumValue: valuesList ? Math.min(value, valuesList[0]) : 0
    // maximumValue: valuesList ? Math.max(value, valuesList[valuesList.length - 1]) : 10000000

    function valueValidation()
    {
        console.log("value : " + value)
    }

    function adaptStepUp()
    {
        stepSize = 0
        for (var i = 0; i < valuesList.length; i++){
            if(valuesList[i] > getRealValue()){
                value = getRealValue() * Math.pow(10, decimals);
                stepSize = (valuesList[i]) * Math.pow(10, decimals) - getValue()
                //console.log("CurrentValue : " + currentValue);
                //console.log("value : " + getValue());
                //console.log("stepUp : " + stepSize);
                return
            }
        }
    }

    function adaptStepDown()
    {
        stepSize = 0
        for (var i = valuesList.length - 1; i >= 0; i--){
            if(valuesList[i] < getRealValue()){
                value = getRealValue() * Math.pow(10, decimals);
                //console.log("valueList : " + valuesList[i]);
                stepSize = getValue() - (valuesList[i]) * Math.pow(10, decimals)
                //console.log("value : " + getValue());
                //console.log("CurrentValue : " + currentValue);
                //console.log("stepDown : " + stepSize);
                return
            }
        }
    }

}
