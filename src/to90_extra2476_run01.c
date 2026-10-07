#pragma force_active on
#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct {void*bg;int layer,axis,pos,target,speed;u16 ticks,done;} Work;

void ov102_021EC298(Work*,void*,int,int,int,int);
void ov102_021EC298(Work*w,void*bg,int layer,int axis,int delta,int ticks){w->bg=bg;w->layer=layer;if(axis==0){w->pos=Bg_GetXpos((const BgConfig*)bg,(GFBgLayer)layer);w->axis=0;}else{w->pos=Bg_GetYpos((const BgConfig*)bg,(GFBgLayer)layer);w->axis=3;}w->target=(int)(((u32)w->pos+(u32)delta)&511u);delta=(s32)((u32)delta<<12);w->pos=(s32)((u32)w->pos<<12);w->speed=delta/ticks;w->ticks=ticks;w->done=0;}
