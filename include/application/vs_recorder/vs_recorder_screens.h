#ifndef POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H
#define POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H

#include "application/vs_recorder/vs_recorder_internal.h"
#include "battle/battle_setup.h"

#include "battle_recording.h"

// VS Recorder (overlay 40): the Global Terminal screens (video lists, video search, video number keypad, delete)
// and the battle video helpers used outside the app. Shared work and framework: vs_recorder_internal.h.

// global_terminal_net.c (not in global_terminal_net.h)
BOOL GlobalTerminalNet_IsIdle(GlobalTerminalNet *net);

// battle_record.c (not in battle_recording.h)
extern BattleRecord *gBattleRecord;
void BattleRecord_GetBattlerAndPartyCount(int type, int *count, int *partySize);
int BattleRecord_WriteSlot(SaveData *save, BattleRecord *record, int index, u16 *state);
void BattleRecord_Encrypt(void *data, u32 size, u32 seed);

// vs_recorder_net.c (the Global Terminal client is embedded at VsRecorderWork.unk_0068)
BOOL VsRecorder_IsNetIdle(VsRecorderWork *work);

// vs_recorder_battle_names.c
void VsRecorder_SanitizeBattleSetupNames(BattleRecord *record, BattleSetup *setup, enum HeapID heapID);

// vs_recorder_record.c
void VsRecorder_LoadPlaceholderTrainerName(String *str, enum HeapID heapID);
void VsRecorder_EncryptRecord(SaveData *saveData);
int VsRecorder_SaveRecordNumber(SaveData *saveData, u32 numberLow, u32 numberHigh, u16 *state, u16 *writeState);

#endif // POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_SCREENS_H
