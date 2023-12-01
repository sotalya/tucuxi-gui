@ECHO off
SETLOCAL
SET _release_name=build-Tucuxi-Desktop_Qt_6_5_2_MSVC2019-Release
SET _demo_name=build-Tucuxi-Desktop_Qt_6_5_2_MSVC2019-Release-demo
SET _extreport_name=build-Tucuxi-Desktop_Qt_6_5_2_MSVC2019-Release-extreport
SET _extreport_nolicense_name=build-Tucuxi-Desktop_Qt_6_5_2_MSVC2019-Release-extreport-nolicense

SET script_path=%~dp0

REM Set tucuxi GUI path
SET tucuxi_gui_path=%script_path%\..\..\..\..\
REM Set tucuxi drug path
SET tucuxi_drugs_path=%tucuxi_gui_path%\..\tucuxi-drugs

REM Check if the number of argument is correct
IF "%1" EQU "" (
  ECHO No argument given!! Defaulting to compiling all!
  SET compile_config="all"
)
IF "%2" NEQ "" (
  ECHO ERROR: Too many arguments given!
  GOTO help
)

ECHO =============================================================================
ECHO ============================== Compiling TUCUXI =============================
ECHO =============================================================================

REM Check what argument was given
IF /i "%1" EQU "release" (
  SET compile_config="release"
  ECHO Release selected!!!
  GOTO release
) ELSE IF /i "%1" EQU "demo" (
    SET compile_config="demo"
    ECHO Demo selected!!!
    GOTO demo
  ) ELSE IF "%1" EQU "extreport" (
      SET compile_config="extreport"
      ECHO Extreport selected!!!
      GOTO extreport
    ) ELSE IF "%1" EQU "extreport-nolicense" (
        SET compile_config="extreport-nolicense"
        ECHO Extreport-nolicense selected!!!
        GOTO extreport-nolicense
      ) ELSE IF "%1" EQU "all" (
          SET compile_config="all"
          ECHO All selected!!!
        )

:release
  ECHO =============================================================================
  ECHO ============================= Compiling Release =============================
  ECHO =============================================================================
  MKDIR %tucuxi_gui_path%\dev\%_release_name%
  CD %tucuxi_gui_path%\dev\%_release_name%
  DEL /S /Q *
  C:\Qt\6.5.2\msvc2019_64\bin\qmake.exe %tucuxi_gui_path%\dev\src\tucuxi.pro -spec win32-msvc CONFIG+=config_tucucore_full DRUGSPATH=%tucuxi_drugs_path%\drugfiles DEFINES+=NOLICENSE && C:\Qt\Tools\QtCreator\bin\jom\jom.exe qmake_all
  C:\Qt\Tools\QtCreator\bin\jom\jom.exe
  ECHO =============================================================================
  ECHO ============================= Release Compiled! =============================
  ECHO =============================================================================

  IF %compile_config% EQU "release" (
    GOTO exit
  )

:demo
  ECHO =============================================================================
  ECHO =============================== Compiling Demo ==============================
  ECHO =============================================================================
  MKDIR %tucuxi_gui_path%\dev\%_demo_name%
  CD %tucuxi_gui_path%\dev\%_demo_name%
  DEL /S /Q *
  C:\Qt\6.5.2\msvc2019_64\bin\qmake.exe %tucuxi_gui_path%\dev\src\tucuxi.pro -spec win32-msvc CONFIG+=config_tucucore_full DRUGSPATH=%tucuxi_drugs_path%\drugfiles CONFIG+=config_demo DEFINES+=NOLICENSE && C:\Qt\Tools\QtCreator\bin\jom\jom.exe qmake_all
  C:\Qt\Tools\QtCreator\bin\jom\jom.exe
  ECHO =============================================================================
  ECHO =============================== Demo Compiled! ==============================
  ECHO =============================================================================

  IF %compile_config% EQU "demo" (
    GOTO exit
  )

:extreport
  ECHO =============================================================================
  ECHO ============================ Compiling Extreport ============================
  ECHO =============================================================================
  MKDIR %tucuxi_gui_path%\dev\%_extreport_name%
  CD %tucuxi_gui_path%\dev\%_extreport_name%
  DEL /S /Q *
  C:\Qt\6.5.2\msvc2019_64\bin\qmake.exe %tucuxi_gui_path%\dev\src\tucuxi.pro -spec win32-msvc CONFIG+=config_tucucore_full DRUGSPATH=%tucuxi_drugs_path%\drugfiles CONFIG+=config_externalreport && C:\Qt\Tools\QtCreator\bin\jom\jom.exe qmake_all
  C:\Qt\Tools\QtCreator\bin\jom\jom.exe
  ECHO =============================================================================
  ECHO ============================ Extreport Compiled! ============================
  ECHO =============================================================================

  IF %compile_config% EQU "extreport" (
    GOTO exit
  )

:extreport-nolicense
  ECHO =============================================================================
  ECHO ======================= Compiling Extreport noLicense =======================
  ECHO =============================================================================
  MKDIR %tucuxi_gui_path%\dev\%_extreport-nolicense_name%
  CD %tucuxi_gui_path%\dev\%_extreport-nolicense_name%
  DEL /S /Q *
  C:\Qt\6.5.2\msvc2019_64\bin\qmake.exe %tucuxi_gui_path%\dev\src\tucuxi.pro -spec win32-msvc DEFINES+=NOLICENSE CONFIG+=config_tucucore_full DRUGSPATH=%tucuxi_drugs_path%\drugfiles CONFIG+=config_externalreport && C:\Qt\Tools\QtCreator\bin\jom\jom.exe qmake_all
  C:\Qt\Tools\QtCreator\bin\jom\jom.exe
  ECHO =============================================================================
  ECHO ======================= Extreport noLicense Compiled! =======================
  ECHO =============================================================================

  IF %compile_config% EQU "extreport-nolicense" (
    GOTO exit
  )

:help
  ECHO To use this script you will need to run it with none or one of the following argument
  ECHO    release             : To compile Tucuxi in release mode
  ECHO    demo                : To compile Tucuxi in demo mode
  ECHO    extreport           : To compile Tucuxi in external report mode
  ECHO    extreport-nolicense : To compile Tucuxi in external report no license mode
  ECHO    all                 : To compile Tucuxi in all mode (default mode)

:exit
  ENDLOCAL