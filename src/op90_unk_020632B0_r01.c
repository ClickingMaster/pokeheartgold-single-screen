#pragma force_active on
#include "global.h"
typedef struct {u8 angle;s8 x,y,z;fx32 dx,dy,dz;VecFx32 pos;} Jump;
typedef struct {s8 facing;u8 time,pad,state;fx32 height;Jump *jump;} Work;
extern Work *sub_0205F3C0(void *,int);extern void *Heap_AllocAtEnd(int,int);
extern void MapObject_CopyPositionVector(void *,VecFx32 *),sub_02060F78(void *),MapObject_SetFlagsBits(void *,u32),MapObject_SetOrQueueFacing(void *,int),sub_0205F328(void *,u16),MapObject_IncrementMovementStep(void *),PlaySE(int);
extern int MapObject_CheckVisible(void *);
void sub_020632B0(void *,int,int,s16,s8,s16,u16);
// @fn sub_020632B0
void sub_020632B0(void *object,int x,int y,s16 z,s8 facing,s16 time,u16 state){fx32 t; Work *w; w=sub_0205F3C0(object,12); w->jump=Heap_AllocAtEnd(11,sizeof(Jump)); MI_CpuFill8(w->jump,0,sizeof(Jump)); w->facing=facing; w->time=time; w->state=state; t=FX32_CONST(time); w->jump->x=x; w->jump->y=y; w->jump->z=z; w->jump->dx=FX_Div(FX32_CONST(x*16),t); w->jump->dy=FX_Div(FX32_CONST(y*16),t); w->jump->dz=FX_Div(FX32_CONST(z*16),t); w->jump->angle=180/time;MapObject_CopyPositionVector(object,&w->jump->pos);w->height=w->jump->pos.y;sub_02060F78(object);MapObject_SetFlagsBits(object,0x10004);MapObject_SetOrQueueFacing(object,facing);sub_0205F328(object,state);MapObject_IncrementMovementStep(object);if(!MapObject_CheckVisible(object))PlaySE(1546);}
