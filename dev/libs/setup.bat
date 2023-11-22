@ECHO off
SETLOCAL 

SET scriptpath=%~dp0

REM Check if the number of argument is correct
IF "%1" EQU "" (
  ECHO No argument given!! Defaulting to building all!
  SET build_config="all"
)
IF "%2" NEQ "" (
  ECHO Too many arguments given!!
  ECHO Rerun build.bat with only one of the following arguments
  ECHO    release : To build the library in release mode
  ECHO    debug   : To build the library in debug mode
  ECHO    all     : To build the library in both release and debug mode
  GOTO exit
)

ECHO =============================================================================
ECHO ================================== Xerces-c =================================
ECHO =============================================================================

REM Check what argument was given
IF /i "%1" EQU "release" (
  SET build_config="release"
  ECHO Release selected!!!
) ELSE IF /i "%1" EQU "debug" (
    SET build_config="debug"
    ECHO Debug selected!!!
  ) ELSE IF "%1" EQU "all" (
      SET build_config="all"
      ECHO All selected!!!
    )

ECHO =============================================================================
ECHO =========================== Creating build folder ===========================
ECHO =============================================================================
CD %scriptpath%/xerces-c
MKDIR build
CD build

ECHO ==============================================================================
ECHO ============================== Building xerces-c =============================
ECHO ==============================================================================
cmake -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX="%scriptpath%/xerces-c/build" -DBUILD_SHARED_LIBS:BOOL=OFF -Dnetwork:BOOL=OFF -Dtranscoder=iconv ..

IF %build_config% EQU "debug" (
  GOTO debug
)

:release
ECHO ==============================================================================
ECHO =============================== Release library ==============================
ECHO ==============================================================================
CMAKE --build . --config Release
ECHO ==============================================================================
ECHO ========================== Testing xerces-c release ==========================
ECHO ==============================================================================
CTEST -V -C Release -j 4 --stop-on-failure
ECHO ==============================================================================
ECHO ========================= Installing xerces-c release ========================
ECHO ==============================================================================
CMAKE --build . --config Release --target install
ECHO ==============================================================================
ECHO ======================== xerces-c release installed!!! =======================
ECHO ==============================================================================

IF %build_config% EQU "release" (
  GOTO end
)

:debug
ECHO ==============================================================================
ECHO ================================ Debug library ===============================
ECHO ==============================================================================
CMAKE --build . --config Debug
ECHO ==============================================================================
ECHO =========================== Testing xerces-c debug ===========================
ECHO ==============================================================================
CTEST -V -C Debug -j 4 --stop-on-failure
ECHO ==============================================================================
ECHO ========================== Installing xerces-c debug =========================
ECHO ==============================================================================
CMAKE --build . --config Debug --target install
ECHO ==============================================================================
ECHO ========================= xerces-c debug installed!!! ========================
ECHO ==============================================================================

:end
CALL "%scriptpath%/tucuxi-core/setup.bat" %1

:exit
ENDLOCAL 
