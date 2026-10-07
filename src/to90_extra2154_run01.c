#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_lifetimes off
extern int STD_GetStringLength(const char*);
int STD_CopyLString(char*,const char*,int);
int STD_CopyLString(char *dst,const char *src,int count){int i;const char *p;for(i=0,p=src;i<count-1;i++,p++){dst[i]=*p;if(!*p)break;}if(i>=count-1&&count)dst[i]=0;return STD_GetStringLength(src);}
