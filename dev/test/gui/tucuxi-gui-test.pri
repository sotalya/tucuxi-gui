
# CONFIG += c++17

SOURCES += $$PWD/src/guitest.cpp \
    $$PWD/src/addeditremovetest.cpp \
    $$PWD/src/adjustmenttest.cpp \
    $$PWD/src/cachetest.cpp \
    $$PWD/src/daylightsavingtest.cpp \
    $$PWD/src/functionstester.cpp \
    $$PWD/src/guidedstresstest.cpp \
    $$PWD/src/issueshighlighter.cpp \
    $$PWD/src/minimaldatatest.cpp \
    $$PWD/src/randomtest.cpp \
    $$PWD/src/reporttest.cpp \
    $$PWD/src/savedtestcomparison.cpp \
    $$PWD/src/stresstest.cpp \
    $$PWD/src/targettest.cpp \
    $$PWD/src/validationloadfile.cpp \
    $$PWD/src/validationtest.cpp

LIBS += -lgtest
