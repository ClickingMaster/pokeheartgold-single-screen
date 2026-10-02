#pragma force_active on
#include "global.h"
extern void Camera_OffsetLookAtPosAndTarget(void *,void *);extern void Camera_SetLookAtCamTarget(void *,void *);extern void Camera_SetLookAtCamPos(void *,void *);extern int ov02_0224D178(void *);extern void Field3dObject_SetActiveFlag(void *,int);
typedef struct {u8 p[0x24];void *camera;} Work;
typedef struct {u8 p[0xec];s32 pos[3],target[3],delta[3];volatile s8 timer;u8 toggle,count,state;} Object;
void ov02_0224DB9C(void *,Work *,Object *);
void ov02_0224DB9C(void *unused,Work *w,Object *o){switch(o->state){case 0:if(o->count<2){if(o->timer<=0){o->timer=1;o->toggle=(o->toggle+1)%2;if(o->toggle){Camera_OffsetLookAtPosAndTarget(o->delta,w->camera);}else{Camera_SetLookAtCamTarget(o->target,w->camera);Camera_SetLookAtCamPos(o->pos,w->camera);o->count++;}}else{o->timer--;}}if(ov02_0224D178(o)==1){Field3dObject_SetActiveFlag(o,0);o->state++;}break;case 1:Camera_SetLookAtCamTarget(o->target,w->camera);Camera_SetLookAtCamPos(o->pos,w->camera);break;}}
