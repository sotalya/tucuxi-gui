@echo off

REM "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

REM get current location to return to path where script got launched
set mypath=%cd%

REM Set default path for the Tucuxi repos
set DefaultPath=%TUCUXI_ROOT%\..\

REM Source -> https://stackoverflow.com/questions/8666225/how-to-test-if-a-path-is-a-file-or-directory-in-windows-batch-file
REM Check if a path as been provided, if not use the base path
for /f "tokens=1,2 delims=d" %%A in ("-%~a1") do if "%%B" neq "" (
  echo %1 is a folder  
  goto :set_path
) else (
  echo No input path specified or "%1" is either a file or does not exist
  echo Default path %DefaultPath% will be used
  goto :set_default
)

:set_default
set FolderPath=%DefaultPath%
goto :continue

:set_path
set FolderPath=%1
goto :continue

:continue
REM Release
mkdir %FolderPath%\tucuxi-gui\dev\build-ezechiel-tucucore-Desktop_Qt_6_5_2_MSVC2019_64bit-Release
cd %FolderPath%\tucuxi-gui\dev\build-ezechiel-tucucore-Desktop_Qt_6_5_2_MSVC2019_64bit-Release
DEL /S /Q *s
C:\Qt\6.5.2\msvc2019_64\bin\qmake.exe %FolderPath%\tucuxi-gui\dev\src\tucuxi.pro -spec win32-msvc CONFIG+=config_tucucore_full DRUGSPATH=%FolderPath%\tucuxi-drugs\drugfiles DEFINES+=NOLICENSE && C:\Qt\Tools\QtCreator\bin\jom\jom.exe qmake_all
C:\Qt\Tools\QtCreator\bin\jom\jom.exe

REM Release-Demo
mkdir %FolderPath%\tucuxi-gui\dev\build-ezechiel-tucucore-Desktop_Qt_6_5_2_MSVC2019_64bit-Release-Demo
cd %FolderPath%\tucuxi-gui\dev\build-ezechiel-tucucore-Desktop_Qt_6_5_2_MSVC2019_64bit-Release-Demo
DEL /S /Q *
C:\Qt\6.5.2\msvc2019_64\bin\qmake.exe %FolderPath%\tucuxi-gui\dev\src\tucuxi.pro -spec win32-msvc CONFIG+=config_tucucore_full DRUGSPATH=%FolderPath%\tucuxi-drugs\drugfiles CONFIG+=config_demo DEFINES+=NOLICENSE && C:\Qt\Tools\QtCreator\bin\jom\jom.exe qmake_all
C:\Qt\Tools\QtCreator\bin\jom\jom.exe

REM Release-extreport
mkdir %FolderPath%\tucuxi-gui\dev\build-ezechiel-tucucore-Desktop_Qt_6_5_2_MSVC2019_64bit-Release-extreport
cd %FolderPath%\tucuxi-gui\dev\build-ezechiel-tucucore-Desktop_Qt_6_5_2_MSVC2019_64bit-Release-extreport
DEL /S /Q *
C:\Qt\6.5.2\msvc2019_64\bin\qmake.exe %FolderPath%\tucuxi-gui\dev\src\tucuxi.pro -spec win32-msvc CONFIG+=config_tucucore_full DRUGSPATH=%FolderPath%\tucuxi-drugs\drugfiles CONFIG+=config_externalreport && C:\Qt\Tools\QtCreator\bin\jom\jom.exe qmake_all
C:\Qt\Tools\QtCreator\bin\jom\jom.exe

REM Release-extreport-nolicense
mkdir %FolderPath%\tucuxi-gui\dev\build-ezechiel-tucucore-Desktop_Qt_6_5_2_MSVC2019_64bit-Release-extreport-nolicense
cd %FolderPath%\tucuxi-gui\dev\build-ezechiel-tucucore-Desktop_Qt_6_5_2_MSVC2019_64bit-Release-extreport-nolicense
DEL /S /Q *
C:\Qt\6.5.2\msvc2019_64\bin\qmake.exe %FolderPath%\tucuxi-gui\dev\src\tucuxi.pro -spec win32-msvc DEFINES+=NOLICENSE CONFIG+=config_tucucore_full DRUGSPATH=%FolderPath%\tucuxi-drugs\drugfiles CONFIG+=config_externalreport && C:\Qt\Tools\QtCreator\bin\jom\jom.exe qmake_all
C:\Qt\Tools\QtCreator\bin\jom\jom.exe

REM Return to the folder where the script was executed
cd %mypath%
