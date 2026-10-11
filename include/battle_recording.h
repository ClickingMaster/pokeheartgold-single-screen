#ifndef POKEHEARTGOLD_BATTLE_RECORDING_H
#define POKEHEARTGOLD_BATTLE_RECORDING_H

#include "heap.h"
#include "save.h"

// A recorded battle (battle video): the record in progress or one loaded from the extra save chunks.
typedef struct BattleRecord {
    u32 unk_0000;
    u8 profile[0x80];  // 0x0004, BattleRecord_GetProfile
    u8 header[0x64];   // 0x0084, BattleRecordHeader: what the battle video lists show
    u8 data[0x1C68];   // 0x00E8, BattleRecordData, encrypted while stored (magic + CRC at the end)
} BattleRecord;        // size = 0x1D50

void BattleRecord_Create(SaveData *save, enum HeapID heapID, u32 *out);
BOOL BattleRecord_Exists(void);
void BattleRecord_SetCommand(int battlerId, u32 a1, u8 data);
u8 BattleRecord_GetCommand(int battlerId, u32 a1);

#endif // POKEHEARTGOLD_BATTLE_RECORDING_H
