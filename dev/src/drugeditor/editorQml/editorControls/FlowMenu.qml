import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import drugeditor.qml.controls 1.0
import guiutils.qml.styles 1.0

Item {
    id: root
    width:  1280
    height: 128

    default property alias content:  layout.data
    property alias informationPanel: informationPanelLoader.sourceComponent

    Item {
        id: impl
        anchors.fill: parent

        Rectangle {
            id: background
            anchors.fill: parent
            color: Style.flow.background.information
        }

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
}
