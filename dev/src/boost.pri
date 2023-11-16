# Link to the boost library

win32 {
    INCLUDEPATH  += "C:\Program Files (x86)\boost"
}

macx {
    INCLUDEPATH  += /Users/reds/libs/boost \
                    /usr/local/include
}

unix:!android {

    exists($${TUCUXIROOTDIR}/libs/boost) {
        INCLUDEPATH += $${TUCUXIROOTDIR}/libs/boost
    }
    else: {
        INCLUDEPATH += ../../../ezechiel-deps-boost/modular-boost/boost
        INCLUDEPATH  += /usr/include/boost
    }
}

android {
    INCLUDEPATH += /opt/android
}
