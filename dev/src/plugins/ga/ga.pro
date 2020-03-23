#EzeCHieL configuration
include(../../tucuxi.pri)

#Plugin ID
PLUGIN_ID=ch.heig-vd.ezechiel.ga

#Engine ID
DEFINES += GA_ID=\\\"$${PLUGIN_ID}\\\"

#Normally included in the pluging option, the dll option must be specified if the static option was specified
CONFIG += plugin dll

TARGET      = ga
TEMPLATE    = lib
DESTDIR     = $${BUILD_DIR}/$${DIST_DIR}/plugins

INCLUDEPATH += ../../core ../../core/interfaces ../../core/utils ../../core/dal \
    libGGA/EvolutionEngine/SelectionMethod \
    libGGA/EvolutionEngine/ReproductionMethod/Mutation \
    libGGA/EvolutionEngine/ReproductionMethod/Crossover \
    libGGA/EvolutionEngine \
    libGGA/StatisticEngine/EvolutionaryMesure \
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity \
    libGGA/StatisticEngine \
    libGGA \
    libGGA/Utility \
    libGGA/Logger \
    libGGA/Population/Individual/Genotype \
    libGGA/Population/Individual \
    libGGA/Population \
    libGGA/SaveEngine

HEADERS	+= ga.h \
    evolutionezechiel.h \
    libGGA/EvolutionEngine/SelectionMethod/rankbasedselection.h \
    libGGA/EvolutionEngine/SelectionMethod/elitism.h \
    libGGA/EvolutionEngine/SelectionMethod/proportionalselection.h \
    libGGA/EvolutionEngine/SelectionMethod/elitismwithrandom.h \
    libGGA/EvolutionEngine/SelectionMethod/entityselection.h \
    libGGA/EvolutionEngine/SelectionMethod/roulettewheel.h \
    libGGA/EvolutionEngine/SelectionMethod/tournamentselection.h \
    libGGA/EvolutionEngine/SelectionMethod/truncatedrankbasedselection.h \
    libGGA/EvolutionEngine/SelectionMethod/selectionpressure.h \
    libGGA/EvolutionEngine/SelectionMethod/generationalreplacement.h \
    libGGA/EvolutionEngine/ReproductionMethod/Mutation/mutate.h \
    libGGA/EvolutionEngine/ReproductionMethod/Mutation/toggling.h \
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/arithmetic.h \
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/crossover.h \
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/uniform.h \
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/onepoint.h \
    libGGA/EvolutionEngine/evolutionengine.h \
    libGGA/StatisticEngine/EvolutionaryMesure/evolutionarymesure.h \
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/allpairpossibility.h \
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/populationdiversity.h \
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/entropic.h \
    libGGA/StatisticEngine/statisticengine.h \
    libGGA/Utility/timertool.h \
    libGGA/Utility/randomgenerator.h \
    libGGA/Utility/qbitarrayutility.h \
    libGGA/Logger/populationlogger.h \
    libGGA/Logger/statisticlogger.h \
    libGGA/Logger/logger.h \
    libGGA/Population/Individual/Genotype/genotype.h \
    libGGA/Population/Individual/representative.h \
    libGGA/Population/Individual/popentity.h \
    libGGA/Population/Individual/individual.h \
    libGGA/Population/population.h \
    libGGA/SaveEngine/saveengine.h

SOURCES += ga.cpp \
    evolutionezechiel.cpp \
    libGGA/EvolutionEngine/SelectionMethod/rankbasedselection.cpp \
    libGGA/EvolutionEngine/SelectionMethod/elitism.cpp \
    libGGA/EvolutionEngine/SelectionMethod/proportionalselection.cpp \
    libGGA/EvolutionEngine/SelectionMethod/elitismwithrandom.cpp \
    libGGA/EvolutionEngine/SelectionMethod/entityselection.cpp \
    libGGA/EvolutionEngine/SelectionMethod/roulettewheel.cpp \
    libGGA/EvolutionEngine/SelectionMethod/tournamentselection.cpp \
    libGGA/EvolutionEngine/SelectionMethod/truncatedrankbasedselection.cpp \
    libGGA/EvolutionEngine/SelectionMethod/selectionpressure.cpp \
    libGGA/EvolutionEngine/SelectionMethod/generationalreplacement.cpp \
    libGGA/EvolutionEngine/ReproductionMethod/Mutation/mutate.cpp \
    libGGA/EvolutionEngine/ReproductionMethod/Mutation/toggling.cpp \
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/arithmetic.cpp \
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/crossover.cpp \
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/uniform.cpp \
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/onepoint.cpp \
    libGGA/EvolutionEngine/evolutionengine.cpp \
    libGGA/StatisticEngine/EvolutionaryMesure/evolutionarymesure.cpp \
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/allpairpossibility.cpp \
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/populationdiversity.cpp \
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/entropic.cpp \
    libGGA/StatisticEngine/statisticengine.cpp \
    libGGA/Utility/timertool.cpp \
    libGGA/Utility/randomgenerator.cpp \
    libGGA/Utility/qbitarrayutility.cpp \
    libGGA/Logger/populationlogger.cpp \
    libGGA/Logger/statisticlogger.cpp \
    libGGA/Logger/logger.cpp \
    libGGA/Population/Individual/Genotype/genotype.cpp \
    libGGA/Population/Individual/representative.cpp \
    libGGA/Population/Individual/popentity.cpp \
    libGGA/Population/Individual/individual.cpp \
    libGGA/Population/population.cpp \
    libGGA/SaveEngine/saveengine.cpp

CODECFORTR      += UTF-8
TRANSLATIONS    += $${TRANS_DIR}/$${PLUGIN_ID}_fr.ts

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDANCIES = ezechiel-core ezechiel-math
for(dep, DEPENDANCIES) {
   LIBNAME = $${BUILD_DIR}/$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
   LIBS += $${LIBNAME}
   PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../../translation.pri)

#Module extra dependencies
include(../../boost.pri)
