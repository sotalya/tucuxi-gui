@ECHO off
SETLOCAL

REM Target is Debug or Release
SET target=%1%

SET script_path=%~dp0

REM set tucuxi GUI path
SET tucuxi_gui_path = %script_path%\..\..\..\..\

SET prj_dir=%tucuxi_gui_path%\dev
SET src_dir=%prj_dir%\src
SET dist_dir=%prj_dir%\build-Tucuxi-Desktop_Qt_6_5_3_MSVC2019-%target%\dist\
SET deploy_dir=%prj_dir%\deploy
SET zip_dir=%deploy_dir%\zips
SET setup_dir=%deploy_dir%\setups

SET ZIP_EXE="C:\Program Files\7-Zip\7z.exe"

SET BOTANT_DIR=%prj_dir%\libs\tucuxi-core\libs\botan
SET VISUAL_STUDIO_DIR=C:\Program Files\Microsoft Visual Studio\2022\Community
SET QT_DIR=C:\Qt\6.5.3\msvc2019_64
SET NSIS_DIR=C:\Program Files (x86)\NSIS

REM Create the deployment structure
RD /s /q %deploy_dir%\%target%
MKDIR %deploy_dir%
MKDIR %deploy_dir%\%target%
MKDIR %deploy_dir%\%target%\dbs
MKDIR %deploy_dir%\%target%\drugfiles
MKDIR %deploy_dir%\%target%\requests
MKDIR %deploy_dir%\%target%\reports

REM Copy the application
COPY %dist_dir%\tucuxi.exe %deploy_dir%\%target%\Tucuxi.exe
XCOPY /s %dist_dir%\dbs %deploy_dir%\%target%\dbs\
COPY %src_dir%\core\xml\definitions.xml %deploy_dir%\%target%\

REM copy drugs
REM COPY %src_dir%\drugs\*.xml %deploy_dir%\%target%\drugs\
REM COPY %src_dir%\drugs\*.xsl %deploy_dir%\%target%\drugs\
REM COPY %src_dir%\drugs\*.css %deploy_dir%\%target%\drugs\
COPY %dist_dir%\drugfiles\*.tdd %deploy_dir%\%target%\drugfiles\
COPY %dist_dir%\reports\* %deploy_dir%\%target%\reports\

REM Copy openssl libs
COPY libeay32.dll %deploy_dir%\%target%
COPY ssleay32.dll %deploy_dir%\%target%

REM Copy request samples
COPY %src_dir%\requests\*.xml %deploy_dir%\%target%\requests\

REM Copy report files
COPY %src_dir%\guiutils\reports\*.* %deploy_dir%\%target%\reports\

REM Copy Visual C++ librairies
SET VCREDISTSUBDIR=x64
COPY "%VISUAL_STUDIO_DIR%\VC\Redist\MSVC\14.36.32532\x64\Microsoft.VC143.CRT\*.dll" %deploy_dir%\%target%
COPY "%VISUAL_STUDIO_DIR%\VC\Redist\MSVC\14.36.32532\x64\Microsoft.VC143.CXXAMP\*.dll" %deploy_dir%\%target%
COPY "%VISUAL_STUDIO_DIR%\VC\Redist\MSVC\14.36.32532\x64\Microsoft.VC143.OpenMP\*.dll" %deploy_dir%\%target%

REM Copy external libraries
COPY %BOTANT_DIR%\botan.dll %deploy_dir%\%target%

REM Launch QT deployment app
%QT_DIR%\bin\windeployqt --qmldir %src_dir% %deploy_dir%\%target%\Tucuxi.exe

REM Add QtWebEngine stuff which is missed by windeployqt!
XCOPY /s %QT_DIR%\qml\QtWebEngine %deploy_dir%\%target%\QtWebEngine\

REM Create a zip file
DEL %zip_dir%\Tucuxi-%target%.zip
%ZIP_EXE% a -r %zip_dir%\Tucuxi-%target%.zip %deploy_dir%\%target%\*

REM Create setup.exe
DEL %setup_dir%\Setup-%target%.exe
MKDIR %setup_dir%
"%NSIS_DIR%\makensis.exe" /DARCH=64 /DTARGET=%TARGET% %script_path%\tucuxi.nsi 

ENDLOCAL
