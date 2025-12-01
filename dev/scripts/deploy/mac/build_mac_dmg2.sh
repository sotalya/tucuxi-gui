
#!/bin/sh

# Check if required build directory (where 'dist' lives) is in first argument or environment variable
if [ $# -eq 1 ]
  then
    export TUCUXI_BUILD_DIR=$1
  else
    : ${TUCUXI_BUILD_DIR?"Need to set TUCUXI_BUILD_DIR or supply single build dir path argument."}
fi

ABSPATH=$(cd "$(dirname "$0")";pwd -P)
TUCUXI_GLROOT=${ABSPATH}/../../../..

# Check if required environment variables are set
: ${TUCUXI_DRUGS_DIR?"Need to set TUCUXI_DRUGS_DIR"}
# : ${BOTAN_LIB}?"Need to set BOTAN_LIB to libbotan.dylib"}

# Helper variable
TUCUXI_SRC_DIR=${TUCUXI_GLROOT}/dev/src

MACDEPLOYQT=/opt/Qt/6.10.1/macOS/bin/macdeployqt
QMAKE=/opt/Qt/6.10.1/macOS/bin/qmake

# Verify qmake and macdeployqt are in PATH.
# The macdeployqt creates the bundle and dmg installer
command -v ${QMAKE} >/dev/null 2>&1 || { echo >&2 "I require qmake but it's not installed.  Aborting."; exit 1; }
command -v ${MACDEPLOYQT} >/dev/null 2>&1 || { echo >&2 "I require macdeployqt but it's not installed.  Aborting."; exit 1; }

# Get into the build directory
cd ${TUCUXI_BUILD_DIR}

# Execute qmake with a bunch of args specifying no license and build for mac
${QMAKE} ${TUCUXI_SRC_DIR}/tucuxi.pro -spec macx-clang CONFIG+=x86_64 CONFIG+=qml_debug CONFIG+=NOLICENSE CONFIG+=qtquickcompiler DRUGSPATH=${TUCUXI_DRUGS_DIR} 

# Build Tucuxi
make -j8
# make install

BUNDLE=${TUCUXI_BUILD_DIR}/dist/tucuxi.app
# Execute macdeployqt with args to get the right qml files, make a dmg installer
${MACDEPLOYQT} ${BUNDLE} -always-overwrite -qmldir=${TUCUXI_SRC_DIR}/guiutils/qml -dmg

