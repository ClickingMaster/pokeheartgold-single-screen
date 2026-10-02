#pragma force_active on
#include "global.h"
extern void ov48_0225A4B4(void*,int),ov48_0225AD38(void*),ov48_0225A668(void*,int,int);
extern int ov48_0225A4C0(void*,int,int);
typedef struct{u8 p[60];u16 ticks,count;}Work;
int ov48_0225A43C(Work*,int,int);
int ov48_0225A43C(Work*w,int arg,int step){int done=1,i,t=w->ticks;if(t%step==0)ov48_0225A4B4(w,t/step);t=w->ticks+1;if(t<18*step)w->ticks=t;for(i=0;i<w->count;i++){if(ov48_0225A4C0(w,i,arg)==0)done=0;}if(done==1){ov48_0225AD38((u8*)w+0x1b8);ov48_0225A668((u8*)w+0x2e4,0,0);}return done;}
