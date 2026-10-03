#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov96_021F8128(void*,void*,void*);
void ov96_021F8160(u8*w,void*a,void*b);
void ov96_021F8160(u8*w,void*a,void*b){int i;u8*p;i=0;p=w+4;for(;i<4;i++,p+=16)ov96_021F8128(p,a,b);}
