#!/bin/sh

PWD_PATH=`pwd`
EXT_LIB_NAME=protobuf
EXT_LIB_PATH=$PWD_PATH/../extlib/$EXT_LIB_NAME 
INSTALL_PATH=$PWD_PATH/installDir

copy_lib()
{
	cd $INSTALL_PATH 
	cp lib/lib$EXT_LIB_NAME.a $EXT_LIB_PATH/lib/
	cp -rf include/*  $EXT_LIB_PATH/include/
	cp bin/protoc $PWD_PATH/../bin/
}

./configure --prefix=$INSTALL_PATH && make && make install
echo "copy libevent to $EXT_LIB_PATH"
copy_lib 

