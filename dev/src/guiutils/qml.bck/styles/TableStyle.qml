pragma Singleton

import QtQuick 2.5

QtObject {
    readonly property int spacing: 3
    readonly property string backgroundColor: "transparent"
    readonly property string fontfamily: "sans-serif"
    readonly property int textminpxsize : 12
    readonly property int textpxsize : 16//20
    readonly property int borderwidth: 1
    readonly property color bordercolor: "black"

    property QtObject header: QtObject {
//        readonly property int width: 75
        readonly property int height: 25

        readonly property int spacing: 0
        readonly property int radius: 2
//        readonly property int margin: 5

        readonly property string backgroundColor: "transparent"
        readonly property string foregroundColor: "black"

        property QtObject button: QtObject {
            readonly property string foregroundColor: "lightsteelblue"
        }
    }

    property QtObject filter: QtObject {
        readonly property int width: 64
        readonly property int height: 64

        readonly property int spacing: header.spacing
        readonly property int radius: header.radius
        readonly property int margin: 5

        readonly property string backgroundColor: header.backgroundColor
        readonly property string foregroundColor: header.foregroundColor

        property QtObject button: QtObject {
            readonly property string foregroundColor: header.button.foregroundColor
        }
    }

    property QtObject row: QtObject {
//        readonly property int width: header.width
        readonly property int height: header.height

        readonly property int spacing: header.spacing
//        readonly property int margin: header.margin

        property QtObject button: QtObject {
            readonly property string foregroundColor: "#708090"
        }
    }

    property QtObject cell: QtObject {
//        readonly property int width: header.width
        readonly property int height: header.height

        property int colwidth: 100//0
        onColwidthChanged: console.log("style colwidth changed: " + colwidth)

        readonly property int spacing: header.spacing
        readonly property int radius: header.radius
//        readonly property int margin: header.margin

//        readonly property string backgroundcolor1: "#e2e6e9"
//        readonly property string backgroundcolor2: "#c5ccd3"
//        readonly property string highlightedcolor: "#c9d7e8"
//        readonly property string foregroundcolor: "black"
        readonly property color backgroundcolor1: "transparent"
        readonly property color backgroundcolor2: "transparent"
        readonly property color highlightedcolor: "#c9d7e8"
        readonly property color foregroundcolor: "black"
    }

    property QtObject dialog: QtObject {
        readonly property int width: 300
        readonly property int height: 150

        readonly property int spacing: 1
        readonly property int radius: 2
        readonly property int margin: 20

        readonly property string backgroundColor: "#e2e6e9"
        readonly property string foregroundColor: "black"

        property QtObject button: QtObject {
            readonly property string foregroundColor: "#708090"
        }

        property QtObject border: QtObject {
            readonly property int size: 3
            readonly property string color: "lightslategray"
        }
    }
}
