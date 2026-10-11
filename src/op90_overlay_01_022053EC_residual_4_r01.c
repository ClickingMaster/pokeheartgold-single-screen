#include "global.h"
#include "task.h"
#include "field_system.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "unk_02062108.h"
#include "overlay_01.h"
typedef struct{u8 bytes[8];} Bytes8;
typedef struct{u32 move[2];} Moves;
typedef struct{Bytes8 offsets;Moves moves;} Table;
typedef struct{u8 state,step,timer,movement;u8 pad[64];u32 direction;} Environment;
const Table ov01_022096E0 = { { 1, 2, 2, 3, 3, 2, 2, 0 }, { 14, 12 } };
extern Bytes8 ov01_022096F0;
extern VecFx32 ov01_02209704;
extern u32 ov01_02205CF0(void *,void *);
extern void *ov01_021F771C(void *);
extern void BillboardObject_SetScale(void *,VecFx32 *);
BOOL ov01_02205B14(TaskManager *);
#pragma force_active on
// @fn ov01_02205B14
BOOL ov01_02205B14(TaskManager *task){
 FieldSystem *field=TaskManager_GetFieldSystem(task);Environment *env=TaskManager_GetEnvironment(task);LocalMapObject *object; Moves moves; Bytes8 z,y; VecFx32 position,reset; int delta;
 switch(env->state){
 case 0:
  if(!FollowMon_IsActive(field)){Heap_Free(env);return 1;}
  if(ov01_022057C4(field)){Heap_Free(env);return 1;}
  MapObject_UnpauseMovement(FollowMon_GetMapObject(field));env->state++;
 case 1:
  if(MapObject_IsMovementPaused(FollowMon_GetMapObject(field)))env->state=ov01_02205CF0(field,env);
  break;
 case 2: {

  moves=ov01_022096E0.moves;object=FollowMon_GetMapObject(field);
  if(!MapObject_IsMovementPaused(object))break;
  MapObject_SetHeldMovement(object,moves.move[env->movement++]);
  if(env->movement>=2)env->state++;
  break; }
 case 3:
  object=FollowMon_GetMapObject(field);if(!MapObject_IsMovementPaused(object))break;
  MapObject_SetHeldMovement(object,0);env->state++;break;
 case 4: {

  z=ov01_022096F0;y=ov01_022096E0.offsets;object=FollowMon_GetMapObject(field);delta=0x2000;
  if(!env->direction)delta*=-1;
  MapObject_CopyPositionVector(object,&position);position.z-=((s8)z.bytes[env->step])*4096;position.x+=delta;position.y+=((s8)y.bytes[env->step])*4096;
  MapObject_SetPositionVector(object,&position);env->step++;if(env->step>=8)env->state++;break; }
 case 5:ov01_0220329C(FollowMon_GetMapObject(field),3);env->state++;break;
 case 6: {

  env->timer++;if(env->timer<20)break;
  ov01_02205790(field,0);reset=ov01_02209704;BillboardObject_SetScale(ov01_021F771C(field->mapObjectManager),&reset);sub_02069E84(field->followMon.mapObject,1);env->state++;break; }
 case 7:Heap_Free(env);return 1;
 }
 return 0;
}
