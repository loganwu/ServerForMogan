#!/bin/sh

CACHE_SHM_KEY=20140808

key=`echo "obase=16;$CACHE_SHM_KEY"|bc|tr '[A-Z]' '[a-z]'`
NUM=`ipcs -m|awk '{print $1}'|grep -i $key|wc -l`
if [ $NUM -ge 1 ] 
then
    key=0x$key
    ipcrm -M $key 
    echo "shm($key) removed."
else
    echo "shm key($key) no found."
fi
