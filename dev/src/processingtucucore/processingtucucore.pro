include (../tucuxi.pri)

!config_tucucore_lib {
unix {
    include($${TUCUXIROOTDIR}/make/qtcreator/general.pri)
}
win32 {
    include ($${TUCUXIROOTDIR}/make/qtcreator/includepaths.pri)
}
    include ($${TUCUXIROOTDIR}/make/qtcreator/tucucore.pri)
    include ($${TUCUXIROOTDIR}/make/qtcreator/tucucommon.pri)
    include ($${TUCUXIROOTDIR}/make/qtcreator/tinyjs.pri)
}
else {
    include ($${TUCUXIROOTDIR}/make/qtcreator/includepaths.pri)
}


# Adding this, as in tucuxi general.pri removes qt
CONFIG += qt

#Removing the QtGui module (the maths must NOT use any gui element)
QT -= gui

TARGET      = ezechiel-processingtucucore
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

# CONFIG += warn_on
unix {
    QMAKE_CXXFLAGS = -Wunused-variable
}

ezechiel-processingtucucore.depends = ../core


#CONFIG += shared

INCLUDEPATH += ..

HEADERS	+= \
    src/processingtucucore.h \
    src/eztotucucoretranslator.h \
    src/tucucoretoeztranslator.h \
    src/drugs2manager.h

#CODECFORTR      += UTF-8
#TRANSLATIONS    += $${TRANS_DIR}/processing_fr.ts

#Translation configuration
include(../translation.pri)

SOURCES += \
    src/processingtucucore.cpp \
    src/eztotucucoretranslator.cpp \
    src/tucucoretoeztranslator.cpp \
    src/drugs2manager.cpp

SOURCES += $${TUCUXIROOTDIR}/libs/spdlog-master-20170622/include/spdlog/fmt/bundled/format.cc

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = ezechiel-core
for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}
