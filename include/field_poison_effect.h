#ifndef POKEHEARTGOLD_FIELD_POISON_EFFECT_H
#define POKEHEARTGOLD_FIELD_POISON_EFFECT_H

#include "global.h"

#include "constants/heap.h"

#include "field/hblank_system.h"

#include "heap.h"

typedef struct PoisonEffect PoisonEffect;

void *PoisonEffect_New(enum HeapID heapID, HBlankSystem *hBlankSystem);
void PoisonEffect_Delete(PoisonEffect *effect);
void FieldSystem_DoPoisonEffect(PoisonEffect *effect);

#endif // POKEHEARTGOLD_FIELD_POISON_EFFECT_H
