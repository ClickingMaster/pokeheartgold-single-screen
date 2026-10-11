#pragma force_active on
#include "global.h"
#include <string.h>
#pragma force_active on
typedef struct {u32 pad;VecFx32 position;} Base;
typedef struct {u16 x,y,z;} Rotation;
typedef struct {u8 pad[0x20];Base **base;u32 unk24;VecFx32 position;u8 pad34[0x1c];VecFx16 rotation;} Emitter;
extern void *ov07_0221C478(void*);
extern int BattleAnimUtil_GetBattlerType(void*,int);
extern void ov07_0221F9A8(void*,void*,int);
extern s8 ov07_0221FF34(void*,int,int);
extern void ov07_02231B90(void*,int,VecFx32*);
extern void ov07_0222036C(void*,Emitter*,int,int,int,s8,VecFx32*);
extern void ov07_0222043C(void*,Emitter*,int,int,int,s8,VecFx32*);
void ov07_0222050C(void*,Emitter*,int,int,int,int);
static inline void convert(volatile Rotation *dst,VecFx32 *src) {dst->x=src->x;dst->y=src->y;dst->z=src->z;}
static inline void negate(volatile Rotation *dst,VecFx32 *src) {dst->x=-src->x;dst->y=-src->y;dst->z=-src->z;}
void ov07_0222050C(void *context,Emitter *emitter,int a,int b,int mode,int arg) {
 VecFx32 final,v0,v1,v3,v2,v6,v4,v5;volatile VecFx32 v7;VecFx32 offset;
 volatile Rotation rotation;
 int kind,other;s8 factor;
 ov07_0221C478(context);
 kind=BattleAnimUtil_GetBattlerType(context,a);other=BattleAnimUtil_GetBattlerType(context,b);
 ov07_0221F9A8(context,&v0,3);ov07_0221F9A8(context,&v1,3);
 ov07_0221F9A8(context,&v2,3);ov07_0221F9A8(context,&v3,3);
 ov07_0221F9A8(context,&v4,3);ov07_0221F9A8(context,&v5,3);
 ov07_0221F9A8(context,&v6,3);
 switch(kind) {
 default:case 0:convert(&rotation,&v0);break;
 case 1:negate(&rotation,&v0);break;
 case 2:if(other==3)convert(&rotation,&v1);else if(other==5)convert(&rotation,&v2);else convert(&rotation,&v3);break;
 case 3:if(other==2)negate(&rotation,&v1);else if(other==5)negate(&rotation,&v3);else negate(&rotation,&v4);break;
 case 4:if(other==3)convert(&rotation,&v4);else if(other==5)convert(&rotation,&v5);else convert(&rotation,&v6);break;
 case 5:if(other==3)negate(&rotation,&v6);else if(other==2)negate(&rotation,&v2);else negate(&rotation,&v5);break;
 }
 factor=ov07_0221FF34(context,a,b);
 memset((void*)&v7,0,sizeof(v7));ov07_0221F9A8(context,(void*)&v7,3);
 ov07_02231B90(context,a,&offset);
 if(v7.x==0 && v7.y==0 && v7.z==0) {
  final=offset;
  emitter->position.x=offset.x+(*emitter->base)->position.x;
  emitter->position.y=offset.y+(*emitter->base)->position.y;
  emitter->position.z=offset.z+(*emitter->base)->position.z;
 }else {
  final.x=v7.x;final.y=v7.y;final.z=v7.z;
  if(final.x==0 && final.y==3200)factor=1;
  final.x*=factor;final.y*=factor;final.z=offset.z*factor;
  emitter->position.x=final.x+(*emitter->base)->position.x;
  emitter->position.y=final.y+(*emitter->base)->position.y;
  emitter->position.z=final.z+(*emitter->base)->position.z;
 }
 switch(mode) {
 case 0:break;
 case 1:ov07_0222036C(context,emitter,a,b,arg,factor,&final);break;
 case 2:ov07_0222043C(context,emitter,a,b,arg,factor,&final);break;
 }
 emitter->rotation.x=rotation.x;emitter->rotation.y=rotation.y;emitter->rotation.z=rotation.z;
}
