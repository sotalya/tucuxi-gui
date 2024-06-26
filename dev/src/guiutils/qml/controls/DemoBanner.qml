import QtQuick
import QtQuick.Controls

//import QtWebKit
import QtWebEngine

Item {
    id: root
    objectName: "bannerView"
    width:  960
    height: 128

    Rectangle {
        id: errorView
        anchors.fill: parent
        visible: false
        color: "#454C57"

        Text {
            id: title
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -22
            text:  "No connection..."
            color: "#fff"
            font.pixelSize: 34
            font.family: "arial"
        }

        Text {
            id: content
            anchors.horizontalCenter: title.horizontalCenter
            anchors.top:              title.bottom
            anchors.topMargin:        5
            text:  "Please check your internet connection before relaunching the application."
            color: "#fff"
            font.pixelSize: 22
            font.family: "arial"
            font.italic: true
        }
    }

    WebEngineView {
        id: bannerView
        anchors.fill: parent
        url: "http://www.ezechiel.ch/demo_banner/index.html"

        onLoadingChanged: {
            if (loadRequest.status !== WebEngineView.LoadSucceededStatus && loadRequest.status !== WebEngineView.LoadFailedStatus)
                return;

            bannerView.visible = loadRequest.status === WebEngineView.LoadSucceededStatus;
            errorView.visible  = loadRequest.status === WebEngineView.LoadFailedStatus;
        }

        onNavigationRequested: {
            if (request.url === url) {
                request.action = WebEngineView.AcceptRequest;
                return;
            }

            request.action = WebEngineView.IgnoreRequest;
            Qt.openUrlExternally(request.url);
        }

        Timer {
            interval: 10 * 60 * 1000
            running:  true
            repeat:   true
            onTriggered: bannerView.reload()
        }
    }
}
