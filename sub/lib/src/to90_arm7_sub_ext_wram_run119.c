#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef union{u16 raw;struct{u16 beacon:1,broadcast:1,preamble:1,antenna:1,diversity:1,invert:1,indicate:1,nullkey:1,rest:8;}b;} F35419;
typedef struct{u8 before[0x33a];F35419 flags;u8 after[0x500-0x33c];} W35419;
#define G (*(W35419*volatile*)0x0380fff4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define D(p,n) (*(u32*)((u8*)(p)+(n)))
#define R(n) (*(volatile u16*)(n))
extern void __VENEER_MIi_CpuClear16(u32,void*,u32),__VENEER_MIi_CpuCopy16(const void*,void*,u32),WSetMacAdrs1(void*,const void*),WSetPowerMgtMode(u32),WSetTxTimeStampOffset(void),WSetFrameLifeTime(u32),WL_WriteByte(void*,u32),BBP_Write(u32,u32);extern u32 WL_ReadByte(const void*);

u32 WSetWepMode(u32 x);
// @fn WSetWepMode
u32 WSetWepMode(u32 x){W35419*w=G;u8*s=(u8*)w+0x344;if(x>3)return 5;H(w,0x334)=x;if(x==0){H(s,0x7c)&=~0x10;H(s,0x8a)&=~0x4000;}else{H(s,0x7c)|=0x10;H(s,0x8a)|=0x4000;}if(H(s,8)==0x40&&x==1)H((void*)D(G,0x4ac),0x2e)=H(s,0x7c);if(x==0)x=1;R(0x04808006)=(R(0x04808006)&0xffc7)|(x<<3);return 0;}
