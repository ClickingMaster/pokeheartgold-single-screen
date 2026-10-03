#pragma force_active on
#include "global.h"
#pragma force_active on

int ov96_0220B744(u8*w);
int ov96_0220B744(u8*w){int result=1;int n=(*(u32*)(w+64)<<22)>>24;if(n!=0&&n!=1)result=0;return result;}
