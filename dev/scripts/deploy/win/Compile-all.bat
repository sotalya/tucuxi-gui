
REM "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall" amd64

mkdir C:\Projects\Ezechiel\dev\build-ezechiel-tucucore-Desktop_Qt_5_7_1_MSVC2015_64bit-Release
cd C:\Projects\Ezechiel\dev\build-ezechiel-tucucore-Desktop_Qt_5_7_1_MSVC2015_64bit-Release
DEL /S /Q *
C:\Qt\Qt5.7.1\5.7\msvc2015_64\bin\qmake.exe C:\Projects\Ezechiel\dev\src\ezechiel.pro -spec win32-msvc2015 CONFIG+=config_tucucore_full DRUGSPATH=E:\docs\tucuxi\tucuxi-drugs\drugfiles && C:/Qt/Qt5.7.1/Tools/QtCreator/bin/jom.exe qmake_all
C:/Qt/Qt5.7.1/Tools/QtCreator/bin/jom.exe

mkdir C:\Projects\Ezechiel\dev\build-ezechiel-tucucore-Desktop_Qt_5_7_1_MSVC2015_64bit-Release-Demo
cd C:\Projects\Ezechiel\dev\build-ezechiel-tucucore-Desktop_Qt_5_7_1_MSVC2015_64bit-Release-Demo
DEL /S /Q *
C:\Qt\Qt5.7.1\5.7\msvc2015_64\bin\qmake.exe C:\Projects\Ezechiel\dev\src\ezechiel.pro -spec win32-msvc2015 CONFIG+=config_tucucore_full DRUGSPATH=D:\docs\tucuxi\tucuxi-drugs\drugfiles CONFIG+=config_demo && C:/Qt/Qt5.7.1/Tools/QtCreator/bin/jom.exe qmake_all
C:/Qt/Qt5.7.1/Tools/QtCreator/bin/jom.exe

mkdir C:\Projects\Ezechiel\dev\build-ezechiel-tucucore-Desktop_Qt_5_7_1_MSVC2015_64bit-Release-extreport
cd C:\Projects\Ezechiel\dev\build-ezechiel-tucucore-Desktop_Qt_5_7_1_MSVC2015_64bit-Release-extreport
DEL /S /Q *
C:\Qt\Qt5.7.1\5.7\msvc2015_64\bin\qmake.exe C:\Projects\Ezechiel\dev\src\ezechiel.pro -spec win32-msvc2015 CONFIG+=config_tucucore_full DRUGSPATH=E:\docs\tucuxi\tucuxi-drugs\drugfiles CONFIG+=config_externalreport && C:/Qt/Qt5.7.1/Tools/QtCreator/bin/jom.exe qmake_all
C:/Qt/Qt5.7.1/Tools/QtCreator/bin/jom.exe


mkdir C:\Projects\Ezechiel\dev\build-ezechiel-tucucore-Desktop_Qt_5_7_1_MSVC2015_64bit-Release-extreport-nolicense
cd C:\Projects\Ezechiel\dev\build-ezechiel-tucucore-Desktop_Qt_5_7_1_MSVC2015_64bit-Release-extreport-nolicense
DEL /S /Q *
C:\Qt\Qt5.7.1\5.7\msvc2015_64\bin\qmake.exe C:\Projects\Ezechiel\dev\src\ezechiel.pro -spec win32-msvc2015 CONFIG+=config_tucucore_full CONFIG+=NOLICENSE DRUGSPATH=E:\docs\tucuxi\tucuxi-drugs\drugfiles CONFIG+=config_externalreport && C:/Qt/Qt5.7.1/Tools/QtCreator/bin/jom.exe qmake_all
C:/Qt/Qt5.7.1/Tools/QtCreator/bin/jom.exe
