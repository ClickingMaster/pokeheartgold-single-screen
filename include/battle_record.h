#ifndef POKEHEARTGOLD_BATTLE_RECORD_H
#define POKEHEARTGOLD_BATTLE_RECORD_H

#include "battle/battle_setup.h"

#include "battle_recording.h"
#include "heap.h"
#include "save.h"

// Battle Video functions; the record type and the recording interface are in battle_recording.h.

u32 BattleRecord_sizeof(void);
void BattleRecord_Init(BattleRecord *);
void BattleRecord_Delete(void);
BattleRecord *BattleRecord_Get(void);
void *BattleRecord_GetProfile2(void);
BOOL BattleRecord_Load(SaveData *save, enum HeapID heapId, int *out, void *battle, int index);
BOOL BattleRecord_IsSlotValid(SaveData *save, enum HeapID heapId, int *out, int index);
int BattleRecord_WriteSlot(SaveData *, BattleRecord *, int, u16 *);
int BattleRecord_Save(SaveData *save, int type, int arg, int index, u16 *state, u16 *writeState);
void BattleRecord_GetBattlerAndPartyCount(int, int *, int *);
void BattleRecord_Encrypt(void *, u32, u32);
void BattleRecord_StoreSetup(BattleSetup *);
void BattleRecord_SetAiFlags(int, u32);
BOOL BattleRecord_AreAiFlagsValid(void);
void BattleRecord_RestoreSetup(BattleSetup *, SaveData *);
void *BattleRecord_CopyHeader(enum HeapID);
void *BattleRecord_CopyProfile(enum HeapID);
void *BattleRecord_GetProfile(void);
void *BattleRecord_GetHeader(void);
void BattleRecord_Import(void *, void *, void *, void *, SaveData *);
u64 BattleRecordHeader_GetField(void *, int, int);
void *BattleRecordHeader_New(enum HeapID);
void BattleRecordHeader_Free(void *);

#endif // POKEHEARTGOLD_BATTLE_RECORD_H
