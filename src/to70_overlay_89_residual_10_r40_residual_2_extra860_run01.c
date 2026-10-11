#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x94];volatile int timer;int data[4];u8 rest[0x198];union{u32 flags;struct{u32 mode:4;u32 extra:28;};};}Work;
extern void ov89_0225ADA4(void*,Work*,void*,int),ModelInstance_SetVisible(void*,int),ov89_0225AED0(void*,Work*),ov89_0225AFC0(Work*,int);extern int ov89_0225AFFC(Work*);
int ov89_0225B010(void*,Work*);
int ov89_0225B010(void*p,Work*w){int*t=(int*)&w->timer;w->timer++;if(w->timer==2){ov89_0225ADA4((u8*)w+0x1c,w,t+1,0);ModelInstance_SetVisible((u8*)w+0x1c,1);w->mode=1;}if(*t>40){ov89_0225AED0(p,w);ov89_0225AFC0(w,ov89_0225AFFC(w));}return 0;}
