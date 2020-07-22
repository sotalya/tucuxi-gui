
#version check qt
!contains(QT_VERSION, ^5\\.*\\..*) {
message("Cannot build Quotations with Qt version $${QT_VERSION}.")
error("Use at least Qt 5.0.")
}

DEFINES += GIT_REVISION='\\"$$system(git rev-parse --short HEAD)\\"'

# Builds with Tucucore and use it as processing engine
DEFINES += COMPILE_WITH_TUCUCORE


config_guitest {
    DEFINES += CONFIG_GUITEST
}

# saves preprocessor output to .ii files in build dir
config_view_preproc {
    QMAKE_CXXFLAGS += -save-temps
}

config_disablepercs {
   DEFINES += DISABLEPERCS
}
# Customized project configurations
config_demo {
    DEFINES += CONFIG_DEMO
    CONFIG += config_rest
}

!config_demo {
    CONFIG += config_connected
    CONFIG += config_rest
    DEFINES += CONFIG_CONNECTED
}

config_externalreport {
    DEFINES += CONFIG_EXTERNALREPORT
}

# Common settings for the whole project
CONFIG += qt thread rtti silent c++14 warn_off embed_manifest_dll embed_manifest_exe

CXXFLAGS += -Wwrite-strings -fexceptions -Wall
CFLAGS += -fexceptions
DEFINES += QT_MESSAGELOGCONTEXT
QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
#QMAKE_CXXFLAGS_WARN_OFF += -Wunused-parameter
#Qt modules required by the core and its dependencies
QT += xml xmlpatterns network svg qml

#Depending on the patform, set the libs as static/shared
win32 {
    CONFIG -= shared
    CONFIG += static

	DEFINES += COMPILE_WINDOWS
}

macx {
    CONFIG -= shared
    CONFIG += static
}

unix:!macx {
    CONFIG -= shared
    CONFIG += static

	DEFINES += COMPILE_LINUX
}

#Separate the build directories (no more on the different plateforms, prefer shadow build)
MOC_DIR = build/moc
OBJECTS_DIR = build/obj
UI_DIR = build/ui
RCC_DIR = build/rcc
BUILD_DIR = $${OUT_PWD}
# BUILD_DIR = $$quote($${PWD}/../build)


#Set the translation directories for all plateforms
TRANS_DIR  = trans
DIST_DIR = dist

#Set the link flags of libraries for each platforms
static {
    win32 {
        SONAME=lib
    } else {
        SONAME=a
    }
} else {
    win32 {
        SONAME=dll
    } else:macx {
        SONAME=dylib
    } else:unix {
        SONAME=so
    } else {
        error("Error: Unsupported platform")
    }
}

#Set the prefix of libraries on Linux and MacOS (please note the prefix must be added on Windows
#as well when building in static mode, since qmake prefixes the produced static libraries anyway)
!win32 : LIBPRE=lib

TUCUXIROOTDIR = $$(TUCUXI_ROOT)
TUCUXIDRUGSDIR = $$(TUCUXI_DRUGS_DIR)

macx {
    DEFINES+=MACOS
}


!win32 {
    # Because of Eigen:
    QMAKE_CXXFLAGS += -Wno-int-in-bool-context

    # Because of macros and clang:
    QMAKE_CXXFLAGS += -Wno-extra-semi-stmt
}
