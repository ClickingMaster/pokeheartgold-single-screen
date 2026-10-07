#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef union{u16 raw;struct{u16 beacon:1,broadcast:1,preamble:1,antenna:1,diversity:1,invert:1,indicate:1,nullkey:1,rest:8;}b;} F35434;
typedef struct{u8 before[0x33a];F35434 flags;u8 after[0x500-0x33c];} W35434;
#define G (*(W35434*volatile*)0x0380fff4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define D(p,n) (*(u32*)((u8*)(p)+(n)))
#define R(n) (*(volatile u16*)(n))
extern void __VENEER_MIi_CpuClear16(u32,void*,u32),__VENEER_MIi_CpuCopy16(const void*,void*,u32),WSetMacAdrs1(void*,const void*),WSetPowerMgtMode(u32),WSetTxTimeStampOffset(void),WSetFrameLifeTime(u32),WL_WriteByte(void*,u32),BBP_Write(u32,u32);extern u32 WL_ReadByte(const void*);

u32 WSetBssid(const u16*p);
// @fn WSetBssid
u32 WSetBssid(const u16*p){WSetMacAdrs1((u8*)G+0x3a8,p);WSetMacAdrs1((void*)0x04808020,p);if(*p&1)R(0x048080d0)&=~0x400;else R(0x048080d0)|=0x400;return 0;}
