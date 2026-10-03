#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
struct State{u8 p[0x24];u32 state;u8 q[0x74];int count;};
extern struct State *ov00_0221A784;
extern void ov00_021EF3C0(int,int),ov00_021F2EE4(int,int),ov00_021F0238(int,int);
u32 ov00_021EE8B4(u32);
u32 ov00_021EE8B4(u32 code){int value,kind;if(!code)return 0;switch(code){case 1:kind=9;value=-1;break;case 2:kind=9;value=-2;break;case 3:kind=6;value=-10;break;case 4:kind=6;value=-20;break;}switch(ov00_0221A784->state){case 1:value+=-61000;ov00_021EF3C0(kind,value);break;case 2:value+=-61000;if(ov00_0221A784->count<1)ov00_021EF3C0(kind,value);break;case 5:value+=-81000;ov00_021F2EE4(kind,value);break;case 4:value+=-71000;break;default:value+=-91000;break;}ov00_021F0238(kind,value);return code;}
