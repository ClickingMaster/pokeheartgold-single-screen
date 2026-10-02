#pragma force_active on
#include "global.h"
#pragma force_active on
int ov89_0225AEA8(const void*,const void*);int ov89_0225B2BC(void*,u8*,u8*);
int ov89_0225B2BC(void*a,u8*b,u8*c){u8*q=c+148;int i=0;u8*p=b+512;for(;i<4;i++){if(ov89_0225AEA8(p,q+4)==1)return 1;p+=16;}return 0;}
