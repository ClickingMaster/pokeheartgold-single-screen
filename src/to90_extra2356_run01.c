#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W31684 {u8 pad[0x40];void *avatar;u8 pad2[0xa0];void *object;} W31684;
extern W31684 *TaskManager_GetFieldSystem(void *);
extern int *TaskManager_GetEnvironment(void *);
extern u32 *TaskManager_GetStatePtr(void *);
extern void sub_020659CC(void *),sub_0205F484(void *),ov01_02206028(void *,void *),MapObject_SetFacingDirection(void *,int),sub_0206A040(void *,int),MapObject_SetHeldMovement(void *,int),FollowMonBall_Spawn(void *,int),sub_0206A054(W31684 *),Heap_Free(void *);
extern void *PlayerAvatar_GetMapObject(void *);
extern int MapObject_AreBitsSetForMovementScriptInit(void *),MapObject_GetFacingDirection(void *),sub_0206234C(int,int);
BOOL ov01_02205F00(void *);
BOOL ov01_02205F00(void *task){W31684 *ctx=TaskManager_GetFieldSystem(task);int *env=TaskManager_GetEnvironment(task);u32 *state=TaskManager_GetStatePtr(task);switch(*state){case 0:sub_020659CC(ctx->object);sub_0205F484(ctx->object);++*state;break;case 1:if(MapObject_AreBitsSetForMovementScriptInit(ctx->object)==1){ov01_02206028(PlayerAvatar_GetMapObject(ctx->avatar),ctx->object);++*state;}break;case 2:if(MapObject_AreBitsSetForMovementScriptInit(ctx->object)==1){MapObject_SetFacingDirection(ctx->object,MapObject_GetFacingDirection(PlayerAvatar_GetMapObject(ctx->avatar)));++*state;}break;case 3:if(++*env>10)++*state;break;case 4:if(MapObject_AreBitsSetForMovementScriptInit(ctx->object)==1){sub_0206A040(ctx->object,0);MapObject_SetHeldMovement(ctx->object,sub_0206234C(MapObject_GetFacingDirection(PlayerAvatar_GetMapObject(ctx->avatar)),52));++*state;}break;case 5:if(MapObject_AreBitsSetForMovementScriptInit(ctx->object)==1){FollowMonBall_Spawn(ctx->object,2);sub_0206A054(ctx);++*state;}break;case 6:Heap_Free(env);return TRUE;}return FALSE;}
