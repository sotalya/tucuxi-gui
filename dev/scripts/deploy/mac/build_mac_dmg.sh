#!/bin/sh

# Check if required build directory (where 'dist' lives) is in first argument or environment variable
if [ $# -eq 1 ]
  then
    export TUCUXI_BUILD_DIR=$1
  else
    : ${TUCUXI_BUILD_DIR?"Need to set TUCUXI_BUILD_DIR or supply single build dir path argument."}
fi

# Check if required environment variables are set
: ${TUCUXI_GLROOT?"Need to set TUCUXI_GLROOT"}
: ${TUCUXI_ROOT?"Need to set TUCUXI_ROOT"}
: ${TUCUXI_DRUGS_DIR?"Need to set TUCUXI_DRUGS_DIR"}
# : ${BOTAN_LIB}?"Need to set BOTAN_LIB to libbotan.dylib"}

# Helper variable
export TUCUXI_SRC_DIR=${TUCUXI_GLROOT}/dev/src

# Verify qmake and macdeployqt are in PATH.
# The macdeployqt creates the bundle and dmg installer
command -v qmake >/dev/null 2>&1 || { echo >&2 "I require qmake but it's not installed.  Aborting."; exit 1; }
command -v macdeployqt >/dev/null 2>&1 || { echo >&2 "I require macdeployqt but it's not installed.  Aborting."; exit 1; }
command -v install_name_tool >/dev/null 2>&1 || { echo >&2 "I require install_name_tool but it's not installed. This comes with the mac dev command line tools. Aborting."; exit 1; }

# Get into the build directory
cd ${TUCUXI_BUILD_DIR}

# Execute qmake with a bunch of args specifying no license and build for mac
qmake ${TUCUXI_SRC_DIR}/tucuxi.pro -spec macx-clang CONFIG+=x86_64 CONFIG+=qml_debug DEFINES+=NOLICENSE CONFIG+=qtquickcompiler CONFIG+=config_nobotan DRUGSPATH=${TUCUXI_DRUGS_DIR} && /usr/bin/make qmake_all

# Build Tucuxi
make -j8
make install

export BUNDLE=${TUCUXI_BUILD_DIR}/dist/tucuxi.app
export TARGET_BINARY=${BUNDLE}/Contents/MacOS/tucuxi
# Execute macdeployqt with args to get the right qml files, make a dmg installer
macdeployqt ${BUNDLE} -verbose=3 -always-overwrite -executable=${TARGET_BINARY} -qmldir=${TUCUXI_SRC_DIR}/guiutils/qml

# cp -r ${TUCUXI_DRUGS_DIR} ${BUNDLE}/Contents/Resources/drugfiles
cp -r ${TUCUXI_BUILD_DIR}/dist/dbs ${BUNDLE}/Contents/Resources
# cp ${BOTAN_LIB} ${BUNDLE}/Contents/Frameworks

# Change the paths in the executable to be able to find the Frameworks (shared libs)
install_name_tool -rpath @executable_path/Frameworks @executable_path/../Frameworks ${TARGET_BINARY}

echo "Generating the dmg image from the bundle folder..."
sudo hdiutil create Tucuxi.dmg -volname "Tucuxi" -srcfolder ${BUNDLE}

