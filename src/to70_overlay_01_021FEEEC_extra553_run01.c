#pragma force_active on
#include "global.h"
typedef struct {int x,y,z;void*a,*b,*c,*obj;s16 dx,dz;s8 facing;u8 pad[3];} Params;
typedef struct {s32 x,y,z;} Vec;
extern void *ov01_021F146C(void*);extern int MapObject_GetYCoord(void*);extern int MapObject_GetXCoord(void*);extern int MapObject_GetZCoord(void*);extern int MapObject_GetFacingDirection(void*);extern void *ov01_021F1468(void*);extern void *ov01_021F1450(void*,int);extern void MapObject_CopyPositionVector(void*,Vec*);extern int MapObject_GetPriorityPlusValue(void*,int);extern void *ov01_021F1620(void*,void*,Vec*,int,Params*,int);extern u8 ov01_02209138[];

void *ov01_021FF0E4(void*obj,int arg,int x,int z,int facing);

void *ov01_021FF0E4(void*obj,int arg,int x,int z,int facing){Vec v;Params p;void*w=ov01_021F146C(obj);p.x=x;p.y=MapObject_GetYCoord(obj);p.z=z;p.dx=MapObject_GetXCoord(obj)-x;p.dz=MapObject_GetZCoord(obj)-z;if(facing)p.facing=MapObject_GetFacingDirection(obj);else p.facing=-1;p.b=w;p.a=ov01_021F1468(w);p.c=ov01_021F1450(w,8);p.obj=obj;MapObject_CopyPositionVector(obj,&v);return ov01_021F1620(w,ov01_02209138,&v,arg,&p,MapObject_GetPriorityPlusValue(obj,2));}
