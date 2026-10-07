#pragma force_active on
#include <stdarg.h>
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern int __StringRead(void*,int,int),__sformatter(int(*)(void*,int,int),void*,const char*,void*,int);
int vsscanf(const char*,const char*,va_list);
int vsscanf(const char *text,const char *format,va_list args){struct {const char *text;int field;} data;data.text=text;if(!text||!*text)return -1;data.field=0;return __sformatter(__StringRead,&data,format,args,0);}
