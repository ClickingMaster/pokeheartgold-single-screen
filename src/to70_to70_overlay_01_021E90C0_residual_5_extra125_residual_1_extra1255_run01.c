#pragma force_active on
#include "global.h"
typedef struct{u8 state,count;u16 angle;int active;} Work;
typedef struct{u8 p[0x24];void *camera;u8 p2[0x18];void *avatar;} Field;
extern Field *TaskManager_GetFieldSystem(void *);extern Work *TaskManager_GetEnvironment(void *);
extern void *PlayerAvatar_GetMapObject(void *);extern int PlayerAvatar_GetFacingDirection(void *);
extern void MapObject_SetVisible(void *,int),MapObject_SetHeldMovement(void *,int),MapObject_ClearHeldMovementIfActive(void *);
extern int MapObject_IsMovementPaused(void *),IsPaletteFadeFinished(void);
extern u16 Camera_GetPerspectiveAngle(void *);extern void Camera_AdjustPerspectiveAngle(u16,void *),Heap_Free(void *),ov01_021E9610(void *,u8 *),NewFieldFadeEnvironment(void *,int,int,int,int,int,int,int);
int ov01_021E9FF8(void *);
int ov01_021E9FF8(void *task){Field *f=TaskManager_GetFieldSystem(task);Work *w=TaskManager_GetEnvironment(task);void *obj;u8 dir;int kind;switch(w->state){case 0:obj=PlayerAvatar_GetMapObject(f->avatar);dir=PlayerAvatar_GetFacingDirection(f->avatar);if(dir==1){MapObject_SetVisible(obj,1);w->state=1;}else{MapObject_SetVisible(obj,0);w->state=3;}w->active=0;w->count=0;w->angle=Camera_GetPerspectiveAngle(f->camera);Camera_AdjustPerspectiveAngle(0xffa0,f->camera);switch(dir){case 0:kind=3;break;case 1:kind=5;break;case 2:kind=7;break;case 3:kind=39;break;default:GF_AssertFail();break;}NewFieldFadeEnvironment(task,0,kind,1,0,6,1,11);w->active=1;break;case 1:obj=PlayerAvatar_GetMapObject(f->avatar);MapObject_SetVisible(obj,0);MapObject_SetHeldMovement(obj,13);w->state++;break;case 2:obj=PlayerAvatar_GetMapObject(f->avatar);if(MapObject_IsMovementPaused(obj)==1){MapObject_ClearHeldMovementIfActive(obj);w->state++;}break;case 3:if(IsPaletteFadeFinished()&&w->angle==Camera_GetPerspectiveAngle(f->camera)){Heap_Free(w);return 1;}break;}if(w->active)ov01_021E9610(f->camera,&w->count);return 0;}
