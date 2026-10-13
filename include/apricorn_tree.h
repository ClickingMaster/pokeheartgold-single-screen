#ifndef POKEHEARTGOLD_APRICORN_TREE_H
#define POKEHEARTGOLD_APRICORN_TREE_H

#include "save_misc_data.h"
#include "script.h"

void ApricornTrees_Init(APRICORN_TREE *trees);
void ApricornTrees_GrowAll(APRICORN_TREE *trees);
void ApricornTree_Clear(APRICORN_TREE *tree);
u8 ApricornTrees_HasApricorn(const APRICORN_TREE *trees, int idx);
u8 ApricornTrees_TryGetApricorn(const APRICORN_TREE *trees, int idx);
int ApricornTrees_GetApricorn(const APRICORN_TREE *trees, int idx);
void ApricornTrees_PickApricorn(APRICORN_TREE *trees, int idx);
void ApricornTrees_RegrowEmpty(APRICORN_TREE *trees);
void ApricornTrees_RegrowEmpty(APRICORN_TREE *trees);

#endif // POKEHEARTGOLD_APRICORN_TREE_H
