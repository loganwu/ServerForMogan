#####################################################
PLATFORM=linux

EXT_LIB_ROOT=${BUILD_ROOT_PATH}/extlib

MYSQL_ROOT= /usr/local/mysql
INC_MYSQL = -I$(MYSQL_ROOT)/include/mysql
INC_LIBEVENT= -I$(EXT_LIB_ROOT)/libevent/include/
INC_PROBUFEVENT= -I$(EXT_LIB_ROOT)/protobuf/include/
INC_SHMQUEUE= -I$(EXT_LIB_ROOT)/shmqueue/include/

INC_EXT =  $(INC_LIBEVENT) $(INC_PROBUFEVENT) $(INC_SHMQUEUE)

LIB_MYSQL = -L$(MYSQL_ROOT)/lib/mysql -lmysqlclient -lz
ifeq ($(PLATFORM),macosx)
	LIB_EVENT= -L$(EXT_LIB_ROOT)/libevent/lib/ -levent 
else #linux
	LIB_EVENT= -L$(EXT_LIB_ROOT)/libevent/lib/ -levent -lrt #linux libevent depends on librt
endif
LIB_PROBUF= -L$(EXT_LIB_ROOT)/protobuf/lib/ -lprotobuf 
LIB_SHMQUEUE= -L$(EXT_LIB_ROOT)/shmqueue/lib -lshmq

LIB_EXT = $(LIB_EVENT) $(LIB_PROBUF) $(LIB_SHMQUEUE)

CC              = g++
CXX             = g++ 
RANLIB          = ranlib
AR              = ar

#debug with generate call graph profile data
CFLAGS = -g -DDEBUG -Wall -Wextra -pipe 

#release 
#CFLAGS += -g -Wall -Wextra -O3 -pipe 

#####################################################

