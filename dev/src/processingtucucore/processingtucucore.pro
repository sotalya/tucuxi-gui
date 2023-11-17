include (../tucuxi.pri)

!config_tucucore_lib {
unix {
    include($${TUCUXIROOTDIR}/make/qtcreator/general.pri)
}
win32 {
    include ($${TUCUXIROOTDIR}/make/qtcreator/includepaths.pri)
}
    include ($${TUCUXIROOTDIR}/make/qtcreator/tucucore.pri)
    include ($${TUCUXIROOTDIR}/make/qtcreator/tucucrypto.pri)
    include ($${TUCUXIROOTDIR}/make/qtcreator/tucucommon.pri)
    config_sign {
        include ($${TUCUXIROOTDIR}/make/qtcreator/tucusign.pri)
    }
}
else {
    include ($${TUCUXIROOTDIR}/make/qtcreator/includepaths.pri)
}


# Adding this, as in tucuxi general.pri removes qt
CONFIG += qt

#Removing the QtGui module (the maths must NOT use any gui element)
QT -= gui

TARGET      = tucuxi-gui-processingtucucore
TEMPLATE    = lib
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

# CONFIG += warn_on
unix {
    QMAKE_CXXFLAGS = -Wunused-variable
}

tucuxi-gui-processingtucucore.depends = ../core


#CONFIG += shared

INCLUDEPATH += ..

HEADERS	+= \
    src/guitotucucoretranslator.h \
    src/processingtucucore.h \
    src/drugs2manager.h \
    src/tqflogger.h \
    src/tucucoretoguitranslator.h

#CODECFORTR      += UTF-8
#TRANSLATIONS    += $${TRANS_DIR}/processing_fr.ts

#Translation configuration
include(../translation.pri)

SOURCES += \
    src/guitotucucoretranslator.cpp \
    src/processingtucucore.cpp \
    src/drugs2manager.cpp \
    src/tqflogger.cpp \
    src/tucucoretoguitranslator.cpp

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = tucuxi-gui-core
for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}
