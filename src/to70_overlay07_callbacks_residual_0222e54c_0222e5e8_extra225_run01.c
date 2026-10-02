#pragma force_active on
#include "global.h"
#pragma force_active on
struct A{u8 pad[12];void*pic;u8 anim[20];int z;u8 pad28[12];int delay,state,x,y;};
int ov07_02222768(void*,void*);void ov07_022226C4(void*,s16,s16,int,int);void ov07_02222508(void*,int,int,int,int);void Pokepic_SetAttr(void*,int,int);int ov07_0222E5E8(struct A*);
struct B{int a;void*context;void*pic;s16 x,y;u8 pad[0x54-16];volatile int side;};void*ov07_022324D8(void*,int);int ov07_0221C468(void*);int ov07_02222004(void*,int);int ov07_0221C4A8(void*,int);void*ov07_0221FA48(void*,int);int Pokepic_GetAttr(void*,int);int ov07_0221C470(void*);void ov07_0222E3A4(struct B*,int);void ov07_0222E4F8(void);void ov07_0221C410(void*,void(*)(void),void*);void ov07_0222E54C(void*);
int ov07_0222E5E8(struct A*s){int done=0;switch(s->state){case 0:if(ov07_02222768(s->anim,s->pic))ov07_022226C4(s->pic,(s16)s->x,(s16)s->y,s->z,0);else s->state++;break;case 1:if(--s->delay<0){s->state++;ov07_02222508(s->anim,15,10,10,8);}break;case 2:if(ov07_02222768(s->anim,s->pic))ov07_022226C4(s->pic,(s16)s->x,(s16)s->y,s->z,0);else s->state++;break;case 3:Pokepic_SetAttr(s->pic,12,256);Pokepic_SetAttr(s->pic,13,256);done=1;break;}return done;}
