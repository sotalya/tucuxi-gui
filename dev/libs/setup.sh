
SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

#cd $SCRIPTPATH/xerces-c
#./configure --disable-network --enable-transcoder-iconv CFLAGS=-fPIC CXXFLAGS=-fPIC
#make -j8

cd $SCRIPTPATH/xerces-c
mkdir build
cd build
cmake -Dnetwork:BOOL=OFF -Dtranscoder=iconv -DBUILD_SHARED_LIBS:BOOL=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON  ..
make -j$(nproc)

$SCRIPTPATH/tucuxi-core/setup.sh

