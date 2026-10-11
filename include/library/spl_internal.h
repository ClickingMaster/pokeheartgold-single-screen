#ifndef POKEHEARTGOLD_SPL_INTERNAL_H
#define POKEHEARTGOLD_SPL_INTERNAL_H

// Software particle library: types and prototypes shared by the library's own modules
// (lib/src/spl_*.c). Callers outside the library use spl.h.

// The library is built with MWCC 1.2, which warns (an error under -W error) about inline helpers in
// the NitroSDK headers that drop a call's result.
#pragma push
#pragma warn_resultnotused off
#include <nitro.h>
#pragma pop

#include "spl.h"

// NitroSDK vector functions the nitro headers do not declare
fx32 VEC_Fx16DotProduct(const VecFx16 *a, const VecFx16 *b);
void VEC_Fx16CrossProduct(const VecFx16 *a, const VecFx16 *b, VecFx16 *axb);
void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *axb);
// matrix functions the nitro headers do not declare
void MTX_MultVec43(const VecFx32 *vec, const MtxFx43 *m, VecFx32 *dst);
void G3_LoadMtx43(const MtxFx43 *m);
void G3_MultMtx43(const MtxFx43 *m);
// GX_load3d.h declares these without a parameter list
void GX_BeginLoadTex(void);
void GX_LoadTex(const void *pSrc, u32 destSlotAddr, u32 szByte);
void GX_EndLoadTex(void);
void GX_BeginLoadTexPltt(void);
void GX_LoadTexPltt(const void *pSrc, u32 destSlotAddr, u32 szByte);
void GX_EndLoadTexPltt(void);

// Emitters and particles both start with a next / prev link, and their lists share one layout
// (SPLEmitterList, SPLParticleList), so the list helpers work on either through these views.
typedef struct SplNode {
    struct SplNode *p_next;
    struct SplNode *p_prev;
} SplNode;

typedef struct SplList {
    SplNode *p_begin;
    int node_num;
    SplNode *p_end;
} SplList;

// per-particle animation step, run with the particle's life rate (0-255)
typedef void (*SplAnimFunc)(SPLParticle *ptcl, SPLResource *res, int life_rate);
// particle draw function for one draw type
typedef void (*SplDrawFunc)(SPLManager *mgr, SPLParticle *ptcl);

// spl_manager.c
SPLManager *SPL_Init(SPLAlloc alloc, int max_emtr_num, int max_ptcl_num, u16 fix_polygon_id, u16 min_polygon_id, u16 max_polygon_id);
void SPL_Load(SPLManager *mgr, const void *data);
BOOL SPL_LoadTexByVRAMManager(SPLManager *mgr);
BOOL SPL_LoadTexPlttByVRAMManager(SPLManager *mgr);
BOOL SPL_LoadTexByCallbackFunction(SPLManager *mgr, u32 (*alloc)(u32 size, BOOL is4x4comp));
BOOL SPL_LoadTexPlttByCallbackFunction(SPLManager *mgr, u32 (*alloc)(u32 size, BOOL is4pltt));
SPLEmitter *SPL_Create(SPLManager *mgr, int res_no, const VecFx32 *pos);
void SPL_Delete(SPLManager *mgr, SPLEmitter *emtr);
void SPL_DeleteAll(SPLManager *mgr);
void SPL_Calc(SPLManager *mgr);
void SPL_Draw(SPLManager *mgr, const MtxFx43 *camera);
u32 sAllocTex(u32 size, BOOL is4x4comp);
u32 sAllocTexPalette(u32 size, BOOL is4pltt);

// spl_draw.c
void spl_draw(SPLManager *mgr);
void sDrawChild(SPLManager *mgr);
void sDrawParent(SPLManager *mgr);

// spl_emitter.c
void spl_calc(SPLManager *mgr, SPLEmitter *emtr);
void spl_init(SPLEmitter *emtr, SPLResource *res, const VecFx32 *pos);

// spl_draw_ptcl.c
void spl_set_tex_dummy(const SPLTexture *tex);
void spl_set_tex(const SPLTexture *tex);
void spl_draw_bb(SPLManager *mgr, SPLParticle *ptcl);
void spl_draw_dbb(SPLManager *mgr, SPLParticle *ptcl);
void spl_draw_pol(SPLManager *mgr, SPLParticle *ptcl);
void spl_draw_dpl(SPLManager *mgr, SPLParticle *ptcl);
void spl_draw_chld_bb(SPLManager *mgr, SPLParticle *ptcl);
void spl_draw_chld_dbb(SPLManager *mgr, SPLParticle *ptcl);
void spl_draw_chld_pol(SPLManager *mgr, SPLParticle *ptcl);
void spl_draw_chld_dpl(SPLManager *mgr, SPLParticle *ptcl);
void rotTypeY(fx32 sin, fx32 cos, MtxFx43 *mtx);
void rotTypeXYZ(fx32 sin, fx32 cos, MtxFx43 *mtx);
void drawXYPlane(fx32 s, fx32 t, fx32 x, fx32 y);
void drawXZPlane(fx32 s, fx32 t, fx32 x, fx32 z);
// polygon draw types: draw one textured quad in the XY / XZ plane
extern void (*const gSplDrawPlaneFuncs[2])(fx32 s, fx32 t, fx32 x, fx32 y);
// polygon rotation axes: rotTypeY, rotTypeXYZ
extern void (*const gSplRotateFuncs[2])(fx32 sin, fx32 cos, MtxFx43 *mtx);
// (0, 1, 0): the default base vector for the emitter's cross-to-axis vectors
extern const VecFx16 gSplAxisY;

// spl_generate.c
void spl_gen_child(SPLParticle *parent, SPLEmitter *emtr, SPLParticleList *ptcl_list);
void spl_gen_ptcl(SPLEmitter *emtr, SPLParticleList *ptcl_list);
void spl_set_circle_axis(VecFx32 *out, const VecFx32 *vec, const SPLEmitter *emtr);
void spl_set_cross_to_axis(SPLEmitter *emtr);

// spl_anim.c
void spl_chld_alp_out(SPLParticle *ptcl, SPLResource *res, int life_rate);
void spl_chld_scl_out(SPLParticle *ptcl, SPLResource *res, int life_rate);
void spl_tex_ptn_anm(SPLParticle *ptcl, SPLResource *res, int life_rate);
void spl_alp_in_out(SPLParticle *ptcl, SPLResource *res, int life_rate);
void spl_clr_in_out(SPLParticle *ptcl, SPLResource *res, int life_rate);
void spl_scl_in_out(SPLParticle *ptcl, SPLResource *res, int life_rate);

// spl_field.c (the SPLField p_exec functions)
// parameter blocks of the fields, in the resource after the base / animation / child blocks in this
// order (gravity, random, magnet, spin, collision, convergence) when the base flag enables them
typedef struct SplConvergenceField {
    VecFx32 pos;
    fx16 ratio;
    u16 reserved;
} SplConvergenceField;

// simple collision plane at height y; the emitter's simple_collision_y overrides it unless unset
typedef struct SplCollisionField {
    fx32 y;
    fx16 coeff_bounce;
    u16 type : 2; // 0: kill the particle, 1: bounce
    u16 reserved : 14;
} SplCollisionField;

typedef struct SplSpinField {
    u16 rotation;
    u16 axis_type; // 0: x, 1: y, 2: z
} SplSpinField;

typedef struct SplMagnetField {
    VecFx32 pos;
    fx16 mag;
    u16 reserved;
} SplMagnetField;

typedef struct SplRandomField {
    VecFx16 mag;
    u16 interval;
} SplRandomField;

typedef struct SplGravityField {
    VecFx16 mag;
    u16 reserved;
} SplGravityField;

void spl_calc_convergence(const void *obj, SPLParticle *ptcl, VecFx32 *acc, SPLEmitter *emtr);
void spl_calc_scfield(const void *obj, SPLParticle *ptcl, VecFx32 *acc, SPLEmitter *emtr);
void spl_calc_spin(const void *obj, SPLParticle *ptcl, VecFx32 *acc, SPLEmitter *emtr);
void spl_calc_magnet(const void *obj, SPLParticle *ptcl, VecFx32 *acc, SPLEmitter *emtr);
void spl_calc_random(const void *obj, SPLParticle *ptcl, VecFx32 *acc, SPLEmitter *emtr);
void spl_calc_gravity(const void *obj, SPLParticle *ptcl, VecFx32 *acc, SPLEmitter *emtr);

// spl_list.c
SplNode *spl_del(SplList *list, SplNode *node);
SplNode *spl_pop_front(SplList *list);
void spl_push_front(SplList *list, SplNode *node);

// spl_random.c
extern u32 spl_rndm_seed;
// one step of the library's linear congruential random generator; evaluates to the new seed
#define SPL_RNDM_NEXT() (spl_rndm_seed = spl_rndm_seed * 0x5EEDF715 + 0x1B0CB173)
void spl_rndm_get_arb_vec_xy(VecFx32 *vec);
void spl_rndm_get_arb_vec_xyz(VecFx32 *vec);

#endif // POKEHEARTGOLD_SPL_INTERNAL_H
