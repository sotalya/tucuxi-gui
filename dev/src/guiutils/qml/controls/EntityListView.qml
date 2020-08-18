import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import guiutils.qml.styles 1.0
import guiutils.qml.controls 1.0

import ezechiel 1.0

ListView {
    Layout.fillWidth:  true
    Layout.fillHeight: true
    highlightFollowsCurrentItem: true
    snapMode: ListView.SnapToItem
    spacing: 2
    clip: true

}
