import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import guiutils.qml.controls 1.0
import guiutils.qml.styles 1.0

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
