#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <stdarg.h>
extern OSMutex __cs[];
extern u32 __cs_id[],__cs_ref[];
static inline void lock(int index) {
 if(!OS_TryLockMutex(&__cs[index])) {__cs_id[index]=OS_GetCurrentThread()->id;__cs_ref[index]=1;}
 else if(__cs_id[index]==OS_GetCurrentThread()->id) __cs_ref[index]++;
 else {OS_LockMutex(&__cs[index]);__cs_id[index]=OS_GetCurrentThread()->id;__cs_ref[index]=1;}
}
static inline void unlock(int index) {if(--__cs_ref[index]==0)OS_UnlockMutex(&__cs[index]);}
extern u8 __files[];
extern int fwide(void*,int);
extern int __FileWrite(void*,const char*,u32);
extern int __pformatter(int(*)(void*,const char*,u32),void*,const char*,va_list);
int printf(const char*,...);
int printf(const char *format,...) {
 va_list args;int result;
 if(fwide(__files+0x4c,-1)>=0)return -1;
 lock(3);va_start(args,format);
 result=__pformatter(__FileWrite,__files+0x4c,format,args);
 va_end(args);unlock(3);return result;
}
