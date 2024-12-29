@ECHO off
SETLOCAL
SET _release_name=build-Tucuxi-Desktop_Qt_6_5_2_MSVC2019-Release

SET script_path=%~dp0

REM Set tucuxi GUI path
SET tucuxi_gui_path=%script_path%\..\..\..\..\


ECHO =============================================================================
ECHO ======================= Compiling and Deploying TUCUXI ======================
ECHO =============================================================================
CALL %script_path%/compile.bat release
CALL %script_path%/deploy_setup.bat release
ECHO =============================================================================
ECHO ======================= TUCUXI Compiled and Deployed! =======================
ECHO =============================================================================

:choice
  SET /P c=Do you want to keep the compiled files[Y/N]?
  IF /I "%c%" EQU "Y" GOTO yes_answer
  IF /I "%c%" EQU "N" GOTO no_answer
  GOTO choice

:yes_answer
  ECHO Keeping the compiled files!
  GOTO exit

:no_answer
  echo Deleting the compiled files!
  RMDIR /S /Q %tucuxi_gui_path%\dev\%_release_name%
  GOTO exit

:help
  ECHO To use this script just run it. It will compile and deploy Tucuxi in release mode

:exit
  PAUSE
  ENDLOCAL