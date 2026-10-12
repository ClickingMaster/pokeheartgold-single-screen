#ifndef POKEHEARTGOLD_HEADBUTT_TREE_H
#define POKEHEARTGOLD_HEADBUTT_TREE_H

#include "field_system.h"
#include "field_types_def.h"
#include "script.h"

BOOL MapModel_IsHeadbuttTree(u32 mapModelNo);
u16 MapCoordToMatrixIndex(FieldSystem *fieldSystem, int coordX, int coordY);
void sub_02054EB0(FieldSystem *fieldSystem, int buildModel, BOOL culled);

#endif // POKEHEARTGOLD_HEADBUTT_TREE_H
