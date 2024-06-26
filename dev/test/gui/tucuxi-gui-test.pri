
# CONFIG += c++17

SOURCES += $$PWD/src/guitest.cpp \
    $$PWD/src/addeditremovetest.cpp \
    $$PWD/src/adjustmenttest.cpp \
    $$PWD/src/cachetest.cpp \
    $$PWD/src/daylightsavingtest.cpp \
    $$PWD/src/flatrequeststest.cpp \
    $$PWD/src/functionstester.cpp \
    $$PWD/src/guidedstresstest.cpp \
    $$PWD/src/issueshighlighter.cpp \
    $$PWD/src/minimaldatatest.cpp \
    $$PWD/src/pendingrequesttest.cpp \
    $$PWD/src/randomtest.cpp \
    $$PWD/src/reporttest.cpp \
    $$PWD/src/savedtestcomparison.cpp \
    $$PWD/src/sentencesshortcutstest.cpp \
    $$PWD/src/signaturetest.cpp \
    $$PWD/src/stresstest.cpp \
    $$PWD/src/targettest.cpp \
    $$PWD/src/validationloadfile.cpp \
    $$PWD/src/validationtest.cpp \
    $$PWD/src/voriconazoletest.cpp

HEADERS += \
    $$PWD/src/defaultdrugconfig.h \
    $$PWD/src/guitest.h

win32 {
    LIBS += "C:\Program Files\googletest-distribution\lib\gtest.lib"
    INCLUDEPATH += "C:\Program Files\googletest-distribution\include"
    INCLUDEPATH += "C:\Program Files\Spix\include"
}
else {
    LIBS += -lgtest
}
