# Tucuxi configuration
include(tucuxi.pri)

TEMPLATE    =	subdirs
SUBDIRS     +=	 core admin apputils guiutils gui \
    flatrequests

SUBDIRS += processingtucucore
	processingtucucore.depends  = core
        guiutils.depends           += processingtucucore

#!config_demo {
#        SUBDIRS += cliutils cli \
#                processingrestserversimple
#}

SUBDIRS += rest

#config_connected {
#SUBDIRS += restclient restserver
#}

macx {
    QMAKE_CXXFLAGS += -arch arm64
    QMAKE_LFLAGS += -arch arm64
    QMAKE_MACOSX_DEPLOYMENT_TARGET=15.3
}

#Drug editor has its own sub-project
#xmleditor.depends = core

#All submodules dependencies
apputils.depends             = core
admin.depends                = core
plugins.depends              = core
rest.depends                 = core admin
databases.depends            = core apputils admin
config_rest {
guiutils.depends             = core apputils admin rest
gui.depends                  = core apputils admin rest guiutils flatrequests
cli.depends                  = core apputils admin cliutils rest
cliutils.depends             = core apputils admin rest
}
else {
guiutils.depends             = core apputils admin
gui.depends                  = core apputils admin guiutils flatrequests
}
drugeditor.depends	     = core guiutils
flatrequests.depends                 = core admin apputils rest

guiutils.depends             += processingtucucore
gui.depends += processingtucucore


#Custom target for doxygen
doc.target   = doc
doc.commands = doxygen Doxyfile
doc.depends  = FORCE

QMAKE_EXTRA_TARGETS += doc

#Install the drugs xml files
#drugs.path  =  $${OUT_PWD}/../$${DIST_DIR}/drugs
#drugs.files =  drugs/*
#INSTALLS    += drugs

#Install the html doc files
#manual.path = $${OUT_PWD}/../$${DIST_DIR}/doc
#manual.files = ../doc/user_manual/build/html
#INSTALLS    += manual

#Install the html doc files
#manualtopics.path = $${OUT_PWD}/../$${DIST_DIR}/doc/html
#manualtopics.files = ../doc/user_manual/source/topicslist.txt
#INSTALLS    += manualtopics

android {
DISTFILES += \
    ../../build/Debug/dist/android-source/AndroidManifest.xml \
    ../../build/Debug/dist/android-source/gradle/wrapper/gradle-wrapper.jar \
    ../../build/Debug/dist/android-source/gradlew \
    ../../build/Debug/dist/android-source/res/values/libs.xml \
    ../../build/Debug/dist/android-source/build.gradle \
    ../../build/Debug/dist/android-source/gradle/wrapper/gradle-wrapper.properties \
    ../../build/Debug/dist/android-source/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../../build/Debug/dist/android-source
}


!win32 {
    # Because of Eigen:
    QMAKE_CXXFLAGS += -Wno-int-in-bool-context

    # Because of macros and clang:
    QMAKE_CXXFLAGS += -Wno-extra-semi-stmt
}

