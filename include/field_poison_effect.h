#ifndef POKEHEARTGOLD_FIELD_POISON_EFFECT_H
#define POKEHEARTGOLD_FIELD_POISON_EFFECT_H

#include "global.h"

#include "constants/heap.h"

#include "field/hblank_system.h"

#include "heap.h"

void *PoisonEffect_New(enum HeapID heapID, HBlankSystem *hBlankSystem);
void PoisonEffect_Delete(void *unk20);
void FieldSystem_DoPoisonEffect(void *unk20);

#endif // POKEHEARTGOLD_FIELD_POISON_EFFECT_H
