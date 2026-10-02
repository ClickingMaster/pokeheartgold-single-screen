#pragma force_active on
#include "global.h"
#pragma force_active on
void ov52_021E9300(u8*,const u8*);
void ov52_021E9300(u8*out,const u8*in){int n=0,bit=0,i=0;*out=0;for(i=0;i<6144;i++){if((in[i]&15)==1)*out|=1<<bit;if((in[i]>>4)==1)*out|=1<<(bit+1);bit+=2;if(bit==8){out++;n++;*out=0;bit=0;}GF_ASSERT(n<6144);}}
