#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 *ov13_0224F544;
extern void ov13_0223237C(void);
void ov13_022325A4(void);
void ov13_022325A4(void){u8 *p;int i=0;int j;int offset=i;int space=32,digit=48,zero=i;do{p=ov13_0224F544+8+offset;j=zero;do{u8 c=p[j];if(c!=digit&&c!=space&&c!=zero)break;p[j]=j==2?digit:space;j++;}while(j<3);i++;offset+=3;}while(i<4);ov13_0223237C();}
