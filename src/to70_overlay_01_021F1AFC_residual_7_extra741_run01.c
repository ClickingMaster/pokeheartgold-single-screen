#pragma force_active on
#include "global.h"
#include "constants/sndseq.h"
typedef struct{u16 state,pad;int facing;int unused;void*field,*avatar,*obj;int result;int pad2[4];int other;}Work;
extern int PlayerAvatar_GetXCoord(void*),PlayerAvatar_GetZCoord(void*),GetDeltaXByFacingDirection(int),GetDeltaYByFacingDirection(int),RockClimbEffect_Create(void*,int,int,int,int),FollowMon_IsActive(void*),ov02_0224D9A4(void*);extern void*FollowMon_GetMapObject(void*);extern void MapObject_UnpauseMovement(void*),PlaySE(int);
int ov01_021F2758(Work*);
int ov01_021F2758(Work*w){int x=PlayerAvatar_GetXCoord(w->avatar);int dx=GetDeltaXByFacingDirection(w->facing);int z=PlayerAvatar_GetZCoord(w->avatar);int dz=GetDeltaYByFacingDirection(w->facing);w->result=RockClimbEffect_Create(w->obj,x+dx,z+dz,w->facing,0);if(FollowMon_IsActive(w->field)){MapObject_UnpauseMovement(FollowMon_GetMapObject(w->field));}w->other=ov02_0224D9A4(w->field);PlaySE(SEQ_SE_DP_UG_023);w->state++;return 0;}
