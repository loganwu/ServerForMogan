#!/bin/sh

echo "BUILD_ROOT_PATH=`pwd`" > ./BUILD_ROOT

SCRPT_PATH=`pwd`
PATH=$PATH:$SCRPT_PATH/bin/
chmod +x ./bin/*
PROTO_SRC_PATH=./ConndServerSrc/protocol/
PROTO_CLIENT_SRC_PATH=./ClientSrc/protocol/
#create protocal tdr file
function create_protocal_file
{
	cd $PROTO_SRC_PATH && for file in `ls *.proto`;do protoc --cpp_out=. $file;done 
	find . -name "*.cc"|xargs rename cc cpp
}

function create_client_protocal_file
{
	cd $PROTO_CLIENT_SRC_PATH && for file in `ls *.proto`;do protoc --cpp_out=. $file;done 
	find . -name "*.cc"|xargs rename cc cpp
}

cd $SCRPT_PATH && create_protocal_file
cd $SCRPT_PATH && create_client_protocal_file
