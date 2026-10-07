#include "global.h"
#pragma force_active on
#include "map_follow_state_internal.h"
#include "map_object.h"
#include "player_avatar.h"
#include "follow_mon.h"
#include "unk_02062108_internal.h"
extern int sub_02065D78(LocalMapObject*);
extern int sub_02061200(int,int,int,int);
extern u32 ov01_0220542C(u32,u32);
extern int sub_02066444(int);
extern void MapObject_ForceSetHeldMovement(LocalMapObject*,u32);
int sub_02065DF4(LocalMapObject*,FollowStateSol*);
// @fn sub_02065DF4
int sub_02065DF4(LocalMapObject*obj,FollowStateSol*w){u8 result;u32 x,z,px,pz,next;int old,dir,flag;PlayerAvatar*avatar;avatar=FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(obj));x=MapObject_GetXCoord(obj);z=MapObject_GetZCoord(obj);px=PlayerAvatar_GetPreviousXCoord(avatar);pz=PlayerAvatar_GetPreviousZCoord(avatar);
 if(x==px && z==pz)goto done;
 old=sub_02065D78(obj);dir=sub_02061200((int)x,(int)z,(int)px,(int)pz);next=sub_02069EC0(obj);flag=sub_020623C8(old);result=1;
 if(next){if(flag){next=ov01_0220542C(next,sub_020623D8(next));w->bits=(w->bits&~6)|(((u16)sub_02066444((int)next)&3)<<1);sub_02069E50(obj,(u8)next);result=2;((u8 *)&w->x)[0]=0;((u8 *)&w->x)[1]=0;sub_02069E28(obj,(u8)PlayerAvatar_GetFacingDirection(avatar));}
 else{if(!PlayerAvatar_CheckFlag6(avatar))return 0;next=ov01_0220542C(next,(u32)old);w->bits=(w->bits&~6)|(((u16)sub_02066444((int)next)&3)<<1);sub_02069E50(obj,(u8)next);result=2;((u8 *)&w->x)[0]=0;((u8 *)&w->x)[1]=0;sub_02069E28(obj,0);}}
 else{if(flag){next=sub_020623D8(dir);sub_02069E28(obj,(u8)PlayerAvatar_GetFacingDirection(avatar));}else next=sub_0206234C(dir,(u32)old);}
 MapObject_ForceSetHeldMovement(obj,next);w->state=result;return 1;
 done:return 0;
}
