#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 state;u8 p[3];void *owner,*pic;u32 params[3];u32 a,b;int type,mode;s16 x,y;} State;
extern int ov07_0221BFD0(void *),ov07_0221C4A8(void *,int),ov07_0221C470(void *),ov07_0221C468(void *),ov07_02222004(void *,int);extern void *Heap_Alloc(int,u32),*ov07_0221FA48(void *,int);extern void ov07_022223F0(void *,int,int,int),Pokepic_SetAttr(void *,int,int),ov07_0221C410(void *,void (*)(void),void *),ov07_02223A1C(void);
void ov07_02223A94(void *);
void ov07_02223A94(void *owner){State *s=Heap_Alloc(ov07_0221BFD0(owner),44);int mode,index,sign,value;s->state=0;s->owner=owner;s->mode=ov07_0221C4A8(owner,2);s->type=ov07_0221C4A8(owner,0);mode=ov07_0221C4A8(owner,3);if(mode==0||mode==2)index=ov07_0221C470(s->owner);else index=ov07_0221C468(s->owner);ov07_022223F0(s->params,s->type,ov07_0221C4A8(owner,1),s->mode);sign=ov07_02222004(owner,index);s->params[2]*=sign;if(mode==2)sign*=-1;s->pic=ov07_0221FA48(s->owner,index);if(ov07_0221C4A8(owner,3)==1){s->x=ov07_0221C4A8(owner,4);value=ov07_0221C4A8(owner,5);}else{value=40;s->x=value*sign;}s->y=value;Pokepic_SetAttr(s->pic,10,s->x);Pokepic_SetAttr(s->pic,11,s->y);ov07_0221C410(s->owner,ov07_02223A1C,s);}
