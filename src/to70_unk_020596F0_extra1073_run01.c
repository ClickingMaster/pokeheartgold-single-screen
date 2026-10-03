#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x4c];void *context;u8 p2[0x86-0x50];u16 state;u8 p3,flags;}Work;
extern int sub_02059B08(void),sub_02037088(int,void*,int),sub_02036FD8(int,void*,int);

int sub_020596F0(Work*w);

int sub_020596F0(Work*w){int result;if((w->flags&1)==0){void*context=w->context;int value=sub_02059B08();if(w->state==0)result=sub_02037088(106,context,value);else result=sub_02036FD8(106,context,value);if(result)w->flags|=1;}else result=1;return result;}
