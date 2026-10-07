#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
extern u8*ov00_0221A764;extern const u8 ov00_022179AC[];void*ov00_021EC2A8(int,u32);void ov00_021EC24C(int,int),ov00_021EC2EC(int,void*,int);int ov00_021E7B70(void*,void*);int ov00_021ECD04(void);
int ov00_021ECD04(void){void*allocated;if(ov00_0221A764)OS_Terminate();allocated=ov00_021EC2A8(4,7712);ov00_0221A764=allocated;if(!allocated){ov00_021EC24C(9,-20100);return 0;}MI_CpuFill8(allocated,0,7712);MI_CpuCopy8(ov00_022179AC,ov00_0221A764+0x1c48,9);*(void*(**)(int,u32))(ov00_0221A764+0x1c54)=ov00_021EC2A8;*(void(**)(int,void*,int))(ov00_0221A764+0x1c58)=ov00_021EC2EC;ov00_021E7B70(ov00_0221A764+0x1c14,ov00_0221A764);return 1;}
