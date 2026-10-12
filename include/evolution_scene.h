#ifndef POKEHEARTGOLD_EVOLUTION_SCENE_H
#define POKEHEARTGOLD_EVOLUTION_SCENE_H

#include "bag_types_def.h"
#include "game_stats.h"
#include "heap.h"
#include "party.h"
#include "pokedex.h"
#include "pokemon.h"

// The evolution scene (src/evolution_scene.c): plays an evolution and teaches the moves of the new level
typedef struct EvolutionTaskData EvolutionTaskData;

EvolutionTaskData *EvolutionScene_New(Party *party, Pokemon *mon, int species, Options *options, u32 a4, Pokedex *dex, Bag *bag, GameStats *stats, u32 evolutionCondition, u32 a9, enum HeapID heapID);
BOOL EvolutionScene_IsDone(EvolutionTaskData *);
void EvolutionScene_Delete(EvolutionTaskData *);

#endif // POKEHEARTGOLD_EVOLUTION_SCENE_H
