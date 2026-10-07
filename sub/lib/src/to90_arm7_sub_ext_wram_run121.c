#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef union{u16 raw;struct{u16 beacon:1,broadcast:1,preamble:1,antenna:1,diversity:1,invert:1,indicate:1,nullkey:1,rest:8;}b;} F35421;
typedef struct{u8 before[0x33a];F35421 flags;u8 after[0x500-0x33c];} W35421;
#define G (*(W35421*volatile*)0x0380fff4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define D(p,n) (*(u32*)((u8*)(p)+(n)))
#define R(n) (*(volatile u16*)(n))
extern void __VENEER_MIi_CpuClear16(u32,void*,u32),__VENEER_MIi_CpuCopy16(const void*,void*,u32),WSetMacAdrs1(void*,const void*),WSetPowerMgtMode(u32),WSetTxTimeStampOffset(void),WSetFrameLifeTime(u32),WL_WriteByte(void*,u32),BBP_Write(u32,u32);extern u32 WL_ReadByte(const void*);

u32 WSetWepKey(const u8*p);
// @fn WSetWepKey
u32 WSetWepKey(const u8*p){__VENEER_MIi_CpuCopy16(p,(void*)0x04805f80,20);__VENEER_MIi_CpuCopy16(p+20,(void*)0x04805fa0,20);__VENEER_MIi_CpuCopy16(p+40,(void*)0x04805fc0,20);__VENEER_MIi_CpuCopy16(p+60,(void*)0x04805fe0,20);return 0;}
