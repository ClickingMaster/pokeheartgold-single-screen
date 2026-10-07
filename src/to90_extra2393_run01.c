#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov88_0225918C(void*,int,int,int,int);
void ov88_02259264(void*w,int a,int b,int c,u32 mask);
#pragma opt_propagation off
void ov88_02259264(void*w,int a,int b,int c,u32 mask){if(!(mask&7)){u32 bit=mask>>3;int flag=1;if(1&bit)flag=0;ov88_0225918C(w,a,b,c,flag);}}
