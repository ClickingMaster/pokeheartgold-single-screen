#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef union{u16 raw;struct{u16 beacon:1,broadcast:1,preamble:1,antenna:1,diversity:1,invert:1,indicate:1,nullkey:1,rest:8;}b;} F35431;
typedef struct{u8 before[0x33a];F35431 flags;u8 after[0x500-0x33c];} W35431;
#define G (*(W35431*volatile*)0x0380fff4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define D(p,n) (*(u32*)((u8*)(p)+(n)))
#define R(n) (*(volatile u16*)(n))
extern void __VENEER_MIi_CpuClear16(u32,void*,u32),__VENEER_MIi_CpuCopy16(const void*,void*,u32),WSetMacAdrs1(void*,const void*),WSetPowerMgtMode(u32),WSetTxTimeStampOffset(void),WSetFrameLifeTime(u32),WL_WriteByte(void*,u32),BBP_Write(u32,u32);extern u32 WL_ReadByte(const void*);

u32 WSetDiversity(u32 x,u32 y);
// @fn WSetDiversity
u32 WSetDiversity(u32 x,u32 y){if(x>1||y>1)return 5;switch(x){case 0:{W35431*w=G;w->flags.raw=(w->flags.raw&~32u)|(((u32)(u16)y<<31)>>26);}break;case 1:{W35431*w=G;if(H(w,0x32e)!=1)return 11;w->flags.raw&=~32u;}break;}{W35431*w=G;w->flags.raw=(w->flags.raw&~16u)|(((u32)(u16)x<<31)>>27);}{W35431*w=G;R(0x04808290)=w->flags.b.invert^w->flags.b.antenna;}return 0;}
