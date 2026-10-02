#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[24];void*pic;u8 anim[20];int attr;u8 gap[12];u32 state;s16 x,y;}W;
extern int ov07_02222788(void*,void*);extern void ov07_022226C4(void*,int,int,int,int),ov07_02222590(void*,int,int,int,int,int,int),Pokepic_SetAttr(void*,int,int);int ov07_0222EE14(W*);
int ov07_0222EE14(W*w){int result=0;switch(w->state){case 0:if(ov07_02222788(w->anim,w->pic))ov07_022226C4(w->pic,w->x,w->y,w->attr,0);else{w->state++;ov07_02222590(w->anim,9,11,8,15,10,2);}break;case 1:if(ov07_02222788(w->anim,w->pic))ov07_022226C4(w->pic,w->x,w->y,w->attr,0);else{w->state++;ov07_02222590(w->anim,11,10,15,10,10,2);}break;case 2:if(ov07_02222788(w->anim,w->pic))ov07_022226C4(w->pic,w->x,w->y,w->attr,0);else{Pokepic_SetAttr(w->pic,12,256);Pokepic_SetAttr(w->pic,13,256);w->state++;result=1;}break;case 3:result=1;break;}return result;}
