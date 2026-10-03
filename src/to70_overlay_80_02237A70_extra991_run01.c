#pragma force_active on
#include "global.h"
#pragma force_active on
extern u16 ov80_02237A70(u32,u32,int);void ov80_02237ADC(u32,u32,u16 *,int);
void ov80_02237ADC(u32 a,u32 b,u16 *out,int count){int i=0;u16 *dst=out;do{int j;*dst=ov80_02237A70(a,b,i);for(j=0;j<i;j++){if(out[j]==out[i])break;}if(j==i){dst++;i++;}}while(i<count);}
