#pragma force_active on
#include "global.h"
#include "player_avatar.h"
#include "sound.h"
extern int sub_0205DDD4(PlayerAvatar*,int,int),sub_0205DEC0(PlayerAvatar*,int);
extern BOOL sub_0205DE88(int,int);
extern BOOL PlaySE(u16);
extern u32 GetMovementForDirection(s32,u32);
u32 sub_0205DF0C(PlayerAvatar*,int,int,u32,int,int);
#pragma force_active on
u32 sub_0205DF0C(PlayerAvatar *avatar,int a,int b,u32 speed,int flag,int collision){struct {volatile int direction;} saved;saved.direction=sub_0205DDD4(avatar,a,b);int state=sub_0205DEC0(avatar,saved.direction),motion;PlayerAvatar_SetMoveState(avatar,state);if(state==0)return GetMovementForDirection(PlayerAvatar_GetFacingDirection(avatar),0);if(state==2)return GetMovementForDirection(saved.direction,40);PlayerAvatar_GetMapObject(avatar);motion=4;{u32 blocked=(u32)collision;if(blocked&4)motion=56;else if(blocked){motion=28;if(!(blocked&8))PlaySE(1536);}else{switch(speed){case 0:motion=4;break;case 1:motion=8;break;case 2:motion=12;break;case 3:motion=76;break;case 4:motion=16;break;case 5:motion=20;break;default:motion=4;break;}if(flag==1&&sub_0205DE88((int)avatar,b)==1)motion=88;}}return GetMovementForDirection(saved.direction,motion);}
