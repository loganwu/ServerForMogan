#!/bin/sh
for file in `ls *.cc`
do
	prefix=`echo $file|awk '{print substr($0,1,length($0)-3)}'`
	echo $prefix
	mv $file $prefix.cpp
done
