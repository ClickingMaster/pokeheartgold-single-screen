#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef union{u16 raw;struct{u16 beacon:1,broadcast:1,preamble:1,antenna:1,diversity:1,invert:1,indicate:1,nullkey:1,rest:8;}b;} F35422;
typedef struct{u8 before[0x33a];F35422 flags;u8 after[0x500-0x33c];} W35422;
#define G (*(W35422*volatile*)0x0380fff4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define D(p,n) (*(u32*)((u8*)(p)+(n)))
#define R(n) (*(volatile u16*)(n))
extern void __VENEER_MIi_CpuClear16(u32,void*,u32),__VENEER_MIi_CpuCopy16(const void*,void*,u32),WSetMacAdrs1(void*,const void*),WSetPowerMgtMode(u32),WSetTxTimeStampOffset(void),WSetFrameLifeTime(u32),WL_WriteByte(void*,u32),BBP_Write(u32,u32);extern u32 WL_ReadByte(const void*);

u32 WSetBeaconType(u32 x);
// @fn WSetBeaconType
u32 WSetBeaconType(u32 x){if(x>1)return 5;{W35422*w=G;w->flags.raw=(w->flags.raw&~1u)|(x&1);}return 0;}
