#ifndef POKEHEARTGOLD_FIELD_UNUSED_BILLBOARD_EFFECT_H
#define POKEHEARTGOLD_FIELD_UNUSED_BILLBOARD_EFFECT_H

#include "field/field_effect_manager.h"

// A billboard effect in the style of the grass effects that nothing spawns (field effect renderer 13).

typedef struct UnusedBillboard UnusedBillboard;

UnusedBillboard *UnusedBillboard_New(FieldEffectManager *manager);
void UnusedBillboard_Free(UnusedBillboard *renderer);

#endif // POKEHEARTGOLD_FIELD_UNUSED_BILLBOARD_EFFECT_H
