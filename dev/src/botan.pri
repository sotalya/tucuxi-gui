!config_nobotan {

#Paths and linker flags for Windows
win32 {
#    exists(C:/Botan-1.10.5) {
#        LIBS += -LC:/Botan-1.10.5/lib -lbotan-1.10
#        INCLUDEPATH += C:/Botan-1.10.5/include/botan-1.10
#    } else {
#        error("Error: Impossible to find botan")
#    }
    32bit {
        exists(C:/Botan/Botan2-32) {
            CONFIG(debug, debug|release) {
                LIBS += -LC:\Botan\botan2-32\lib -lbotand
            }
            CONFIG(release, debug|release) {
                LIBS += -LC:\Botan\botan2-32\lib -lbotan
            }
            INCLUDEPATH += C:\Botan\botan2-32\include\botan-2
        }
        else {
            error("Error: Botan2-32 directory not found in C:/Botan")
        }
    }
    else {
        exists(C:/Botan/Botan2-64) {
            CONFIG(debug, debug|release) {
                LIBS += -LC:\Botan\botan2-64\lib -lbotan
            }
            CONFIG(release, debug|release) {
                LIBS += -LC:\Botan\botan2-64\lib -lbotan
            }
            INCLUDEPATH += C:\Botan\botan2-64\include\botan-2
        }
        else {
            error("Error: Botan-64 directory not found in C:/Botan")
        }
    }
}


#Paths and linker flags for Linux/MacOS
!android {
    unix {
        exists($${TUCUXIROOTDIR}/bin/botan.a) {
            LIBS += $${TUCUXIROOTDIR}/bin/botan.a
            INCLUDEPATH += $${TUCUXIROOTDIR}/libs/botan-2.19.3/build/include/
        }
        else:exists($${TUCUXIROOTDIR}/libs/botan-2.19.3) {
            macx {
                LIBS += -L $${TUCUXIROOTDIR}/libs/botan-2.19.3 -lbotan
            }
            else {
                LIBS += $${TUCUXIROOTDIR}/libs/botan-2.19.3/objs/botan.a
            }
            INCLUDEPATH += $${TUCUXIROOTDIR}/libs/botan-2.19.3/build/include/
        }
        else:exists(/usr/include/botan-1.10) {
            LIBS           += -L/usr/lib -lbotan-1.10
            INCLUDEPATH    += /usr/include/botan-1.10
        }
        else:exists(/usr/include/botan-1.11) {
            LIBS           += -L/usr/lib -lbotan-1.11
            INCLUDEPATH    += /usr/include/botan-1.11
        }
        else:exists(../../../ezechiel-deps-botan) {
            LIBS           += -L../../../ezechiel-deps-botan -lbotan-1.11
            INCLUDEPATH += ../../../ezechiel-deps-botan/build/include/botan
        }
        else {
            error("Error: Impossible to find botan")
        }
    }
}

} # config_nobotan
