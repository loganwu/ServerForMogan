// =====================================================================================
// 
//       Filename:  CacheDef.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/26/2009 02:49:47 PM CST
//       Revision:  none
//       Compiler:  g++
//         Editor:  vim (ts=4,sw=4,et)
//         Author:  Jaco Wu (), jacowu@tencent
//        Company:  Tencent
// 
//        History:
//               1. Date:
//                  Author:
//                  Modification:
// =====================================================================================

#ifndef  CACHEDEF_INC
#define  CACHEDEF_INC

typedef int HMEM;
#define INVALID 0 
#define IS_VALID_HMEM(h) ((h)>=1 && (h)<=size)
#define GET_MB(h) (IS_VALID_HMEM(h)?&(mb[h-1]):NULL)

#endif   /* ----- #ifndef CACHEDEF_INC  ----- */
/* vi:set ts=4: */ 

