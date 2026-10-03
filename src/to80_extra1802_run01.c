#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov90_0225B53C(void*,int,int,u8);
void ov90_02258B2C(u8*w,int a,int b);
void ov90_02258B2C(u8*w,int a,int b){if(!w[21])ov90_0225B53C(w+488,a,b,w[20]);}
