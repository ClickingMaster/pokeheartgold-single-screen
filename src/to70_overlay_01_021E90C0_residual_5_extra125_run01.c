#pragma force_active on
#include "global.h"
#pragma force_active on
u8 *TaskManager_GetFieldSystem(void *);
u8 *TaskManager_GetEnvironment(void *);
void *PlayerAvatar_GetMapObject(void *);
int PlayerAvatar_GetFacingDirection(void *);
void MapObject_SetVisible(void *,int);
void MapObject_SetHeldMovement(void *,int);
int MapObject_IsMovementPaused(void *);
void MapObject_ClearHeldMovementIfActive(void *);
void NewFieldFadeEnvironment(void *,int,int,int,int,int,int,int);
BOOL IsPaletteFadeFinished(void);
void Heap_Free(void *);
BOOL ov01_021EA128(void *);
BOOL ov01_021EA128(void *task){
 u8 *fs=TaskManager_GetFieldSystem(task);
 u8 *state=TaskManager_GetEnvironment(task);
 void *obj;
 switch(*state){
 case 0:
  obj=PlayerAvatar_GetMapObject(*(void **)(fs+0x40));
  if((u8)PlayerAvatar_GetFacingDirection(*(void **)(fs+0x40))==1){
   MapObject_SetVisible(obj,1);*state=1;
  }else{MapObject_SetVisible(obj,0);*state=3;}
  NewFieldFadeEnvironment(task,0,1,1,0,6,1,11);break;
 case 1:
  obj=PlayerAvatar_GetMapObject(*(void **)(fs+0x40));
  MapObject_SetVisible(obj,0);MapObject_SetHeldMovement(obj,13);(*state)++;break;
 case 2:
  obj=PlayerAvatar_GetMapObject(*(void **)(fs+0x40));
  if(MapObject_IsMovementPaused(obj)==1){MapObject_ClearHeldMovementIfActive(obj);(*state)++;}break;
 case 3:
  if(IsPaletteFadeFinished()){Heap_Free(state);return TRUE;}break;
 }
 return FALSE;
}
