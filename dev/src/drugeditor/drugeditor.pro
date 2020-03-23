include (../tucuxi.pri)

TARGET      = drugeditor
TEMPLATE    = app
DESTDIR     = $${OUT_PWD}/../$${DIST_DIR}

QT += gui printsupport quick quickwidgets printsupport svg
!win32 {
QT += webengine webenginewidgets
}

CONFIG += qml_debug

DEPENDPATH += .$${OUT_PWD}/../$${DIST_DIR}
INCLUDEPATH +=	..

SOURCES += src/main.cpp \
    src/models/editorlistmodel.cpp \
    src/models/editormodel.cpp \
    src/models/druglistmodel.cpp \
    src/undoredocontroller.cpp \
    src/undocommands.cpp \
    src/models/commentlistmodel.cpp \
    src/models/doselistmodel.cpp \
    src/models/infusionlistmodel.cpp \
    src/models/intervallistmodel.cpp

EXAMPLE_FILES = \
    window.qml

QML_IMPORT_PATH += $$PWD/..

#Libraries that must be linked into the project (LIBS) and that the target depends on (PRE_TARGETDEPS)
DEPENDENCIES = ezechiel-guiutils ezechiel-apputils ezechiel-admin ezechiel-core

for(dep, DEPENDENCIES) {
    LIBNAME = $${OUT_PWD}/../$${DIST_DIR}/$${LIBPRE}$${dep}.$${SONAME}
    LIBS += $${LIBNAME}
    PRE_TARGETDEPS += $${LIBNAME}
}

#Translation configuration
include(../translation.pri)

include(../botan.pri)

HEADERS += \
    src/errors_drugeditor.h \
    src/models/editorlistmodel.h \
    src/models/editormodel.h \
    src/models/druglistmodel.h \
    src/undoredocontroller.h \
    src/undocommands.h \
    src/models/commentlistmodel.h \
    src/models/doselistmodel.h \
    src/models/intervallistmodel.h \
    src/models/infusionlistmodel.h


RESOURCES += \
    window.qrc \
    editorQml/editorQml.qrc
