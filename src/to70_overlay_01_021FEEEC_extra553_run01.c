#pragma force_active on
#include "global.h"
typedef struct {int x,y,z;void*a,*b,*c,*obj;s16 dx,dz;s8 facing;u8 pad[3];} Params;
typedef struct {s32 x,y,z;} Vec;
extern void *FieldEffectManager_GetFromMapObject(void*);extern int MapObject_GetYCoord(void*);extern int MapObject_GetXCoord(void*);extern int MapObject_GetZCoord(void*);extern int MapObject_GetFacingDirection(void*);extern void *FieldEffectManager_GetFieldSystem(void*);extern void *FieldEffectManager_GetRendererData(void*,int);extern void MapObject_CopyPositionVector(void*,Vec*);extern int MapObject_GetPriorityPlusValue(void*,int);extern void *FieldEffectManager_CreateEffect(void*,void*,Vec*,int,Params*,int);extern u8 ov01_02209138[];

void *ov01_021FF0E4(void*obj,int arg,int x,int z,int facing);

void *ov01_021FF0E4(void*obj,int arg,int x,int z,int facing){Vec v;Params p;void*w=FieldEffectManager_GetFromMapObject(obj);p.x=x;p.y=MapObject_GetYCoord(obj);p.z=z;p.dx=MapObject_GetXCoord(obj)-x;p.dz=MapObject_GetZCoord(obj)-z;if(facing)p.facing=MapObject_GetFacingDirection(obj);else p.facing=-1;p.b=w;p.a=FieldEffectManager_GetFieldSystem(w);p.c=FieldEffectManager_GetRendererData(w,8);p.obj=obj;MapObject_CopyPositionVector(obj,&v);return FieldEffectManager_CreateEffect(w,ov01_02209138,&v,arg,&p,MapObject_GetPriorityPlusValue(obj,2));}
