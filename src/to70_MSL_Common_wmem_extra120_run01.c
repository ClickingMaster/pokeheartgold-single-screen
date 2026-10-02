#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
void*memcpy(void*,const void*,u32);u16*wmemcpy(u16*,const u16*,u32);u16*wmemchr(const u16*,u16,u32);
u16*wmemcpy(u16*d,const u16*s,u32 n){return memcpy(d,s,n*2);}
u16*wmemchr(const u16*p,u16 c,u32 n){if(n){do{if(*p==c)return (u16*)p;p++;}while(--n);}return 0;}
