#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0xc];void*arg;u32 pad,state;u8 p2[4];u8 first[0x20],second[4];} Work;
extern int ov102_021EC450(void*),ov102_021EC2EC(void*);
extern void ov102_021EC298(void*,void*,int,int,int,int);
int ov102_021EBA5C(Work*);
int ov102_021EBA5C(Work*w){switch(w->state){case 0:if(ov102_021EC450(w->first)){ov102_021EC298(w->second,w->arg,1,1,0,6);w->state++;}break;case 1:return ov102_021EC2EC(w->second);}return 0;}
