#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef union{u16 raw;struct{u16 beacon:1,broadcast:1,preamble:1,antenna:1,diversity:1,invert:1,indicate:1,nullkey:1,rest:8;}b;} F35414;
typedef struct{u8 before[0x33a];F35414 flags;u8 after[0x500-0x33c];} W35414;
#define G (*(W35414*volatile*)0x0380fff4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define D(p,n) (*(u32*)((u8*)(p)+(n)))
#define R(n) (*(volatile u16*)(n))
extern void __VENEER_MIi_CpuClear16(u32,void*,u32),__VENEER_MIi_CpuCopy16(const void*,void*,u32),WSetMacAdrs1(void*,const void*),WSetPowerMgtMode(u32),WSetTxTimeStampOffset(void),WSetFrameLifeTime(u32),WL_WriteByte(void*,u32),BBP_Write(u32,u32);extern u32 WL_ReadByte(const void*);

u32 WSetMacAdrs(const u16*p);
// @fn WSetMacAdrs
u32 WSetMacAdrs(const u16*p){if(*p&1)return 5;WSetMacAdrs1((u8*)G+0x324,p);WSetMacAdrs1((void*)0x04808018,p);D(G,0x340)|=2;return 0;}
