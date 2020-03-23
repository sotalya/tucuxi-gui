# Link to the eigen library

linux {
    INCLUDEPATH += $${TUCUXIROOTDIR}/libs/eigen-3.3.2
}

win32 {
    INCLUDEPATH += "C:\Program Files (x86)\eigen"
}

macx {
    INCLUDEPATH += /usr/local/Cellar/eigen/3.2.6/include/eigen3
    INCLUDEPATH  += /Users/reds/libs/eigen/eigen3
}

android {
    INCLUDEPATH += /opt/android
}
