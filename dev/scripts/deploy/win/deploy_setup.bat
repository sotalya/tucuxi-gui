@ECHO off
SETLOCAL

SET script_path=%~dp0

REM Set tucuxi GUI path
SET tucuxi_gui_path=%script_path%\..\..\..\..\

REM Set build path 
SET tucuxi_build_path=%tucuxi_gui_path%\dev\deploy\build

REM Check if the number of argument is correct
IF "%1" EQU "" (
  ECHO No argument given!! Defaulting to deploying all!
  SET deploy_config="all"
)
IF "%2" NEQ "" (
  ECHO ERROR: Too many arguments given!!
  GOTO help
)

ECHO =============================================================================
ECHO ============================== Deploying TUCUXI =============================
ECHO =============================================================================

REM Check what argument was given
IF /i "%1" EQU "release" (
  SET deploy_config="release"
  ECHO Release selected!!!
  GOTO release
) ELSE IF /i "%1" EQU "demo" (
    SET deploy_config="demo"
    ECHO Demo selected!!!
    GOTO demo
  ) ELSE IF "%1" EQU "extreport" (
      SET deploy_config="extreport"
      ECHO Extreport selected!!!
      GOTO extreport
    ) ELSE IF "%1" EQU "extreport-nolicense" (
        SET deploy_config="extreport-nolicense"
        ECHO Extreport-nolicense selected!!!
        GOTO extreport-nolicense
      ) ELSE IF "%1" EQU "all" (
          SET deploy_config="all"
          ECHO All selected!!!
        )


:release
  ECHO =============================================================================
  ECHO ============================= Deploying Release =============================
  ECHO =============================================================================
  CALL %script_path%\deploy.bat Release
  ECHO =============================================================================
  ECHO ============================= Release Deployed! =============================
  ECHO =============================================================================

  IF %deploy_config% EQU "release" (
    GOTO exit
  )

:demo
  ECHO =============================================================================
  ECHO =============================== Deploying Demo ==============================
  ECHO =============================================================================
  CALL %script_path%\deploy.bat Release-Demo
  ECHO =============================================================================
  ECHO =============================== Demo Deployed! ==============================
  ECHO =============================================================================

  IF %deploy_config% EQU "demo" (
    GOTO exit
  )

:extreport
  ECHO =============================================================================
  ECHO ============================ Deploying Extreport ============================
  ECHO =============================================================================
  CALL %script_path%\deploy.bat Release-extreport
  ECHO =============================================================================
  ECHO ============================ Extreport Deployed! ============================
  ECHO =============================================================================

  IF %deploy_config% EQU "extreport" (
    GOTO exit
  )

:extreport-nolicense
  ECHO =============================================================================
  ECHO ======================= Deploying Extreport noLicense =======================
  ECHO =============================================================================
  CALL %script_path%\deploy.bat Release-extreport-nolicense
  ECHO =============================================================================
  ECHO ======================= Extreport noLicense Deployed! =======================
  ECHO =============================================================================

  IF %deploy_config% EQU "extreport-nolicense" (
    GOTO exit
  )


:help
  ECHO To use this script you will need to run it with none or one of the following argument
  ECHO    release             : To deploy Tucuxi in release mode
  ECHO    demo                : To deploy Tucuxi in demo mode
  ECHO    extreport           : To deploy Tucuxi in external report mode
  ECHO    extreport-nolicense : To deploy Tucuxi in external report no license mode
  ECHO    all                 : To deploy Tucuxi in all mode (default mode)

:exit
  ENDLOCAL
