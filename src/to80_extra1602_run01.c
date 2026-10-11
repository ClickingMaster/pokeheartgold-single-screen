#pragma force_active on
#include "global.h"
typedef struct {s16 x,y;u8 pad4[0x10];int value;u8 pad18[0xc];}Curve;
typedef struct {u8 state,x1;s16 height;int param;u8 x8,timer,delay,xb;s32 first,second;int limit;void *ctx,*pic;Curve curve,other;}Work;
extern int ov07_0222260C(Curve *),ov07_02222674(int,int,int);
extern void ov07_022227D8(Curve *),Pokepic_SetAttr(void *,int,int),Pokepic_AddAttr(void *,int,int),ov07_02222590(Curve *,int,int,int,int,int,int),BattleAnimSystem_DestroyTask(void *,void *),Heap_Free(void *);
void ov07_02224EF4(void *,Work *);
#define UPDATE() Pokepic_SetAttr(w->pic,12,w->curve.x);Pokepic_SetAttr(w->pic,13,w->curve.y);Pokepic_SetAttr(w->pic,1,w->height+ov07_02222674(w->height,w->param,w->curve.value))
void ov07_02224EF4(void *task,Work *w){switch(w->state){case 0:if(ov07_0222260C(&w->curve)==1){ov07_022227D8(&w->other);Pokepic_SetAttr(w->pic,12,w->curve.x);Pokepic_SetAttr(w->pic,13,w->curve.y);Pokepic_AddAttr(w->pic,0,w->other.x);Pokepic_SetAttr(w->pic,1,w->height+ov07_02222674(w->height,w->param,w->curve.value));}else w->state++;break;
case 1:w->timer++;if(w->timer>w->delay){ov07_02222590(&w->curve,(s16)((w->first>>16)&0xffff),(s16)(w->first&0xff),(s16)((w->second>>16)&0xffff),(s16)(w->second&0xff),100,w->limit);w->state++;}break;
case 2:if(ov07_0222260C(&w->curve)==1){UPDATE();}else w->state++;break;
case 3:if(ov07_0222260C(&w->curve)==1){UPDATE();}else{Pokepic_SetAttr(w->pic,12,256);Pokepic_SetAttr(w->pic,13,256);Pokepic_SetAttr(w->pic,1,w->height);w->state++;}break;
default:BattleAnimSystem_DestroyTask(w->ctx,task);Heap_Free(w);break;}}
