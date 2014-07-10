#!/bin/sh

PWD_PATH=`pwd`
EXT_LIB_PATH=$PWD_PATH/../extlib/libevent
INSTALL_PATH=$PWD_PATH/installDir

function copy_lib
{
	cd $INSTALL_PATH 
	cp lib/libevent.a $EXT_LIB_PATH/lib
	cp -rf include/*  $EXT_LIB_PATH/include
}

./configure --prefix=$INSTALL_PATH && make && make install
echo "copy libevent to $EXT_LIB_PATH"
copy_lib 

