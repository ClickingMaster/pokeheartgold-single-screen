#ifndef POKEHEARTGOLD_OV_01_021E6880_H
#define POKEHEARTGOLD_OV_01_021E6880_H

#include "field_system.h"

BOOL ov01_021E690C(FieldSystem *fieldSystem);
void ov01_021F6B64_thunk(FieldSystem *fieldSystem, int arg1);
void FieldInput_Update(FieldInput *fieldInput, FieldSystem *fieldSystem, u16 newKeys, u16 heldKeys);
int FieldInput_Process(FieldInput *fieldInput, FieldSystem *fieldSystem);
BOOL FieldInput_Process_Colosseum(FieldInput *fieldInput, FieldSystem *fieldSystem);
BOOL FieldInput_Process_UnionRoom(FieldInput *fieldInput, FieldSystem *fieldSystem);
BOOL FieldInput_Process_BattleTower(FieldInput *fieldInput, FieldSystem *fieldSystem);
BOOL ov01_021E6DC4(FieldSystem *fieldSystem);
BOOL ov01_021E7114(FieldSystem *fieldSystem);
BOOL ov01_021E7198(FieldSystem *fieldSystem, FieldInput *fieldInput);
BOOL ov01_021E7628(FieldSystem *fieldSystem);
u8 ov01_021E7B54(FieldSystem *fieldSystem);
void ov01_021E7C70(FieldSystem *fieldSystem);
void ov01_021E7F00(FieldSystem *fieldSystem, BOOL a1);
u32 ov01_021E7F38(FieldSystem *fieldSystem);

// TODO: Put in the relevant headers.
void ov01_021F6B64(FieldSystem *fieldSystem, int arg1); // Could be s16 or int? Neither are working on their own.
int sub_0205DD94(PlayerAvatar *avatar, u16 newKeys, u16 heldKeys); // DEFINITELY CORRECT
BOOL TryGetSeenByNpcTrainers(FieldSystem *fieldSystem, BOOL doublesEligible);
BOOL BugContestTimeoutCheck(FieldSystem *fieldSystem);
BOOL ov01_021F1D94(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int playerEvents);
BOOL sub_0203DC64(FieldSystem *fieldSystem, LocalMapObject**);
u16 GetInteractedBackgroundEventScript(FieldSystem *fieldSystem, BG_EVENT *bgEvents, int numEvents);
u16 GetInteractedMetatileScript(FieldSystem *fieldSystem, u8 metatileBehavior);

#endif
