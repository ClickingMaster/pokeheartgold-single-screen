#pragma force_active on
#include "global.h"
typedef struct{u16 a,b;int c,d;volatile u16 ticks,flag;} State;
typedef struct{u8 pad[8];u8 state;} Work;
extern void ov43_0222D654(State*,Work*,void*,void*),ov43_0222D4C4(State*),ov43_0222D87C(State*,void*),ov43_0222A318(Work*,int,int),ov43_0222D778(State*,void*),PlaySE(int);
extern int ov43_0222D47C(State*),ov43_0222D508(State*,Work*,void*,void*),ov43_0222DCC4(Work*,int);
int ov43_0222D15C(State*,Work*,void*,void*);
int ov43_0222D15C(State*s,Work*w,void*p,void*q){switch(w->state){case 0:s->flag=0;s->a=0;case 1:ov43_0222D654(s,w,p,q);w->state=2;break;case 2:w->state=ov43_0222D47C(s);break;case 3:ov43_0222D4C4(s);ov43_0222D508(s,w,p,q);w->state=4;break;case 4:if(ov43_0222D508(s,w,p,q)==1)w->state=2;break;case 5:if(s->flag){s->ticks--;if(s->ticks)break;}if(ov43_0222DCC4(w,s->d)){PlaySE(0x5e5);ov43_0222D87C(s,p);ov43_0222A318(w,4,1);s->flag=1;return 1;}w->state=2;break;case 6:ov43_0222D778(s,p);ov43_0222A318(w,1,9);return 1;}return 0;}
