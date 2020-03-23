INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += elitism.cpp \
    entityselection.cpp \
    generationalreplacement.cpp \
    proportionalselection.cpp \
    rankbasedselection.cpp \
    roulettewheel.cpp \
    selectionpressure.cpp \
    tournamentselection.cpp \
    truncatedrankbasedselection.cpp \
    libGGA/EvolutionEngine/SelectionMethod/elitismwithrandom.cpp

HEADERS += elitism.h \
    entityselection.h \
    generationalreplacement.h \
    proportionalselection.h \
    rankbasedselection.h \
    roulettewheel.h \
    selectionpressure.h \
    tournamentselection.h \
    truncatedrankbasedselection.h \
    libGGA/EvolutionEngine/SelectionMethod/elitismwithrandom.h
