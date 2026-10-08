#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u32 p0:17,rotation:2,type:1,p1:3,world:1,p2:8; } Flags;
typedef struct { u32 p0:8,dbbScale:16,p1:4,scale:3,p2:1; } ScaleFlags;
typedef struct { Flags flags; VecFx32 origin; u8 p10[32]; s16 aspect; u8 p32[22]; ScaleFlags flags2; u8 p4C[4]; s16 x,y; } Definition;
typedef struct { u16 p0:9,rotation:2,type:1,p1:4; } ChildFlags;
typedef struct { ChildFlags flags; } Child;
typedef struct { Definition *definition; u8 p04[16]; Child *child; } Resource;
typedef struct { u8 p0[32]; Resource *resource; u8 p24[78]; u16 color; u8 p74[4]; s16 x,y,x2,y2; } Emitter;
typedef struct { u8 p0[60]; u32 attributes; Emitter *emitter; MtxFx43 *matrix; } Renderer;
typedef struct {u16 alpha:5,modifier:5,id:6;} Polygon;
typedef struct { u8 p0[8]; VecFx32 position; VecFx32 velocity; u16 angle; u8 p22[12]; Polygon polygon; fx32 scale; s16 factor; u16 color; VecFx32 offset; } Particle;
extern void G3_LoadMtx43(const MtxFx43*), G3_MultMtx43(const MtxFx43*);
extern void (*_021105E4[])(fx16,fx16,MtxFx43*);
extern void (*_021105DC[])(s16,s16,s16,s16);

#include <nitro/fx/fx_mtx33.h>
extern void MTX_MultVec43(const VecFx32 *, const MtxFx43 *, VecFx32 *);
extern void G3_MultMtx43(const MtxFx43 *);
extern void VEC_CrossProduct(const VecFx32 *, const VecFx32 *, VecFx32 *);
extern void VEC_Normalize(const VecFx32 *, VecFx32 *);
extern void drawXYPlane(fx32 s, fx32 t, fx32 x, fx32 y);
void spl_draw_dbb(Renderer *r, Particle *p);
void kbest(Renderer *r, Particle *p);
// @fn spl_draw_dbb
void spl_draw_dbb(Renderer *r, Particle *p) {
    VecFx32 vel;
    VecFx32 pos;
    VecFx32 dir;
    VecFx32 camZ;
    MtxFx33 rot;
    MtxFx43 m;
    const MtxFx43 *cmr;
    s16 aspect;
    fx32 sx;
    fx32 sy32;
    int alpha;
    fx32 syd;
    fx32 dot;
    s64 sy;
    aspect = r->emitter->resource->definition->aspect;
    cmr = r->matrix;
    alpha = p->polygon.alpha * (p->polygon.modifier + 1) >> 5;
    *(vu32 *)0x040004A4 = (p->polygon.id << 24) | (r->attributes | 0xC0) | (alpha << 16);
    (void)*(vu32 *)0x040004A4;
    if (alpha == 0) return;
    sx = FX_MUL(p->scale, aspect);
    sy = p->scale;
    switch (r->emitter->resource->definition->flags2.scale) {
    case 0:
        sx = FX_MUL(sx, p->factor);
        sy = (sy * p->factor + 0x800) >> 12;
        break;
    case 1:
        sx = FX_MUL(sx, p->factor);
        break;
    case 2:
        sy = (sy * p->factor + 0x800) >> 12;
        break;
    }
    sy32 = sy;
    if (!r->emitter->resource->definition->flags.world) {
        pos.x = p->position.x + p->offset.x;
        pos.y = p->position.y + p->offset.y;
        pos.z = p->position.z + p->offset.z;
        dir = p->velocity;
        camZ.x = cmr->_02;
        camZ.y = cmr->_12;
        camZ.z = cmr->_22;
        VEC_CrossProduct(&dir, &camZ, &dir);
        if (dir.x == 0 && dir.y == 0 && dir.z == 0) return;
        VEC_Normalize(&dir, &dir);
        MTX_Copy33((const MtxFx33 *)cmr, &rot);
        MTX_MultVec33(&dir, &rot, &dir);
        MTX_MultVec43(&pos, cmr, &pos);
        vel = p->velocity;
        VEC_Normalize(&vel, &vel);
        dot = FX_MUL(vel.z, -cmr->_22) + (FX_MUL(vel.x, -cmr->_02) + FX_MUL(vel.y, -cmr->_12));
        if (dot < 0) dot = -dot;
        syd = FX_MUL(sy32, FX_MUL(FX32_ONE - dot, (fx32)r->emitter->resource->definition->flags2.dbbScale) + FX32_ONE);
        m._00 = FX_MUL(dir.x, sx);
        m._10 = FX_MUL(-dir.y, syd);
        m._20 = 0;
        m._21 = 0;
        m._30 = pos.x;
        m._01 = FX_MUL(dir.y, sx);
        m._11 = FX_MUL(dir.x, syd);
        m._31 = pos.y;
        m._02 = 0;
        m._12 = 0;
        m._22 = FX32_ONE;
        m._32 = pos.z;
        *(vu32 *)0x04000454 = 0;
        G3_MultMtx43(&m);
    } else {
        pos.x = p->position.x + p->offset.x - r->emitter->resource->definition->origin.x;
        pos.y = p->position.y + p->offset.y - r->emitter->resource->definition->origin.y;
        pos.z = p->position.z + p->offset.z - r->emitter->resource->definition->origin.z;
        dir = p->velocity;
        camZ.x = cmr->_02;
        camZ.y = cmr->_12;
        camZ.z = cmr->_22;
        VEC_CrossProduct(&dir, &camZ, &dir);
        if (dir.x == 0 && dir.y == 0 && dir.z == 0) return;
        VEC_Normalize(&dir, &dir);
        MTX_Copy33((const MtxFx33 *)cmr, &rot);
        MTX_MultVec33(&dir, &rot, &dir);
        MTX_MultVec43(&pos, cmr, &pos);
        vel = p->velocity;
        VEC_Normalize(&vel, &vel);
        dot = FX_MUL(vel.z, -cmr->_22) + (FX_MUL(vel.x, -cmr->_02) + FX_MUL(vel.y, -cmr->_12));
        if (dot < 0) dot = -dot;
        syd = FX_MUL(sy32, FX_MUL(FX32_ONE - dot, (fx32)r->emitter->resource->definition->flags2.dbbScale) + FX32_ONE);
        m._00 = FX_MUL(dir.x, sx);
        m._10 = FX_MUL(-dir.y, syd);
        m._20 = 0;
        m._30 = pos.x;
        m._01 = FX_MUL(dir.y, sx);
        m._11 = FX_MUL(dir.x, syd);
        m._21 = 0;
        m._02 = 0;
        m._31 = pos.y;
        m._12 = 0;
        m._22 = FX32_ONE;
        m._32 = pos.z;
        *(vu32 *)0x04000454 = 0;
        {
            Definition *d = r->emitter->resource->definition;
            int z = d->origin.z, y = d->origin.y, x = d->origin.x;
            *(vu32 *)0x04000470 = x;
            *(vu32 *)0x04000470 = y;
            *(vu32 *)0x04000470 = z;
        }
        G3_MultMtx43(&m);
    }
    {
        int a = p->color, b = r->emitter->color;
        *(vu32 *)0x04000480 = (u16)((((a & 0x1f) * (b & 0x1f)) >> 5) | ((((a & 0x3e0) * (b & 0x3e0)) >> 15) << 5) | ((((a & 0x7c00) * (b & 0x7c00)) >> 25) << 10));
    }
    drawXYPlane(r->emitter->x, r->emitter->y, r->emitter->resource->definition->x, r->emitter->resource->definition->y);
}
