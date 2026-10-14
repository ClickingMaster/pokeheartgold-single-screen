#ifndef POKEHEARTGOLD_FIELD_FIELD_EFFECT_HELPERS_H
#define POKEHEARTGOLD_FIELD_FIELD_EFFECT_HELPERS_H

#include "field/field_effect_manager.h"

// What the field effect renderers share from modules that keep it to themselves for now:
// render_animation.c's model animation and render object helpers, field_effects.c's effect accessors and
// the billboard_lists.c object calls (billboard_lists.h has BillboardObject_SetVisible).

// Embedded by value: one copy loaded per renderer (FieldEffectManager_LoadRenderAnimation), one per
// running effect bound to the renderer's model (FieldEffectManager_InitRenderAnimation).
typedef struct RenderAnimation {
    u32 flags;
    fx32 frame;
    void *file;
    void *resource;
    NNSG3dAnmObj *object;
    NNSFndAllocator allocator;
} RenderAnimation;

void RenderAnimation_Free(RenderAnimation *animation);
BOOL RenderAnimation_Advance(RenderAnimation *animation, fx32 step, BOOL loop);
BOOL RenderAnimation_IsDone(RenderAnimation *animation);
void RenderObj_InitWithAnimation(NNSG3dRenderObj *object, RenderModelStorage *model, RenderAnimation *animation);
void RenderObj_Draw(NNSG3dRenderObj *object, const VecFx32 *position, const VecFx32 *scale, const MtxFx33 *rotation);

void *FieldEffect_GetState(FieldEffect *effect);
int FieldEffect_GetParam(FieldEffect *effect);
void FieldEffect_SetPosition(FieldEffect *effect, const VecFx32 *position);
void FieldEffect_RunUpdate(FieldEffect *effect);

BOOL BillboardObject_Delete(void *billboard);
void BillboardObject_SetPosition(void *billboard, const VecFx32 *position);
// TRUE once a non-looping sequence has reached its last frame: the result of BillboardSequence_Step, which
// it ends with (billboard_lists.c types it void).
BOOL BillboardObject_AdvanceFrame(void *billboard, fx32 delta);
void BillboardObject_SetFrame(void *billboard, fx32 frame);
fx32 BillboardObject_GetFrame(void *billboard);
void BillboardObject_SetSequenceFrame(void *billboard, fx32 frame);
fx32 BillboardObject_GetSequenceFrame(void *billboard);
void BillboardObject_SetSequence(void *billboard, u32 sequence);
u32 BillboardObject_GetSequence(void *billboard);
void BillboardObject_SetScale(void *billboard, VecFx32 *scale);
void BillboardObject_UpdateTextureAnimation(void *billboard);

#endif // POKEHEARTGOLD_FIELD_FIELD_EFFECT_HELPERS_H
