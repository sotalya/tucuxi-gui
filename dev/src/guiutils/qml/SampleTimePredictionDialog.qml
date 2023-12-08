import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

DialogBase {
    title: "Sample time prediciotn"
    width: 600
    height: 600
    property var self

    function init() {
        self = this
    }

    RowLayout {

        Button {
            id: cancelBtn
            text: "OK"
            Layout.preferredWidth: 125
            onClicked: function() {
                self.exit(false);
            }
        }
    }
}
