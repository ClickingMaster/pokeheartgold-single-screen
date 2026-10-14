#ifndef POKEHEARTGOLD_FIELD_WARP_STATE_H
#define POKEHEARTGOLD_FIELD_WARP_STATE_H

#include "field_types_def.h"
#include "heap.h"
#include "pokemon_types_def.h"

struct BattleSetupSub_134;
struct FlyTaskEnv;

struct BattleSetupSub_134 *BattleSetupSub134_New(enum HeapID heapID);
void BattleSetupSub134_Delete(struct BattleSetupSub_134 *);

struct FlyTaskEnv *FlyTaskEnv_New(enum HeapID heapID, FieldSystem *fieldSystem, Pokemon *pokemon, u16 r3, u16 mapNo, s16 x, s16 y);
BOOL FieldTask_Fly(TaskManager *taskManager);

#endif // POKEHEARTGOLD_FIELD_WARP_STATE_H
