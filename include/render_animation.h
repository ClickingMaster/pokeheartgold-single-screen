#ifndef POKEHEARTGOLD_RENDER_ANIMATION_H
#define POKEHEARTGOLD_RENDER_ANIMATION_H

#include "field/field_effect_manager.h"
#include "player_data.h"
#include "pokemon_types_def.h"

void RenderObj_InitFromModel(NNSG3dRenderObj *object, void *model);
void RenderObj_DrawAt(int object, VecFx32 *position);

#endif // POKEHEARTGOLD_RENDER_ANIMATION_H
