#ifndef POKEHEARTGOLD_RENDER_ANIMATION_H
#define POKEHEARTGOLD_RENDER_ANIMATION_H

#include "field/field_effect_manager.h"
#include "player_data.h"
#include "pokemon_types_def.h"

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

void RenderAnimation_LoadFromNarc(RenderAnimation *, int, NARC *, int, enum HeapID, BOOL);
void RenderAnimation_FreeFile(RenderAnimation *);
void RenderAnimation_AllocObjectForModel(RenderAnimation *, RenderModelStorage *, enum HeapID);
void RenderAnimation_InitObjectForModel(RenderAnimation *, RenderModelStorage *);
void RenderAnimation_InitForModel(RenderAnimation *, RenderModelStorage *, RenderAnimation *, int, enum HeapID);
void RenderAnimation_Free(RenderAnimation *);
BOOL RenderAnimation_Advance(RenderAnimation *, fx32, BOOL);
BOOL RenderAnimation_IsDone(RenderAnimation *);
void RenderObj_InitFromModel(NNSG3dRenderObj *object, RenderModelStorage *model);
void RenderObj_InitWithAnimation(NNSG3dRenderObj *, RenderModelStorage *, RenderAnimation *);
void RenderObj_Draw(NNSG3dRenderObj *, const VecFx32 *, const VecFx32 *, const MtxFx33 *);
void RenderObj_DrawAt(NNSG3dRenderObj *object, VecFx32 *position);
void RenderObj_DrawRotated(NNSG3dRenderObj *, const VecFx32 *, const VecFx32 *, const u16 *);

#endif // POKEHEARTGOLD_RENDER_ANIMATION_H
