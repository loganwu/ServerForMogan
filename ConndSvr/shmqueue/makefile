CC=gcc
AR=ar rcs
CFLAGS += -ggdb
OK = \\e[1m\\e[32m OK \\e[m
FAILURE = \\e[1m\\e[31m FAILURE \\e[m
.PHONY: all clean 

TARGET_DIR=../extlib/shmqueue/lib
TARGET=$(TARGET_DIR)/libshmq.a 
all: $(TARGET)
######################################################################
DIRS :=./ 
SRCS := $(foreach it,$(DIRS),$(wildcard $(it)/*.c)) 
OBJS =$(patsubst %.c,%.o,$(SRCS))
INCS = $(foreach it, $(DIRS), -I./$(it))
	

clean:
	rm  -f $(TARGET) $(OBJS) 

$(TARGET): $(OBJS) 
	@echo -ne Linking $(TARGET) ... && echo  -e $(OK) || echo -e $(FAILURE) 
	@$(AR) $(TARGET) $(OBJS)

%.o: %.c 
	@echo -ne Compiling $<  ... 
	@$(CC) $(CFLAGS)  -o $@ -c $< $(INCS) && echo  -e $(OK) || echo -e $(FAILURE)

