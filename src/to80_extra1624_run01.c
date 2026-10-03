#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8*volatile ov00_0221B9DC;
int ov00_02212DB4(u32,u32,int);
int ov00_022139F0(u32*,u32*);
int ov00_022139F0(u32*a,u32*b){u32 av=*(volatile u32*)a;u8*ctx=ov00_0221B9DC;u32 bv=*b;int left=ov00_02212DB4(av,*(u32*)(ctx+0x498),0);int right=ov00_02212DB4(bv,*(u32*)(ov00_0221B9DC+0x498),0);int diff=left-right;if(!*(u32*)(ov00_0221B9DC+0x49c))diff=-diff;return diff;}
