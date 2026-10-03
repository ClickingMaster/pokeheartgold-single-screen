#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0xc];void*arg;u32 pad,state;u8 p2[4];u8 first[0x20],second[4];} Work;
extern int ov102_021EC450(void*),ov102_021EC2EC(void*);
extern void ov102_021EC410(void*,int,int,int,int);
int ov102_021EBAD0(Work*);
int ov102_021EBAD0(Work*w){switch(w->state){case 0:if(ov102_021EC2EC(w->second)){ov102_021EC410(w->first,2,0,-10,16);w->state++;}break;case 1:return ov102_021EC450(w->first);}return 0;}
