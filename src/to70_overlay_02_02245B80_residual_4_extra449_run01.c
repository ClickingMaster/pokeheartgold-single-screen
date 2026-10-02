#pragma force_active on
#include "global.h"
#include "camera.h"
#pragma force_active on
typedef struct C{u8 pad[36];Camera*camera;}C;typedef struct W{u8 pad[4];C*ctx;s16 x,y;u16 padc,duration,current,angle;fx32 step;Camera*camera;u8 gap[4];VecFx32 target,original;}W;void ov02_02246548(W*,int,int,u16,u16);
void ov02_02246548(W*w,int x,int y,u16 duration,u16 angle){VecFx32 target;w->x=(fx32)(x>0?0.5f+(float)(x<<12):(float)(x<<12)-0.5f);w->y=(fx32)(y>0?0.5f+(float)(y<<12):(float)(y<<12)-0.5f);w->duration=duration;w->angle=angle;w->current=0;w->step=FX_Div(90<<14,(fx32)(w->angle>0?0.5f+(float)(w->angle<<12):(float)(w->angle<<12)-0.5f));Camera_Copy(w->ctx->camera,w->camera);target=Camera_GetLookAtCamTarget(w->camera);w->target=target;w->original=w->target;Camera_SetFixedTarget(&w->target,w->camera);Camera_SetStaticPtr(w->camera);}
