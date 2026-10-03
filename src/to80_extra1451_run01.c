#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 x;const u16*name,*description;void*icon1,*icon2;u32 game;u8 flag;}Entry;extern int MBi_ReadIconInfo(void*,void*,int);extern u16 mystrlen(const u16*);void MBi_MakeGameInfo(void*,Entry*,const void*);
void MBi_MakeGameInfo(void*info,Entry*e,const void*extra){int fail;u8*p=info;MIi_CpuClear16(0,info,0x4c0);p[0x4b2]=0;fail=MBi_ReadIconInfo(e->icon1,info,1)==0;fail|=MBi_ReadIconInfo(e->icon2,info,0)==0;if(fail){p[0x4b2]=1;MIi_CpuClearFast(0,info,0x220);}*(u32*)(p+0x4b8)=e->game;if(extra)MIi_CpuCopy16(extra,(u16*)(p+0x220),22);p[0x236]=e->flag;MIi_CpuCopy16(e->name,(u16*)(p+0x238),(u16)(mystrlen(e->name)*2));MIi_CpuCopy16(e->description,(u16*)(p+0x298),192);p[0x358]=1;*(u16*)(p+0x35a)=1;*(u16*)(p+0x4b0)=1;}
