
# CONFIG += c++17

SOURCES += $$PWD/src/guitest.cpp \
    $$PWD/src/adjustmenttest.cpp \
    $$PWD/src/cachetest.cpp \
    $$PWD/src/randomtest.cpp \
    $$PWD/src/reporttest.cpp \
    $$PWD/src/stresstest.cpp \
    $$PWD/src/targettest.cpp

LIBS += -lgtest