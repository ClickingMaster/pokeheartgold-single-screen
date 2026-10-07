#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 ov112_021FFA18[];typedef struct State{u8 flag;u8 pad[0x2b];u32 retry;}State;extern State _021FF9E0;extern u32 ov112_021E5900(void*,u32);extern void ov112_021E5938(u32);
int ov112_021E5B28(u32);
#pragma opt_rotateloops off
int ov112_021E5B28(u32 size){u8*p=ov112_021FFA18;if(size==1){if(p[0]==252)return 1;return 0;}if(size<4)return 0;u16 sum=p[2]+(p[3]<<8);p[2]=0;p[3]=0;u32 actual=ov112_021E5900(p,size);p[2]=sum;p[3]=sum>>8;if(sum==actual){_021FF9E0.flag=0;return 1;}if(_021FF9E0.retry){u32 i=0;goto test;loop:i++;test:if(i<size)goto loop;ov112_021E5938(i);}return 0;}
