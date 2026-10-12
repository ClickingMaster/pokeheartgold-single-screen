#pragma force_active on
#include "global.h"
extern int MapObject_IsMovementPaused(void *);
extern int MapObject_GetNextFacingDirection(void *);
extern u32 MapObject_GetMetatileBehaviorInDirection(void *,int);
extern int MetatileBehavior_IsRockClimbInDirection(u8,int);
extern u32 GetMovementForDirection(int,int);
extern void MapObject_SetHeldMovement(void *,u32);
extern void RockClimbEffect_SetFollow(void *,int);
extern void ov02_0224D9B8(void *);
typedef struct {u16 state,next;int dir,counter;u8 p[8];void *obj,*effect;u8 p2[16];void *other;} Work;
int ov01_021F2830(Work *w);
int ov01_021F2830(Work *w){int dir;if(!MapObject_IsMovementPaused(w->obj))return 0;dir=MapObject_GetNextFacingDirection(w->obj);if(MetatileBehavior_IsRockClimbInDirection((u8)MapObject_GetMetatileBehaviorInDirection(w->obj,dir),dir)==1){w->state=w->next;return 2;}MapObject_SetHeldMovement(w->obj,GetMovementForDirection(w->dir,52));w->counter=0;w->state++;RockClimbEffect_SetFollow(w->effect,0);ov02_0224D9B8(w->other);w->other=0;return 0;}
