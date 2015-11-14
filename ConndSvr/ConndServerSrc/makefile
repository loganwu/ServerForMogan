#        Version:  1.0
#        Created:  03/27/2014 
#       Revision:  none
#       Compiler:  g++
#         Editor:  vim (ts=4,sw=4,et)
#         Author:  Logan
#        History:
#               1. Date:
#                  Author:
#                  Modification:

include ../makefile.mk
include ../BUILD_ROOT
CFLAGS += -ggdb -DDEBUG
OK = \\e[1m\\e[32m OK \\e[m
FAILURE = \\e[1m\\e[31m FAILURE \\e[m
.PHONY: all clean cleandep

GAME_BIN = ../bin/Connd
all: $(GAME_BIN)
######################################################################

EXCLUDE_DIRS := tools 
DIRS := $(shell find . -maxdepth 2 -type d |egrep -v '\.svn')
DIRS := $(basename $(patsubst ./%,%,$(DIRS)))
DIRS := $(filter-out $(EXCLUDE_DIRS),$(DIRS))

SRCS := $(foreach it,$(DIRS),$(wildcard $(it)/*.cpp)) 
OBJS =$(patsubst %.cpp,%.o,$(SRCS))

INCS = $(INC_EXT) 
INCS += $(foreach it, $(DIRS), -I./$(it))
	
LIBS = $(LIB_EXT) 

clean:
	rm  -f $(GAME_BIN) $(OBJS) 

cleandep:
	rm -f $(SRCS:.cpp=.d)

sinclude $(SRCS:.cpp=.d) 

%.d:%.cpp 
	@set -e; rm -f $@; \
	echo -ne make dep $<  ... ; \
	${CC} -MM -MT $*.o ${INCS} $< > $@.$$$$ && echo  -e $(OK) || echo -e $(FAILURE); \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$;

$(GAME_BIN): $(OBJS) 
	#echo "${LIBS}" 
	@echo -ne Linking $(GAME_BIN) ... 
	@$(CC) -o $(GAME_BIN) $(CFLAGS) $^ $(INCS) $(LIBS) && echo  -e $(OK) || echo -e $(FAILURE)

%.o: %.cpp 
	@echo -ne Compiling $<  ... 
	@$(CC) $(CFLAGS) $(DEBUG) -o $@ -c $< $(INCS) && echo  -e $(OK) || echo -e $(FAILURE)

