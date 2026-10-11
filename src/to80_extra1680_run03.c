#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u32 p0:17,rotation:2,type:1,p1:3,world:1,p2:8; } Flags;
typedef struct { u32 p0:28,scale:3,p1:1; } ScaleFlags;
typedef struct { Flags flags; VecFx32 origin; u8 p10[32]; s16 aspect; u8 p32[22]; ScaleFlags flags2; u8 p4C[4]; s16 x,y; } Definition;
typedef struct { u16 p0:9,rotation:2,type:1,p1:4; } ChildFlags;
typedef struct { ChildFlags flags; } Child;
typedef struct { Definition *definition; u8 p04[16]; Child *child; } Resource;
typedef struct { u8 p0[32]; Resource *resource; u8 p24[78]; u16 color; u8 p74[4]; s16 x,y,x2,y2; } Emitter;
typedef struct { u8 p0[60]; u32 attributes; Emitter *emitter; MtxFx43 *matrix; } Renderer;
typedef struct {u16 alpha:5,modifier:5,id:6;} Polygon;
typedef struct { u8 p0[8]; VecFx32 position; u8 p14[12]; u16 angle; u8 p22[12]; Polygon polygon; fx32 scale; s16 factor; u16 color; VecFx32 offset; } Particle;
extern void G3_LoadMtx43(const MtxFx43*), G3_MultMtx43(const MtxFx43*);
extern void (*gSplRotateFuncs[])(fx16,fx16,MtxFx43*);
extern void (*gSplDrawPlaneFuncs[])(s16,s16,s16,s16);
void spl_draw_chld_pol(Renderer*,Particle*);
void spl_draw_chld_pol(Renderer *r, Particle *p) {
 MtxFx43 matrix,rotation,scale;

 u32 attributes=r->attributes;
 int alpha=p->polygon.alpha*(p->polygon.modifier+1)>>5;
 *(vu32*)0x040004A4= (p->polygon.id<<24)|(attributes|0xC0)|(alpha<<16);
 (void)*(vu32*)0x040004A4;
 if(alpha==0)return;
 gSplRotateFuncs[r->emitter->resource->child->flags.rotation](FX_SinCosTable_[(p->angle>>4)*2],FX_SinCosTable_[(p->angle>>4)*2+1],&rotation);
 {
 s64 sy=p->scale;
 fx32 sx=((s64)p->scale*r->emitter->resource->definition->aspect+2048)>>12;
 switch(r->emitter->resource->definition->flags2.scale) {
 case 0:sx=((s64)sx*p->factor+2048)>>12;sy=(sy*p->factor+2048)>>12;break;
 case 1:sx=((s64)sx*p->factor+2048)>>12;break;
 case 2:sy=(sy*p->factor+2048)>>12;break;
 }
 MTX_Scale43_(&scale,sx,sy,sy);
 }
 MTX_Concat43(&rotation,&scale,&matrix);
 if(!r->emitter->resource->definition->flags.world) {
 matrix._30=p->position.x+p->offset.x;matrix._31=p->position.y+p->offset.y;matrix._32=p->position.z+p->offset.z;
 G3_LoadMtx43(r->matrix);G3_MultMtx43(&matrix);
 } else {
 matrix._30=p->position.x+p->offset.x-r->emitter->resource->definition->origin.x;
 matrix._31=p->position.y+p->offset.y-r->emitter->resource->definition->origin.y;
 matrix._32=p->position.z+p->offset.z-r->emitter->resource->definition->origin.z;
 *(vu32*)0x04000454=0;
 { Definition *d=r->emitter->resource->definition;int z=d->origin.z,y=d->origin.y,x=d->origin.x;*(vu32*)0x04000470=x;*(vu32*)0x04000470=y;*(vu32*)0x04000470=z; }
 G3_MultMtx43(r->matrix);G3_MultMtx43(&matrix);
 }
 {
 int a=p->color,b=r->emitter->color;
 *(vu32*)0x04000480=(u16)((((a&31)*(b&31))>>5)|((((a&0x3E0)*(b&0x3E0))>>15)<<5)|((((a&0x7C00)*(b&0x7C00))>>25)<<10));
 }
 gSplDrawPlaneFuncs[r->emitter->resource->child->flags.type](r->emitter->x2,r->emitter->y2,0,0);
}
