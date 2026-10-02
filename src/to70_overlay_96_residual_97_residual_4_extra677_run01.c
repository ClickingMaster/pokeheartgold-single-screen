#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void*system,*bg,*task;s8 distance,ticks;u8 pad[2];u32 sign;} Work;
extern void ScheduleSetBgPosText(void*,int,int,int);
extern void* SpriteSystem_GetRenderer(void*);
extern void G2dRenderer_SetMainSurfaceCoords(void*,int,int);
extern void SysTask_Destroy(void*);
void ov96_0220D360(void*task,Work*w);
void ov96_0220D360(void*task,Work*w){int direction,offset;if(w->sign==0)direction=1;else direction=-1;offset=w->distance*direction;ScheduleSetBgPosText(w->bg,1,3,offset+8);ScheduleSetBgPosText(w->bg,0,3,offset);G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(w->system),0,-offset<<12);if(w->ticks%2==1)w->distance--;w->sign^=1;w->ticks--;if(w->ticks<=0){ScheduleSetBgPosText(w->bg,1,3,8);ScheduleSetBgPosText(w->bg,0,3,0);G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(w->system),0,0);w->task=0;SysTask_Destroy(task);}}
