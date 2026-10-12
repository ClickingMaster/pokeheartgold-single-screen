#ifndef POKEHEARTGOLD_BATTLE_RECORD_H
#define POKEHEARTGOLD_BATTLE_RECORD_H

#include "heap.h"
#include "save.h"

// Battle Video functions of battle_record.c; the record type and the recording interface are in battle_recording.h.

BOOL BattleRecord_IsSlotValid(SaveData *save, enum HeapID heapId, int *out, int index);
int BattleRecord_Save(SaveData *save, int type, int arg, int index, u16 *state, u16 *writeState);
BOOL BattleRecord_AreAiFlagsValid(void);
void BattleRecord_Delete(void);

#endif // POKEHEARTGOLD_BATTLE_RECORD_H
