import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Controls.Styles

import guiutils.qml.styles
import guiutils.qml.controls

import ezechiel

ListView {
    Layout.fillWidth:  true
    Layout.fillHeight: true
    highlightFollowsCurrentItem: true
    snapMode: ListView.SnapToItem
    spacing: 2
    clip: true
}
