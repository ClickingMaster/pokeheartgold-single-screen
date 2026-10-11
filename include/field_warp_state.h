#ifndef POKEHEARTGOLD_FIELD_WARP_STATE_H
#define POKEHEARTGOLD_FIELD_WARP_STATE_H

#include "field_types_def.h"
#include "heap.h"
#include "pokemon_types_def.h"

struct UnkStruct_02067A60;
struct FlyTaskEnv;

struct UnkStruct_02067A60 *sub_02067A60(enum HeapID heapID);
void sub_02067A78(struct UnkStruct_02067A60 *);

struct FlyTaskEnv *FlyTaskEnv_New(enum HeapID heapID, FieldSystem *fieldSystem, Pokemon *pokemon, u16 r3, u16 a4, s16 a5, s16 a6);
BOOL FieldTask_Fly(TaskManager *taskManager);

#endif // POKEHEARTGOLD_FIELD_WARP_STATE_H
