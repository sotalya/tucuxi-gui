#Path to the library on Unix
unix:!macx {
    LIBS += -lgsl -lgslcblas
}

#Path to the library on Windows
win32 {
    exists(C:/Qwt-6.1.0/lib) {
        LIBS        += -LC:/GSL/GSL-1.15/lib -lgsl-0 -lgslcblas-0
        INCLUDEPATH += C:/GSL/GSL-1.15/include
    } else {
        error("Error: Impossible to find GSL")
    }
}

#Path to the library on Mac OS X
macx {
    error("Error: Impossible to find GSL")
}

