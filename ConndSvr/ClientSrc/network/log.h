#ifndef __LOG_HEADER__
#define __LOG_HEADER__

#include<stdio.h>
#include<syslog.h>
#include<string.h>
#include<errno.h>

#define ERROR_RETURN(op) \
    do { if ((op) < 0) return(-__LINE__); } while(0)

#define NULL_RETURN(op) \
    do { if ((op) == NULL) return(-__LINE__); } while(0)

#define LOG_ENABLE

#ifdef  LOG_ENABLE
#define logerr(fmt, args...)   do { syslog(LOG_ERR, "logerr %s.%d,%s:" fmt, __FILE__,__LINE__,strerror(errno), ##args);\
                                    fprintf(stderr, "logerr %s.%d,%s:" fmt, __FILE__,__LINE__,strerror(errno), ##args);\
                                  }while (0)
#define logdbg(fmt, args...)  do { syslog(LOG_INFO, "logdbg %s.%d:" fmt, __FILE__,__LINE__,##args);\
									fprintf(stdout, "logdbg %s.%d:" fmt, __FILE__,__LINE__, ##args);\
								  }while (0)
#else
#define logerr(fmt, args...)
#define logdbg(fmt, args...)
#endif

#endif
