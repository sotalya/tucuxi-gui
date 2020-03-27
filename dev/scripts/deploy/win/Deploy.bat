
REM Architecture is 32 or 64 bits
set "ARCH=%1%"

REM Target is Debug or Release
set "TARGET=%2%" 

set PRJ_DIR=E:\docs\tucuxi\tucuxi-gui\dev
set SRC_DIR=%PRJ_DIR%\src
set DIST_DIR=%PRJ_DIR%\build-ezechiel-tucucore-Desktop_Qt_5_7_1_MSVC2015_%ARCH%bit-%TARGET%\dist\
set DEPLOY_DIR=%PRJ_DIR%\Deploy
set INSTALL_DIR=C:\Tucuxi

set ZIP_EXE="c:\Program Files\7-Zip\7z.exe"

set BOTANT_DIR=C:\botan\botan2-%ARCH%\lib
set VISUAL_STUDIO_DIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0
set QT_DIR=C:\Qt\Qt5.7.1\5.7\msvc2015_%ARCH%
set NSIS_DIR=C:\Program Files (x86)\NSIS

REM Create the deployment structure
rd /s /q %DEPLOY_DIR%\%ARCH%\%TARGET%
mkdir %DEPLOY_DIR%
mkdir %DEPLOY_DIR%\%ARCH%
mkdir %DEPLOY_DIR%\%ARCH%\%TARGET%
mkdir %DEPLOY_DIR%\%ARCH%\%TARGET%\dbs
REM mkdir %DEPLOY_DIR%\%ARCH%\%TARGET%\drugs
mkdir %DEPLOY_DIR%\%ARCH%\%TARGET%\drugs2
mkdir %DEPLOY_DIR%\%ARCH%\%TARGET%\requests
mkdir %DEPLOY_DIR%\%ARCH%\%TARGET%\reports

REM Copy the application
copy %DIST_DIR%\tucuxi.exe %DEPLOY_DIR%\%ARCH%\%TARGET%\Tucuxi.exe
xcopy /s %DIST_DIR%\dbs %DEPLOY_DIR%\%ARCH%\%TARGET%\dbs\
copy %SRC_DIR%\core\xml\definitions.xml %DEPLOY_DIR%\%ARCH%\%TARGET%\

REM Copy drugs
REM copy %SRC_DIR%\drugs\*.xml %DEPLOY_DIR%\%ARCH%\%TARGET%\drugs\
REM copy %SRC_DIR%\drugs\*.xsl %DEPLOY_DIR%\%ARCH%\%TARGET%\drugs\
REM copy %SRC_DIR%\drugs\*.css %DEPLOY_DIR%\%ARCH%\%TARGET%\drugs\
copy %DIST_DIR%\drugs2\*.tdd %DEPLOY_DIR%\%ARCH%\%TARGET%\drugs2\
copy %DIST_DIR%\reports\* %DEPLOY_DIR%\%ARCH%\%TARGET%\reports\

REM Copy openssl libs
copy libeay32.dll %DEPLOY_DIR%\%ARCH%\%TARGET%
copy ssleay32.dll %DEPLOY_DIR%\%ARCH%\%TARGET%

REM Copy request samples
copy %SRC_DIR%\requests\*.xml %DEPLOY_DIR%\%ARCH%\%TARGET%\requests\

REM Copy report files
copy %SRC_DIR%\guiutils\reports\*.* %DEPLOY_DIR%\%ARCH%\%TARGET%\reports\

REM Copy Visual C++ librairies
set VCREDISTSUBDIR=x64
IF %ARCH%==32 set VCREDISTSUBDIR=x86
copy "%VISUAL_STUDIO_DIR%\VC\redist\%VCREDISTSUBDIR%\Microsoft.VC140.CRT\*.dll" %DEPLOY_DIR%\%ARCH%\%TARGET%
copy "%VISUAL_STUDIO_DIR%\VC\redist\%VCREDISTSUBDIR%\Microsoft.VC140.CXXAMP\*.dll" %DEPLOY_DIR%\%ARCH%\%TARGET%
copy "%VISUAL_STUDIO_DIR%\VC\redist\%VCREDISTSUBDIR%\Microsoft.VC140.OpenMP\*.dll" %DEPLOY_DIR%\%ARCH%\%TARGET%

REM Copy external libraries
copy %BOTANT_DIR%\botan.dll %DEPLOY_DIR%\%ARCH%\%TARGET%

REM Launch QT deployment app
%QT_DIR%\bin\windeployqt --qmldir %SRC_DIR% %DEPLOY_DIR%\%ARCH%\%TARGET%\Tucuxi.exe

REM Add QtWebEngine stuff which is missed by windeployqt!
xcopy /s %QT_DIR%\qml\QtWebEngine %DEPLOY_DIR%\%ARCH%\%TARGET%\QtWebEngine\

REM Create a zip file
rm %DEPLOY_DIR%\Tucuxi-%ARCH%-%TARGET%.zip
%ZIP_EXE% a -r %PRJ_DIR%\Tucuxi-%ARCH%-%TARGET%.zip %DEPLOY_DIR%\%ARCH%\%TARGET%\*

REM Create setup.exe
"%NSIS_DIR%\makensis.exe" /DARCH=%ARCH% /DTARGET=%TARGET% %PRJ_DIR%\Tucuxi.nsi 

REM Run setup.exe!
REM rd /s /q %INSTALL_DIR%
REM %PRJ_DIR%\Setup-%TARGET%%ARCH%.exe /S

