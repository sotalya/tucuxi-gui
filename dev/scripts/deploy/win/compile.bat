@ECHO off
SETLOCAL
SET _release_name=build-Tucuxi-Desktop_Qt_6_5_3_MSVC2019-Release

SET script_path=%~dp0

REM Set tucuxi GUI path
SET tucuxi_gui_path=%script_path%..\..\..\..
REM Set tucuxi drug path
SET tucuxi_drugs_path=%tucuxi_gui_path%\..\tucuxi-drugs

ECHO =============================================================================
ECHO ============================== Compiling TUCUXI =============================
ECHO =============================================================================

REM Check what argument was given
SET compile_config="release"
GOTO release

:release
  ECHO =============================================================================
  ECHO ============================= Compiling Release =============================
  ECHO =============================================================================
  IF EXIST "%tucuxi_gui_path%\dev\%_release_name%" (
    CD /d %tucuxi_gui_path%\dev\%_release_name%
    DEL /S /Q *
  ) ELSE (
    MKDIR %tucuxi_gui_path%\dev\%_release_name%
    CD %tucuxi_gui_path%\dev\%_release_name%
  )
  
  C:\Qt\6.5.3\msvc2019_64\bin\qmake.exe %tucuxi_gui_path%\dev\src\tucuxi.pro -spec win32-msvc DRUGSPATH=%tucuxi_drugs_path%\drugfiles && C:\Qt\Tools\QtCreator\bin\jom\jom.exe qmake_all
  C:\Qt\Tools\QtCreator\bin\jom\jom.exe
  ECHO =============================================================================
  ECHO ============================= Release Compiled! =============================
  ECHO =============================================================================

  IF %compile_config% EQU "release" (
    GOTO exit
  )


:help
  ECHO To use this script just run it and it will compile the release mode of Tucuxi

:exit
  ENDLOCAL
