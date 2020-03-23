import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    visible: false
    modality: Qt.ApplicationModal
    title: "Dosage parameters"
    flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint | Qt.MSWindowsFixedSizeDialogHint
    width: 500
    height: 250

    property bool bCreatingNewItem: false

    // Following methods should be overidden by derived classes
    function init() {}                                  // Initialize dialog content
    function validate() { return true }                 // Return true if data is valid
    function onUpdated(bApplied, bCreatingNewItem) {}   // Called when the user pressed on Apply, Ok or Cancel

    function open(_bCreatingNewItem)
    {
        bCreatingNewItem = _bCreatingNewItem
        this.showNormal();
    }

    function apply()
    {
        if (this.validate()) {
            if (this.applyChanges()) {
                this.onUpdated(true, bCreatingNewItem)
                bCreatingNewItem = false; // If applied once, the item becomes accepted.
                return true;
            }
        }
        return false;
    }

    function exit(bApplied)
    {
        this.onUpdated(bApplied, bCreatingNewItem)
        this.close();
    }
}
