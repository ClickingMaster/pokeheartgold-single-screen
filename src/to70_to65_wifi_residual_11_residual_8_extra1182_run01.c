#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include <string.h>
// clang-format on
typedef struct{u32 ip;u8 mac[6];u16 tick;}Entry;
extern Entry _021D46A0[8];extern u16 _021D46AA[];extern u32 _021D4620[];
extern int ip_islocal(u32);extern int is_multicast(u32);
void reg_arpcache(void*,u32,int);
void reg_arpcache(void*mac,u32 ip,int create){u16 tick;u32 i,best;int age;Entry*p;
 if(ip==0x7f000001||ip==_021D4620[20])return;if(!ip_islocal(ip))return;if(is_multicast(ip))return;
 tick=(u16)(OS_GetTick()>>16);p=_021D46A0;for(i=0;i<8;i++,p++){if(ip==p->ip){_021D46AA[i*6]=tick;MI_CpuCopy8(mac,_021D46A0[i].mac,6);return;}}
 if(!create)return;age=0;p=_021D46A0;best=0;for(i=0;i<8;i++,p++){if(!p->ip){best=i;break;}if(age<(s16)(tick-p->tick)){best=i;age=(u16)(tick-p->tick);}}
 _021D46A0[best].ip=ip;MI_CpuCopy8(mac,_021D46A0[best].mac,6);_021D46AA[best*6]=tick;}
