#ifndef  CACHEDEF_INC
#define  CACHEDEF_INC

typedef int HMEM;
#define INVALID 0 
#define IS_VALID_HMEM(h) ((h)>=1 && (h)<=size)
#define GET_MB(h) (IS_VALID_HMEM(h)?&(mb[h-1]):NULL)

#endif   /* ----- #ifndef CACHEDEF_INC  ----- */

