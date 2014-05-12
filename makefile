#       Compiler:  g++
#         Author:  Logan
#        History:
#               1. Date:
#                  Author:
#                  Modification:

CC = g++
CFLAGS += -pg -g
DEP_INCS = -I/usr/include/mysql/ -I/usr/local/homebrew/include/mysql
DEP_LIBS = -L /usr/lib64/mysql/ -L /usr/local/homebrew/lib
GAME_BIN = ./bin/MainGame

OK = \\e[1m\\e[32m OK \\e[m
FAILURE = \\e[1m\\e[31m FAILURE \\e[m
.PHONY: all clean cleandep
all: $(GAME_BIN)

######################################################################

EXCLUDE_DIRS := tools 
DIRS := $(shell find . -maxdepth 2 -type d |egrep -v '\.svn')
DIRS := $(basename $(patsubst ./%,%,$(DIRS)))
DIRS := $(filter-out $(EXCLUDE_DIRS),$(DIRS))

SRCS := $(foreach it,$(DIRS),$(wildcard $(it)/*.cpp)) 
OBJS =$(patsubst %.cpp,%.o,$(SRCS))

INCS = $(DEP_INCS) 
INCS += $(foreach it, $(DIRS), -I./$(it))
	
LIBS = $(DEP_LIBS) -lmysqlclient

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
	echo "${DIRS}" 
	@echo -ne Linking $(GAME_BIN) ... 
	@$(CC) -o $(GAME_BIN) $(CFLAGS) $^ $(INCS) $(LIBS) && echo  -e $(OK) || echo -e $(FAILURE)

%.o: %.cpp
	@echo -ne Compiling $<  ... 
	@$(CC) $(CFLAGS) $(DEBUG) -o $@ -c $< $(INCS) && echo  -e $(OK) || echo -e $(FAILURE)

