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

void RenderObj_InitFromModel(NNSG3dRenderObj *object, RenderModelStorage *model);
void RenderObj_DrawAt(int object, VecFx32 *position);

#endif // POKEHEARTGOLD_RENDER_ANIMATION_H
