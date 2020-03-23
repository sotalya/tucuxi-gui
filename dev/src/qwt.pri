#Add the Qwt library to the include and libs paths
CONFIG += qwt

#Paths and linker flags for Windows
win32 {
    exists(C:/Qwt-6.1.0/lib) {                          #Native compilation
        LIBS        += -LC:/Qwt-6.1.0/lib
        INCLUDEPATH += C:/Qwt-6.1.0/include
    } else:exists(/usr/i486-mingw32/lib) {              #Cross-compilation
        LIBS        += -L/usr/i486-mingw32/lib
        INCLUDEPATH += /usr/i486-mingw32/include/qwt
    } else {
        error("Error: Impossible to find Qwt")
    }

    debug {
        LIBS += -lqwtd
    } else {
        LIBS += -lqwt
    }
}

#Paths and linker flags for MacOS
macx {
    QWT_HEADERS = qwt.framework/Headers

    exists(/usr/local/qwt-6.1.0-svn/lib/$${QWT_HEADERS}/qwt_plot.h) {
        LIBS         += -framework qwt
        INCLUDEPATH  += /usr/local/qwt-6.1.0-svn/lib/$${QWT_HEADERS}
        QMAKE_LFLAGS += -F/usr/local/qwt-6.1.0-svn/lib
    } else:exists(/usr/local/qwt-6.1.0/lib/$${QWT_HEADERS}/qwt_plot.h) {
        LIBS         += -framework qwt
        INCLUDEPATH  += /usr/local/qwt-6.1.0/lib/$${QWT_HEADERS}
        QMAKE_LFLAGS += -F/usr/local/qwt-6.1.0/lib
    } else {
        error("Error: Impossible to find Qwt")
    }
}

#Paths and linker flags for Linux
unix:!macx {
    exists(/usr/local/qwt-6.1.3-svn/include/qwt_plot.h) {
        LIBS           += -L/usr/local/qwt-6.1.3-svn/lib -lqwt
        INCLUDEPATH    += /usr/local/qwt-6.1.3-svn/include
        QMAKE_RPATHDIR += /usr/local/qwt-6.1.3-svn/lib
    } else:exists(/usr/local/qwt-6.1.1-svn/include/qwt_plot.h) {
        LIBS           += -L/usr/local/qwt-6.1.1-svn/lib -lqwt
        INCLUDEPATH    += /usr/local/qwt-6.1.1-svn/include
        QMAKE_RPATHDIR += /usr/local/qwt-6.1.1-svn/lib
    } else:exists(/usr/local/qwt-6.1.0-svn/include/qwt_plot.h) {
        LIBS           += -L/usr/local/qwt-6.1.0-svn/lib -lqwt
        INCLUDEPATH    += /usr/local/qwt-6.1.0-svn/include
        QMAKE_RPATHDIR += /usr/local/qwt-6.1.0-svn/lib
    } else:exists(/usr/local/qwt-6.1.0/include/qwt_plot.h) {
        LIBS           += -L/usr/local/qwt-6.1.0/lib -lqwt
        INCLUDEPATH    += /usr/local/qwt-6.1.0/include
        QMAKE_RPATHDIR += /usr/local/qwt-6.1.0/lib
    } else {
        error("Error: Impossible to find Qwt")
    }
}
