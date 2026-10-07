#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern int printf(const char*,...);
extern void abort(void);
#pragma force_active on
extern const char ov45_02255160[];
int ov45_02238024(const u8*,u32,u32,const void*,u32);
static inline const u32 *Min(const u32 *a,const u32 *b){return *a<*b?a:b;}
int ov45_02238024(const u8 *s,u32 pos,u32 length,const void *other,u32 otherLength){const u8 *data;u32 size,n,remaining;int r;if(((const struct {u32 flag:1;u32 rest:31;}*)s)->flag){data=*(u8**)(s+8);size=*(u32*)(s+4);}else{data=s+1;size=((const struct {u8 flag:1;u8 size:7;}*)s)->size;}if(pos>size){printf(ov45_02255160);abort();}remaining=size-pos;n=*Min(&length,&remaining);r=memcmp(data+pos,other,*Min(&otherLength,&n));if(r)return r;if(n<otherLength)return -1;return n!=otherLength;}
