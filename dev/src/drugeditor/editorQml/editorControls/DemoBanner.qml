import QtQuick 2.5
import QtQuick.Controls 1.4

import QtWebKit 3.0

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

    WebView {
        id: bannerView
        anchors.fill: parent
        url: "http://www.ezechiel.ch/demo_banner/index.html"

        onLoadingChanged: {
            if (loadRequest.status !== WebView.LoadSucceededStatus && loadRequest.status !== WebView.LoadFailedStatus)
                return;

            bannerView.visible = loadRequest.status === WebView.LoadSucceededStatus;
            errorView.visible  = loadRequest.status === WebView.LoadFailedStatus;
        }

        onNavigationRequested: {
            if (request.url === url) {
                request.action = WebView.AcceptRequest;
                return;
            }

            request.action = WebView.IgnoreRequest;
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
