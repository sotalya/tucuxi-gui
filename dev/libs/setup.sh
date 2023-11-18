
SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

cd $SCRIPTPATH/xerces-c
./configure --disable-network --enable-transcoder-iconv CFLAGS=-fPIC CXXFLAGS=-fPIC
make -j8


$SCRIPTPATH/tucuxi-core/setup.sh

