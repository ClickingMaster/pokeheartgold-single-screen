#ifndef POKEHEARTGOLD_BATTLE_RECORD_H
#define POKEHEARTGOLD_BATTLE_RECORD_H

#include "heap.h"
#include "save.h"

// Battle Video functions of battle_record.c; the rest of its interface is in unk_0202FBCC.h.

BOOL sub_0202FD28(SaveData *save, enum HeapID heapId, int *out, int index);
int sub_0202FE14(SaveData *save, int type, int arg, int index, u16 *state, u16 *writeState);
BOOL sub_020304B4(void);

#endif // POKEHEARTGOLD_BATTLE_RECORD_H
