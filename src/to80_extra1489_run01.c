#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct{u8 a[5];}Row;
extern const Row ov07_02236713[],ov07_02236714[],ov07_02236715[],ov07_02236716[],ov07_02236717[];
typedef struct{u8 state,index;s16 y,angle;u16 pad;void*task,*pic;s16 x,yoff;u8 extra[16];int value;}Work;
extern void ov07_02222590(void*,s16,s16,s16,s16,int,s16),Pokepic_SetAttr(void*,int,int),ov07_0221C448(void*,void*),Heap_Free(void*);extern int ov07_0222260C(void*),ov07_02222674(int,int,int);
void ov07_022280A8(void*,Work*);
void ov07_022280A8(void*p,Work*w){int a,b,d;switch(w->state){case 0:d=ov07_02236716[w->index].a[0];a=ov07_02236713[w->index].a[0];b=ov07_02236714[w->index].a[0];ov07_02222590(&w->x,a,b,ov07_02236715[w->index].a[0],d,100,ov07_02236717[w->index].a[0]);w->index++;w->state++;break;case 1:if(ov07_0222260C(&w->x)==0){if(w->index<3)w->state--;else w->state++;}Pokepic_SetAttr(w->pic,12,w->x);Pokepic_SetAttr(w->pic,13,w->yoff);Pokepic_SetAttr(w->pic,1,w->y+ov07_02222674(w->y,w->angle,w->value));break;default:ov07_0221C448(w->task,p);Heap_Free(w);break;}}
