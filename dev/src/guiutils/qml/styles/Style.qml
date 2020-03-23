pragma Singleton

import QtQuick 2.5

QtObject {

    //Spash screen
    property QtObject splash: QtObject {

        readonly property string image:                 "/images/Tucuxi_fond-vagues.png"
        property QtObject logo: QtObject {
            readonly property string source:            "/images/Tucuxi_logo-blanc.png"
            readonly property int height:                300
        }
        property QtObject buttons: QtObject {
            readonly property int margin:                10
            readonly property int width:                 159
            readonly property int height:                300
            property QtObject icon: QtObject {
                readonly property string add:            "qrc:/icons/splash/new-patient.png"
                readonly property string add_hover:      "qrc:/icons/splash/new-patient-hover.png"
                readonly property string folder:         "qrc:/icons/splash/patients.png"
                readonly property string folder_hover:   "qrc:/icons/splash/patients-hover.png"
                readonly property string requests:       "qrc:/icons/splash/pending-requests.png"
                readonly property string requests_hover: "qrc:/icons/splash/pending-requests-hover.png"
                readonly property string shutdown:       "qrc:/icons/splash/shutdown.png"
                readonly property string shutdown_hover: "qrc:/icons/splash/shutdown-hover.png"
            }
            property QtObject font: QtObject {
                readonly property string family:         "#open-sans"
                readonly property int size:              14
                property QtObject color: QtObject {
                    readonly property string standard:   "#b0b3b8"
                    readonly property string hover:      "#16242a"
                }
            }
            property QtObject background: QtObject {
                readonly property string standard:       "/images/Tucuxi_fond_pictos.png"
                readonly property string hover:          "/images/Tucuxi_fond_pictos-hover.png"
            }
        }
        property QtObject copyright: QtObject {
            readonly property int height:                50
            readonly property string family:             "#open-sans"
            readonly property int size:                  10
            readonly property string color:              "#d7d8dc"
        }
    }

    //Toolbar settings
    property QtObject toolbar: QtObject {
        property QtObject background: QtObject {
            readonly property string button:       "#454c57"
            readonly property string button_hover: "#5bbcdd"
            readonly property string toolbar:      "#454c57"
        }

        property QtObject foreground: QtObject {
            readonly property string button:       "#b0b3b8"
            readonly property string button_hover: "#b0b3b8"
        }

        property QtObject font: QtObject {
            readonly property string button: "#open-sans"
        }

        property QtObject size: QtObject {
            readonly property int button: 14
        }

        property QtObject border: QtObject {
            readonly property string color: "#141d20"
            readonly property int width: 0
        }
    }

    //Flow settings
    property QtObject flow: QtObject {
        property QtObject background: QtObject {
            readonly property string button:          "#383d46"
            readonly property string button_disabled: "#383d46"
            readonly property string button_hover:    "#5bbcdd"
            readonly property string button_selected: "#141d20"
            readonly property string information:     "#6b7585"
        }

        property QtObject foreground: QtObject {
            readonly property string button:          "#d7d8dc"
            readonly property string button_disabled: "#5a6270"
            readonly property string button_hover:    "#fff"
            readonly property string button_selected: "#d7d8dc"
            readonly property string information:     "#fff"
        }

        property QtObject font: QtObject {
            readonly property string button:                "#open-sans"
            readonly property string information:           "#open-sans"
            readonly property string information_birthdate: "#open-sans"
        }

        property QtObject size: QtObject {
            readonly property int button:                14
            readonly property int information:           16
            readonly property int information_birthdate: 12
        }

        property QtObject border: QtObject {
            readonly property string color:          "#141d20"
            readonly property string color_disabled: "#141d20"
            readonly property string color_hover:    "#5bbcdd"
            readonly property string color_selected: "#141d20"
            readonly property int width: 1
        }
    }

    //Tables settings
    property QtObject table: QtObject {
        property QtObject background: QtObject {
            readonly property string row:          "#fff"
            readonly property string row_hover:    "#9bfcfd" // "#5bbcdd"
            readonly property string row_selected: "#5bbcdd" // "#6b7585"
            readonly property string header:       "#fff" // "#eee"  // "#383d46"
            readonly property string table:        "#fff"
        }

        property QtObject foreground: QtObject {
            readonly property string row:          "#000"
            readonly property string row_gray:     "#bbb"
            readonly property string row_hover:    "#000"
            readonly property string row_selected: "#fff"
            readonly property string header:       "#5bbcdd" //"#d7d8dc"
            readonly property string table:        "#000"
        }

        property QtObject font: QtObject {
            readonly property string row:    "#open-sans"
            readonly property string header: "#open-sans"
        }

        property QtObject size: QtObject {
            readonly property int row:    12
            readonly property int header: 14
        }

        property QtObject border: QtObject {
            readonly property string color: "#141d20"
            readonly property int width: 0
        }
    }


    //Info blocks settings
    property QtObject infoBlock: QtObject {
        property QtObject background: QtObject {
            readonly property string row:          "#fff"
            readonly property string row_hover:    "#5bbcdd"
            readonly property string row_selected: "#6b7585"
            readonly property string header:       "#eee"  // "#383d46"
            readonly property string table:        "#fff"
        }

        property QtObject foreground: QtObject {
            readonly property string row:          "#000"
            readonly property string row_hover:    "#fff"
            readonly property string row_selected: "#fff"
            readonly property string header:       "#000" //"#d7d8dc"
            readonly property string table:        "#000"
        }

        property QtObject font: QtObject {
            readonly property string row:    "#open-sans"
            readonly property string header: "#open-sans"
        }

        property QtObject size: QtObject {
            readonly property int row:    12
            readonly property int header: 14
        }

        property QtObject border: QtObject {
            readonly property string color: "#141d20"
            readonly property int width: 0
        }

        property QtObject lines: QtObject {
            readonly property string color: "#141d20"
            readonly property int width: 2
        }
    }

    //Forms settings
    property QtObject form: QtObject {
        property QtObject background: QtObject {
            readonly property string frame:  "#fff"
        }

        property QtObject foreground: QtObject {
            //readonly property string label: "#000"
            readonly property string label: "#5bbcdd"
            readonly property string input: "#000"
        }

        property QtObject font: QtObject {
            readonly property string label: "#open-sans"
            readonly property string input: "#open-sans"
        }

        property QtObject size: QtObject {
            readonly property int label: 12
            readonly property int input: 12
        }

        property QtObject border: QtObject {
            //         readonly property string color: "#383d46"
            readonly property string color: "#fff"
            readonly property int radius: 2
            readonly property int width:  1
        }
    }

    property QtObject tooltip: QtObject {
        readonly property string color: "#5bbcdd"
        readonly property string border_color: "#000000"
    }

}
