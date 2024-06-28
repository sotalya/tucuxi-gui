@ECHO off
SETLOCAL
SET _release_name=build-Tucuxi-Desktop_Qt_6_5_2_MSVC2019-Release
SET _demo_name=build-Tucuxi-Desktop_Qt_6_5_2_MSVC2019-Release-demo
SET _extreport_name=build-Tucuxi-Desktop_Qt_6_5_2_MSVC2019-Release-extreport
SET _extreport_nolicense_name=build-Tucuxi-Desktop_Qt_6_5_2_MSVC2019-Release-extreport-nolicense

SET script_path=%~dp0

REM Set tucuxi GUI path
SET tucuxi_gui_path=%script_path%\..\..\..\..\

REM Check if the number of argument is correct
IF "%1" EQU "" (
  ECHO No argument given!! Defaulting to compiling all!
  SET compile_config="all"
)
IF "%2" NEQ "" (
  ECHO ERROR: Too many arguments given!!
  GOTO help
)

ECHO =============================================================================
ECHO ======================= Compiling and Deploying TUCUXI ======================
ECHO =============================================================================
CALL %script_path%/compile.bat %1
CALL %script_path%/deploy_setup.bat %1
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
  IF /i "%1" EQU "release" (
    RMDIR /S /Q %tucuxi_gui_path%\dev\%_release_name%
  )
  IF /i "%1" EQU "demo" (
    RMDIR /S /Q %tucuxi_gui_path%\dev\%_demo_name%
  ) 
  IF "%1" EQU "extreport" (
    RMDIR /S /Q %tucuxi_gui_path%\dev\%_extreport_name%
  ) 
  IF "%1" EQU "extreport-nolicense" (
    RMDIR /S /Q %tucuxi_gui_path%\dev\%_extreport_nolicense_name%
  )
  GOTO exit

:help
  ECHO To use this script you will need to run it with none or one of the following argument
  ECHO    release             : To compile and deploy Tucuxi in release mode
  ECHO    demo                : To compile and deploy Tucuxi in demo mode
  ECHO    extreport           : To compile and deploy Tucuxi in external report mode
  ECHO    extreport-nolicense : To compile and deploy Tucuxi in external report no license mode
  ECHO    all                 : To compile and deploy Tucuxi in all mode (default mode)

:exit
  PAUSE
  ENDLOCAL