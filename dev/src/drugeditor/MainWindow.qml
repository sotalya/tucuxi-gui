import QtQuick 2.5
import QtQuick.Controls 1.4

import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

import drugeditor.qml.controls 1.0

Item {


    id: mainWindow
    width:  1366
    height: 768

    readonly property int minimumWidth:  960
    readonly property int minimumHeight: 540

    readonly property int contentPreferredWidth:  width
    readonly property int contentPreferredHeight: height - applicationBar.height

    readonly property int contentMinimumWidth:  minimumWidth
    readonly property int contentMinimumHeight: minimumHeight - applicationBar.height

    visible: true

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ApplicationBar {
            id: applicationBar
            objectName: "applicationBarView"
            Layout.fillWidth: true
        }

        Flow {
            id: flow
            objectName: "flowView"
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: true

        }
    }





}
