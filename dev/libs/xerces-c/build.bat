@echo off

REM Check if the number of argument is correct
set res=F
if "%1" EQU "" set res=T
if "%2" NEQ "" set res=T
if "%res%" EQU "T" (
  if "%1" EQU "" echo No argument given!!
  if "%2" NEQ "" echo Too many arguments given!!
  echo Rerun build.bat with only one of the following arguments
  echo    - release : To build the library in release mode
  echo    - debug   : To build the library in debug mode
  echo    - all     : To build the library in both release and debug mode
  goto exit
)

REM Check what argument was given
if /i "%1" EQU "release" (
  set build_config="release"
  echo Release selected!!!
) else if /i "%1" EQU "debug" (
    set build_config="debug"
    echo Debug selected!!!
  ) else if "%1" EQU "all" (
      set build_config="all"
      echo All selected!!!
    )

echo =============================================================================
echo =========================== Creating build folder ===========================
echo =============================================================================
mkdir build
cd build

echo ==============================================================================
echo ============================== Building xerces-c =============================
echo ==============================================================================
cmake -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX=C:\xerces-c -DBUILD_SHARED_LIBS:BOOL=OFF -Dnetwork:BOOL=OFF -Dtranscoder=iconv ..

if %build_config% EQU "debug" (
  goto debug
)

:release
echo ==============================================================================
echo =============================== Release library ==============================
echo ==============================================================================
cmake --build . --config Release
echo ==============================================================================
echo ========================== Testing xerces-c release ==========================
echo ==============================================================================
ctest -V -C Release -j 4 --stop-on-failure
echo ==============================================================================
echo ========================= Installing xerces-c release ========================
echo ==============================================================================
cmake --build . --config Release --target install
echo ==============================================================================
echo ======================== xerces-c release installed!!! =======================
echo ==============================================================================

if %build_config% EQU "release" (
  goto end
)

:debug
echo ==============================================================================
echo ================================ Debug library ===============================
echo ==============================================================================
cmake --build . --config Debug
echo ==============================================================================
echo =========================== Testing xerces-c debug ===========================
echo ==============================================================================
ctest -V -C Debug -j 4 --stop-on-failure
echo ==============================================================================
echo ========================== Installing xerces-c debug =========================
echo ==============================================================================
cmake --build . --config Debug --target install
echo ==============================================================================
echo ========================= xerces-c debug installed!!! ========================
echo ==============================================================================

:end
cd ..

:exit
