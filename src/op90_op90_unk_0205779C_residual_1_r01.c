#pragma force_active on
#include "global.h"
#include "communication_player_movement_internal.h"
extern u32 sub_0205DF0C(void*,int,int,u32,int,int);
extern BOOL sub_0205DFC8(void*),sub_0205DE38(void*);
// @fn sub_02057818
void sub_02057818(int id){PlayerPosition*p=(PlayerPosition*)(_021D41C4->raw+offsetof(CommPlayerPositionLayout,remote)+id*sizeof(PlayerPosition));if((_021D41C4->raw+0xdc)[id]){(_021D41C4->raw+0xdc)[id]--;return;}
if(p->x!=65535&&p->z!=65535&&!_021D41C4->raw[0xee]&&(id!=Comm_GetLocalNetId()||_021D41C4->raw[0xf1])){
void*avatar;int facing;int dx;int direction;u32 speed;int dz;u16 keys;int movement;
avatar=((CommPlayerManagerLayout*)_021D41C4)->avatars[id];if(!avatar)return;
dx=(s32)((u32)PlayerAvatar_GetXCoord(avatar)-p->x);dz=(s32)((u32)PlayerAvatar_GetZCoord(avatar)-p->z);direction=PlayerAvatar_GetFacingDirection(avatar);
if(!dx&&!dz)keys=0;else if(abs(dx)>abs(dz)){if(dx>0)keys=32;else keys=16;}else{if(dz>0)keys=64;else keys=128;}(void)abs(dz);(void)abs(dx);
facing=p->pad5[0];movement=255;
switch(facing){case 0:speed=5;break;case 1:speed=4;keys|=2;break;case 2:speed=2;break;case 3:speed=1;break;}
if(_021D41C4->raw[0xf0] && id==Comm_GetLocalNetId())_021D41C4->raw[0xf0]--;else{int masked=keys&~2;if(!masked&&p->pad5[2]){facing=3;switch(p->facing){case 0:movement=28;break;default:case 1:movement=29;break;case 2:movement=30;break;case 3:movement=31;break;}}else if(!masked&&p->facing!=direction)movement=sub_0206234C(p->facing,36);else movement=sub_0205DF0C(avatar,keys,keys,speed,1,0);}
if(sub_0205DFC8(avatar)||sub_0205DE38(avatar)){if(movement!=255){sub_0205DFD4(avatar,movement);if(keys&~2){if(!(_021D41C4->raw+0xdc)[id])(_021D41C4->raw+0xdc)[id]=sub_02057524(facing);if((_021D41C4->raw+0xdc)[id])(_021D41C4->raw+0xdc)[id]--;}}}}}
