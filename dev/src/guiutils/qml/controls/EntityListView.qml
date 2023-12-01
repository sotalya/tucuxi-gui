import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

ListView {
    // Used for testing
    function getCurrentItemName(){
        try{
            return currentItem.objectName
        }
        catch(typeError){
            //console.log(typeError);
            return "noValidItemFound"
        }
    }
    // Used for testing
    function getItemsCount(){
        return count
    }
    // Used for testing
    function setPositionAtIndex(myIndex){
        positionViewAtIndex(myIndex, ListView.Beginning)
    }

    Layout.fillWidth:  true
    Layout.fillHeight: true
    highlightFollowsCurrentItem: true
    snapMode: ListView.SnapToItem
    spacing: 2
    clip: true
}
