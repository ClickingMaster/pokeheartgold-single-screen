#pragma force_active on
#include "global.h"
extern int sub_0205DE98(void*),MapObject_GetParam(void*,u32),sub_02061200(int,int,int,int);extern u32 PlayerAvatar_GetXCoord(void*),PlayerAvatar_GetZCoord(void*),MapObject_GetType(void*),MapObject_GetMovement(void*),MapObject_GetXCoord(void*),MapObject_GetZCoord(void*);
extern void*MapObject_GetFieldSystem(void*),*FieldSystem_GetPlayerAvatar(void*),*PlayerAvatar_GetMapObject(void*);extern u32 MapObject_GetPositionVectorYCoordUInt(void*);extern const int _020FD800[];
int sub_02061E90(void*);
// @fn sub_02061E90
int sub_02061E90(void*obj){void*avatar;int type=MapObject_GetType(obj);int i,m,entry,x,z,d,ox,oz,maxz,minx,minz,maxx;u32 y; if(type!=1&&type!=2)return -1;avatar=FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(obj));if(!sub_0205DE98(avatar))return -1; i=0;m=MapObject_GetMovement(obj);do{entry=_020FD800[i++];}while(entry!=m&&entry!=255);if(m!=entry)return -1; y=MapObject_GetPositionVectorYCoordUInt(PlayerAvatar_GetMapObject(avatar));if(y!=MapObject_GetPositionVectorYCoordUInt(obj))return -1; x=PlayerAvatar_GetXCoord(avatar);z=PlayerAvatar_GetZCoord(avatar);d=MapObject_GetParam(obj,0);ox=MapObject_GetXCoord(obj);oz=MapObject_GetZCoord(obj); minx=ox-d;maxx=ox+d;minz=oz-d;maxz=oz+d;if(minz>z)goto fail;if(maxz<z)goto fail;if(minx>x)goto fail;if(maxx<x)goto fail;return sub_02061200(ox,oz,x,z); fail:return -1;}
