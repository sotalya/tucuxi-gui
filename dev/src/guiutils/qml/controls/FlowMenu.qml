import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import guiutils.qml.controls
import guiutils.qml.styles

Rectangle {
    width:  1280
    height: parent.height * 0.1
    color: Style.flow.background.information

    default property alias content:  layout.data
    property alias informationPanel: informationPanelLoader.sourceComponent


    RowLayout {
        id: layout
        anchors.fill: parent
        // This spacing is related to the delta of the FlowMenuButton.
        // It should be the same value, but negative
        spacing: -10

        Item {
            Layout.fillWidth:  true
            Layout.fillHeight: true

            Loader {
                id: informationPanelLoader
                anchors.fill: parent
                visible: informationPanelLoader.status == Loader.Ready
            }
        }
    }
}
